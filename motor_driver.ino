 /*********************************************************************
 *  motor_driver.ino
 *  Pololu VNH5019MotorShiel library tunned for ESP32. Based on original 
 *  for Arduino https://github.com/pololu/dual-vnh5019-motor-shield
 *  It was tested with the NeveRest 40 gearmotor with an attached encoder
 *  https://www.andymark.com/products/neverest-classic-40-gearmotor.
 *********************************************************************/

/* Include the Pololu VNH5019MotorShiel library tunned for ESP32*/
#include "DualVNH5019MotorShieldESP32.h"
//Load the ESP32 connected to the Pololu VNH5019MotorShiel with onnections remapped by user
#include "motor_driver.h"

/* Create the motor driver object for shield connections remapped by user*/
DualVNH5019MotorShieldESP32 drive(INA1, INB1, PWM1, EN1DIAG1, INA2, INB2, PWM2, EN2DIAG2);

//#define LEFT 1
//#define RIGHT 2

void initMotorController() {
  drive.init();
}

// Wrap the drive motor set speed function
void setMotorSpeed(int i, int spd) {
  if (i == LEFT)
    drive.setM1Speed(spd);
  else
    drive.setM2Speed(spd);
}

// A convenience function for setting both motor speeds
void setMotorSpeeds(int leftSpeed, int rightSpeed) {
  setMotorSpeed(LEFT, leftSpeed);
  setMotorSpeed(RIGHT, rightSpeed);
}
// Specific to VNH5019MotorShiel when braking motor
void setMotorBrake(int i, int brk) {
  if (i == LEFT)
    drive.setM1Brake(brk);
  else
    drive.setM2Brake(brk);
}
void setMotorBrakes(int leftSpeed, int rightSpeed) {
  setMotorBrake(LEFT, leftSpeed);
  setMotorBrake(RIGHT, rightSpeed);
}