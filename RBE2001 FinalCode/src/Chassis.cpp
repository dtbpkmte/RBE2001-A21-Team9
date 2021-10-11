#include "Chassis.h"
#include <RBE1001Lib.h>


void Chassis::driveDistance(float inches)
{
    int degrees = -(inches * 360 / PI / wheelDiameter); 
    left_motor.startMoveFor(degrees, chassisAngularSpeed);
    right_motor.startMoveFor(degrees, chassisAngularSpeed);
}

void Chassis::turnAngle(float robotDegrees)
{
    int wheelDegrees = -(robotDegrees * wheelTrack / wheelDiameter);

    left_motor.startMoveFor(wheelDegrees, chassisAngularSpeed);
    right_motor.moveFor(-wheelDegrees, chassisAngularSpeed);
}

// Positive is counterclockwise
bool Chassis::turnAnglePID(float angle){
    static bool setPoint_Hadset = false;
    if (!setPoint_Hadset) {
        leftSetpoint = -(angle * wheelTrack / wheelDiameter) + left_motor.getCurrentDegrees();
        rightSetpoint = (angle * wheelTrack / wheelDiameter) + right_motor.getCurrentDegrees();
        setPoint_Hadset = true;
    }
    float errorLeft = leftSetpoint - left_motor.getCurrentDegrees();
    float errorRight = rightSetpoint - right_motor.getCurrentDegrees();
    float effortLeft = KpAngle * errorLeft;
    float effortRight = KpAngle * 0.8 * errorRight;
    printf("Left Setpoint %f Right Setpoint %f Left deg %f Left motor %f Right def %f Right motor %f\n", 
            leftSetpoint, rightSetpoint, left_motor.getCurrentDegrees(), effortLeft, right_motor.getCurrentDegrees(),effortRight);
    
    left_motor.setEffort(effortLeft);
    right_motor.setEffort(effortRight);

    if (withinEpsilon(errorLeft, threshold) && 
        withinEpsilon(errorRight, threshold)) {
            setPoint_Hadset = false;
            return true;
        }
    return false;
}

void Chassis::drive(float effort) {
    left_motor.setEffort(effort);
    right_motor.setEffort(effort);
}

void Chassis::stop(){
    left_motor.setEffort(0);
    right_motor.setEffort(0);
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


void Chassis::turnAround()
{
//  left_motor.setSpeed(180);
//  right_motor.setSpeed(-180);
//  delay(2100);
//  stop();
//  Serial.println("Done turnning around");
//   drive(60);
//   delay (1000);
//   stop();
    turnAngle(180.0);
}

void Chassis::turnRight(){
    // left_motor.moveFor(360, 100);
    // Serial.println("turned Right");
    turnAngle(-90);
}

void Chassis::turnLeft(){
    // right_motor.moveFor(360, 100);
    // Serial.println("Turned Left");
    turnAngle(90);
}