#include <RBE1001Lib.h>
#include <ESP32AnalogRead.h>
#include <button.h>
#include <Chassis.h>

extern Chassis chassis;

void lineFollow(int);
bool meetIntersection();

//
bool followLineUntilIntersection();

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

float LINE_FOLLOW_ANGULAR_SPEED = 300.0;