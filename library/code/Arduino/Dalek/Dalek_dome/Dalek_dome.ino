/*
 * WROOM-32 — DALEK TOP: dome + arm + sound + LED
 * ------------------------------------------------------------------
 * Uses the FULL controller state (relay from base, or its own in individual).
 * Mode at boot by D4:  GND = individual (own BT),  open = relay (nRF24).
 *
 * Controls:
 *   RT / LT  = dome rotate
 *   A / Y    = arm extend / retract
 *   X        = play 0001.mp3
 *   LED      = always ON (solid color)
 *   Hold B   = tune the LED — right stick = hue, left stick up/down = brightness
 *
 * Board: "ESP32 Dev Module"
 * Libraries: Bluepad32, RF24, DFRobotDFPlayerMini, Adafruit NeoPixel
 * nRF24:   MOSI->32 IRQ->33 MISO->25 SCK->26 CE->27 CSN->13  (+10uF cap, 3V3)
 * MDD10A:  dome PWM->5, dome DIR->18, arm PWM->19, arm DIR->21
 * DFPlayer: RX->1k->GPIO12(TX), TX->GPIO14(RX), 5V, GND, DAC->amp AUX
 * LED:     GPIO22 -> TC4420 level shifter -> strip DIN; strip +12V/GND from battery, common ground
 */
#include <Bluepad32.h>
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <HardwareSerial.h>
#include <DFRobotDFPlayerMini.h>
#include <Adafruit_NeoPixel.h>

const int MODE_PIN = 4;
bool individualMode = false;

// ===== nRF24 =====
RF24 radio(27, 13);
const byte address[6] = "DALEK";
struct __attribute__((packed)) DalekPacket {   // MUST match the base sketch
  bool     active;
  int16_t  lx, ly, rx, ry;
  int16_t  lt, rt;
  uint8_t  dpad;
  bool     a, b, x, y, l1, r1;
};
DalekPacket pkt;
unsigned long lastRxMs = 0;
const unsigned long FAILSAFE_MS = 300;

// ===== MDD10A =====
const int DOME_PWM_PIN = 5, DOME_DIR_PIN = 18, ARM_PWM_PIN = 19, ARM_DIR_PIN = 21;
const int DOME_DEADZONE = 20;

// ===== DFPlayer =====
HardwareSerial dfSerial(2);
DFRobotDFPlayerMini player;
bool playerOk = false;
bool lastX = false;

// ===== LED strip =====
#define LED_PIN   22
#define NUM_LEDS  22
Adafruit_NeoPixel strip(NUM_LEDS, LED_PIN, NEO_RGB + NEO_KHZ400);  // WS2811; try NEO_GRB/BRG if colors wrong
uint8_t ledHue = 0;
int     ledBright = 150;
const int STICK_DEADZONE = 80;

// ===== Bluepad32 =====
ControllerPtr myController;
void onConnected(ControllerPtr ctl){ if(!myController){ Serial.println(">>> XBOX ONLINE"); myController=ctl; } }
void onDisconnected(ControllerPtr ctl){ if(myController==ctl){ myController=nullptr; Serial.println(">>> XBOX OFFLINE"); } }

// ---- helpers ----
void stopAll(){ analogWrite(DOME_PWM_PIN,0); analogWrite(ARM_PWM_PIN,0); }
void driveDome(int cmd){
  if(abs(cmd) > DOME_DEADZONE){
    digitalWrite(DOME_DIR_PIN, cmd > 0 ? HIGH : LOW);
    analogWrite(DOME_PWM_PIN, map(abs(cmd),0,1023,0,255));
  } else analogWrite(DOME_PWM_PIN, 0);
}
void driveArm(bool a, bool y){
  if(a){ digitalWrite(ARM_DIR_PIN,HIGH); analogWrite(ARM_PWM_PIN,255); }
  else if(y){ digitalWrite(ARM_DIR_PIN,LOW); analogWrite(ARM_PWM_PIN,255); }
  else analogWrite(ARM_PWM_PIN, 0);
}
void playIfX(bool x){
  if(x && !lastX && playerOk){ player.playMp3Folder(1); Serial.println("X -> playing 0001"); }
  lastX = x;
}
void showLED(){
  uint32_t c = strip.gamma32(strip.ColorHSV((uint16_t)ledHue * 256, 255, ledBright));
  for(int i=0;i<NUM_LEDS;i++) strip.setPixelColor(i, c);
  strip.show();
}
void updateLED(bool b, int rStickX, int lStickY){
  if(b){                    // B held = tuning mode
    if(abs(rStickX) > STICK_DEADZONE) ledHue += (int8_t)(rStickX / 300);           // right stick scrubs hue (wraps)
    if(abs(lStickY) > STICK_DEADZONE) ledBright = constrain(ledBright - (lStickY / 200), 10, 255); // up = brighter
    showLED();
  }
  // LED is always ON: re-assert the color about once a second so it holds
  static unsigned long lastRefresh = 0;
  if(millis() - lastRefresh > 1000){ showLED(); lastRefresh = millis(); }
}

// one place that turns controller signals into motion / sound / light
void applyControls(int lt, int rt, bool a, bool y, bool x, bool b, int rStickX, int lStickY){
  driveDome(rt - lt);
  driveArm(a, y);
  playIfX(x);
  updateLED(b, rStickX, lStickY);
}

void setup(){
  Serial.begin(115200);
  delay(300);
  pinMode(MODE_PIN, INPUT_PULLUP);
  individualMode = (digitalRead(MODE_PIN) == LOW);

  pinMode(DOME_PWM_PIN,OUTPUT); pinMode(DOME_DIR_PIN,OUTPUT);
  pinMode(ARM_PWM_PIN, OUTPUT); pinMode(ARM_DIR_PIN, OUTPUT);
  stopAll();

  strip.begin(); showLED();   // LED on at the current color

  dfSerial.begin(9600, SERIAL_8N1, 14, 12);      // DFPlayer RX=14, TX=12
  if(player.begin(dfSerial)){ playerOk = true; player.volume(25); Serial.println("DFPlayer ready."); }
  else Serial.println("DFPlayer NOT found - sound off (rest still works).");

  if(individualMode){
    Serial.println("MODE: INDIVIDUAL - own Bluetooth. Pair the Xbox to THIS board.");
    BP32.setup(&onConnected, &onDisconnected);
    BP32.forgetBluetoothKeys();
    BP32.enableNewBluetoothConnections(true);
  } else {
    Serial.println("MODE: RELAY - full controller state from the base over nRF24.");
    SPI.begin(26, 25, 32, 13);
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
      applyControls(myController->brake(), myController->throttle(),
                    myController->a(), myController->y(), myController->x(),
                    myController->b(), myController->axisRX(), myController->axisY());
    } else { stopAll(); updateLED(false, 0, 0); }
  } else {
    if(radio.available()){ radio.read(&pkt, sizeof(pkt)); lastRxMs = millis(); }
    bool live = (millis() - lastRxMs <= FAILSAFE_MS) && pkt.active;
    if(live) applyControls(pkt.lt, pkt.rt, pkt.a, pkt.y, pkt.x, pkt.b, pkt.rx, pkt.ly);
    else { stopAll(); updateLED(false, 0, 0); }
  }
  delay(5);
}