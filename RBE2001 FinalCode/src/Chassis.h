#pragma once
#include <RBE1001Lib.h>
#include "Utility.h"

class Chassis {
 public:
  bool driveDistance(float inches);
  void turnAngle(float angle);
  bool turnAnglePID(float Angle);
  void drive(float effort);
  bool turnLeft();
  bool turnRight();
  void turnStraight();
  bool stop();
  bool turnAround();
  LeftMotor left_motor;
  RightMotor right_motor;
 private:

  const float wheelDiameter = 2.8;
  const float degreesPerInch = 360 / (wheelDiameter * 3.14);
  const float wheelTrack = 5.75;
  const float chassisMaxSpeed = 300.0; // deg/s
  const float Kp = chassisMaxSpeed/(180*4);
  const float threshold = 20; // deg

  float leftSetpoint = 0;
  float rightSetpoint = 0;
};