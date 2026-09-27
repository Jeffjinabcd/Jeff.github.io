/*
 * WROOM-32 — DALEK TOP: dome + arm + sound + LED   (v2 control layout)
 * ------------------------------------------------------------------
 * Mode at boot by D23:  GND = individual (own BT),  open = relay (nRF24).
 *
 * CONTROL MAP
 *   RT / LT ........ dome rotate
 *   X / Y .......... arm extend / retract   (Y is re-used in tuning, see below)
 *   A .............. PRESET: play 0100.mp3 + 100% rainbow flash. Press A again = stop + normal.
 *   B .............. EXTERMINATE: play 0001.mp3 + loudness-synced flash.
 *   View (two-rect)  toggle COLOR-TUNING (base wheels held while tuning; quick 50% flash on toggle)
 *        in tuning:  R-stick L/R = hue,  R-stick up/down = white<->solid,  L-stick = brightness  (all stay put)
 *                    Y (tap) = cycle LED mode (solid / slow / med / fast flash)
 *                    Y (hold 3s) = back to SOLID
 *   Menu (3-line) .. play the currently-selected audio track
 *   Dpad L / R ..... select audio track (0001 .. NUM_TRACKS)
 *   Share/export ... toggle DEMO: actuator full up/down + dome ~20deg swing, gentle (50%). Base keeps driving.
 *   L1 / R1 ........ volume down / up (DFPlayer 0..30)
 *
 * Board: "ESP32 Dev Module". Libraries: Bluepad32, RF24, DFRobotDFPlayerMini, Adafruit NeoPixel
 * nRF24:   MOSI->32 IRQ->33 MISO->25 SCK->26 CE->27 CSN->13  (+10uF cap, 3V3)
 * MDD10A:  dome PWM->5, dome DIR->18, arm PWM->19, arm DIR->21
 * DFPlayer: RX->1k->GPIO12(TX), TX->GPIO14(RX), 5V, GND, DAC->amp AUX
 * LED:     data GPIO15/2/4 -> 74HCT125 (220R each) -> strip DIN; 12V/GND from battery, common gnd
 * Encoder: Ch A(yellow)->GPIO22, Ch B(brown)->GPIO34(+ext 10k to 3V3), V+(orange)->3V3, gnd(green)->GND
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
  uint8_t  misc;                       // View/Menu/Share bitmask
  bool     demoMode;                   // from base: dome demo
  bool     tuning;                     // from base: color tuning
};
DalekPacket pkt;
unsigned long lastRxMs = 0;
const unsigned long FAILSAFE_MS = 300;

// misc-button bits (Bluepad32 miscButtons()). If a tiny button doesn't respond,
// watch the "misc=0x.." serial print to see which bit it sets, then fix these.
const uint8_t MB_VIEW  = 0x02;   // two-rectangles (Select / Back / View)
const uint8_t MB_MENU  = 0x04;   // three-lines (Start / Menu)
const uint8_t MB_SHARE = 0x08;   // upload / Share (Capture)

// ===== MDD10A =====
const int DOME_PWM_PIN = 5, DOME_DIR_PIN = 18, ARM_PWM_PIN = 19, ARM_DIR_PIN = 21;
const int DOME_DEADZONE = 20;

// ===== DFPlayer =====
HardwareSerial dfSerial(2);
DFRobotDFPlayerMini player;
bool playerOk = false;
int  volume   = 25;              // DFPlayer 0..30, adjusted live by L1 / R1
int  trackIndex = 1;                 // selected track (dpad L/R), played by MENU
const int NUM_TRACKS = 10;           // 0001..0010 — raise as you add more
const int EXTERMINATE_TRACK = 1;     // B -> 0001.mp3
const int PRESET_A_TRACK    = 100;   // A -> 0100.mp3
const int PREVIEW_MS = 200;          // preview length (ms) when scrubbing tracks with the dpad
unsigned long previewStopAt = 0;

// ===== LED strips (3, same color) =====
#define NUM_LEDS 20
#define COLOR_ORDER (NEO_RGB + NEO_KHZ800)
Adafruit_NeoPixel strip1(NUM_LEDS, 15, COLOR_ORDER);
Adafruit_NeoPixel strip2(NUM_LEDS,  2, COLOR_ORDER);
Adafruit_NeoPixel strip3(NUM_LEDS,  4, COLOR_ORDER);
Adafruit_NeoPixel* strips[3] = { &strip1, &strip2, &strip3 };
uint16_t ledHue = 0;
uint8_t  ledSat = 255;
int      ledBright = 150;
const int STICK_DEADZONE = 80;
uint32_t g_lastShown = 0xFF123456;
// White balance: these strips run blue-ish, so trim blue (and a little green) to neutralize white.
// White STILL blue? lower WB_B.  Too warm/orange? raise WB_B back toward 255.
const uint8_t WB_R = 255, WB_G = 230, WB_B = 175;

enum LedMode { LM_SOLID, LM_SLOW, LM_MED, LM_FAST, LM_COUNT };
int  ledMode = LM_SOLID;
bool tuning  = false;                 // color-tuning toggle (View)
unsigned long togFlashUntil = 0;      // quick 50% flash overlay window

// ===== presets / effects =====
bool presetA = false;                 // 0100 + rainbow flash (A)
bool flashActive = false;             // exterminate envelope flash (B)
unsigned long flashStart = 0;
const uint8_t FLASH_R = 255, FLASH_G = 255, FLASH_B = 255;
// loudness envelope of exterminate.mp3, 25 ms/frame (0 silent, 255 loud)
const int ENV_FRAME_MS = 25;
const uint8_t ENV[] = {
    0,  0,230,203,187,214,227,  0,  0,215,  0,  0,  0,  0,  0,  0,
  203,224,  0,  0,  0,  0,  0,  0,247,251,200,196,236,206,  0,192,
  208,190,  0,211,  0,  0,  0,188,239,  0,190,233,201,  0,  0,209,
  202,181,219,237,181,  0,196,  0,  0,190,227,202,  0,192,  0,  0,
    0,  0,  0,  0,  0,190,255,197,  0,  0
};
const int ENV_LEN = sizeof(ENV);

// ===== dome encoder (quadrature) =====
const int ENC_A_PIN = 22;   // yellow (Ch A) -> GPIO22 (internal pull-up)
const int ENC_B_PIN = 34;   // brown  (Ch B) -> GPIO34 (input-only: add external 10k to 3V3)
// orange (sensor V+) -> 3V3,  green (gnd) -> GND
const long COUNTS_PER_REV = 2442;                          // datasheet, full quadrature
volatile long domeCount = 0;
volatile uint8_t encState = 0;

// ===== demo pattern (Share) =====
bool demo = false;
bool demoGoHigh = true;
long demoCenter = 0;
unsigned long demoSwingStart = 0;
const int DEMO_SPEED = 128;                                // ~50%, gentle
const long DOME_SWING_COUNTS = COUNTS_PER_REV * 10 / 360;  // ~68 counts = 10 deg each side of center (20 deg peak-to-peak)
const long DOME_TOL = 8;                                   // "close enough" window (counts)
const unsigned long SWING_TIMEOUT_MS = 1500;               // safety: reverse if a swing stalls
const unsigned long ACT_HALF_MS = 2500;                    // full actuator stroke time at 50% — TUNE

// ===== Bluepad32 (individual mode) =====
ControllerPtr myController;
void onConnected(ControllerPtr c){ if(!myController){ Serial.println(">>> XBOX ONLINE"); myController=c; } }
void onDisconnected(ControllerPtr c){ if(myController==c){ myController=nullptr; Serial.println(">>> XBOX OFFLINE"); } }

// ---------- LED helpers ----------
uint32_t applyWB(uint32_t c){                    // warm the strip's blue-ish white toward neutral
  uint8_t r = (((c>>16)&0xFF)*WB_R)/255;
  uint8_t g = (((c>>8)&0xFF)*WB_G)/255;
  uint8_t b = (( c     &0xFF)*WB_B)/255;
  return ((uint32_t)r<<16)|((uint32_t)g<<8)|b;
}
void fillIfChanged(uint32_t c){
  c = applyWB(c);
  if(c == g_lastShown) return;
  for(int s=0;s<3;s++){ for(int i=0;i<NUM_LEDS;i++) strips[s]->setPixelColor(i,c); strips[s]->show(); }
  g_lastShown = c;
}
bool blinkPhase(unsigned long periodMs){ return (millis() % periodMs) < (periodMs/2); }

void renderNormal(){
  uint32_t base = strip1.gamma32(strip1.ColorHSV(ledHue, ledSat, ledBright));
  bool on = true;
  switch(ledMode){
    case LM_SOLID: on = true;              break;
    case LM_SLOW:  on = blinkPhase(1200);  break;
    case LM_MED:   on = blinkPhase(500);   break;
    case LM_FAST:  on = blinkPhase(160);   break;
  }
  fillIfChanged(on ? base : 0);
}
void renderPresetA(){                       // 0100: rainbow hue-cycle; blink follows the Y-selected mode
  static uint16_t h = 0; static unsigned long t = 0;
  if(millis()-t > 20){ h += 400; t = millis(); }      // cycle hue
  bool on = true;
  switch(ledMode){                                    // same pattern Y cycles: solid = no blink
    case LM_SOLID: on = true;             break;
    case LM_SLOW:  on = blinkPhase(1200); break;
    case LM_MED:   on = blinkPhase(500);  break;
    case LM_FAST:  on = blinkPhase(160);  break;
  }
  uint32_t base = strip1.gamma32(strip1.ColorHSV(h, 255, 255));
  fillIfChanged(on ? base : 0);
}
void renderExterminate(){                   // brightness follows the clip loudness
  static int lastIdx = -1;
  int idx = (millis() - flashStart) / ENV_FRAME_MS;
  if(idx >= ENV_LEN){ flashActive=false; lastIdx=-1; g_lastShown=0xFF123456; renderNormal(); return; }
  if(idx == lastIdx) return;
  lastIdx = idx;
  uint8_t b = ENV[idx];
  fillIfChanged(strip1.Color((FLASH_R*b)/255,(FLASH_G*b)/255,(FLASH_B*b)/255));
}
void renderLEDs(){
  if(millis() < togFlashUntil){ fillIfChanged(strip1.Color(128,128,128)); return; }  // quick 50% flash
  if(presetA){ renderPresetA(); return; }
  if(flashActive){ renderExterminate(); return; }
  renderNormal();
}

// ---------- motion helpers ----------
void stopAll(){ analogWrite(DOME_PWM_PIN,0); analogWrite(ARM_PWM_PIN,0); }
void driveDome(int cmd){
  if(abs(cmd) > DOME_DEADZONE){
    digitalWrite(DOME_DIR_PIN, cmd > 0 ? HIGH : LOW);
    analogWrite(DOME_PWM_PIN, map(abs(cmd),0,1023,0,255));
  } else analogWrite(DOME_PWM_PIN, 0);
}
void driveArm(bool ext, bool ret){          // X extend, Y retract
  if(ext){ digitalWrite(ARM_DIR_PIN,HIGH); analogWrite(ARM_PWM_PIN,255); }
  else if(ret){ digitalWrite(ARM_DIR_PIN,LOW); analogWrite(ARM_PWM_PIN,255); }
  else analogWrite(ARM_PWM_PIN, 0);
}
void IRAM_ATTR encISR(){                     // quadrature decode (x4) on A or B change
  uint8_t s = (digitalRead(ENC_A_PIN)<<1) | digitalRead(ENC_B_PIN);
  static const int8_t tbl[16] = {0,-1,1,0, 1,0,0,-1, -1,0,0,1, 0,1,-1,0};
  domeCount += tbl[(encState<<2)|s];
  encState = s;
}
void runDemo(){                             // gentle: actuator up/down + encoder-accurate dome swing
  // dome: swing +/-20deg around the position captured when demo started
  long target = demoGoHigh ? (demoCenter + DOME_SWING_COUNTS) : (demoCenter - DOME_SWING_COUNTS);
  long err = target - domeCount;
  long aerr = (err < 0) ? -err : err;
  bool stalled = (millis() - demoSwingStart > SWING_TIMEOUT_MS);   // safety vs wrong-direction runaway
  if(aerr < DOME_TOL || stalled){
    demoGoHigh = !demoGoHigh;
    demoSwingStart = millis();
    analogWrite(DOME_PWM_PIN, 0);
  } else {
    digitalWrite(DOME_DIR_PIN, err > 0 ? HIGH : LOW);   // flip this test if the dome swings the wrong way
    analogWrite(DOME_PWM_PIN, DEMO_SPEED);
  }
  // actuator: full up/down; its built-in limit switches stop it at each end
  static unsigned long actT=0; static bool actDir=false;
  if(millis()-actT > ACT_HALF_MS){ actDir=!actDir; actT=millis(); }
  digitalWrite(ARM_DIR_PIN, actDir?HIGH:LOW);
  analogWrite(ARM_PWM_PIN, DEMO_SPEED);
}

// ---------- tuning stick input ----------
void updateTuning(int rx, int ry, int ly){
  static unsigned long t=0;
  if(millis()-t < 20) return; t = millis();
  if(abs(rx) > STICK_DEADZONE) ledHue += (int16_t)rx;                         // L/R  = hue (stays put)
  if(abs(ry) > STICK_DEADZONE) ledSat  = constrain(ledSat + ry/100, 0, 255);  // UP = whiter, DOWN = solid color
  if(abs(ly) > STICK_DEADZONE) ledBright = constrain(ledBright - (ly/150), 10, 255);  // left stick = brightness
}

void previewTrack(){                        // scrub: play the first PREVIEW_MS of the new track
  Serial.printf("track %d\n", trackIndex);
  if(playerOk){ player.playMp3Folder(trackIndex); previewStopAt = millis() + PREVIEW_MS; }
}

// ---------- main control processing (operates on global pkt) ----------
void processControls(){
  bool menu  = pkt.misc & MB_MENU;

  static uint8_t lastMisc = 0;             // debug: reveal which bit each tiny button sets
  if(pkt.misc != lastMisc){ Serial.printf("misc=0x%02x\n", pkt.misc); lastMisc = pkt.misc; }

  // TUNING is owned by the base (in the packet) — follow it, flash on change
  if(pkt.tuning != tuning){ togFlashUntil = millis()+150; }
  tuning = pkt.tuning;

  // DEMO is owned by the base (sent in the packet) — just follow it, never toggle locally
  if(pkt.demoMode && !demo){ demoCenter = domeCount; demoGoHigh = false; demoSwingStart = millis(); }  // rising edge
  demo = pkt.demoMode;
  static bool lastDemoDbg=false;
  if(demo!=lastDemoDbg){ Serial.println(demo?"DOME demo ON":"DOME demo OFF"); lastDemoDbg=demo; }

  // Menu -> play the FULL selected track (cancels any preview)
  static bool lastMenu=false;
  if(menu && !lastMenu){ if(playerOk) player.playMp3Folder(trackIndex); previewStopAt=0; }
  lastMenu = menu;

  // Dpad L/R -> choose track, and preview the first 0.2s of it
  static uint8_t lastDpad=0;
  bool rN = pkt.dpad & 0x04, rP = lastDpad & 0x04;   // right
  bool lN = pkt.dpad & 0x08, lP = lastDpad & 0x08;   // left
  if(rN && !rP){ trackIndex++; if(trackIndex>NUM_TRACKS) trackIndex=1; previewTrack(); }
  if(lN && !lP){ trackIndex--; if(trackIndex<1) trackIndex=NUM_TRACKS; previewTrack(); }
  lastDpad = pkt.dpad;

  // end the preview after PREVIEW_MS
  if(previewStopAt && millis() >= previewStopAt){ if(playerOk) player.stop(); previewStopAt=0; }

  // A -> 0100 preset (toggle); press again while playing = stop + normal
  static bool lastA=false;
  if(pkt.a && !lastA){
    if(!presetA){ presetA=true; flashActive=false; if(playerOk) player.playMp3Folder(PRESET_A_TRACK); Serial.println("A -> preset ON (0100 + rainbow)"); }
    else       { presetA=false; if(playerOk) player.stop(); Serial.println("A -> preset OFF"); }
  }
  lastA = pkt.a;   // A is a clean toggle: press once = rainbow + 0100, press again = stop

  // B -> exterminate preset
  static bool lastB=false;
  if(pkt.b && !lastB){
    if(playerOk) player.playMp3Folder(EXTERMINATE_TRACK);
    flashActive=true; flashStart=millis(); presetA=false;
  }
  lastB = pkt.b;

  // L1 / R1 -> volume down / up (DFPlayer 0..30)
  static bool lastL1=false, lastR1=false;
  if(pkt.r1 && !lastR1){ volume = constrain(volume+2,0,30); if(playerOk) player.volume(volume); Serial.printf("vol %d\n",volume); }
  if(pkt.l1 && !lastL1){ volume = constrain(volume-2,0,30); if(playerOk) player.volume(volume); Serial.printf("vol %d\n",volume); }
  lastR1=pkt.r1; lastL1=pkt.l1;

  // Y -> (in tuning) tap = cycle LED mode, hold 3s = solid.  (outside tuning it's the arm, handled below)
  static bool lastY=false; static unsigned long yDown=0; static bool yHeld=false;
  bool yNow = pkt.y;
  if(tuning){
    if(yNow && !lastY){ yDown=millis(); yHeld=false; }
    if(yNow && !yHeld && (millis()-yDown > 3000)){ ledMode=LM_SOLID; yHeld=true; togFlashUntil=millis()+150; }
    if(!yNow && lastY && !yHeld){ ledMode=(ledMode+1)%LM_COUNT; }
  }
  lastY = yNow;

  // ---- motion ----
  if(demo){ runDemo(); }
  else{
    driveDome(pkt.rt - pkt.lt);
    if(!tuning) driveArm(pkt.x, pkt.y);    // X extend, Y retract
    else        analogWrite(ARM_PWM_PIN, 0);
  }

  // ---- tuning stick input ----
  if(tuning) updateTuning(pkt.rx, pkt.ry, pkt.ly);
}

void setup(){
  Serial.begin(115200);
  delay(300);
  pinMode(MODE_PIN, INPUT_PULLUP);
  individualMode = (digitalRead(MODE_PIN) == LOW);

  pinMode(DOME_PWM_PIN,OUTPUT); pinMode(DOME_DIR_PIN,OUTPUT);
  pinMode(ARM_PWM_PIN, OUTPUT); pinMode(ARM_DIR_PIN, OUTPUT);
  stopAll();

  pinMode(ENC_A_PIN, INPUT_PULLUP);   // Ch A
  pinMode(ENC_B_PIN, INPUT);          // Ch B on GPIO34: needs external 10k pull-up to 3V3
  encState = (digitalRead(ENC_A_PIN)<<1) | digitalRead(ENC_B_PIN);
  attachInterrupt(digitalPinToInterrupt(ENC_A_PIN), encISR, CHANGE);
  attachInterrupt(digitalPinToInterrupt(ENC_B_PIN), encISR, CHANGE);

  strip1.begin(); strip2.begin(); strip3.begin();
  renderLEDs();

  dfSerial.begin(9600, SERIAL_8N1, 14, 12);
  if(player.begin(dfSerial)){ playerOk=true; player.volume(volume); Serial.println("DFPlayer ready."); }
  else Serial.println("DFPlayer NOT found - sound off (rest still works).");

  if(individualMode){
    Serial.println("MODE: INDIVIDUAL - pair the Xbox to THIS board.");
    BP32.setup(&onConnected, &onDisconnected);
    BP32.forgetBluetoothKeys();
    BP32.enableNewBluetoothConnections(true);
  } else {
    Serial.println("MODE: RELAY - full controller state from base over nRF24.");
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
      pkt.active=true;
      pkt.lx=myController->axisX();  pkt.ly=myController->axisY();
      pkt.rx=myController->axisRX(); pkt.ry=myController->axisRY();
      pkt.lt=myController->brake();  pkt.rt=myController->throttle();
      pkt.dpad=myController->dpad();
      pkt.a=myController->a(); pkt.b=myController->b();
      pkt.x=myController->x(); pkt.y=myController->y();
      pkt.l1=myController->l1(); pkt.r1=myController->r1();
      pkt.misc=myController->miscButtons();
      static bool lastShareI=false, demoI=false;    // individual: export toggles demo
      bool shareI = pkt.misc & MB_SHARE;
      if(shareI && !lastShareI) demoI=!demoI;
      lastShareI=shareI;
      pkt.demoMode=demoI;
      static bool lastViewI=false, tuneI=false;     // individual: View toggles tuning
      bool viewI = pkt.misc & MB_VIEW;
      if(viewI && !lastViewI) tuneI=!tuneI;
      lastViewI=viewI;
      pkt.tuning=tuneI;
      lastRxMs=millis();
    } else pkt.active=false;
  } else {
    if(radio.available()){ radio.read(&pkt, sizeof(pkt)); lastRxMs=millis(); }
  }

  bool live = (millis() - lastRxMs <= FAILSAFE_MS) && pkt.active;
  if(live) processControls();
  else { stopAll(); demo=false; }

  renderLEDs();
  delay(5);
}