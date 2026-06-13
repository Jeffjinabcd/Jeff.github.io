#include <Bluepad32.h>

// --- Corrected Motor Pins for ESP-WROOM-32 ---
const int DOME_PWM_PIN = 25; // Speed
const int DOME_DIR_PIN = 33; // Direction
const int ARM_PWM_PIN = 27;  // Speed
const int ARM_DIR_PIN = 26;  // Direction

ControllerPtr myControllers[BP32_MAX_GAMEPADS];

// --- Bluepad32 Callbacks ---
void onConnectedController(ControllerPtr ctl) {
  for (int i = 0; i < BP32_MAX_GAMEPADS; i++) {
    if (myControllers[i] == nullptr) {
      Serial.printf("CALLBACK: Controller is connected, index=%d\n", i);
      myControllers[i] = ctl;
      break;
    }
  }
}

void onDisconnectedController(ControllerPtr ctl) {
  for (int i = 0; i < BP32_MAX_GAMEPADS; i++) {
    if (myControllers[i] == ctl) {
      Serial.printf("CALLBACK: Controller disconnected, index=%d\n", i);
      myControllers[i] = nullptr;
      break;
    }
  }
}

void setup() {
  Serial.begin(115200);

  // Initialize Motor Pins as Outputs
  pinMode(DOME_PWM_PIN, OUTPUT);
  pinMode(DOME_DIR_PIN, OUTPUT);
  pinMode(ARM_PWM_PIN, OUTPUT);
  pinMode(ARM_DIR_PIN, OUTPUT);

  // Safety: Ensure motors are completely stopped on boot
  analogWrite(DOME_PWM_PIN, 0);
  analogWrite(ARM_PWM_PIN, 0);

  // Initialize Bluetooth Controller Library
  BP32.setup(&onConnectedController, &onDisconnectedController);
  BP32.forgetBluetoothKeys(); 
  Serial.println("System Ready. Waiting for Xbox Controller connection...");
}

void loop() {
  // Update controller data
  BP32.update();

  // If Controller 0 is connected, read its inputs
  if (myControllers[0] && myControllers[0]->isConnected()) {
    
    // ==========================================
    // 1. DOME CONTROL (Left Trigger & Right Trigger)
    // ==========================================
    // Values range from 0 to 1023
    int leftTrigger = myControllers[0]->brake();    // LT
    int rightTrigger = myControllers[0]->throttle(); // RT

    // Subtract LT from RT to get a net turn value (-1023 to 1023)
    // If you pull both triggers equally, it cancels out to 0 (Stops)
    int domeCommand = rightTrigger - leftTrigger;

    if (abs(domeCommand) > 20) { // Tiny deadzone just in case triggers rest at 1 or 2
      
      // Set Direction
      if (domeCommand > 0) {
        digitalWrite(DOME_DIR_PIN, HIGH); // RT pulled = Spin Right
      } else {
        digitalWrite(DOME_DIR_PIN, LOW);  // LT pulled = Spin Left
      }

      // Convert the 0-1023 trigger pull into a 0-255 PWM speed
      int speed = map(abs(domeCommand), 0, 1023, 0, 255);
      analogWrite(DOME_PWM_PIN, speed);

    } else {
      analogWrite(DOME_PWM_PIN, 0); // Stop if triggers are released
    }


    // ==========================================
    // 2. ARM CONTROL (Y and A Buttons)
    // ==========================================
    bool btnY = myControllers[0]->y(); // True if Y is pressed
    bool btnA = myControllers[0]->a(); // True if A is pressed

    if (btnY) {
      // Y Pressed: Extend Arm
      digitalWrite(ARM_DIR_PIN, HIGH);
      analogWrite(ARM_PWM_PIN, 255); // Full power
      
    } else if (btnA) {
      // A Pressed: Retract Arm
      digitalWrite(ARM_DIR_PIN, LOW);
      analogWrite(ARM_PWM_PIN, 255); // Full power
      
    } else {
      // Neither pressed: Stop Arm
      analogWrite(ARM_PWM_PIN, 0);
    }

  } else {
    // FAILSAFE: Stop motors if controller disconnects
    analogWrite(DOME_PWM_PIN, 0);
    analogWrite(ARM_PWM_PIN, 0);
  }

  delay(10); 
}