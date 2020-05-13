// Include the AccelStepper Library
#include <AccelStepper.h>

// Define pin connections
const int dirPin_r = 2;
const int stepPin_r = 3;
const int dirPin_l = 4;
const int stepPin_l = 5;

// Define motor interface type
#define motorInterfaceType 1 //external stepper driver with Step and Direction pins

// Creates an instance
AccelStepper stepper_l(motorInterfaceType, stepPin_l, dirPin_l);
AccelStepper stepper_r(motorInterfaceType, stepPin_r, dirPin_r);

void setup() {
  // set the maximum speed, acceleration factor,
  // initial speed and the target position
  stepper_l.setMaxSpeed(1000);
  stepper_l.setAcceleration(50);
  stepper_l.setSpeed(200);
  stepper_l.moveTo(200);

  stepper_r.setMaxSpeed(1000);
  stepper_r.setAcceleration(50);
  stepper_r.setSpeed(200);
  stepper_r.moveTo(200);
}

void loop() {
  // Change direction once the motor reaches target position

  if (stepper_l.distanceToGo() == 0) 
   stepper_l.moveTo(-stepper_l.currentPosition());

  if (stepper_r.distanceToGo() == 0) 
   stepper_r.moveTo(-stepper_r.currentPosition());

  // Move the motor one step
  stepper_l.run();
  stepper_r.run();
}
