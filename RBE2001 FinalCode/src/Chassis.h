#pragma once
#include <RBE1001Lib.h>
#include "Utility.h"

class Chassis {
 public:
  void driveDistance(float inches);
  void turnAngle(float angle);
  bool turnAnglePID(float Angle);
  void drive(float effort);
  void turnLeft();
  void turnRight();
  void turnStraight();
  void stop();
  void turnAround();
  LeftMotor left_motor;
  RightMotor right_motor;
 private:

  const float wheelDiameter = 2.8;
  const float degreesPerInch = 360 / (wheelDiameter * 3.14);
  const float wheelTrack = 5.75;
  const float chassisAngularSpeed = 100.0; // deg/s
  const float KpAngle = 1.0/(180*4);
  const float threshold = 50; // deg

  float leftSetpoint = 0;
  float rightSetpoint = 0;
};