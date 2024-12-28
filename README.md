# Smart Shelves  

The Smart Shelves project is an interactive and automated shelving system designed to enhance user convenience. It integrates gesture recognition and Wi-Fi controls to enable seamless operation of shelf layers. The system utilizes Arduino and ESP32 microcontrollers for hardware control and includes features such as motorized rotation, OLED-based feedback, and remote control via a custom web interface.  

Key features include:  
- Gesture-based controls for selecting and rotating shelves.  
- Wi-Fi functionality for remote control using any web-enabled device.  
- Dynamic OLED feedback for real-time status updates.  
- Custom-designed 3D-printed casing for enhanced stability and usability.  

The following code files contribute to the core functionality of Smart Shelves:  

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

### gesture_motor.ino  

This file implements gesture-based control for two stepper motors using the **SparkFun APDS-9960 Gesture Sensor**. It enables intuitive interactions to activate and rotate the motors based on user gestures. Key features include:  

- **Gesture-Based Motor Activation**:  
  - `UP` Gesture: Activates Motor 1.  
  - `DOWN` Gesture: Activates Motor 2.  

- **Directional Motor Control**:  
  - `LEFT` Gesture: Rotates the active motor clockwise for a full revolution (360°).  
  - `RIGHT` Gesture: Rotates the active motor counterclockwise for a full revolution (360°).  

- **Motor Control Logic**:  
  - Uses a 4-step sequence to control motor coils for precise and smooth rotation.  
  - Supports independent motor selection and activation.  

- **Interrupt-Driven Gesture Handling**:  
  - Leverages the gesture sensor’s interrupt pin for responsive detection of gestures.  
  - Processes gestures efficiently with the `SparkFun_APDS9960` library.  

- **Hardware Integration**:  
  - Configures and controls stepper motors through ULN2003A driver inputs.  
  - Ensures stable operation with configurable motor delay and step count for full revolutions.  

This file highlights the project’s interactive capabilities, enabling real-time gesture recognition and motor control for dynamic shelf operation.  

### WifiControl.ino  
This file establishes Wi-Fi connectivity for the Smart Shelves system and allows remote control via a web-based interface. It integrates ESP32 capabilities to provide an intuitive user experience with the following features:  

- **Wi-Fi Access Point Setup**:  
  - Creates a local network named **SmartShelves** for user connection.  
  - Hosts an HTTP server on port 80 for user interactions.  

- **Remote Control via Web Interface**:  
  - Provides a dynamic HTML interface to control shelf rotation.  
  - Allows users to select top/bottom shelves and rotate left/right using intuitive button controls.  

- **Real-Time Shelf Feedback**:  
  - Displays whether the shelf is spinning or idle.  
  - A refresh mechanism updates the shelf's status on the interface.  

- **Hardware Communication**:  
  - Reads input signals from the shelf's status pins to prevent operation during spinning.  
  - Sends commands to the Arduino via dedicated pins for shelf selection, direction, and readiness.  

This code ensures seamless integration between the physical shelf hardware and the web-based control interface.  

---

### ArduinoWifiControl.ino  
This file bridges gesture-based and Wi-Fi-based controls for the Smart Shelves system. It enables dual modes of interaction while providing additional features for advanced usability. Key functionalities include:  

- **Gesture and Wi-Fi Control Integration**:  
  - Detects gestures using the **SparkFun APDS-9960** sensor and processes commands for shelf rotation and layer activation.  
  - Reads Wi-Fi commands from the ESP32 to control shelves and manage states.  

- **Advanced Motor Control**:  
  - Controls two stepper motors using the **AccelStepper** library for smooth and precise movements.  
  - Implements "Crazy Mode," a randomized motion mode for shelves.  
  - Features a reset function to return shelves to their initial positions.  

- **OLED Feedback**:  
  - Displays real-time messages such as "Left," "Right," or "Reset" for user feedback.  
  - Provides idle status updates during inactivity.  

- **State Management**:  
  - Tracks shelf states (off, bottom, top, all on) and toggles LED indicators accordingly.  
  - Prevents conflicting commands by ensuring motors are idle before processing new gestures or Wi-Fi inputs.  

This code integrates hardware and software functionalities, providing a unified and efficient control system for Smart Shelves.  
