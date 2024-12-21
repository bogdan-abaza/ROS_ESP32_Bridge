/*********************************************************************
 *  encoder_driver.ino
 *  ESP32 with a optical quadrature encoder.
 *  ESP32s NodeMCU https://docs.ai-thinker.com/_media/esp32/docs/nodemcu-32s_product_specification.pdf
 *  It was tested with the NeveRest 40 gearmotor with an attached encoder.
 * https://www.andymark.com/products/neverest-classic-40-gearmotor
 *********************************************************************/

#include "encoder_driver.h"
// Encoder Variables
volatile long encoderPositionM1 = 0;
volatile long encoderPositionM2 = 0;
volatile int lastEncodedM1 = 0;
volatile int lastEncodedM2 = 0;

void IRAM_ATTR handleEncoderM1() {
    int MSB = digitalRead(ENCODER_M1_A);
    int LSB = digitalRead(ENCODER_M1_B);
    int encoded = (MSB << 1) | LSB;
    int sum = (lastEncodedM1 << 2) | encoded;

    if (sum == 0b1101 || sum == 0b0100 || sum == 0b0010 || sum == 0b1011) encoderPositionM1++;
    if (sum == 0b1110 || sum == 0b0111 || sum == 0b0001 || sum == 0b1000) encoderPositionM1--;

    lastEncodedM1 = encoded;
}

void IRAM_ATTR handleEncoderM2() {
    int MSB = digitalRead(ENCODER_M2_A);
    int LSB = digitalRead(ENCODER_M2_B);
    int encoded = (MSB << 1) | LSB;
    int sum = (lastEncodedM2 << 2) | encoded;

    if (sum == 0b1101 || sum == 0b0100 || sum == 0b0010 || sum == 0b1011) encoderPositionM2++;
    if (sum == 0b1110 || sum == 0b0111 || sum == 0b0001 || sum == 0b1000) encoderPositionM2--;

    lastEncodedM2 = encoded;
}
void initEncoders() {
    pinMode(ENCODER_M1_A, INPUT_PULLUP);
    pinMode(ENCODER_M1_B, INPUT_PULLUP);
    pinMode(ENCODER_M2_A, INPUT_PULLUP);
    pinMode(ENCODER_M2_B, INPUT_PULLUP);

    attachInterrupt(digitalPinToInterrupt(ENCODER_M1_A), handleEncoderM1, CHANGE);
    attachInterrupt(digitalPinToInterrupt(ENCODER_M1_B), handleEncoderM1, CHANGE);
    attachInterrupt(digitalPinToInterrupt(ENCODER_M2_A), handleEncoderM2, CHANGE);
    attachInterrupt(digitalPinToInterrupt(ENCODER_M2_B), handleEncoderM2, CHANGE);

    Serial.println("Encoders initialized.");
}

void resetEncoders() {
    noInterrupts();
    encoderPositionM1 = 0;
    encoderPositionM2 = 0;
    interrupts();
}

long readEncoder(int i) {
    if (i == LEFT) return encoderPositionM1;
    else if (i == RIGHT) return encoderPositionM2;
    return 0;
}
