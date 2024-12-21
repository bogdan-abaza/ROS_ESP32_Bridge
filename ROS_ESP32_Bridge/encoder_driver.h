/*********************************************************************
 *  encoder_driver.h
 *  Pin map for ESP32 with a optical quadrature encoder.
 *  It was tested with the NeveRest 40 gearmotor with an attached encoder.
 *********************************************************************/
// Encoder Pin Definitions
#define ENCODER_M2_A 22  // Channel A Motor 2
#define ENCODER_M2_B 23  // Channel B Motor 2
#define ENCODER_M1_A 19  // Channel A Motor 1
#define ENCODER_M1_B 18  // Channel B Motor 1

long readEncoder(int i);
void resetEncoder(int i);
void resetEncoders();