/*
 * WROOM-32 — DALEK TOP  (RECEIVER)
 * ------------------------------------------------------------------
 * Receives dome/arm commands from the base over nRF24 and drives the
 * dome motor + arm through the MDD10A. No LED / sound / encoder yet.
 *
 * Board: "ESP32 Dev Module"
 * nRF24:  MOSI->27  IRQ->26  MISO->25  SCK->33  CE->32  CSN->14
 *         VCC->3V3  GND->GND  (+10uF cap)
 * MDD10A: dome PWM->5, dome DIR->18, arm PWM->19, arm DIR->21
 */
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

RF24 radio(27, 13);                 // CE=27, CSN=13  (matches your wiring)
const byte address[6] = "DALEK";

// MUST be identical in the base sketch
struct __attribute__((packed)) DalekPacket {
  bool    active;
  int16_t domeCmd;
  uint8_t armState;   // 0 stop, 1 extend, 2 retract
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

void stopAll(){ analogWrite(DOME_PWM_PIN,0); analogWrite(ARM_PWM_PIN,0); }

void setup(){
  Serial.begin(115200);
  delay(300);
  pinMode(DOME_PWM_PIN,OUTPUT); pinMode(DOME_DIR_PIN,OUTPUT);
  pinMode(ARM_PWM_PIN, OUTPUT); pinMode(ARM_DIR_PIN, OUTPUT);
  stopAll();

  SPI.begin(26, 25, 32, 13);        // SCK=26, MISO=25, MOSI=32, SS=13
  if(!radio.begin(&SPI)) Serial.println("nRF24 NOT found - check wiring / 3.3V / cap");
  Serial.printf("nRF24 chip connected: %s\n", radio.isChipConnected() ? "YES" : "NO");
  radio.setPALevel(RF24_PA_LOW);
  radio.setChannel(100);            // must match base
  radio.openReadingPipe(0, address);
  radio.startListening();
  Serial.println("Top ready: nRF24 receiver + dome/arm.");
}

void loop(){
  if(radio.available()){
    radio.read(&rx, sizeof(rx));
    lastRxMs = millis();
  }

  bool live = (millis() - lastRxMs <= FAILSAFE_MS) && rx.active;

  if(live){
    // --- dome ---
    if(abs(rx.domeCmd) > DOME_DEADZONE){
      digitalWrite(DOME_DIR_PIN, rx.domeCmd > 0 ? HIGH : LOW);
      analogWrite(DOME_PWM_PIN, map(abs(rx.domeCmd),0,1023,0,255));
    } else analogWrite(DOME_PWM_PIN, 0);
    // --- arm ---
    if(rx.armState == 1){ digitalWrite(ARM_DIR_PIN,HIGH); analogWrite(ARM_PWM_PIN,255); }   // extend
    else if(rx.armState == 2){ digitalWrite(ARM_DIR_PIN,LOW); analogWrite(ARM_PWM_PIN,255); } // retract
    else analogWrite(ARM_PWM_PIN, 0);
  } else {
    stopAll();   // failsafe: no packets or controller off
  }
  delay(5);
}