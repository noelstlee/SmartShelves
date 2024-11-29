#include <Wire.h>
#include <SparkFun_APDS9960.h>

// Gesture sensor and motor control pins
#define APDS9960_INT 2 // Interrupt pin for the gesture sensor

// Define pins for stepper motor 1
int coil1_1 = 8;
int coil1_2 = 9;
int coil1_3 = 10;
int coil1_4 = 11;

// Define pins for stepper motor 2
int coil2_1 = 4;
int coil2_2 = 5;
int coil2_3 = 6;
int coil2_4 = 7;

// Step sequence for motors
int stepSequence[4][4] = {
  {1, 0, 0, 0},
  {0, 1, 0, 0},
  {0, 0, 1, 0},
  {0, 0, 0, 1}
};

// Motor control variables
SparkFun_APDS9960 apds = SparkFun_APDS9960();
volatile int isr_flag = 0;
bool motor1_active = false;
bool motor2_active = false;

// Constants
const int stepsPerRevolution = 516; // Steps for 360° for a 2048-step motor
const int delayTime = 3; // Delay for motor speed control

void setup() {
  Serial.begin(9600);

  // Initialize gesture sensor
  pinMode(APDS9960_INT, INPUT);
  attachInterrupt(digitalPinToInterrupt(APDS9960_INT), interruptRoutine, FALLING);

  if (apds.init()) {
    Serial.println("Gesture sensor initialized.");
  } else {
    Serial.println("Gesture sensor initialization failed!");
  }
  
  if (apds.enableGestureSensor(true)) {
    Serial.println("Gesture sensor enabled.");
  } else {
    Serial.println("Gesture sensor enable failed!");
  }

  // Initialize motor pins individually
  pinMode(coil1_1, OUTPUT);
  pinMode(coil1_2, OUTPUT);
  pinMode(coil1_3, OUTPUT);
  pinMode(coil1_4, OUTPUT);

  pinMode(coil2_1, OUTPUT);
  pinMode(coil2_2, OUTPUT);
  pinMode(coil2_3, OUTPUT);
  pinMode(coil2_4, OUTPUT);
}

void loop() {
  if (isr_flag == 1) {
    detachInterrupt(digitalPinToInterrupt(APDS9960_INT));
    handleGesture();
    isr_flag = 0;
    attachInterrupt(digitalPinToInterrupt(APDS9960_INT), interruptRoutine, FALLING);
  }
}

void interruptRoutine() {
  isr_flag = 1;
}

void handleGesture() {
  if (apds.isGestureAvailable()) {
    int gesture = apds.readGesture();
    if (gesture == DIR_UP) {
      Serial.println("Gesture: UP - Activating Motor 1");
      motor1_active = true;
      motor2_active = false;
    } else if (gesture == DIR_DOWN) {
      Serial.println("Gesture: DOWN - Activating Motor 2");
      motor1_active = false;
      motor2_active = true;
    } else if (gesture == DIR_LEFT) {
      Serial.println("Gesture: LEFT - Rotating 120° clockwise");
      if (motor1_active) {
        rotateMotor(true, true); // Motor 1 clockwise
      } else if (motor2_active) {
        rotateMotor(false, true); // Motor 2 clockwise
      }
    } else if (gesture == DIR_RIGHT) {
      Serial.println("Gesture: RIGHT - Rotating 120° counterclockwise");
      if (motor1_active) {
        rotateMotor(true, false); // Motor 1 counterclockwise
      } else if (motor2_active) {
        rotateMotor(false, false); // Motor 2 counterclockwise
      }
    } else {
      Serial.println("Gesture: NONE");
    }
  }
}

// Function to rotate the specified motor in the given direction for 120 degrees
void rotateMotor(bool motor1, bool clockwise) {
  int steps = 165; // Calculate steps for 120 degrees
  for (int i = 0; i < steps; i++) {
    if (clockwise) {
      for (int step = 0; step < 4; step++) {
        setCoils(stepSequence[step], motor1);
        delay(delayTime);
      }
    } else {
      for (int step = 3; step >= 0; step--) {
        setCoils(stepSequence[step], motor1);
        delay(delayTime);
      }
    }
  }
}

// Function to activate coils based on step sequence
void setCoils(int step[4], bool motor1) {
  if (motor1) {
    digitalWrite(coil1_1, step[0]);
    digitalWrite(coil1_2, step[1]);
    digitalWrite(coil1_3, step[2]);
    digitalWrite(coil1_4, step[3]);
  } else {
    digitalWrite(coil2_1, step[0]);
    digitalWrite(coil2_2, step[1]);
    digitalWrite(coil2_3, step[2]);
    digitalWrite(coil2_4, step[3]);
  }
}