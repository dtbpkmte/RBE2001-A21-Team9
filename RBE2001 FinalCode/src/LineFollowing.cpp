#include <LineFollowing.h>
#include <math.h>

unsigned long lastTime = 0;
const unsigned long LINE_FOLLOWING_INTERVAL = 2;
Rangefinder ultra;
Chassis chassis;

int setPointOn = 3400;
int setPointOff = 70;
//int setPoint = 55;

float K_P = 0.1;
float K_I = 0.1;
bool prevLeft = false;
bool prevRight = false;

float lastErrorLeft = 0;
float lastErrorRight = 0;

float leftMotorSpeed = 0 ;
float rightMotorSpeed = 0;

float LINE_FOLLOW_ANGULAR_SPEED = 150.0;
bool followLineUntilIntersection(float lol) {
    int leftReading = sqrt(analogRead(LEFT_LINE_SENSE));
    int rightReading = sqrt(analogRead(RIGHT_LINE_SENSE));
    int error = leftReading - rightReading;
    
    printf("L=%f R=%f error=%f\n", leftReading, rightReading, error);
    chassis.left_motor.setSpeed(-(LINE_FOLLOW_ANGULAR_SPEED - error * K_P*10));
    chassis.right_motor.setSpeed(-(LINE_FOLLOW_ANGULAR_SPEED + error * K_P*10));


    bool mi;
    if (mi = meetIntersection()) {
        chassis.stop();
    }
    return mi;
}

bool followLineUntilDistance(float distance) {
    float currentDistance = ultra.getDistanceCM();
    float leftReading = sqrt(analogRead(LEFT_LINE_SENSE));
    float rightReading = sqrt(analogRead(RIGHT_LINE_SENSE));
    float error = leftReading - rightReading;
    
    // printf("L=%f R=% error=%f\n", error);
    chassis.left_motor.setSpeed(-(LINE_FOLLOW_ANGULAR_SPEED - error * K_P*10));
    chassis.right_motor.setSpeed(-(LINE_FOLLOW_ANGULAR_SPEED + error * K_P*10));

    bool mi = false;
    if (withinEpsilon(currentDistance-distance, 1)) {
        chassis.stop();
        mi = true;
    }
    return mi;
}

// TODO: test this
bool meetIntersection(void){
    bool value = false;
    int currLeft = analogRead(LEFT_LINE_SENSE);
    int currRight = analogRead(RIGHT_LINE_SENSE);
    if(currLeft > setPointOn && currRight > setPointOn){
       value = true;   
    } 
    return value;

}

bool driveUntilLine(float f) {
    if (!meetIntersection()) {
        chassis.left_motor.setSpeed(-300.0);
        chassis.right_motor.setSpeed(-300.0);
        return false;
    } 
    chassis.stop();
    return true;
}