/*
 * WROOM-32 — RECEIVE MONITOR (receiver)
 * Receives the controller data from the Nano over nRF24 and prints it.
 * No motors. Pair with Nano_Host_Monitor.
 *
 * Board: "ESP32 Dev Module"
 * nRF24:  MOSI->27 IRQ->26 MISO->25 SCK->33 CE->32 CSN->14  VCC->3V3 GND->GND (+10uF cap)
 */
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

RF24 radio(32, 13);                 // CE=32, CSN=14
const byte address[6] = "DALEK";

// MUST match the Nano sketch (packed = identical byte layout on both boards)
struct __attribute__((packed)) CtrlPacket {
  bool     connected;
  int16_t  lx, ly, rx, ry;
  int16_t  lt, rt;
  uint8_t  dpad;
  bool     a, b, x, y;
};
CtrlPacket rx;
unsigned long lastRxMs = 0;

void setup(){
  Serial.begin(115200);
  delay(300);
  SPI.begin(33, 25, 27, 14);        // SCK=33, MISO=25, MOSI=27, SS=14
  if(!radio.begin(&SPI)) Serial.println("nRF24 NOT found - check wiring / 3.3V / cap");
  // Chip-detect: NO = the WROOM can't talk to its nRF24 over SPI (wiring/pins/solder)
  Serial.printf("nRF24 chip connected: %s\n", radio.isChipConnected() ? "YES" : "NO");
  radio.setPALevel(RF24_PA_LOW);
  radio.setChannel(100);
  radio.openReadingPipe(0, address);
  radio.startListening();
  Serial.println("WROOM receiver ready: printing what the Nano sends.");
}

void loop(){
  if(radio.available()){
    radio.read(&rx, sizeof(rx));
    lastRxMs = millis();
    Serial.printf("GOT %s | L(%5d,%5d) R(%5d,%5d) LT:%4d RT:%4d dpad:0x%02x A:%d B:%d X:%d Y:%d\n",
      rx.connected?"CTRL":"----", rx.lx,rx.ly,rx.rx,rx.ry,rx.lt,rx.rt,rx.dpad,
      rx.a,rx.b,rx.x,rx.y);
  }

  if(millis() - lastRxMs > 1000){
    Serial.println("...no packets from Nano (check power / channel / wiring)...");
    lastRxMs = millis();
  }
  delay(5);
}