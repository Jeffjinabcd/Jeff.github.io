/*
 * Nano ESP32 (S3)  —  DALEK DRIVETRAIN, DUAL MODE
 * ------------------------------------------------------------------
 * The mode is chosen AT BOOT by a switch/button on MODE_PIN:
 *
 *   MODE_PIN OPEN  (not pressed)  -> RELAY MODE
 *        WiFi/ESP-NOW. Gets drive commands from the WROOM. No Bluetooth.
 *
 *   MODE_PIN to GND (pressed / switch closed) -> STANDALONE MODE
 *        Bluetooth. Nano pairs the Xbox ITSELF and drives. No WROOM needed.
 *
 * To change mode: set the switch, then press RESET. (The S3 can't run
 * Bluetooth and WiFi at once, so we pick one radio at startup.)
 *
 * Board:  "Arduino Nano ESP32"
 */

#include <Bluepad32.h>     // used only in standalone mode
#include <WiFi.h>          // used only in relay mode
#include <esp_now.h>
#include <esp_wifi.h>

// ===== MODE SELECT =====
const int MODE_PIN = 4;    // wire a switch/button between this pin and GND.
                           // OPEN = relay, CLOSED(GND) = standalone.
                           // (any free GPIO works — change if pin 4 is used)
bool standaloneMode = false;

// ===== drivetrain layout (same as before) =====
const int L_MOTOR_PINS[] = {5, 6};   // D2, D3  -> left Victor(s)
const int R_MOTOR_PINS[] = {7, 9};   // D4, D6  -> right Victor(s)
const int L_CHANNEL = 0;
const int R_CHANNEL = 1;
const int PWM_FREQ = 50;
const int PWM_RES  = 14;
const int PWM_MIN     = 819;
const int PWM_NEUTRAL = 1229;
const int PWM_MAX     = 1638;
const int DEADBAND = 50;
bool REVERSE_THROTTLE = false;

// ===== relay-mode packet (must match the WROOM sketch) =====
typedef struct DrivePacket {
  bool    connected;
  int16_t throttle;
  int16_t turn;
  uint8_t speedLimitPct;
} DrivePacket;

DrivePacket rxData;
unsigned long lastRxMs = 0;
const unsigned long FAILSAFE_MS = 300;

// ===== standalone-mode state =====
ControllerPtr myController;
float globalSpeedLimit = 0.5;
uint8_t lastDpadState = 0;

// ---------- shared helpers ----------
void driveNeutral(){
  ledcWrite(L_CHANNEL, PWM_NEUTRAL);
  ledcWrite(R_CHANNEL, PWM_NEUTRAL);
}
void driveArcade(int throttle, int turn){
  int leftPower  = throttle + turn;
  int rightPower = throttle - turn;
  int leftDuty  = map(constrain(leftPower,  -512, 512), -512, 512, PWM_MIN, PWM_MAX);
  int rightDuty = map(constrain(rightPower, -512, 512), -512, 512, PWM_MAX, PWM_MIN);
  ledcWrite(L_CHANNEL, leftDuty);
  ledcWrite(R_CHANNEL, rightDuty);
}

// ---------- relay mode ----------
// If this line won't compile, you're on core 3.x — use the alt signature:
//   void onRecv(const esp_now_recv_info_t *info, const uint8_t *in, int len) {
void onRecv(const uint8_t *mac, const uint8_t *in, int len){
  if(len == sizeof(DrivePacket)){ memcpy(&rxData, in, sizeof(rxData)); lastRxMs = millis(); }
}
void loopRelay(){
  if(millis()-lastRxMs > FAILSAFE_MS || !rxData.connected) driveNeutral();
  else driveArcade(rxData.throttle, rxData.turn);
  delay(10);
}

// ---------- standalone mode ----------
void onConnected(ControllerPtr ctl){ if(!myController){ Serial.println(">>> XBOX ONLINE"); myController=ctl; } }
void onDisconnected(ControllerPtr ctl){ if(myController==ctl){ myController=nullptr; Serial.println(">>> XBOX OFFLINE"); } }
void loopStandalone(){
  BP32.update();
  if(myController && myController->isConnected()){
    uint8_t dpad = myController->dpad();
    if((dpad&0x01)&&!(lastDpadState&0x01)) globalSpeedLimit+=0.10;
    if((dpad&0x02)&&!(lastDpadState&0x02)) globalSpeedLimit-=0.10;
    lastDpadState = dpad;
    globalSpeedLimit = constrain(globalSpeedLimit, 0.1, 1.0);

    int throttle = myController->axisY();
    int turn     = myController->axisRX();
    if(REVERSE_THROTTLE) throttle = -throttle;
    if(abs(throttle)<DEADBAND) throttle=0;
    if(abs(turn)<DEADBAND) turn=0;
    throttle = (int)(throttle*globalSpeedLimit);
    turn     = (int)(turn*globalSpeedLimit);
    driveArcade(throttle, turn);
  } else {
    driveNeutral();
  }
  delay(10);
}

void setup(){
  Serial.begin(115200);
  delay(300);

  pinMode(MODE_PIN, INPUT_PULLUP);
  delay(10);
  standaloneMode = (digitalRead(MODE_PIN) == LOW);   // LOW (to GND) = standalone

  // motor PWM (shared, both modes)
  ledcSetup(L_CHANNEL, PWM_FREQ, PWM_RES);
  ledcSetup(R_CHANNEL, PWM_FREQ, PWM_RES);
  ledcAttachPin(L_MOTOR_PINS[0], L_CHANNEL);
  ledcAttachPin(L_MOTOR_PINS[1], L_CHANNEL);
  ledcAttachPin(R_MOTOR_PINS[0], R_CHANNEL);
  ledcAttachPin(R_MOTOR_PINS[1], R_CHANNEL);
  driveNeutral();

  if(standaloneMode){
    Serial.println("\n=== MODE: STANDALONE (Nano pairs Xbox directly) ===");
    BP32.setup(&onConnected, &onDisconnected);
    BP32.forgetBluetoothKeys();
  } else {
    Serial.println("\n=== MODE: RELAY (ESP-NOW from WROOM, channel 1) ===");
    WiFi.mode(WIFI_STA);
    WiFi.disconnect();
    esp_wifi_set_channel(1, WIFI_SECOND_CHAN_NONE);
    if(esp_now_init()!=ESP_OK) Serial.println("ESP-NOW init FAILED");
    esp_now_register_recv_cb(onRecv);
  }
}

void loop(){
  if(standaloneMode) loopStandalone();
  else               loopRelay();
}