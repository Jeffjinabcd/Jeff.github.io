const int analogPin = 34; 
const int digitalPin = 4; 

void setup() {
  Serial.begin(115200);
  pinMode(digitalPin, INPUT);
  
  // Print a few blank lines to clear space
  Serial.println("\n\n\nStarting TCRT5000 Distance Test...");
}

void loop() {
  int analogValue = analogRead(analogPin);
  int digitalValue = digitalRead(digitalPin);
  
  // --- DISTANCE ESTIMATION ---
  // The ESP32 reads 0-4095. 
  // Based on your data: 50 is very close (approx 2mm), 4095 is far/nothing (approx 25mm+).
  // This map() function converts the analog reading to an estimated millimeter value.
  // Note: We constrain the value first so it doesn't go out of bounds.
  int constrainedValue = constrain(analogValue, 50, 4095);
  
  // Map: (value, fromLow, fromHigh, toLow, toHigh)
  // Because higher analog = further away, we invert the map.
  int distanceMM = map(constrainedValue, 50, 4095, 2, 25); 

  // --- STICK TO BOTTOM PRINTING ---
  // The '\r' at the start returns the cursor to the beginning of the line instead of a new line.
  Serial.print("\rAnalog: ");
  Serial.print(analogValue);
  Serial.print("  |  Est. Distance: ");
  
  if (analogValue > 3800) {
    // If it's too high, it's out of range
    Serial.print("Out of Range (>25mm)   "); 
  } else {
    Serial.print(distanceMM);
    Serial.print(" mm                  "); // Extra spaces at the end erase leftover characters from previous longer lines
  }

  // Delay for readability
  delay(100); 
}