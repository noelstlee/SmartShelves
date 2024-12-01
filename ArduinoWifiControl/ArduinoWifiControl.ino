#include <AccelStepper.h>
#include <SparkFun_APDS9960.h>
#include <SFE_MicroOLED.h>

// Stepper motor to complete one revolution: 2038 steps
#define APDS9960_INT 2
#define MotorInterfaceType 4
#define PIN_RESET -1

// Motor control variables
SparkFun_APDS9960 apds = SparkFun_APDS9960();
volatile int isr_flag = 0;
int motorActive = 0;
bool isSpinning = false;

const int rotation = 680;

// Creates instances of AccelStepper for two motors
AccelStepper stepperTop(MotorInterfaceType, 8, 10, 9, 11);
AccelStepper stepperBot(MotorInterfaceType, 4, 6, 5, 7);

// Store initial positions
long initPosTop = 0;
long initPosBot = 0;

// OLED instance
MicroOLED oled(PIN_RESET);

// Track idle state
unsigned long lastGestureTime = 0; // Timestamp of the last gesture
const unsigned long idleDelay = 5000; // Delay (in ms) before displaying "Idle"

// ESP-32 Controls

// Inputs from ESP-32
const int webIsReadyPin = 53;
const int webShelfPin = 51;
const int webDirectionPin = 49;

// Outputs to ESP-32
const int isSpinningPin = 52;
const int ack = 50;



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
  stepperBot.setCurrentPosition(0); // Align motor 2 to home position
  stepperBot.setMaxSpeed(1000.0);
  stepperBot.setAcceleration(200.0);
  initPosBot = stepperBot.currentPosition();

  stepperTop.setCurrentPosition(0); // Align motor 1 to home position
  stepperTop.setMaxSpeed(1000.0);
  stepperTop.setAcceleration(200.0);
  initPosTop = stepperTop.currentPosition();

  pinMode(webIsReadyPin, INPUT);
  pinMode(webShelfPin, INPUT);
  pinMode(webDirectionPin, INPUT);

  pinMode(isSpinningPin, OUTPUT);
  pinMode(ack, OUTPUT);

  digitalWrite(isSpinningPin, LOW);
  digitalWrite(ack, LOW);

}

void loop() {
  // Handle gesture interrupts
  if (stepperBot.distanceToGo() != 0 || stepperTop.distanceToGo() != 0) {
    isSpinning = true;
    digitalWrite(isSpinningPin, HIGH);
  } else {
    digitalWrite(ack, LOW);
    isSpinning = false;
    digitalWrite(isSpinningPin, LOW);
  }
  if (isSpinning == false) {
    Serial.println("Not Spinning, checking for inputs");
    if (digitalRead(webIsReadyPin) == HIGH) {
      digitalWrite(ack, HIGH);
      if (digitalRead(webShelfPin) == LOW) {         // Bottom Shelf
      Serial.println("Bottom");
        if (digitalRead(webDirectionPin) == LOW) {   // Left
          rotateMotor(0, true);
        } else {                                     // Right
          rotateMotor(0, false);
        }
      } else if (digitalRead(webShelfPin) == HIGH) {  // Top Shelf
        if (digitalRead(webDirectionPin) == LOW) {    // Left
          rotateMotor(1, true); 
        } else {                                      // Right
          rotateMotor(1, false);
        }
      }
    }
    // Serial.println(isr_flag);
    // if (isr_flag == 1) {
    //   Serial.println("In Detach");
    //   detachInterrupt(digitalPinToInterrupt(APDS9960_INT));
    //   Serial.println("After Detach");
    //   handleGesture();
    //   isr_flag = 0;
    //   attachInterrupt(digitalPinToInterrupt(APDS9960_INT), interruptRoutine, FALLING);

    //   // Update last gesture time
    //   lastGestureTime = millis();
    // }
  }

  // Run stepper motors
    stepperBot.run();
    stepperTop.run();

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
      motorActive = 1;
      displayMessage("Upper Shelf");
    } else if (gesture == DIR_DOWN) {
      Serial.println("Gesture: DOWN - Activating Motor 2 (Lower Shelf)");
      motorActive = 0;
      displayMessage("Lower Shelf");
    } else if (gesture == DIR_LEFT) {
      Serial.println("Gesture: LEFT - Rotating 120° clockwise");
      rotateMotor(motorActive, true); // Rotate clockwise
    } else if (gesture == DIR_RIGHT) {
      Serial.println("Gesture: RIGHT - Rotating 120° counterclockwise");
      rotateMotor(motorActive, false); // Rotate counterclockwise
    } else if (gesture == DIR_FAR) {
      Serial.println("Gesture: FAR - Returning to initial position (Reset)");
      returnToInitialPosition(motorActive); // Go back to initial position
      displayMessage("Reset");
    } else {
      Serial.println("Gesture: NONE - Gesture Not Recognized");
      displayMessage("Gesture Not Recognized");
    }
  }
}

void rotateMotor(int motor1, bool clockwise) {
  isSpinning = true;
  digitalWrite(isSpinningPin, HIGH);
  if (motor1 == 1) {
    if (clockwise) {
      displayMessage("LEFT");
      stepperTop.moveTo(stepperTop.currentPosition() + rotation);
    } else {
      displayMessage("RIGHT");
      stepperTop.moveTo(stepperTop.currentPosition() - rotation);
    }
  } else {
    if (clockwise) {
      displayMessage("LEFT");
      stepperBot.moveTo(stepperBot.currentPosition() + rotation);
    } else {
      displayMessage("RIGHT");
      stepperBot.moveTo(stepperBot.currentPosition() - rotation);
    }
  }
}

void returnToInitialPosition(int motor1) {
  isSpinning = true;
  digitalWrite(isSpinningPin, HIGH);
  AccelStepper &stepper = motor1 == 0 ? stepperBot : stepperTop;

  // Get the initial position of the active motor
  long targetPosition = motor1 == 0 ? initPosBot : initPosTop;

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