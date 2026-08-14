/*
 * Nano ESP32 (S3) — HOST MONITOR (transmitter)
 * Reads the Xbox controller, prints its inputs, and sends them to the WROOM
 * over nRF24. No motors. Use with WROOM_Receive_Monitor to prove the chain.
 *
 * Board: "Arduino Nano ESP32"
 * nRF24:  MOSI->D8(17) IRQ->D9(18) MISO->D10(21) SCK->D11(38) CE->D12(47)
 *         CSN->D13(48)  VCC->3V3  GND->GND  (+10uF cap)
 */
#include <Bluepad32.h>
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

RF24 radio(47, 48);                 // CE=47, CSN=48
const byte address[6] = "DALEK";

// MUST match the WROOM sketch (packed = identical byte layout on both boards)
struct __attribute__((packed)) CtrlPacket {
  bool     connected;
  int16_t  lx, ly, rx, ry;
  int16_t  lt, rt;
  uint8_t  dpad;
  bool     a, b, x, y;
};
CtrlPacket tx;

ControllerPtr myController;
unsigned long lastPrint = 0;

void onConnected(ControllerPtr ctl){ if(!myController){ Serial.println(">>> XBOX ONLINE"); myController=ctl; } }
void onDisconnected(ControllerPtr ctl){ if(myController==ctl){ myController=nullptr; Serial.println(">>> XBOX OFFLINE"); } }

void setup(){
  Serial.begin(115200);
  delay(300);
  SPI.begin(38, 21, 17, 48);        // SCK=38, MISO=21, MOSI=17, SS=48
  if(!radio.begin(&SPI)) Serial.println("nRF24 NOT found - check wiring / 3.3V / cap");
  radio.setPALevel(RF24_PA_LOW);
  radio.setChannel(100);
  radio.openWritingPipe(address);
  radio.stopListening();

  BP32.setup(&onConnected, &onDisconnected);
  BP32.forgetBluetoothKeys();          // clear stale bond so the Xbox re-pairs
  BP32.enableNewBluetoothConnections(true);
  Serial.println("NANO host ready: reading Xbox + sending over nRF24.");
}

void loop(){
  BP32.update();

  if(myController && myController->isConnected()){
    tx.connected = true;
    tx.lx = myController->axisX();  tx.ly = myController->axisY();
    tx.rx = myController->axisRX(); tx.ry = myController->axisRY();
    tx.lt = myController->brake();  tx.rt = myController->throttle();
    tx.dpad = myController->dpad();
    tx.a = myController->a(); tx.b = myController->b();
    tx.x = myController->x(); tx.y = myController->y();
  } else {
    tx.connected = false;
    tx.lx=tx.ly=tx.rx=tx.ry=tx.lt=tx.rt=0; tx.dpad=0; tx.a=tx.b=tx.x=tx.y=0;
  }

  bool ok = radio.write(&tx, sizeof(tx));

  if(millis() - lastPrint >= 150){
    Serial.printf("SENT %s | L(%5d,%5d) R(%5d,%5d) LT:%4d RT:%4d dpad:0x%02x A:%d B:%d X:%d Y:%d | radio:%s\n",
      tx.connected?"CTRL":"----", tx.lx,tx.ly,tx.rx,tx.ry,tx.lt,tx.rt,tx.dpad,
      tx.a,tx.b,tx.x,tx.y, ok?"OK":"FAIL");
    lastPrint = millis();
  }
  delay(20);
}