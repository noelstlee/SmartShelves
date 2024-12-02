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
AccelStepper stepper2(MotorInterfaceType, 4, 6, 5, 7);

// Store initial positions
long initialPositionMotor1 = 0;
long initialPositionMotor2 = 0;

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

  // Initialize stepper motors and save initial positions
  stepper1.setCurrentPosition(0); // Align motor 1 to home position
  stepper1.setMaxSpeed(2000.0);
  stepper1.setAcceleration(200.0);
  initialPositionMotor1 = stepper1.currentPosition();

  stepper2.setCurrentPosition(0); // Align motor 2 to home position
  stepper2.setMaxSpeed(2000.0);
  stepper2.setAcceleration(200.0);
  initialPositionMotor2 = stepper2.currentPosition();
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
    } else if (gesture == DIR_FAR) {
      Serial.println("Gesture: FAR - Returning to initial position");
      returnToInitialPosition(motor1_active); // Go back to initial position
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

void returnToInitialPosition(bool motor1) {
  AccelStepper &stepper = motor1 ? stepper1 : stepper2;

  // Get the initial position of the active motor
  long targetPosition = motor1 ? initialPositionMotor1 : initialPositionMotor2;

  // Move the motor to the initial position
  if (stepper.currentPosition() != targetPosition) {
    Serial.print("Returning to initial position from ");
    Serial.print(stepper.currentPosition());
    Serial.print(" to ");
    Serial.println(targetPosition);
    stepper.moveTo(targetPosition);
  } else {
    Serial.println("Motor is already at the initial position.");
  }
}
