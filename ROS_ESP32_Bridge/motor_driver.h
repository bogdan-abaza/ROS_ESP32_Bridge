/*********************************************************************
 *  motor_driver.h
 *  Pin map VNH5019MotorShield to ESP32
 * ESP32s NodeMCU https://docs.ai-thinker.com/_media/esp32/docs/nodemcu-32s_product_specification.pdf
  *********************************************************************/
//define pin map 
#define INA1 26
#define INB1 12
#define PWM1 14
#define EN1DIAG1 27
#define INA2 17
#define INB2 5
#define PWM2 2
#define EN2DIAG2 4

void initMotorController();
void setMotorSpeed(int i, int spd);
void setMotorSpeeds(int leftSpeed, int rightSpeed);
void setMotorBrakes(int leftSpeed, int rightSpeed);