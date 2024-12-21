//This code  is updated for ESP32 based on POLOLU DualVNH5019MotorShield.cpp https://github.com/pololu/dual-vnh5019-motor-shield
 /*********************************************************************
 *  Pololu VNH5019MotorShielESP32.cpp code tunned for ESP32. Based on original 
 *  for Arduino https://github.com/pololu/dual-vnh5019-motor-shield
 *  It was tested with ESP32s NodeMCU https://docs.ai-thinker.com/_media/esp32/docs/nodemcu-32s_product_specification.pdf
 * and with the NeveRest 40 gearmotor with an attached encoder
 *  https://www.andymark.com/products/neverest-classic-40-gearmotor.
 *********************************************************************/

#include "DualVNH5019MotorShieldESP32.h"
#include <Arduino.h>

// Constructors ////////////////////////////////////////////////////////////////

DualVNH5019MotorShieldESP32::DualVNH5019MotorShieldESP32()
{
  // Default pin map for ESP32
  _INA1 = 26;
  _INB1 = 12;
  _PWM1 = 14;
  _EN1DIAG1 = 27;
  _INA2 = 17;
  _INB2 = 15;
  _PWM2 = 2;
  _EN2DIAG2 = 4;
}

DualVNH5019MotorShieldESP32::DualVNH5019MotorShieldESP32(unsigned char INA1,
                                                         unsigned char INB1,
                                                         unsigned char PWM1,
                                                         unsigned char EN1DIAG1,
                                                         unsigned char INA2,
                                                         unsigned char INB2,
                                                         unsigned char PWM2,
                                                         unsigned char EN2DIAG2)
{
  _INA1 = INA1;
  _INB1 = INB1;
  _PWM1 = PWM1;
  _EN1DIAG1 = EN1DIAG1;
  _INA2 = INA2;
  _INB2 = INB2;
  _PWM2 = PWM2;
  _EN2DIAG2 = EN2DIAG2;
}

// Public Methods //////////////////////////////////////////////////////////////
//#include <esp_task_wdt.h>

void DualVNH5019MotorShieldESP32::init()
{
//  Serial.println("Adding current task to the watchdog...");
 // esp_task_wdt_add(NULL); // Add the current task to the watchdog

  // Proceed with initialization
  pinMode(_INA1, OUTPUT);
  pinMode(_INB1, OUTPUT);
  pinMode(_EN1DIAG1, INPUT);
  pinMode(_INA2, OUTPUT);
  pinMode(_INB2, OUTPUT);
  pinMode(_EN2DIAG2, INPUT);

  const int freq = 20000;
  const int resolution = 8;
  const int pwmChannel1 = 0;       // Canal PWM pentru motor 1
  const int pwmChannel2 = 1;       // Canal PWM pentru motor 2

  ledcAttachChannel(_PWM1, freq, resolution, pwmChannel1);
//  esp_task_wdt_reset(); // Reset the watchdog

  ledcAttachChannel(_PWM2, freq, resolution, pwmChannel2);
//  esp_task_wdt_reset(); // Reset the watchdog

  Serial.println("Initialization complete.");

}


// Set speed for motor 1, speed is a number between -400 and 400
void DualVNH5019MotorShieldESP32::setM1Speed(int speed)
{
  unsigned char reverse = 0;

  if (speed < 0)
  {
    speed = -speed;  // Make speed a positive quantity
    reverse = 1;     // Preserve the direction
  }
  if (speed > 255) // Max speed
    speed = 255;

  ledcWrite(_PWM1, speed); 

  if (speed == 0)
  {
    digitalWrite(_INA1, LOW);
    digitalWrite(_INB1, LOW);
  }
  else if (reverse)
  {
    digitalWrite(_INA1, LOW);
    digitalWrite(_INB1, HIGH);
  }
  else
  {
    digitalWrite(_INA1, HIGH);
    digitalWrite(_INB1, LOW);
  }
}

// Set speed for motor 2, speed is a number between -400 and 400
void DualVNH5019MotorShieldESP32::setM2Speed(int speed)
{
  unsigned char reverse = 0;

  if (speed < 0)
  {
    speed = -speed;  // Make speed a positive quantity
    reverse = 1;     // Preserve the direction
  }
  if (speed > 255) // Max speed
    speed = 255;

  ledcWrite(_PWM2,speed); // Set PWM duty cycle for channel 1

  if (speed == 0)
  {
    digitalWrite(_INA2, LOW);
    digitalWrite(_INB2, LOW);
  }
  else if (reverse)
  {
    digitalWrite(_INA2, LOW);
    digitalWrite(_INB2, HIGH);
  }
  else
  {
    digitalWrite(_INA2, HIGH);
    digitalWrite(_INB2, LOW);
  }
}

// Set speed for motor 1 and 2
void DualVNH5019MotorShieldESP32::setSpeeds(int m1Speed, int m2Speed)
{
  setM1Speed(m1Speed);
  setM2Speed(m2Speed);
}

// Brake motor 1
void DualVNH5019MotorShieldESP32::setM1Brake(int brake)
{
  if (brake < 0)
  {
    brake = -brake;
  }
  if (brake > 255) // Normalize brake
    brake = 255;
  digitalWrite(_INA1, LOW);
  digitalWrite(_INB1, LOW);
  ledcWrite(_PWM1,brake); // Stop PWM
}

// Brake motor 2
void DualVNH5019MotorShieldESP32::setM2Brake(int brake)
{
  if (brake < 0)
  {
    brake = -brake;
  }
  if (brake > 255) // Normalize brake
    brake = 255;
  digitalWrite(_INA2, LOW);
  digitalWrite(_INB2, LOW);
  ledcWrite(_PWM2,brake); // Stop PWM
}

// Brake both motors
void DualVNH5019MotorShieldESP32::setBrakes(int m1Brake, int m2Brake)
{
  setM1Brake(m1Brake);
  setM2Brake(m2Brake);
}

// Return error status for motor 1
unsigned char DualVNH5019MotorShieldESP32::getM1Fault()
{
  return !digitalRead(_EN1DIAG1);
}

// Return error status for motor 2
unsigned char DualVNH5019MotorShieldESP32::getM2Fault()
{
  return !digitalRead(_EN2DIAG2);
}

