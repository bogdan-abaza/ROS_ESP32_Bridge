 /*********************************************************************
 * An interface for ESP32 to communicate by serial with a computer (with ROS), 
 *  It generates the appropriate PWM signals for Pololu VNH5019MotorShiel to drive two motors. 
 *  It is a simplified adaptation for ESP32 of this Arduino bridge: https://github.com/joshnewans/ros_arduino_bridge/tree/main
 *  It was tested with ESP32s NodeMCU https://docs.ai-thinker.com/_media/esp32/docs/nodemcu-32s_product_specification.pdf
 * and with the NeveRest 40 gearmotor with an attached encoder
 *  https://www.andymark.com/products/neverest-classic-40-gearmotor.
 *********************************************************************/

#include <Arduino.h>
#include "DualVNH5019MotorShieldESP32.h"
#include "commands.h"
#include "motor_driver.h"
#include "encoder_driver.h"


#define LEFT 1 //Motor 1 set to LEFT
#define RIGHT 2 ////Motor 2 set to RIGHT
#define BAUDRATE     115200
#define MAX_PWM      255

int moving = 0;
bool pidUpdated = false;

#include "diff_controller.h"

#define PID_RATE           30     // Hz
const int PID_INTERVAL = 1000 / PID_RATE;
unsigned long nextPID = PID_INTERVAL;
#define AUTO_STOP_INTERVAL 2000
long lastMotorCommand = AUTO_STOP_INTERVAL;

int arg = 0;
int indx = 0;
char chr;
char cmd;
char argv1[16];
char argv2[16];
long arg1;
long arg2;
bool pidAlreadyUpdated = false;

void resetCommand() {
    cmd = NULL;
    memset(argv1, 0, sizeof(argv1));
    memset(argv2, 0, sizeof(argv2));
    arg1 = 0;
    arg2 = 0;
    arg = 0;
    indx = 0;
    pidUpdated = false;
}

int runCommand() {
    int i = 0;
    char *p = argv1;
    char *str;
    int pid_args[4];
    arg1 = atoi(argv1);
    arg2 = atoi(argv2);

    switch(cmd) {
        case GET_BAUDRATE:
            Serial.println(BAUDRATE);
            break;

        case READ_ENCODERS: {
            noInterrupts(); // Dezactivează întreruperile
            long leftEncoder = readEncoder(LEFT); 
            long rightEncoder = readEncoder(RIGHT);
            interrupts(); // Reactivează întreruperile
            //Serial.print(leftEncoder);
            //Serial.print(" ");
            //Serial.println(rightEncoder);
            Serial.printf("%d %d\r\n", leftEncoder, rightEncoder);
            Serial.flush();  // Asigură-te că răspunsul este trimis complet
        }
        break;

        case RESET_ENCODERS:
            resetEncoders();
            resetPID();
            Serial.println("OK");
            break;

        case MOTOR_SPEEDS:
            lastMotorCommand = millis();
            if (arg1 == 0 && arg2 == 0) {
              setMotorBrakes(200, 200);
              resetPID();
              moving = 0;
            }
            else moving = 1;
            leftPID.TargetTicksPerFrame = arg1;
            rightPID.TargetTicksPerFrame = arg2;
            Serial.println("OK"); 
            break;
            

        case MOTOR_RAW_PWM:
            lastMotorCommand = millis();
            resetPID();
            moving = 0;
            setMotorSpeeds(arg1, arg2);
            Serial.println("OK");
            Serial.flush();  // Asigură-te că răspunsul este trimis complet
            break;

        case UPDATE_PID:
            if (pidUpdated) {
                Serial.println("PID already updated. Waiting for new command...");
                break;
            }

            i = 0;
            while ((str = strtok_r(p, ":", &p)) != nullptr) {
                if (i >= 4) {
                    Serial.println("Error: Too many PID arguments");
                    break;
                }
                pid_args[i] = atoi(str);
                i++;
            }

            if (i < 4) {
                Serial.println("Error: Insufficient PID arguments");
                break;
            }

            Kp = pid_args[0];
            Kd = pid_args[1];
            Ki = pid_args[2];
            Ko = pid_args[3];
            pidUpdated = true;

            Serial.println("PID updated successfully:");
            Serial.print("Kp = "); Serial.println(Kp);
            Serial.print("Kd = "); Serial.println(Kd);
            Serial.print("Ki = "); Serial.println(Ki);
            Serial.print("Ko = "); Serial.println(Ko);
            Serial.println("OK");
            break;

        default:
            Serial.println("Invalid Command");
            break;
    }
    if (cmd != 'u') {
        pidAlreadyUpdated = false;
    }
    return 0;
}

void processSerialCommand() {
    if (cmd == NULL || (cmd != 'o' && cmd != 'r' && cmd != 'e' && cmd != 'm' && cmd != 'u' && cmd != 'b')) {
        Serial.println("Invalid Command");
        resetCommand();
        return;
    }

    Serial.print("Processing command: ");
    Serial.println(cmd);

    runCommand();

    while (Serial.available() > 0) {
        Serial.read();
    }

    resetCommand();
}

void setup() {
    Serial.begin(BAUDRATE);
    initEncoders();

    initMotorController();
    resetPID();

    Serial.println("Starting ROS ESP32 Bridge...");
}

void loop() {
  while (Serial.available() > 0) {
    
    // Read the next character
    chr = Serial.read();

    // Terminate a command with a CR
    if (chr == 13) {
      if (arg == 1) argv1[indx] = NULL;
      else if (arg == 2) argv2[indx] = NULL;
      runCommand();
      resetCommand();
    }
    // Use spaces to delimit parts of the command
    else if (chr == ' ') {
      // Step through the arguments
      if (arg == 0) arg = 1;
      else if (arg == 1)  {
        argv1[indx] = NULL;
        arg = 2;
        indx = 0;
      }
      continue;
    }
    else {
      if (arg == 0) {
        // The first arg is the single-letter command
        cmd = chr;
      }
      else if (arg == 1) {
        // Subsequent arguments can be more than one character
        argv1[indx] = chr;
        indx++;
      }
      else if (arg == 2) {
        argv2[indx] = chr;
        indx++;
      }
    }
  }

    if (millis() > nextPID) {
        updatePID();
        nextPID += PID_INTERVAL;
    }

    if ((millis() - lastMotorCommand) > AUTO_STOP_INTERVAL) {
        setMotorBrakes(200, 200);
        moving = 0;
    }
}
