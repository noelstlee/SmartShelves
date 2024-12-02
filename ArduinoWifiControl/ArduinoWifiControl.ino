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
int motorActive = 0; // 0 for Bottom, 1 for Top
bool isSpinning = false;
bool gesturePending = false;

const int rotation = 680;

AccelStepper stepperTop(MotorInterfaceType, 8, 10, 9, 11);
AccelStepper stepperBot(MotorInterfaceType, 4, 6, 5, 7);

long initPosTop = 0;
long initPosBot = 0;

MicroOLED oled(PIN_RESET);

unsigned long lastGestureTime = 0;
const unsigned long idleDelay = 5000;

// ESP-32 Controls
const int webIsReadyPin = 53;
const int webShelfPin = 51;
const int webDirectionPin = 49;

// Outputs to ESP-32
const int isSpinningPin = 52;
const int ack = 50;

// State flags for Wi-Fi commands
bool wifiCommandProcessed = false;

void setup() {
  Serial.begin(9600);

  oled.begin(0x3D, Wire);
  oled.clear(ALL);
  oled.display();
  delay(1000);

  displayMessage("WELCOME");
  delay(2000);
  oled.clear(PAGE);

  pinMode(APDS9960_INT, INPUT);
  attachInterrupt(digitalPinToInterrupt(APDS9960_INT), interruptRoutine, FALLING);

  if (apds.init()) Serial.println("Gesture sensor initialized.");
  else Serial.println("Gesture sensor initialization failed!");

  if (apds.enableGestureSensor(true)) Serial.println("Gesture sensor enabled.");
  else Serial.println("Gesture sensor enable failed!");

  stepperBot.setCurrentPosition(0);
  stepperBot.setMaxSpeed(1000.0);
  stepperBot.setAcceleration(200.0);
  initPosBot = stepperBot.currentPosition();

  stepperTop.setCurrentPosition(0);
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
  // Check if motors are spinning
  isSpinning = (stepperBot.distanceToGo() != 0 || stepperTop.distanceToGo() != 0);
  digitalWrite(isSpinningPin, isSpinning);

  if (!isSpinning) {
    if (gesturePending) {
      handleGesture();
      gesturePending = false;
    }

    if (!wifiCommandProcessed && digitalRead(webIsReadyPin) == HIGH) {
      handleWiFiControl();
      wifiCommandProcessed = true;
    }
  } else {
    wifiCommandProcessed = false; // Reset flag when spinning
  }

  stepperBot.run();
  stepperTop.run();

  if (millis() - lastGestureTime > idleDelay && !isSpinning) {
    displayMessage("Idle");
    lastGestureTime = millis();
  }
}

void interruptRoutine() {
  gesturePending = true;
}

void handleGesture() {
  int gesture = apds.readGesture();
  if (gesture == DIR_UP) {
    motorActive = 1;
    displayMessage("Upper Shelf");
  } else if (gesture == DIR_DOWN) {
    motorActive = 0;
    displayMessage("Lower Shelf");
  } else if (gesture == DIR_LEFT) {
    rotateMotor(motorActive, true);
    displayMessage("LEFT");
  } else if (gesture == DIR_RIGHT) {
    rotateMotor(motorActive, false);
    displayMessage("RIGHT");
  } else if (gesture == DIR_FAR) {
    returnToInitialPosition(motorActive);
    displayMessage("Reset");
  } else {
    displayMessage("Gesture Not Recognized");
  }
}

void handleWiFiControl() {
  digitalWrite(ack, HIGH);

  bool isBottomShelf = (digitalRead(webShelfPin) == LOW);
  bool isLeftDirection = (digitalRead(webDirectionPin) == LOW);

  // Display messages for Wi-Fi actions
  if (isBottomShelf) {
    displayMessage(isLeftDirection ? "Left" : "Right");
  } else {
    displayMessage(isLeftDirection ? "Left" : "Right");
  }

  // Serial.print("Wi-Fi Shelf: ");
  // Serial.print(isBottomShelf ? "Bottom" : "Top");
  // Serial.print(" Direction: ");
  // Serial.println(isLeftDirection ? "Left" : "Right");

  if (isBottomShelf) {
    rotateMotor(0, isLeftDirection);
  } else {
    rotateMotor(1, isLeftDirection);
  }

  digitalWrite(ack, LOW);
}

void rotateMotor(int motor1, bool clockwise) {
  isSpinning = true;
  digitalWrite(isSpinningPin, HIGH);
  AccelStepper &stepper = motor1 == 0 ? stepperBot : stepperTop;
  stepper.moveTo(stepper.currentPosition() + (clockwise ? rotation : -rotation));
}

void returnToInitialPosition(int motor1) {
  isSpinning = true;
  digitalWrite(isSpinningPin, HIGH);
  AccelStepper &stepper = motor1 == 0 ? stepperBot : stepperTop;

  long targetPosition = motor1 == 0 ? initPosBot : initPosTop;

  if (stepper.currentPosition() != targetPosition) {
    stepper.moveTo(targetPosition);
  }
}

void displayMessage(const char *message) {
  oled.setFontType(1);
  oled.clear(PAGE);
  oled.setCursor(0, 0);
  oled.print(message);
  oled.display();
}
