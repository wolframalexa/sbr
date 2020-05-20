#include <AccelStepper.h>

#define motorInterfaceType 1 //external stepper driver with Step and Direction pins

float MAX_SPEED = 0;
float MAX_ACCEL = 0;

float target_ang = 0;
float now_ang = 0;
float now_error = 0;
float last_error = 0;

float Kp = 0;
float Ki = 0;
float Kd = 0;
float Kf = 0;

float P = 0;
float I = 0;
float D = 0;
float F = 0;

float error = 0;
float error_integral = 0;
float error_derivative = 0;

float output = 0;

class Motor {

  public:
  
  Motor(int dirPin_l, int stepPin_l, int dirPin_r, int stepPin_r) {
    AccelStepper stepper_l(motorInterfaceType, stepPin_l, dirPin_l);
    AccelStepper stepper_r(motorInterfaceType, stepPin_r, dirPin_r);
    
  stepper_l.setMaxSpeed(1000);
  stepper_l.setAcceleration(50);
  stepper_l.setSpeed(200);

  stepper_r.setMaxSpeed(1000);
  stepper_r.setAcceleration(50);
  stepper_r.setSpeed(200);
  

    if (stepper_l.distanceToGo() == 0) 
   stepper_l.moveTo(-stepper_l.currentPosition());

  if (stepper_r.distanceToGo() == 0) 
   stepper_r.moveTo(-stepper_r.currentPosition());

  // Move the motor one step
  stepper_l.run();
  stepper_r.run();
  }

  
  

//const int dirPin_r = 2; // = 2
//const int stepPin_r = 3;
//const int dirPin_l = 4;
//const int stepPin_l = 5;





void test() {
  // Change direction once the motor reaches target position


}

void runPID() {

}

void setGains() {
  
}

void RunPID() {

//  current_time = millis(); TODO: handle time in robot.cpp
//t  elapsed_time = (double)(current_time - previous_time);

  target_ang = 0;
  now_ang = 0; //get tilt from accel+gyro

  now_error = target_ang - now_ang;

  P = Kp * now_error;

  error_integral += now_error;
  I = Ki * error_integral;

 // error_derivative = (now_error - last_error) / elapsed_time;
  D = Kd * error_derivative;

  F = Kf * error;

  output = P + I + D + F;

  last_error = error;
 // last_time = now_time; 

}

void SetMotors(float output) {

  //  speed = constrain(output, -MAX_SPEED, MAX_SPEED);

//    int16_t accel = speed - actualMotorSpeed[motorNum];
//    if (accel > MAX_ACCEL) {
//      actualMotorSpeed[motorNum] += MAX_ACCEL;
//    } else if (accel < -MAX_ACCEL) {
//      actualMotorSpeed[motorNum] -= MAX_ACCEL;
//    } else {
//      actualMotorSpeed[motorNum] = speed;
//    }
//
//    if (((speed[0] == 0) && (speed[1] == 0)) && (accel == 0)) {
//  //  digitalWrite(4,HIGH);   // Disable motors
//  } else {
//  //  digitalWrite(4,LOW);   // Enable motors
//  }
}

    //set motor to output

};
