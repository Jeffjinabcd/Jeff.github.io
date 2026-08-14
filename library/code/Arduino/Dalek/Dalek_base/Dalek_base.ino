/*
 * Nano ESP32 (S3) — DALEK BASE  (TRANSMITTER)
 * ------------------------------------------------------------------
 * Xbox host. Drives the drivetrain locally AND sends dome/arm commands
 * to the WROOM over nRF24. Enable switch on A3. No LED / sound.
 *
 * Board: "Arduino Nano ESP32"
 * nRF24 wiring:  MOSI->D8(GPIO17)  IRQ->D9(GPIO18,unused)  MISO->D10(GPIO21)
 *                SCK->D11(GPIO38)  CE->D12(GPIO47)  CSN->D13(GPIO48)
 *                VCC->3V3  GND->GND  (+10uF cap across VCC/GND)
 * Switch: A3 to GND = drive ENABLED, open = everything OFF (fail-safe).
 */
#include <Bluepad32.h>
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

// ===== nRF24 =====
RF24 radio(47, 48);                 // CE=GPIO47(D12), CSN=GPIO48(D13)
const byte address[6] = "DALEK";

// MUST be identical in the WROOM sketch
struct __attribute__((packed)) DalekPacket {
  bool    active;
  int16_t domeCmd;      // RT - LT, -1023..1023
  uint8_t armState;     // 0 stop, 1 extend, 2 retract
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

  SPI.begin(38, 21, 17, 48);        // SCK=38, MISO=21, MOSI=17, SS=48
  if(!radio.begin(&SPI)) Serial.println("nRF24 NOT found - check wiring / 3.3V / cap");
  radio.setPALevel(RF24_PA_LOW);
  radio.setChannel(100);            // must match WROOM
  radio.setRetries(3, 3);           // short retries so a dropped packet can't stall the loop
  radio.openWritingPipe(address);
  radio.stopListening();

  BP32.setup(&onConnected, &onDisconnected);
  BP32.forgetBluetoothKeys();       // clear stale bond so the Xbox re-pairs
  BP32.enableNewBluetoothConnections(true);
  Serial.println("Base ready: drivetrain + nRF24 sender. Flip switch, pair Xbox.");
}

void loop(){
  BP32.update();
  bool active = myController && myController->isConnected();

  if(active){
    uint8_t dpad = myController->dpad();
    if((dpad & 0x01) && !(lastDpadState & 0x01)) globalSpeedLimit += 0.10;
    if((dpad & 0x02) && !(lastDpadState & 0x02)) globalSpeedLimit -= 0.10;
    lastDpadState = dpad;
    globalSpeedLimit = constrain(globalSpeedLimit, 0.1, 1.0);

    // --- drivetrain (local) ---
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

    // --- dome + arm commands (sent to WROOM) ---
    tx.active   = true;
    tx.domeCmd  = myController->throttle() - myController->brake();     // RT - LT
    tx.armState = myController->a() ? 1 : (myController->y() ? 2 : 0);  // A=extend, Y=retract
  } else {
    driveNeutral();
    tx.active = false; tx.domeCmd = 0; tx.armState = 0;
  }

  radio.write(&tx, sizeof(tx));      // send to WROOM (~50 Hz)
  delay(20);
}