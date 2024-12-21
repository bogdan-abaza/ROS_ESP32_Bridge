This project provides an interface for an ESP32 microcontroller to communicate via serial with a computer running ROS (Robot Operating System). It enables precise motor control by generating PWM signals for the Pololu VNH5019 Motor Shield, allowing it to drive two motors.

This implementation is a streamlined adaptation of the Arduino ROS bridge by Josh Newans (https://github.com/joshnewans/ros_arduino_bridge) — special thanks for the original work! The PololuVNH5019MotorShieldESP32.cpp file has been fine-tuned specifically for the ESP32, building on the original Arduino implementation available here: Pololu dual-vnh5019-motor-shield (https://github.com/pololu/dual-vnh5019-motor-shield).

Tested Configuration:
- Microcontroller: ESP32 NodeMCU-32S (https://docs.ai-thinker.com/_media/esp32/docs/nodemcu-32s_product_specification.pdf)
- -Motor: NeveRest 40 Gearmotor with Encoder (https://www.andymark.com/products/neverest-classic-40-gearmotor)
This project simplifies integrating motor control into robotics projects, leveraging ROS for high-level control and the ESP32 for efficient motor handling.
