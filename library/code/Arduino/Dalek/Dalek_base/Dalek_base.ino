#include <Bluepad32.h>

// --- HARDWARE PINS (Nano ESP32 / S3 Dev Module) ---
const int L_MOTOR_PINS[] = {5, 6}; // D2, D3
const int R_MOTOR_PINS[] = {7, 9}; // D4, D6

const int L_CHANNEL = 0;
const int R_CHANNEL = 1;

const int PWM_FREQ = 50; 
const int PWM_RES = 14; 

// 14-bit (0-16383) Duty Cycles
const int PWM_MIN = 819; 
const int PWM_NEUTRAL = 1229;
const int PWM_MAX = 1638;

const int DEADBAND = 50;

// --- SETTINGS ---
// If forward/backward is wrong, change this to 'true'
bool REVERSE_THROTTLE = false; 

// --- PERSISTENT STATE ---
ControllerPtr myController;
float globalSpeedLimit = 0.5; // Starts at 50%
uint8_t lastDpadState = 0;    // To detect single button presses

void onConnected(ControllerPtr ctl) {
    if (!myController) {
        Serial.println("\n>>> SYSTEM ONLINE - DPAD CONTROL ENABLED <<<");
        myController = ctl;
    }
}

void setup() {
    Serial.begin(115200);
    delay(1000);
    
    ledcSetup(L_CHANNEL, PWM_FREQ, PWM_RES);
    ledcSetup(R_CHANNEL, PWM_FREQ, PWM_RES);

    ledcAttachPin(L_MOTOR_PINS[0], L_CHANNEL);
    ledcAttachPin(L_MOTOR_PINS[1], L_CHANNEL);
    ledcAttachPin(R_MOTOR_PINS[0], R_CHANNEL);
    ledcAttachPin(R_MOTOR_PINS[1], R_CHANNEL);

    ledcWrite(L_CHANNEL, PWM_NEUTRAL);
    ledcWrite(R_CHANNEL, PWM_NEUTRAL);

    BP32.setup(&onConnected, NULL);
}

void loop() {
    BP32.update();

    if (myController && myController->isConnected()) {
        
        // --- 1. DPAD SPEED ADJUSTMENT (10% STEPS) ---
        uint8_t currentDpad = myController->dpad();
        
        // Check if Up was just pressed (0x01 is DPAD_UP)
        if ((currentDpad & 0x01) && !(lastDpadState & 0x01)) {
            globalSpeedLimit += 0.10;
            Serial.printf("Speed Limit UP: %d%%\n", (int)(globalSpeedLimit * 100));
        }
        // Check if Down was just pressed (0x02 is DPAD_DOWN)
        if ((currentDpad & 0x02) && !(lastDpadState & 0x02)) {
            globalSpeedLimit -= 0.10;
            Serial.printf("Speed Limit DOWN: %d%%\n", (int)(globalSpeedLimit * 100));
        }
        lastDpadState = currentDpad;

        // Keep limit between 10% and 100%
        globalSpeedLimit = constrain(globalSpeedLimit, 0.1, 1.0);

        // --- 2. GET JOYSTICK DATA ---
        int throttle = myController->axisY(); 
        int turn = myController->axisRX();

        // Apply the manual reverse toggle if needed
        if (REVERSE_THROTTLE) throttle = -throttle;

        if (abs(throttle) < DEADBAND) throttle = 0;
        if (abs(turn) < DEADBAND) turn = 0;

        // --- 3. APPLY LIMIT ---
        throttle = (int)(throttle * globalSpeedLimit);
        turn = (int)(turn * globalSpeedLimit);

        // --- 4. ARCADE MATH ---
        int leftPower = throttle + turn;
        int rightPower = throttle - turn;

        // --- 5. PWM MAPPING ---
        // Right side still inverted (MAX, MIN) because motors are mirrored
        int leftDuty  = map(constrain(leftPower, -512, 512), -512, 512, PWM_MIN, PWM_MAX);
        int rightDuty = map(constrain(rightPower, -512, 512), -512, 512, PWM_MAX, PWM_MIN);

        ledcWrite(L_CHANNEL, leftDuty);
        ledcWrite(R_CHANNEL, rightDuty);

        // --- 6. DEBUG ---
        if (millis() % 500 == 0) {
            Serial.printf("Limit: %d%% | L:%d R:%d\n", (int)(globalSpeedLimit * 100), leftDuty, rightDuty);
        }

    } else {
        ledcWrite(L_CHANNEL, PWM_NEUTRAL);
        ledcWrite(R_CHANNEL, PWM_NEUTRAL);
    }
    delay(10);
}