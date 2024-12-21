/*********************************************************************
 *  diff_controller.h
 *  Original code for PID parameters funtions from https://github.com/joshnewans/ros_arduino_bridge
 *  
 *********************************************************************/
/* Functions and type-defs for PID control.

   Taken mostly from Mike Ferguson's ArbotiX code which lives at:
   
   http://vanadium-ros-pkg.googlecode.com/svn/trunk/arbotix/
*/

/* PID setpoint info For a Motor */
typedef struct {
  double TargetTicksPerFrame;    // target speed in ticks per frame
  long Encoder;                  // encoder count
  long PrevEnc;                  // last encoder count

  /*
  * Using previous input (PrevInput) instead of PrevError to avoid derivative kick,
  * see http://brettbeauregard.com/blog/2011/04/improving-the-beginner%E2%80%99s-pid-derivative-kick/
  */
  int PrevInput;                // last input
  //int PrevErr;                   // last error

  /*
  * Using integrated term (ITerm) instead of integrated error (Ierror),
  * to allow tuning changes,
  * see http://brettbeauregard.com/blog/2011/04/improving-the-beginner%E2%80%99s-pid-tuning-changes/
  */
  //int Ierror;
  int ITerm;                    //integrated term

  long output;                    // last motor setting
}
SetPointInfo;

SetPointInfo leftPID, rightPID;

/* PID Parameters */
int Kp = 60;
int Kd = 20;
int Ki = 0;
int Ko = 15;

//unsigned char moving = 0; // is the base in motion?

/*
* Initialize PID variables to zero to prevent startup spikes
* when turning PID on to start moving
* In particular, assign both Encoder and PrevEnc the current encoder value
* See http://brettbeauregard.com/blog/2011/04/improving-the-beginner%E2%80%99s-pid-initialization/
* Note that the assumption here is that PID is only turned on
* when going from stop to moving, that's why we can init everything on zero.
*/
void resetPID() {
   leftPID.TargetTicksPerFrame = 0.0;
   leftPID.Encoder = readEncoder(LEFT);
   leftPID.PrevEnc = leftPID.Encoder;
   leftPID.output = 0;
   leftPID.PrevInput = 0;
   leftPID.ITerm = 0;

   rightPID.TargetTicksPerFrame = 0.0;
   rightPID.Encoder = readEncoder(RIGHT);
   rightPID.PrevEnc = rightPID.Encoder;
   rightPID.output = 0;
   rightPID.PrevInput = 0;
   rightPID.ITerm = 0;

   if (Ko == 0) Ko = 1;  // Asigură-te că Ko are o valoare validă
}

/* PID routine to compute the next motor commands */


void doPID(SetPointInfo *p) {
  long Perror;
  long output;
  int input;

  input = p->Encoder - p->PrevEnc; 
  Perror = p->TargetTicksPerFrame - input; 

  // PID
  output = (Kp * Perror - Kd * (input - p->PrevInput) + p->ITerm) / Ko;

  // Limit output
  if (output >= MAX_PWM) output = MAX_PWM;
  else if (output <= -MAX_PWM) output = -MAX_PWM;
  else p->ITerm += Ki * Perror;

  p->output = output;
  p->PrevInput = input;
  p->PrevEnc = p->Encoder;

  // Debugging PID
  Serial.print("Target: ");
  Serial.print(p->TargetTicksPerFrame);
  Serial.print(" | Encoder: ");
  Serial.print(p->Encoder);
  Serial.print(" | Output: ");
  Serial.println(p->output);
}


/* Read the encoder values and call the PID routine */
void updatePID() {
  double calibration_factor_left = 1; //1.06 
  double calibration_factor_right = 1;
  double l_PID;
  double r_PID;
  noInterrupts();  

  /* Read the encoders */
  leftPID.Encoder = readEncoder(LEFT);
  rightPID.Encoder = readEncoder(RIGHT);

  interrupts();  

  /* If we're not moving there is nothing more to do */
  if (!moving) {
    if (leftPID.PrevInput != 0 || rightPID.PrevInput != 0) resetPID();
    return;
  }

  /* Compute PID update for each motor */
  doPID(&rightPID);
  doPID(&leftPID);
  l_PID = leftPID.output * calibration_factor_left;
  r_PID = rightPID.output * calibration_factor_right;
  /* Set the motor speeds accordingly */
  setMotorSpeeds(l_PID, r_PID);
}

