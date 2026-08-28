/*
 * WROOM-32 — DALEK TOP: dome + arm + sound + LED
 * ------------------------------------------------------------------
 * Uses the FULL controller state (relay from base, or its own in individual).
 * Mode at boot by D23:  GND = individual (own BT),  open = relay (nRF24).
 *
 * Controls:
 *   RT / LT  = dome rotate
 *   A / Y    = arm extend / retract
 *   X        = play 0001.mp3
 *   LED      = always ON, all 3 strips same color
 *   Hold B   = tune: RIGHT stick = color wheel (center = white, edge = full color),
 *              LEFT stick up/down = brightness
 *
 * Board: "ESP32 Dev Module"
 * Libraries: Bluepad32, RF24, DFRobotDFPlayerMini, Adafruit NeoPixel
 * nRF24:   MOSI->32 IRQ->33 MISO->25 SCK->26 CE->27 CSN->13  (+10uF cap, 3V3)
 * MDD10A:  dome PWM->5, dome DIR->18, arm PWM->19, arm DIR->21
 * DFPlayer: RX->1k->GPIO12(TX), TX->GPIO14(RX), 5V, GND, DAC->amp AUX
 * LED:     3 strips, data GPIO15/2/4 -> 74HCT125 (220R in series each) -> each strip DIN
 *          strip +12V/GND from battery, common ground. All 3 show the SAME color.
 */
#include <Bluepad32.h>
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <HardwareSerial.h>
#include <DFRobotDFPlayerMini.h>
#include <Adafruit_NeoPixel.h>

const int MODE_PIN = 23;              // D23 -> GND = individual (own BT), open = relay
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

// ===== LED strips (3, all show the SAME color) =====
#define NUM_LEDS  22                  // pixels PER strip
// COLOR ORDER: white looking BLUE almost always means this is wrong for your strip.
//   Start NEO_RGB. White still blue/cyan -> try NEO_GRB, then NEO_BRG, then NEO_GBR.
#define COLOR_ORDER (NEO_RGB + NEO_KHZ400)
Adafruit_NeoPixel strip1(NUM_LEDS, 15, COLOR_ORDER);
Adafruit_NeoPixel strip2(NUM_LEDS,  2, COLOR_ORDER);
Adafruit_NeoPixel strip3(NUM_LEDS,  4, COLOR_ORDER);
Adafruit_NeoPixel* strips[3] = { &strip1, &strip2, &strip3 };
uint16_t ledHue    = 0;      // 0..65535 (full color wheel)
uint8_t  ledSat    = 255;    // 0 = white (stick centered), 255 = full color (stick to edge)
int      ledBright = 150;
const int STICK_DEADZONE = 80;

// ---- sound-synced flash: brightness tracks the loudness of exterminate.mp3 ----
bool flashActive = false;
unsigned long flashStart = 0;
// Flash color: pure white; for a colder dome look drop FLASH_R a little.
const uint8_t FLASH_R = 255, FLASH_G = 255, FLASH_B = 255;
uint32_t g_lastShown = 0xFF123456;      // last color pushed to the strips

