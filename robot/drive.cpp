#include "drive.h"

AccelStepper lstepper(MOTOR_INTERFACE_TYPE, LSTEP_PIN, LDIR_PIN);
AccelStepper rstepper(MOTOR_INTERFACE_TYPE, RSTEP_PIN, RDIR_PIN);

Drive::Drive() {

  lstepper.setMaxSpeed(MAX_SPEED);
  lstepper.setAcceleration(MAX_ACCEL); // max or always?

  rstepper.setMaxSpeed(MAX_SPEED);
  rstepper.setAcceleration(MAX_ACCEL);

}

void Drive::setAngle(float imu_ang) {
  now_ang = imu_ang;
}

float Drive::getAngle() {
  return now_ang;
}

void Drive::runPID(float now_time) {

  _now_time = now_time;
  elapsed_time = (float)(_now_time - previous_time);
  previous_time = _now_time;

  target_ang = 0; //always to balance
  now_ang = getAngle();

  now_error = target_ang - now_ang;

  P = Kp * now_error;

  error_integral += now_error;
  I = Ki * error_integral;

  error_derivative = (now_error - last_error) / elapsed_time;
  D = Kd * error_derivative;

  F = Kf * error;

  output = P + I + D + F;

  last_error = error;

}

void Drive::setMotors(float output) {

  output = constrain(output, -MAX_SPEED, MAX_SPEED);

  lstepper.setSpeed(output);
  rstepper.setSpeed(output);

  //limit accel?


  lstepper.run();
  rstepper.run();
}
