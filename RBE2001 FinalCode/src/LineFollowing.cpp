#include <LineFollowing.h>

unsigned long lastTime = 0;
const unsigned long LINE_FOLLOWING_INTERVAL = 2;

Chassis chassis;

int setPointOn = 135;
int setPointOff = 2000;
//int setPoint = 55;

float K_P = 0.1;
float K_I = 0.1;
bool prevLeft = false;
bool prevRight = false;

float lastErrorLeft = 0;
float lastErrorRight = 0;

float leftMotorSpeed = 0 ;
float rightMotorSpeed = 0;
void lineFollow(int baseSpeed)

{
    unsigned long currTime = millis();
   if(currTime - lastTime > LINE_FOLLOWING_INTERVAL)
    {
        //read sensors
        float leftLineSense = analogRead(LEFT_LINE_SENSE);
        float rightLineSense = analogRead(RIGHT_LINE_SENSE);
        int currLeft = leftLineSense;
        int currRight = rightLineSense;

        //comand the motors
        if (currLeft > setPointOn) //need to move right
        {
            float errorLeft = abs(setPointOn - currLeft);
            float deltaErrorLeft = abs(errorLeft - lastErrorLeft);
            float effortLeft = K_P * errorLeft + K_I * deltaErrorLeft;

 
            //move right motor faster
            leftMotorSpeed = baseSpeed + effortLeft;
            rightMotorSpeed = baseSpeed - effortLeft;

            chassis.left_motor.setSpeed(leftMotorSpeed);
            chassis.right_motor.setSpeed(rightMotorSpeed);

            lastErrorLeft = errorLeft;
        }

        if (currRight > setPointOn)  //need to move right
        {

            float errorRight = abs(setPointOn - currRight);
            float deltaErrorRight = abs(errorRight - lastErrorRight);
            float effortRight = K_P * errorRight + K_I * deltaErrorRight;

             //move left motor faster

            leftMotorSpeed = baseSpeed - effortRight;
            rightMotorSpeed = baseSpeed + effortRight;
            chassis.left_motor.setSpeed(leftMotorSpeed);
            chassis.right_motor.setSpeed(rightMotorSpeed);
            lastErrorRight = errorRight;
        }

        if(currLeft < setPointOff && currRight < setPointOff)
        {
           chassis.drive(baseSpeed);

        }
    lastTime = currTime;

    }
 
}

bool followLineUntilIntersection() {
    int leftReading = sqrt(analogRead(LEFT_LINE_SENSE));
    int rightReading = sqrt(analogRead(RIGHT_LINE_SENSE));
    int error = leftReading - rightReading;

    chassis.left_motor.setSpeed(LINE_FOLLOW_ANGULAR_SPEED - error * K_P);
    chassis.right_motor.setSpeed(LINE_FOLLOW_ANGULAR_SPEED + error * K_P);
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