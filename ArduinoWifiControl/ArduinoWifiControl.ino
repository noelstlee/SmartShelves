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
bool isSpinning = false;
bool gestureReady = false;
bool crazyMode = false;
long randNumber;
const int rotation = 680;

int shelfState = 0; //0 for Off, 1 for Bot, 2 for Top, 3 for On

AccelStepper stepperTop(MotorInterfaceType, 8, 10, 9, 11);
AccelStepper stepperBot(MotorInterfaceType, 4, 6, 5, 7);

long initPosTop = 0;
long initPosBot = 0;

MicroOLED oled(PIN_RESET);

unsigned long lastActionTime = 0;
const unsigned long idleDelay = 3000;

// ESP-32 Controls
const int webIsReadyPin = 25;
const int webShelfPin = 27;
const int webDirectionPin = 2 9;
const int webCrazyPin = 28;

// Outputs to ESP-32
const int isSpinningPin = 24;
const int ack = 26;

// State flags for Wi-Fi commands
bool wifiCommandProcessed = false;

// Light Pins
const int topLight = 22;
const int botLight = 23;

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

  pinMode(botLight, OUTPUT);
  digitalWrite(botLight, LOW);
  pinMode(topLight, OUTPUT);
  digitalWrite(topLight, LOW);

  randomSeed(analogRead(0));
}

void loop() {
  // Check if motors are spinning
  isSpinning = (stepperBot.distanceToGo() != 0 || stepperTop.distanceToGo() != 0);
  digitalWrite(isSpinningPin, isSpinning);

  if (shelfState == 0) {
    digitalWrite(botLight, LOW);
    digitalWrite(topLight, LOW);
  } else if (shelfState == 1) {
    digitalWrite(botLight, HIGH);
    digitalWrite(topLight, LOW);
  } else if (shelfState == 2){
    digitalWrite(botLight, LOW);
    digitalWrite(topLight, HIGH);
  } else {
    digitalWrite(botLight, HIGH);
    digitalWrite(topLight, HIGH);
  }

  if (!isSpinning) {

    if (gestureReady) {
      handleGesture();
      gestureReady = false;
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

  if (millis() - lastActionTime > idleDelay && !isSpinning) {
    displayMessage("Idle");
    gestureReady = true;
    lastActionTime = millis();
  }
}

void interruptRoutine() {
  gestureReady = true;
}

void handleGesture() {
  int gesture = apds.readGesture();
  if (gesture == DIR_UP) {
    shelfState++;
    if (shelfState > 3) {
      shelfState = 3;
      crazyMode = true;
      rotateMotor(shelfState, true, crazyMode);
    }
    lastActionTime = millis();
    displayMessage("UP");
  } else if (gesture == DIR_DOWN) {
    shelfState--;
    if (shelfState < 0) {
      shelfState = 0;
    }
    lastActionTime = millis();
    displayMessage("DOWN");
  } 
  if (shelfState == 1 || shelfState == 2) {
    if (gesture == DIR_LEFT) {
      rotateMotor(shelfState, true, false);
      displayMessage("LEFT");
    } else if (gesture == DIR_RIGHT) {
      rotateMotor(shelfState, false, false);
      displayMessage("RIGHT");
    } else if (gesture == DIR_FAR) {
      returnToInitialPosition(shelfState);
      displayMessage("Reset");
    }
  }
}

void handleWiFiControl() {
  bool isBottomShelf = (digitalRead(webShelfPin) == LOW);
  bool isLeftDirection = (digitalRead(webDirectionPin) == LOW);
  bool isCrazy = (digitalRead(crazyPin) == HIGH);

  digitalWrite(ack, HIGH);

  // Display messages for Wi-Fi actions
  if (isCrazy) {
    rotateMotor(1, 1, true);
  } else {
    if (isBottomShelf) {
      displayMessage(isLeftDirection ? "Left" : "Right");
    } else {
      displayMessage(isLeftDirection ? "Left" : "Right");
    }

    if (isBottomShelf) {
      rotateMotor(1, isLeftDirection, false);
    } else {
      rotateMotor(2, isLeftDirection, false);
    }

  }
  digitalWrite(ack, LOW);
}

void rotateMotor(int motor, bool clockwise, bool crazyMode) {
  lastActionTime = millis();
  isSpinning = true;
  digitalWrite(isSpinningPin, HIGH);
  if (crazyMode == true) {
    randNumber = (int)random(3, 9);
    stepperBot.moveTo(stepperBot.currentPosition() + (rotation * randNumber));
    stepperTop.moveTo(stepperTop.currentPosition() - (rotation * randNumber));
  } else {
    if (motor == 1) {
      stepperBot.moveTo(stepperBot.currentPosition() + (clockwise ? rotation : -rotation));
    } else if (motor == 2) {
      stepperTop.moveTo(stepperTop.currentPosition() + (clockwise ? rotation : -rotation));
    }
  }
}

void returnToInitialPosition(int motor) {
  isSpinning = true;
  digitalWrite(isSpinningPin, HIGH);

  if (motor == 1) {
    if (stepperBot.currentPosition() != initPosBot) {
      stepperBot.moveTo(initPosBot);
    }
  } else if (motor == 2) {
    if (stepperTop.currentPosition() != initPosTop) {
      stepperTop.moveTo(initPosTop);
    }
  }
}

void displayMessage(const char *message) {
  oled.setFontType(1);
  oled.clear(PAGE);
  oled.setCursor(0, 0);
  oled.print(message);
  oled.display();
}
