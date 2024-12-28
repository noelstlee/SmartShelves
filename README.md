# SmartShelves

### Control.ino  
This code allows users to control two stepper motors via serial commands. Using an intuitive input format, users can specify the motor number, direction, and rotation angle in degrees. The implementation features:  

- **Motor Control**: Operates two stepper motors through a ULN2003A driver board.  
- **Input Command Format**: Accepts serial commands like `1 1 120`, where:  
  - `1` indicates Motor 1,  
  - `1` specifies clockwise rotation (0 for counterclockwise),  
  - `120` defines the rotation angle in degrees.  
- **Dynamic Motor Control**: Converts degrees to steps and calculates precise motor movement using a 4-step sequence.  
- **Adjustable Speed**: Configures a delay for motor rotation at a consistent 200 steps per second.  
- **Dual Motor Operation**: Independently controls two motors, each with separate input and coil configurations.  

This file is ideal for scenarios requiring straightforward motor control with precise angle adjustments and directional inputs.  

1. Control_serial.ino: Control two stepper motors using Serial Input from User. Ex. 1 0 120 -> means activate motor 1 to move clockwise 120 degrees.
2. gesture_motor3.ino: Control two stepper motors using Gesture Sensors from user. Gesture Up, Down, Left, Right will be used to control sensors
3. oled.ino: test oled
4. gesture_oled.ino: Control two stepper motors with Gesture Sensors and use OLED as an indicator which gesture has been activated. 
