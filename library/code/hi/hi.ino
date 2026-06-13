// Using RAW GPIO numbers for the Nano ESP32 when compiled as a generic S3
const int in1Pin = 5; // This is the physical D2 pin
const int in2Pin = 6; // This is the physical D3 pin

void setup() {
  Serial.begin(115200);
  
  // Wait a moment for the USB Serial connection to establish
  delay(2000); 
  
  // Set the control pins as outputs
  pinMode(in1Pin, OUTPUT);
  pinMode(in2Pin, OUTPUT);
  
  // Ensure the coil starts OFF
  digitalWrite(in1Pin, LOW);
  digitalWrite(in2Pin, LOW);
  
  Serial.println("Starting continuous 5-second cycle test with correct GPIOs...");
}

void loop() {
  // 1. Turn the coil ON
  Serial.println("Magnet is ON! (Drawing power)");
  digitalWrite(in1Pin, HIGH);
  digitalWrite(in2Pin, LOW);
  
  // Wait 5 seconds
  delay(5000); 
  
  // 2. Turn the coil OFF
  Serial.println("Magnet is OFF! (Cooling down)");
  digitalWrite(in1Pin, LOW);
  digitalWrite(in2Pin, LOW);
  
  // Wait 5 seconds before repeating
  delay(5000); 
}