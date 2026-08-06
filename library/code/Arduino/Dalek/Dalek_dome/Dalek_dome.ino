/*
 * WROOM-32  —  DALEK BRAIN (host)
 * ------------------------------------------------------------------
 * - Pairs the Xbox controller (Bluepad32)
 * - Drives the DOME + ARM directly (same pins/mapping as your earlier dome sketch)
 * - Sends drive commands (throttle/turn) to the Nano over ESP-NOW
 * - Prints all controller data to Serial (this board is on your laptop)
 *
 * Board:  "ESP32 Dev Module"  (the WROOM-32)
 * The ORIGINAL ESP32 can run Bluetooth + WiFi together, which the Nano (S3) can't.
 * That's why the host lives here.
 */

#include <Bluepad32.h>
#include <WiFi.h>
#include <esp_now.h>
#include <esp_wifi.h>

// ===== DOME + ARM pins (same layout as your earlier dome sketch) =====
// SWAPPED to match physical wiring: dome is now on MDD10A Ch2, arm on Ch1
const int DOME_PWM_PIN  = 27;   // Cytron MDD10A ch2 PWM  (dome)
const int DOME_DIR_PIN  = 26;   // Cytron MDD10A ch2 DIR  (dome)
const int ARM_PWM_PIN   = 25;   // Cytron MDD10A ch1 PWM  (arm)
const int ARM_DIR_PIN   = 33;   // Cytron MDD10A ch1 DIR  (arm)
const int DOME_DEADZONE = 20;

// ===== Dome encoder (SKU 638326 — open-collector, needs pull-ups) =====
const int ENC_A_PIN = 16;   // Channel A (yellow wire)
const int ENC_B_PIN = 17;   // Channel B (brown wire)
// Output shaft = ~2443 countable events/rev at full 4x. We interrupt on A only
// (2x), so ~1221/rev. CALIBRATE: spin the dome exactly one full turn, read the
// count printed on serial, and put that number here.
const float COUNTS_PER_REV = 1221.48;
volatile long domeCount = 0;

// ===== drivetrain input tuning (matches your Nano arcade drive) =====
const int  DEADBAND = 50;
bool REVERSE_THROTTLE = false;

// ===== ESP-NOW packet sent to the Nano — MUST match the Nano sketch =====
typedef struct DrivePacket {
  bool    connected;
  int16_t throttle;       // already limited + deadbanded, -512..512
  int16_t turn;           // already limited + deadbanded, -512..512
  uint8_t speedLimitPct;  // for display only
} DrivePacket;

DrivePacket tx;
uint8_t nanoBroadcast[6] = {0xFF,0xFF,0xFF,0xFF,0xFF,0xFF}; // broadcast: no MAC needed

ControllerPtr myController;
float globalSpeedLimit = 0.5;   // starts at 50%
uint8_t lastDpadState = 0;
unsigned long lastPrint = 0;

void onConnected(ControllerPtr ctl){ if(!myController){ Serial.println(">>> XBOX ONLINE");  myController=ctl; } }
void onDisconnected(ControllerPtr ctl){ if(myController==ctl){ myController=nullptr; Serial.println(">>> XBOX OFFLINE"); } }

// Encoder interrupt: fires on every edge of Channel A.
// If A and B match, we're turning one way; if not, the other.
void IRAM_ATTR readDomeEncoder(){
  bool a = digitalRead(ENC_A_PIN);
  bool b = digitalRead(ENC_B_PIN);
  if (a == b) domeCount++; else domeCount--;
}

