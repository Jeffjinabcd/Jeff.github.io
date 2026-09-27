/*
 * Nano ESP32 (S3) — DALEK BASE (transmitter)
 * ------------------------------------------------------------------
 * Xbox host. Drives the drivetrain locally AND sends the FULL controller
 * state to the WROOM over nRF24 — so the top board can use ANY button
 * without ever changing this base code again.
 *
 * Board: "Arduino Nano ESP32"
 * nRF24: MOSI->D8(17) IRQ->D9(18) MISO->D10(21) SCK->D11(38) CE->D12(47)
 *        CSN->D13(48)  VCC->3V3  GND->GND  (+10uF cap)
 */
#include <Bluepad32.h>
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

RF24 radio(47, 48);                  // CE=47, CSN=48
const byte address[6] = "DALEK";

// FULL controller state — MUST be byte-identical in the WROOM sketch
struct __attribute__((packed)) DalekPacket {
  bool     active;                   // controller connected
  int16_t  lx, ly, rx, ry;           // sticks (-512..511)
  int16_t  lt, rt;                   // triggers (0..1023)  LT=brake, RT=throttle
  uint8_t  dpad;                     // bitmask 0x01 up 0x02 down 0x04 right 0x08 left
  bool     a, b, x, y, l1, r1;       // buttons
  uint8_t  misc;                     // misc buttons bitmask: View/Menu/Share (miscButtons())
  bool     demoMode;                 // export -> dome demo (base keeps driving)
  bool     tuning;                   // View -> color tuning (base drivetrain disabled)
};
DalekPacket tx;

// ===== drivetrain =====
const int L_MOTOR_PINS[] = {5, 6};
const int R_MOTOR_PINS[] = {7, 9};
const int L_CHANNEL = 0, R_CHANNEL = 1;
const int PWM_FREQ = 50, PWM_RES = 14;
const int PWM_MIN = 819, PWM_NEUTRAL = 1229, PWM_MAX = 1638;
const int DEADBAND = 50;
bool REVERSE_THROTTLE = false;

ControllerPtr myController;
float globalSpeedLimit = 0.5;
uint8_t lastDpadState = 0;
bool demoMode = false;                // export toggles the dome demo (base KEEPS driving)
bool tuning   = false;                // View toggles color tuning -> base drivetrain disabled

void onConnected(ControllerPtr ctl){ if(!myController){ Serial.println(">>> XBOX ONLINE"); myController=ctl; } }
void onDisconnected(ControllerPtr ctl){ if(myController==ctl){ myController=nullptr; Serial.println(">>> XBOX OFFLINE"); } }
void driveNeutral(){ ledcWrite(L_CHANNEL, PWM_NEUTRAL); ledcWrite(R_CHANNEL, PWM_NEUTRAL); }

void setup(){
  Serial.begin(115200);
  delay(300);

  ledcSetup(L_CHANNEL, PWM_FREQ, PWM_RES);
  ledcSetup(R_CHANNEL, PWM_FREQ, PWM_RES);
  ledcAttachPin(L_MOTOR_PINS[0], L_CHANNEL);
  ledcAttachPin(L_MOTOR_PINS[1], L_CHANNEL);
  ledcAttachPin(R_MOTOR_PINS[0], R_CHANNEL);
  ledcAttachPin(R_MOTOR_PINS[1], R_CHANNEL);
  driveNeutral();

  SPI.begin(38, 21, 17, 48);         // SCK=38, MISO=21, MOSI=17, SS=48
  if(!radio.begin(&SPI)) Serial.println("nRF24 NOT found - check wiring / 3.3V / cap");
  radio.setPALevel(RF24_PA_LOW);
  radio.setChannel(100);
  radio.setRetries(3, 3);            // short retries so a dropped packet can't stall
  radio.openWritingPipe(address);
  radio.stopListening();

  BP32.setup(&onConnected, &onDisconnected);
  BP32.forgetBluetoothKeys();
  BP32.enableNewBluetoothConnections(true);
  Serial.println("Base ready: drivetrain + full-state nRF24 sender. Pair Xbox.");
}

void loop(){
  BP32.update();

  if(myController && myController->isConnected()){
    // ---- speed limit (dpad up/down) ----
    uint8_t dpad = myController->dpad();
    if((dpad & 0x01) && !(lastDpadState & 0x01)) globalSpeedLimit += 0.10;
    if((dpad & 0x02) && !(lastDpadState & 0x02)) globalSpeedLimit -= 0.10;
    lastDpadState = dpad;
    globalSpeedLimit = constrain(globalSpeedLimit, 0.1, 1.0);

    // ---- DEMO toggle (Share) mirrors the dome: hold the drivetrain still while it runs ----
    // View (0x02) toggles COLOR TUNING -> hold the drivetrain still (sticks tune color, not drive)
    static bool lastView = false;
    bool view = myController->miscButtons() & 0x02;
    if(view && !lastView){ tuning = !tuning; Serial.println(tuning ? "BASE: tuning ON (wheels held)" : "BASE: tuning OFF"); }
    lastView = view;

    // Export (0x08) toggles the DOME DEMO -> base KEEPS driving (demo is autonomous up top)
    static bool lastShare = false;
    bool share = myController->miscButtons() & 0x08;
    if(share && !lastShare){ demoMode = !demoMode; Serial.println(demoMode ? "BASE: demo ON (still driving)" : "BASE: demo OFF"); }
    lastShare = share;

    // ---- drivetrain (local): held still ONLY while color tuning ----
    if(tuning){
      driveNeutral();
    } else {
      int throttle = myController->axisY();
      int turn     = myController->axisRX();
      if(REVERSE_THROTTLE) throttle = -throttle;
      if(abs(throttle) < DEADBAND) throttle = 0;
      if(abs(turn) < DEADBAND)     turn = 0;
      throttle = (int)(throttle * globalSpeedLimit);
      turn     = (int)(turn * globalSpeedLimit);
      int lP = throttle + turn, rP = throttle - turn;
      ledcWrite(L_CHANNEL, map(constrain(lP,-512,512),-512,512,PWM_MIN,PWM_MAX));
      ledcWrite(R_CHANNEL, map(constrain(rP,-512,512),-512,512,PWM_MAX,PWM_MIN));
    }

    // ---- fill the FULL packet ----
    tx.active = true;
    tx.lx = myController->axisX();  tx.ly = myController->axisY();
    tx.rx = myController->axisRX(); tx.ry = myController->axisRY();
    tx.lt = myController->brake();  tx.rt = myController->throttle();
    tx.dpad = dpad;
    tx.a = myController->a(); tx.b = myController->b();
    tx.x = myController->x(); tx.y = myController->y();
    tx.l1 = myController->l1(); tx.r1 = myController->r1();
    tx.misc = myController->miscButtons();   // View / Menu / Share
    tx.demoMode = demoMode;                  // dome runs its demo
    tx.tuning   = tuning;                    // dome enters tuning; base already held its wheels
  } else {
    driveNeutral();
    tx.active = false;
    tx.lx=tx.ly=tx.rx=tx.ry=tx.lt=tx.rt=0; tx.dpad=0;
    tx.a=tx.b=tx.x=tx.y=tx.l1=tx.r1=false;
    tx.misc=0;
    tx.demoMode=demoMode; tx.tuning=tuning;
  }

  radio.write(&tx, sizeof(tx));       // send full state (~50 Hz)
  delay(20);
}