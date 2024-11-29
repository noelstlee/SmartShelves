#include <AccelStepper.h>
#include <SparkFun_APDS9960.h>

// Stepper motor to complete one revolution: 2038 steps
#define APDS9960_INT 2
#define MotorInterfaceType 4

// Motor control variables
SparkFun_APDS9960 apds = SparkFun_APDS9960();
volatile int isr_flag = 0;
bool motor1_active = false;
bool motor2_active = false;

// Creates instances of AccelStepper for two motors
AccelStepper stepper1(MotorInterfaceType, 8, 10, 9, 11);
AccelStepper stepper2(MotorInterfaceType, 4, 5, 6, 7);

// Array of valid positions (steps)
const int validPositions[] = {0, 680, 1360, 2038, -680, -1360, -2038};
const int numPositions = sizeof(validPositions) / sizeof(validPositions[0]);

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

  // Initialize stepper motors
  stepper1.setCurrentPosition(0); // Align motor 1 to home position
  stepper1.setMaxSpeed(2000.0);
  stepper1.setAcceleration(200.0);

  stepper2.setCurrentPosition(0); // Align motor 2 to home position
  stepper2.setMaxSpeed(2000.0);
  stepper2.setAcceleration(200.0);
}

void loop() {
  // Handle gesture interrupts
  if (isr_flag == 1) {
    detachInterrupt(digitalPinToInterrupt(APDS9960_INT));
    handleGesture();
    isr_flag = 0;
    attachInterrupt(digitalPinToInterrupt(APDS9960_INT), interruptRoutine, FALLING);
  }

  // Run stepper motors
  stepper1.run();
  stepper2.run();
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
      rotateMotor(motor1_active, true); // Rotate clockwise
    } else if (gesture == DIR_RIGHT) {
      Serial.println("Gesture: RIGHT - Rotating 120° counterclockwise");
      rotateMotor(motor1_active, false); // Rotate counterclockwise
    } else if (gesture == DIR_NEAR) {
      Serial.println("Gesture: NEAR@; - Adjusting to nearest valid position");
      adjustPosition(motor1_active); // Adjust position of the active motor
    } else {
      Serial.println("Gesture: NONE");
    }
  }
}

void rotateMotor(bool motor1, bool clockwise) {
  checkPosition(); // Ensure the motor's position is within bounds

  if (motor1) {
    stepper1.moveTo(stepper1.currentPosition() + (clockwise ? 680 : -680));
  } else {
    stepper2.moveTo(stepper2.currentPosition() + (clockwise ? 680 : -680));
  }
}

void checkPosition() {
  // Ensure stepper1 stays within valid positions
  if (stepper1.currentPosition() > 2038) {
    stepper1.setCurrentPosition(0);
  } else if (stepper1.currentPosition() < -2038) {
    stepper1.setCurrentPosition(-2038);
  }

  // Ensure stepper2 stays within valid positions
  if (stepper2.currentPosition() > 2038) {
    stepper2.setCurrentPosition(0);
  } else if (stepper2.currentPosition() < -2038) {
    stepper2.setCurrentPosition(-2038);
  }
}

void adjustPosition(bool motor1) {
  AccelStepper &stepper = motor1 ? stepper1 : stepper2;

  int currentPosition = stepper.currentPosition();
  int closestPosition = validPositions[0];
  int minDifference = abs(currentPosition - validPositions[0]);

  // Find the closest valid position
  for (int i = 1; i < numPositions; i++) {
    int difference = abs(currentPosition - validPositions[i]);
    if (difference < minDifference) {
      closestPosition = validPositions[i];
      minDifference = difference;
    }
  }

  // If the current position is not valid, move to the closest position
  if (currentPosition != closestPosition) {
    Serial.print("Adjusting from ");
    Serial.print(currentPosition);
    Serial.print(" to ");
    Serial.println(closestPosition);
    stepper.moveTo(closestPosition);
  } else {
    Serial.println("Position is already valid.");
  }
}