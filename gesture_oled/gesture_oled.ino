#include <AccelStepper.h>
#include <SparkFun_APDS9960.h>
#include <SFE_MicroOLED.h>

// Stepper motor to complete one revolution: 2038 steps
#define APDS9960_INT 2
#define MotorInterfaceType 4

// Motor control variables
SparkFun_APDS9960 apds = SparkFun_APDS9960();
volatile int isr_flag = 0;
bool motor1_active = false;
bool motor2_active = false;

int rotation;

// Creates instances of AccelStepper for two motors
AccelStepper stepper1(MotorInterfaceType, 8, 10, 9, 11);
AccelStepper stepper2(MotorInterfaceType, 4, 6, 5, 7);

// Store initial positions
long initialPositionMotor1 = 0;
long initialPositionMotor2 = 0;

// OLED instance
MicroOLED oled(-1);

// Track idle state
unsigned long lastGestureTime = 0; // Timestamp of the last gesture
const unsigned long idleDelay = 3000; // Delay (in ms) before displaying "Idle"

void setup() {
  Serial.begin(9600);

  // Initialize OLED display
  oled.begin(0x3D, Wire);    // Initialize OLED (0x3D I2C address)
  oled.clear(ALL);           // Clear the display's internal memory
  oled.display();            // Display splash screen
  delay(1000);               // Wait 1 second

  // Display welcome message
  displayMessage("WELCOME");
  delay(2000); // Keep welcome text for 2 seconds
  oled.clear(PAGE); // Clear the OLED for further use

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

    // Update last gesture time
    lastGestureTime = millis();
  }

  // Run stepper motors
  stepper1.run();
  stepper2.run();

  // Check for idle state
  if (millis() - lastGestureTime > idleDelay) {
    displayMessage("Idle");
    lastGestureTime = millis(); // Prevent repeated updates
  }
}

void interruptRoutine() {
  isr_flag = 1;
}

void handleGesture() {
  if (apds.isGestureAvailable()) {
    int gesture = apds.readGesture();
    if (gesture == DIR_UP) {
      Serial.println("Gesture: UP - Activating Motor 1 (Upper Shelf)");
      motor1_active = true;
      motor2_active = false;
      displayMessage("Upper Shelf");
    } else if (gesture == DIR_DOWN) {
      Serial.println("Gesture: DOWN - Activating Motor 2 (Lower Shelf)");
      motor1_active = false;
      motor2_active = true;
      displayMessage("Lower Shelf");
    } else if (gesture == DIR_LEFT) {
      Serial.println("Gesture: LEFT - Rotating 120° clockwise");
      rotateMotor(motor1_active, true); // Rotate clockwise
      displayMessage("LEFT");
    } else if (gesture == DIR_RIGHT) {
      Serial.println("Gesture: RIGHT - Rotating 120° counterclockwise");
      rotateMotor(motor1_active, false); // Rotate counterclockwise
      displayMessage("RIGHT");
    } else if (gesture == DIR_FAR) {
      Serial.println("Gesture: FAR - Returning to initial position (Reset)");
      returnToInitialPosition(motor1_active); // Go back to initial position
      displayMessage("Reset");
    } else {
      Serial.println("Gesture: NONE - Gesture Not Recognized");
      displayMessage("Gesture Not Recognized");
    }
  }
}

void rotateMotor(bool motor1, bool clockwise) {
  if (clockwise) {
      rotation = 680;
  } else {
      rotation = -680;
  }

  if (motor1) {
    stepper1.moveTo(stepper1.currentPosition() + rotation);
  } else {
    stepper2.moveTo(stepper2.currentPosition() + rotation);
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

void displayMessage(const char *message) {
  oled.setFontType(1);       // Set font type
  oled.clear(PAGE);          // Clear the buffer
  oled.setCursor(0, 0);      // Set the cursor to the top-left corner
  oled.print(message);       // Display the message
  oled.display();            // Update the OLED
}