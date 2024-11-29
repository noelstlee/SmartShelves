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

// Constants
const int stepsPerRevolution = 516; // Steps for 360° for a 2048-step motor
const int delayTime = 3; // Delay for motor speed control


// Inputs from ESP-32
int webIsReadyPin = 53;
int webShelfPin = 51;
int webDirectionPin = 49;

// Outputs to ESP-32
int isSpinningPin = 52;
int ack = 50;

bool isSpinning = false;


int stepSequence[4][4] = {
  {1, 0, 0, 0},
  {0, 1, 0, 0},
  {0, 0, 1, 0},
  {0, 0, 0, 1} 
};

void setup() {
  pinMode(coil1_1, OUTPUT);
  pinMode(coil1_2, OUTPUT);
  pinMode(coil1_3, OUTPUT);
  pinMode(coil1_4, OUTPUT);

  pinMode(coil2_1, OUTPUT);
  pinMode(coil2_2, OUTPUT);
  pinMode(coil2_3, OUTPUT);
  pinMode(coil2_4, OUTPUT);

  pinMode(webIsReadyPin, INPUT);
  pinMode(webShelfPin, INPUT);
  pinMode(webDirectionPin, INPUT);

  pinMode(isSpinningPin, OUTPUT);
  pinMode(ack, OUTPUT);

  digitalWrite(isSpinningPin, LOW);
  digitalWrite(ack, LOW);
}

void loop() {
  if (isSpinning == false) {
    if (digitalRead(webIsReadyPin) == HIGH) {
      digitalWrite(isSpinningPin, HIGH);
      digitalWrite(ack, HIGH);
      if (digitalRead(webShelfPin) == LOW) {
        if (digitalRead(webDirectionPin) == LOW) {
          rotateMotor(false, true);
        } else {
          rotateMotor(false, false);
        }
      } else if (digitalRead(webShelfPin) == HIGH) {
        if (digitalRead(webDirectionPin) == LOW) {
          rotateMotor(true, true); 
        } else {
          rotateMotor(true, false);
        }
      }
    }
    digitalWrite(ack, LOW);
    digitalWrite(isSpinningPin, LOW);
    isSpinning = false;
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