void setup(){
  Serial.begin(115200);
  delay(500);

  // dome/arm outputs
  pinMode(DOME_PWM_PIN,OUTPUT); pinMode(DOME_DIR_PIN,OUTPUT);
  pinMode(ARM_PWM_PIN, OUTPUT); pinMode(ARM_DIR_PIN, OUTPUT);
  analogWrite(DOME_PWM_PIN,0);  analogWrite(ARM_PWM_PIN,0);   // safe on boot

  // dome encoder inputs — open-collector, so use internal pull-ups
  pinMode(ENC_A_PIN, INPUT_PULLUP);
  pinMode(ENC_B_PIN, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(ENC_A_PIN), readDomeEncoder, CHANGE);

  // ESP-NOW (WiFi). Original ESP32 handles BT + WiFi coexistence.
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  esp_wifi_set_channel(1, WIFI_SECOND_CHAN_NONE);   // MUST match the Nano
  if(esp_now_init()!=ESP_OK) Serial.println("ESP-NOW init FAILED");
  esp_now_peer_info_t peer = {};
  memcpy(peer.peer_addr, nanoBroadcast, 6);
  peer.channel = 1; peer.encrypt = false;
  esp_now_add_peer(&peer);

  // Bluetooth controller
  BP32.setup(&onConnected, &onDisconnected);
  BP32.forgetBluetoothKeys();   // clean pairing; comment out once stable

  Serial.println("WROOM ready: Xbox host + dome/arm + ESP-NOW sender (channel 1)");
}

void loop(){
  BP32.update();

  if(myController && myController->isConnected()){
    // ---- read controller ----
    int lx=myController->axisX(),  ly=myController->axisY();
    int rx=myController->axisRX(), ry=myController->axisRY();
    int lt=myController->brake(),  rt=myController->throttle();
    uint8_t dpad=myController->dpad();
    bool bA=myController->a(), bB=myController->b(), bX=myController->x(), bY=myController->y();

    // ---- DOME: RT - LT (same as earlier) ----
    int domeCmd = rt - lt;
    if(abs(domeCmd) > DOME_DEADZONE){
      digitalWrite(DOME_DIR_PIN, domeCmd>0 ? HIGH : LOW);
      analogWrite(DOME_PWM_PIN, map(abs(domeCmd),0,1023,0,255));
    } else analogWrite(DOME_PWM_PIN,0);

    // ---- ARM: Y extend, A retract (same as earlier) ----
    if(bA){ digitalWrite(ARM_DIR_PIN,HIGH); analogWrite(ARM_PWM_PIN,255); }
    else if(bY){ digitalWrite(ARM_DIR_PIN,LOW); analogWrite(ARM_PWM_PIN,255); }
    else analogWrite(ARM_PWM_PIN,0);

    // ---- DRIVE: dpad up/down speed limit (same as earlier) ----
    if((dpad&0x01)&&!(lastDpadState&0x01)) globalSpeedLimit+=0.10;
    if((dpad&0x02)&&!(lastDpadState&0x02)) globalSpeedLimit-=0.10;
    lastDpadState=dpad;
    globalSpeedLimit=constrain(globalSpeedLimit,0.1,1.0);

    int throttle=ly, turn=rx;
    if(REVERSE_THROTTLE) throttle=-throttle;
    if(abs(throttle)<DEADBAND) throttle=0;
    if(abs(turn)<DEADBAND)     turn=0;
    throttle=(int)(throttle*globalSpeedLimit);
    turn    =(int)(turn*globalSpeedLimit);

    tx.connected=true;
    tx.throttle=throttle;
    tx.turn=turn;
    tx.speedLimitPct=(uint8_t)(globalSpeedLimit*100);

    // ---- print everything ----
    if(millis()-lastPrint>=150){
      long c = domeCount;
      float deg = (float)c / COUNTS_PER_REV * 360.0;
      Serial.printf("XBOX L(%5d,%5d) R(%5d,%5d) LT:%4d RT:%4d dpad:0x%02x A:%d B:%d X:%d Y:%d "
                    "| domeCmd:%5d enc:%ld (%.1f deg) | SEND thr:%4d turn:%4d lim:%d%%\n",
        lx,ly,rx,ry,lt,rt,dpad,bA,bB,bX,bY,domeCmd,c,deg,tx.throttle,tx.turn,tx.speedLimitPct);
      lastPrint=millis();
    }
  } else {
    // no controller: stop dome/arm, tell Nano to neutral
    analogWrite(DOME_PWM_PIN,0); analogWrite(ARM_PWM_PIN,0);
    tx.connected=false; tx.throttle=0; tx.turn=0;
    if(millis()-lastPrint>=1000){ Serial.println("...waiting for Xbox..."); lastPrint=millis(); }
  }

  esp_now_send(nanoBroadcast,(uint8_t*)&tx,sizeof(tx));   // ~100 Hz
  delay(10);
}