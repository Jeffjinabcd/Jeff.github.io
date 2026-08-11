/*
 * nRF24 LINK TEST — RECEIVER  (flash to the Nano ESP32)
 * Prints every number it receives from the transmitter.
 *
 * Library: "RF24 by TMRh20" (Library Manager)
 * Wiring:  CE=D9, CSN=D10, SCK=D13, MOSI=D11, MISO=D12, VCC=3V3, GND=GND
 */
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

// Use raw GPIO numbers (this board doesn't define D9/D10).
// Board pin D9 = GPIO18 (CE),  board pin D10 = GPIO21 (CSN).
RF24 radio(18, 21);
const byte address[6] = "DALEK";  // must match the transmitter

void setup(){
  Serial.begin(115200);
  delay(300);
  // ESP32-S3: set the SPI pins explicitly to the ones you wired.
  // SCK=GPIO48(D13), MISO=GPIO47(D12), MOSI=GPIO38(D11), SS=GPIO21(D10)
  SPI.begin(48, 47, 38, 21);
  if(!radio.begin(&SPI)){
    Serial.println("nRF24 NOT found — check wiring & that VCC is 3.3V");
    while(1){ delay(1000); }
  }
  radio.setPALevel(RF24_PA_LOW);
  radio.setChannel(100);          // MUST match the transmitter
  radio.openReadingPipe(0, address);
  radio.startListening();
  Serial.println("Receiver ready.");
}

void loop(){
  if(radio.available()){
    unsigned long n = 0;
    radio.read(&n, sizeof(n));
    Serial.printf("Received: %lu\n", n);
  }
}