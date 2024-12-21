An interface for ESP32 to communicate by serial with a computer (with ROS).
It generates the appropriate PWM signals for Pololu VNH5019MotorShiel to drive two motors. 
It is a simplified adaptation for ESP32 of this Arduino bridge: https://github.com/joshnewans/ros_arduino_bridge/tree/main. Thank you Josh Newans!
It was tested with ESP32s NodeMCU https://docs.ai-thinker.com/_media/esp32/docs/nodemcu-32s_product_specification.pdf and with the NeveRest 40 gearmotor with an attached encoder https://www.andymark.com/products/neverest-classic-40-gearmotor.
