#include <RBE1001Lib.h>
#include <ESP32AnalogRead.h>
#include <button.h>
#include <Chassis.h>
extern Rangefinder ultra;
extern Chassis chassis;

bool meetIntersection();

//
bool followLineUntilIntersection(float);
bool followLineUntilDistance(float distance);
bool driveUntilLine(float);

//unsigned long = positive long integer
extern unsigned long lastTime;
extern const unsigned long LINE_FOLLOWING_INTERVAL;

extern bool prevLeft;
extern bool prevRight;

extern int setPointOn;
extern int setPointOff;
//int setPoint = 55;

extern float K_P ;
extern float K_I;

extern float lastErrorLeft;
extern float lastErrorRight;

extern float leftMotorSpeed;
extern float rightMotorSpeed;

extern float LINE_FOLLOW_ANGULAR_SPEED;