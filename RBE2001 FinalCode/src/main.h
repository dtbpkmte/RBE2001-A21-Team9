#pragma once
#include <RBE1001Lib.h>
#include <PID_v1.h>
#include <Timer.h>
#include <ESP32Encoder.h>
#include <IRdecoder.h>
#include <BlueMotor.h>
#include <Navigation.h>
#include <ESP32AnalogRead.h>
#include <LineFollowing.h>


//define the key on IR Remote
int VOL_PLUS = 2;
int VOL_MINUS = 0;
int STOP_BUTTON = 6;
int KEY1 = 16;
int KEY2 = 17;
int KEY3 = 18;
int KEY4 = 20;
int KEY5 = 21;
int KEY6 = 22;
int ENTER = 9;
int CH_PLUS = 10;
int CH_MINUS = 8;
int PLAY = 1;
int RETURN = 14;


float targetWheelAngle = 0;
float prevWheelAngle = 0;
void turn180(){
    chassis.left_motor.setSpeed(180);
    chassis.right_motor.setSpeed(-180);
    targetWheelAngle = 7800.032/(PI * 7) + chassis.left_motor.getCurrentDegrees();
}

bool CheckDistanceTravelled(void){
    bool retVal = false;
    float currAngle = chassis.left_motor.getCurrentDegrees();
    if((prevWheelAngle <targetWheelAngle) && (currAngle >= targetWheelAngle))
    {
      retVal = true;
    }
    prevWheelAngle = currAngle;
    return retVal;
}

BlueMotor blueMotor;
// define the pin for the IR receiver
const uint8_t IR_DETECTOR_PIN = 15;

// create an IRDecoder object
IRDecoder decoder(IR_DETECTOR_PIN);

Servo lifter;
ESP32AnalogRead servoPositionFeedback;
Rangefinder ultrasonic;
Navigation navigator;
double Kp = 0.05;
double Ki = 0.0; // 0.04;
double Kd = 0.0;

//Ultrasonic pid distance code
const float KpU = 0.05;
const float KiU = 0.04;
const float KdU = 0.0;
float previousDistance = 0.0;
float sumOfErrors = 0.0;
float outputUltra;
int updateKey = 0;

//Blue Motor PID values
const double KpMotor = 700.0/9362; //Kp for PLA: 700/9362
const double KiMotor = 0.00005; //Ki for PLA: 0.00005
const double KdMotor = 0.0005; //Kd for PLA: 0.0005

Timer printTimer(500);
Timer PIDTimer(20);




Rangefinder ultra;

// Create a button object for the built-in button on the ESP32
Button bootButton(BOOT_FLAG_PIN);
Button enButton(ESP_RST_POWERON);


double setpoint, inputValue, outputValue;
double setpointUltra, inputValueUltra, outputValueUltra;
double setpointMotor, inputValueMotor, outputValueMotor;
PID pid(&inputValue, &outputValue, &setpoint, Kp, Ki, Kd, REVERSE);
PID pidUltra(&inputValueUltra, &outputValueUltra, &setpointUltra, KpU, KiU, KdU, REVERSE);
PID pidMotor(&inputValueMotor, &outputValueMotor, &setpointMotor, KpMotor, KiMotor, KdMotor, REVERSE);

enum ROBOT_STATE {ROBOT_IDLE,ROBOT_ACTIVE,ROBOT_PLACING,ROBOT_GRIPPING, ROBOT_APPROARCHING,ROBOT_LIFTING,ROBOT_LINING,ROBOT_TURN180};

// Declare a variable, robotState, of our new type, ROBOT_STATE. Initialize it to ROBOT_IDLE.
ROBOT_STATE robotState = ROBOT_IDLE;