// Loudness envelope of exterminate.mp3, one value per 25 ms (0 = silent, 255 = loudest).
// Auto-generated from the clip so the lights pulse exactly with the voice.
const int ENV_FRAME_MS = 25;
const uint8_t ENV[] = {
    0,  0,230,203,187,214,227,  0,  0,215,  0,  0,  0,  0,  0,  0,
  203,224,  0,  0,  0,  0,  0,  0,247,251,200,196,236,206,  0,192,
  208,190,  0,211,  0,  0,  0,188,239,  0,190,233,201,  0,  0,209,
  202,181,219,237,181,  0,196,  0,  0,190,227,202,  0,192,  0,  0,
    0,  0,  0,  0,  0,190,255,197,  0,  0
};
const int ENV_LEN = sizeof(ENV);

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
  if(x && !lastX){                              // rising edge on X
    if(playerOk) player.playMp3Folder(1);       // play 0001.mp3
    flashActive = true;                         // lights follow the clip's loudness
    flashStart  = millis();
    Serial.println("X -> sound + light flash");
  }
  lastX = x;
}
void fillStrips(uint32_t c){                     // write one color to all 3 strips
  for(int s=0; s<3; s++){
    for(int i=0;i<NUM_LEDS;i++) strips[s]->setPixelColor(i, c);
    strips[s]->show();
  }
  g_lastShown = c;
}
void showLED(){                                  // normal color, only redraws on change
  uint32_t c = strip1.gamma32(strip1.ColorHSV(ledHue, ledSat, ledBright));
  if(c == g_lastShown) return;                   // unchanged -> don't touch strips (no flashing)
  fillStrips(c);
}
void flashTick(){                                // brightness follows the clip's loudness
  static int lastIdx = -1;
  int idx = (millis() - flashStart) / ENV_FRAME_MS;
  if(idx >= ENV_LEN){                            // clip finished -> back to normal color
    flashActive = false; lastIdx = -1;
    g_lastShown = 0xFF123456; showLED();
    return;
  }
  if(idx == lastIdx) return;                     // same 25 ms frame -> nothing new
  lastIdx = idx;
  uint8_t b = ENV[idx];                          // 0..255 loudness at this instant
  fillStrips(strip1.Color((FLASH_R*b)/255, (FLASH_G*b)/255, (FLASH_B*b)/255));
}
void updateLED(bool b, int rx, int ry, int lStickY){
  static unsigned long lastTune = 0;
  if(b && (millis() - lastTune > 20)){          // tune at most ~50x/sec
    lastTune = millis();
    // RIGHT STICK = color wheel projected onto the stick:
    //   direction = hue,  distance from center = saturation (center = white, edge = full color)
    long mag = (long)sqrtf((float)rx*rx + (float)ry*ry);
    if(mag > STICK_DEADZONE){
      float ang = atan2f((float)rx, (float)-ry);          // 0 rad = stick pushed up; increases clockwise
      if(ang < 0) ang += TWO_PI;
      ledHue = (uint16_t)(ang / TWO_PI * 65535.0f);
      ledSat = (uint8_t)map(constrain((int)mag, STICK_DEADZONE, 450), STICK_DEADZONE, 450, 0, 255);
    } else {
      ledSat = 0;                                          // near center -> white
    }
    // LEFT STICK up/down = brightness
    if(abs(lStickY) > STICK_DEADZONE)
      ledBright = constrain(ledBright - (lStickY / 150), 10, 255);
  }
  // while the clip plays, the lights track its loudness envelope
  if(flashActive){ flashTick(); return; }
  showLED();   // normal: redraws only when the color changed (no flashing)
}

// one place that turns controller signals into motion / sound / light
void applyControls(int lt, int rt, bool a, bool y, bool x, bool b, int rx, int ry, int lStickY){
  driveDome(rt - lt);
  driveArm(a, y);
  playIfX(x);
  updateLED(b, rx, ry, lStickY);
}

void setup(){
  Serial.begin(115200);
  delay(300);
  pinMode(MODE_PIN, INPUT_PULLUP);
  individualMode = (digitalRead(MODE_PIN) == LOW);

  pinMode(DOME_PWM_PIN,OUTPUT); pinMode(DOME_DIR_PIN,OUTPUT);
  pinMode(ARM_PWM_PIN, OUTPUT); pinMode(ARM_DIR_PIN, OUTPUT);
  stopAll();

  strip1.begin(); strip2.begin(); strip3.begin();
  showLED();                  // all 3 strips on at the current color

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
                    myController->b(), myController->axisRX(), myController->axisRY(), myController->axisY());
    } else { stopAll(); updateLED(false, 0, 0, 0); }
  } else {
    if(radio.available()){ radio.read(&pkt, sizeof(pkt)); lastRxMs = millis(); }
    bool live = (millis() - lastRxMs <= FAILSAFE_MS) && pkt.active;
    if(live) applyControls(pkt.lt, pkt.rt, pkt.a, pkt.y, pkt.x, pkt.b, pkt.rx, pkt.ry, pkt.ly);
    else { stopAll(); updateLED(false, 0, 0, 0); }
  }
  delay(5);
}