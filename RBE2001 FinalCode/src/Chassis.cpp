#include "Chassis.h"
#include <RBE1001Lib.h>
#include <math.h>

bool Chassis::driveDistance(float inches)
{
    static bool setPoint_Hadset = false;
    if (!setPoint_Hadset) {
        leftSetpoint = -(inches * 360 / PI / wheelDiameter) + left_motor.getCurrentDegrees();
        rightSetpoint = -(inches * 360 / PI / wheelDiameter) + right_motor.getCurrentDegrees();
        setPoint_Hadset = true;
    }
    float errorLeft = leftSetpoint - left_motor.getCurrentDegrees();
    float errorRight = rightSetpoint - right_motor.getCurrentDegrees();
    float speedLeft = Kp * errorLeft / 3;
    float speedRight = Kp * errorRight / 3;
    printf("Left Setpoint %f Right Setpoint %f Left deg %f Left motor %f Right def %f Right motor %f\n", 
            leftSetpoint, rightSetpoint, left_motor.getCurrentDegrees(), speedLeft, right_motor.getCurrentDegrees(),speedRight);
    
    left_motor.setSpeed(speedLeft);
    right_motor.setSpeed(speedRight);

    if (withinEpsilon(errorLeft, threshold) && 
        withinEpsilon(errorRight, threshold)) {
            setPoint_Hadset = false;
            stop();
            return true;
        }
    return false;
}

// void Chassis::turnAngle(float robotDegrees)
// {
//     int wheelDegrees = -(robotDegrees * wheelTrack / wheelDiameter);

//     left_motor.startMoveFor(wheelDegrees, chassisAngularSpeed);
//     right_motor.moveFor(-wheelDegrees, chassisAngularSpeed);
// }

// Positive is counterclockwise
bool Chassis::turnAnglePID(float angle){
    static bool setPoint_Hadset = false;
    if (!setPoint_Hadset) {
        leftSetpoint = (angle * wheelTrack / wheelDiameter) + left_motor.getCurrentDegrees();
        rightSetpoint = -(angle * wheelTrack / wheelDiameter) + right_motor.getCurrentDegrees();
        setPoint_Hadset = true;
    }
    float errorLeft = leftSetpoint - left_motor.getCurrentDegrees();
    float errorRight = rightSetpoint - right_motor.getCurrentDegrees();
    float speedLeft = min(Kp * errorLeft / 3, (float) 100.0);
    float speedRight = min(Kp * errorRight / 3, (float) 100.0);
    printf("Left Setpoint %f Right Setpoint %f Left deg %f Left motor %f Right def %f Right motor %f\n", 
            leftSetpoint, rightSetpoint, left_motor.getCurrentDegrees(), speedLeft, right_motor.getCurrentDegrees(),speedRight);
    
    left_motor.setSpeed(speedLeft);
    right_motor.setSpeed(speedRight);

    if (withinEpsilon(errorLeft, threshold) && 
        withinEpsilon(errorRight, threshold)) {
            setPoint_Hadset = false;
            stop();
            return true;
        }
    return false;
}

// drive by effort [-1, 1]
void Chassis::drive(float effort) {
    left_motor.setSpeed(effort * chassisMaxSpeed);
    right_motor.setSpeed(effort * chassisMaxSpeed);
}

// TODO: test if this can stop immediately
bool Chassis::stop(){
    left_motor.setSpeed(0);
    right_motor.setSpeed(0);
}
// void Chassis::turn180(){
//     left_motor.setSpeed(-180);
//     right_motor.setSpeed(180);
    
// }
//  bool Chassis::CheckDistancetravelled(void){
//     static float targetWheelAngle = 8257.032/(PI * 7) + right_motor.getCurrentDegrees();
//     static float prevWheelAngle = 0;
//     bool retVal = false;
//     float currAngle = right_motor.getCurrentDegrees();
//     if((prevWheelAngle <targetWheelAngle) && (currAngle >= targetWheelAngle))
//     {
//       retVal = true;
//     }
//     prevWheelAngle = currAngle;
//     return retVal;
//  }


bool Chassis::turnAround()
{
//  left_motor.setSpeed(180);
//  right_motor.setSpeed(-180);
//  delay(2100);
//  stop();
//  Serial.println("Done turnning around");
//   drive(60);
//   delay (1000);
//   stop();
    return turnAnglePID(180.0);
}

bool Chassis::turnRight(){
    // left_motor.moveFor(360, 100);
    // Serial.println("turned Right");
    return turnAnglePID(-90);
}

bool Chassis::turnLeft(){
    // right_motor.moveFor(360, 100);
    // Serial.println("Turned Left");
    return turnAnglePID(90);
}