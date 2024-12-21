This project provides an interface for an ESP32 microcontroller to communicate via serial with a computer running ROS (Robot Operating System). It enables precise motor control by generating PWM signals for the Pololu VNH5019 Motor Shield, allowing it to drive two motors.

This implementation is a streamlined adaptation of the Arduino ROS bridge by Josh Newans (https://github.com/joshnewans/ros_arduino_bridge) — special thanks for the original work! The PololuVNH5019MotorShieldESP32.cpp file has been fine-tuned specifically for the ESP32, building on the original Arduino implementation available here: Pololu dual-vnh5019-motor-shield (https://github.com/pololu/dual-vnh5019-motor-shield).

Tested Configuration:
- Microcontroller: ESP32 NodeMCU-32S (https://docs.ai-thinker.com/_media/esp32/docs/nodemcu-32s_product_specification.pdf)
- Motor: NeveRest 40 Gearmotor with Encoder (https://www.andymark.com/products/neverest-classic-40-gearmotor)
This project simplifies integrating motor control into robotics projects, leveraging ROS for high-level control and the ESP32 for efficient motor handling.
Functionality
This project bridges an ESP32 with a computer (e.g., running ROS) via a serial interface to control two motors through the Pololu VNH5019 Motor Shield. It provides precise motor control and real-time encoder feedback. Additional functionality includes updating PID parameters and resetting encoders.

Key Commands:

-Encoder Feedback:
"**e**" - Get the current encoder counts for both motors.

-Reset Encoders:
"**r**" - Reset encoder counts to zero and PID controllers.

-Set Motor Speeds:
"**m Speed1 Speed2**" - Set motor speeds in encoder counts per loop (default: 30 Hz loop rate).

-Set Raw PWM:
"**o PWM1 PWM2**" - Set raw PWM values for both motors (-255 to 255).

-Update PID Parameters:
"*u Kp:Kd:Ki:Ko**" - Update PID gains for the motors.

-Get Baud Rate:
"**b**" - Returns the current serial baud rate.
