// Define the Arduino pins connected to the ULN2003A driver inputs
int coil1_1 = 8; // IN1 on driver board for motor 1
int coil1_2 = 9; // IN2 on driver board for motor 1
int coil1_3 = 10; // IN3 on driver board for motor 1
int coil1_4 = 11; // IN4 on driver board for motor 1

int coil2_1 = 4; // IN1 on driver board for motor 2
int coil2_2 = 5; // IN2 on driver board for motor 2
int coil2_3 = 6; // IN3 on driver board for motor 2
int coil2_4 = 7; // IN4 on driver board for motor 2

// Stepper motor step sequence arrays
int stepSequence[4][4] = {
  {1, 0, 0, 0},
  {0, 1, 0, 0},
  {0, 0, 1, 0},
  {0, 0, 0, 1}
};

// Steps per degree (for a 2048-step motor, 11.37777 steps per degree)
const float stepsPerDegree = 5;

void setup() {
  Serial.begin(9600); // Initialize serial communication

  // Set all coil pins as outputs for stepper motor 1
  pinMode(coil1_1, OUTPUT);
  pinMode(coil1_2, OUTPUT);
  pinMode(coil1_3, OUTPUT);
  pinMode(coil1_4, OUTPUT);

  // Set all coil pins as outputs for stepper motor 2
  pinMode(coil2_1, OUTPUT);
  pinMode(coil2_2, OUTPUT);
  pinMode(coil2_3, OUTPUT);
  pinMode(coil2_4, OUTPUT);
}

void loop() {
  // Check for user input
  if (Serial.available() > 0) {
    int motor = Serial.parseInt(); // Motor number (1 or 2)
    int direction = Serial.parseInt(); // Direction (1 for CW, 0 for CCW)
    int degrees = Serial.parseInt(); // Degrees to move

    int steps = degrees * stepsPerDegree; // Convert degrees to steps

    // Run the motor at 200 steps per second
    int delayTime = 1000 / 200; // Delay for 200 steps per second

    if (motor == 1) {
      moveMotor(steps, direction, true, delayTime); // Move motor 1
    } else if (motor == 2) {
      moveMotor(steps, direction, false, delayTime); // Move motor 2
    }
  }
}

// Function to move motor a specified number of steps
void moveMotor(int steps, int direction, bool motor, int delayTime) {
  if (direction == 1) { // Clockwise
    for (int i = 0; i < steps; i++) {
      for (int step = 0; step < 4; step++) {
        setCoils(stepSequence[step], motor);
        delay(delayTime);
      }
    }
  } else { // Counterclockwise
    for (int i = 0; i < steps; i++) {
      for (int step = 3; step >= 0; step--) {
        setCoils(stepSequence[step], motor);
        delay(delayTime);
      }
    }
  }
}

// Function to activate coils based on step sequence
void setCoils(int step[4], bool motor) {
  if (motor) { // Motor 1
    digitalWrite(coil1_1, step[0]);
    digitalWrite(coil1_2, step[1]);
    digitalWrite(coil1_3, step[2]);
    digitalWrite(coil1_4, step[3]);
  } else { // Motor 2
    digitalWrite(coil2_1, step[0]);
    digitalWrite(coil2_2, step[1]);
    digitalWrite(coil2_3, step[2]);
    digitalWrite(coil2_4, step[3]);
  }
}
