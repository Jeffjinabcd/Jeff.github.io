/*
 * WROOM-32 — DALEK TOP, DUAL MODE
 * ------------------------------------------------------------------
 * Mode chosen at boot by D4 (GPIO4):
 *   D4 -> GND  = INDIVIDUAL: WROOM runs its OWN Bluetooth, pairs the Xbox,
 *                and drives the dome/arm directly.
 *   D4 open    = RELAY: WROOM takes dome/arm commands from the base over nRF24.
 * To change mode: flip D4, then press RESET.
 *
 * Board: "ESP32 Dev Module"
 * nRF24:  MOSI->32 IRQ->33 MISO->25 SCK->26 CE->27 CSN->13  VCC->3V3 GND->GND (+10uF cap)
 * MDD10A: dome PWM->5, dome DIR->18, arm PWM->19, arm DIR->21
 * Controls (individual): RT/LT = dome, A = arm extend, Y = arm retract
 */
#include <Bluepad32.h>
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

const int MODE_PIN = 4;              // GND = individual (own BT), open = relay (nRF24)
bool individualMode = false;

// ===== nRF24 (relay mode) =====
RF24 radio(27, 13);                  // CE=27, CSN=13
const byte address[6] = "DALEK";
struct __attribute__((packed)) DalekPacket {
  bool    active;
  int16_t domeCmd;
  uint8_t armState;                  // 0 stop, 1 extend, 2 retract
};
DalekPacket rx;
unsigned long lastRxMs = 0;
const unsigned long FAILSAFE_MS = 300;

// ===== MDD10A pins =====
const int DOME_PWM_PIN = 5;
const int DOME_DIR_PIN = 18;
const int ARM_PWM_PIN  = 19;
const int ARM_DIR_PIN  = 21;
const int DOME_DEADZONE = 20;

// ===== Bluepad32 (individual mode) =====
ControllerPtr myController;
void onConnected(ControllerPtr ctl){ if(!myController){ Serial.println(">>> XBOX ONLINE"); myController=ctl; } }
void onDisconnected(ControllerPtr ctl){ if(myController==ctl){ myController=nullptr; Serial.println(">>> XBOX OFFLINE"); } }

// ---- shared motor helpers ----
void stopAll(){ analogWrite(DOME_PWM_PIN,0); analogWrite(ARM_PWM_PIN,0); }
void driveDome(int cmd){
  if(abs(cmd) > DOME_DEADZONE){
    digitalWrite(DOME_DIR_PIN, cmd > 0 ? HIGH : LOW);
    analogWrite(DOME_PWM_PIN, map(abs(cmd),0,1023,0,255));
  } else analogWrite(DOME_PWM_PIN, 0);
}
void driveArm(uint8_t state){
  if(state == 1){ digitalWrite(ARM_DIR_PIN,HIGH); analogWrite(ARM_PWM_PIN,255); }       // extend
  else if(state == 2){ digitalWrite(ARM_DIR_PIN,LOW); analogWrite(ARM_PWM_PIN,255); }   // retract
  else analogWrite(ARM_PWM_PIN, 0);
}

void setup(){
  Serial.begin(115200);
  delay(300);
  pinMode(MODE_PIN, INPUT_PULLUP);
  individualMode = (digitalRead(MODE_PIN) == LOW);   // GND = individual

  pinMode(DOME_PWM_PIN,OUTPUT); pinMode(DOME_DIR_PIN,OUTPUT);
  pinMode(ARM_PWM_PIN, OUTPUT); pinMode(ARM_DIR_PIN, OUTPUT);
  stopAll();

  if(individualMode){
    Serial.println("MODE: INDIVIDUAL - WROOM's own Bluetooth. Pair the Xbox to THIS board.");
    BP32.setup(&onConnected, &onDisconnected);
    BP32.forgetBluetoothKeys();
    BP32.enableNewBluetoothConnections(true);
  } else {
    Serial.println("MODE: RELAY - taking dome/arm from the base over nRF24.");
    SPI.begin(26, 25, 32, 13);       // SCK=26, MISO=25, MOSI=32, SS=13
    if(!radio.begin(&SPI)) Serial.println("nRF24 NOT found - check wiring / 3.3V / cap");
    Serial.printf("nRF24 chip connected: %s\n", radio.isChipConnected() ? "YES" : "NO");
    radio.setPALevel(RF24_PA_LOW);
    radio.setChannel(100);
    radio.openReadingPipe(0, address);
    radio.startListening();
  }
}

void loop(){
  if(individualMode){
    BP32.update();
    if(myController && myController->isConnected()){
      int dome = myController->throttle() - myController->brake();       // RT - LT
      uint8_t arm = myController->a() ? 1 : (myController->y() ? 2 : 0); // A=extend, Y=retract
      driveDome(dome);
      driveArm(arm);
    } else stopAll();
  } else {
    if(radio.available()){ radio.read(&rx, sizeof(rx)); lastRxMs = millis(); }
    bool live = (millis() - lastRxMs <= FAILSAFE_MS) && rx.active;
    if(live){ driveDome(rx.domeCmd); driveArm(rx.armState); }
    else stopAll();
  }
  delay(5);
}