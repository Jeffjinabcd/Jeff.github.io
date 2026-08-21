/*
 * Addressable WS2811 COB LED TEST — Adafruit NeoPixel version
 * (avoids the FastLED / esp-dsp compile error on the bluepad32 core)
 * Cycles Red/Green/Blue/White, then a Dalek-style flash.
 *
 * Library: "Adafruit NeoPixel" (Library Manager)
 * Wiring:
 *   Strip +12V -> 12V
 *   Strip GND  -> COMMON GND (tied to WROOM GND!)
 *   Strip DIN  -> WROOM GPIO32 through a 330 ohm resistor  (input end / follow arrows)
 */
#include <Adafruit_NeoPixel.h>

#define LED_PIN   22
#define NUM_LEDS  50          // WS2811 = 3 LEDs per "pixel". Set to your real count.

// NEO_KHZ400 = data speed for WS2811 (fixes random/garbled colors).
// If it's still garbled, try NEO_KHZ800. NEO_RGB = color order (try GRB/BRG if a color is wrong).
Adafruit_NeoPixel strip(NUM_LEDS, LED_PIN, NEO_RGB + NEO_KHZ800);

void fillAll(uint8_t r, uint8_t g, uint8_t b){
  for(int i=0;i<NUM_LEDS;i++) strip.setPixelColor(i, strip.Color(r,g,b));
  strip.show();
}

void setup(){
  Serial.begin(115200);
  strip.begin();
  strip.setBrightness(150);   // 0-255
  strip.show();               // all off
  Serial.println("LED test running (NeoPixel)");
}

void loop(){
  fillAll(255,0,0);     delay(1000);   // Red
  fillAll(0,255,0);     delay(1000);   // Green
  fillAll(0,0,255);     delay(1000);   // Blue
  fillAll(255,255,255); delay(1000);   // White

  // Dalek-style stutter flash
  for(int i=0;i<6;i++){ fillAll(255,255,255); delay(80); fillAll(0,0,0); delay(80); }
}