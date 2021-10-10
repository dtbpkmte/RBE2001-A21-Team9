#include <RBE1001Lib.h>
#include <IRdecoder.h>
#include "BlueMotor.h"
#include <cstdint>
#include <PID_v1.h>
#include <Timer.h>

BlueMotor blueMotor;
const uint8_t IR_DETECTOR_PIN = 15;
IRDecoder decoder(IR_DETECTOR_PIN);
long timeToPrint = 0;
long now = 0;
long newPosition = 0;
long oldPosition = 0;
long sampleTime = 100;
int speedInRPM = 0;
int CPR = 270; //2x
long prevtime = 0;
long curtime = 0;
long offsettime = 0;
int neweffort = 0;

// 2000/9362 is the initial oscillation value for Kp
const double Kp = 700.0/9362; //Kp for PLA: 700/9362
const double Ki = 0.00005; //Ki for PLA: 0.00005
const double Kd = 0.0005; //Kd for PLA: 0.0005
double setpoint, inputValue, outputValue;
PID pid(&inputValue, &outputValue, &setpoint, Kp, Ki, Kd, REVERSE);
bool runpid = false; //this allows us to access the code to move the crank down with a button press instead of commenting out the PID code everytime we need to reset the position of the crank    
bool resetted = false;


Timer timer(10);


void setup()
{
    setpoint = 9362.0; //encoder count when the crank is at the 25 degree roof 
    Serial.begin(9600);
    blueMotor.setup();
    decoder.init();
    pid.SetSampleTime(10);
    pid.SetMode(AUTOMATIC);
    pid.SetOutputLimits(-255.0, 255.0); 
}



void loop()
{    
    int16_t keyPress = decoder.getKeyCode();
    if (timer.isExpired()) {
        inputValue = blueMotor.getPosition();
         
        
        // printf("i = %lf o = %lf, %ld\n", inputValue, outputValue, keyPress);
        newPosition = blueMotor.getPosition();
        curtime = millis();
        speedInRPM = ((newPosition - oldPosition) * 1000 * 60) / ((curtime - prevtime) * CPR);
        printf("%ld,%.1lf,%.1lf,%d,%d,%ld,%lf,%lf,%lf\n", newPosition, setpoint, outputValue, neweffort, speedInRPM, curtime-offsettime, Kp, Ki, Kd);

        oldPosition = newPosition;
        prevtime = curtime;
    }

     pid.Compute();
    // if (keyPress == 20){
    //      runpid = true;
    //      blueMotor.reset();
    // }

    if (keyPress == 16)
    {
        runpid = false;
        blueMotor.reset();
        offsettime = millis();
        for (int i = 0; i < 255; i++) 
        {

            int neweffort = blueMotor.setEffortWithoutDB(i); //positive i moves the crank down
            //blueMotor.setEffort(i);
            delay(100);
            // Serial.printf("The inputted effort is %d\n", i); 
            // printf("The adjusted effort is %d\n", neweffort);
            newPosition = blueMotor.getPosition();
            curtime = millis();
            speedInRPM = ((newPosition - oldPosition) * 1000 * 60) / ((curtime - prevtime) * CPR);
            //printf("%ld,%d,%d,%d\n", curtime, speedInRPM, i, neweffort); //prints the current time, speed in RPM, input effort and adjusted effort
            oldPosition = newPosition;
            prevtime = curtime;
        }
        blueMotor.setEffort(0);
    } 
    else if (keyPress == 17)
    { 
        setpoint = 9362.0; //sets the setpoint to the 25 degree roof     
        runpid = true;
        if (!resetted) {
            resetted = true; 
            pid.resetIntegral(); //resets the sum of errors after pressing the button for more accurate PID results
        }
        offsettime = millis();
        
    }
    else if(keyPress == 20){
        runpid = false;
        blueMotor.reset();
        for (int i = 0; i < 255; i++) 
        {

            int neweffort = blueMotor.setEffortWithoutDB(-i); //negative i moves the crank up
            //blueMotor.setEffort(-i);
            //delay(100);
            // Serial.printf("The inputted effort is %d\n", -i);
            // printf("The adjusted effort is %d\n", neweffort);
            newPosition = blueMotor.getPosition();
            curtime = millis();
            speedInRPM = ((newPosition - oldPosition) * 1000 * 60) / ((curtime - prevtime) * CPR);            
            //printf("%ld,%d,%d,%d\n", curtime, speedInRPM, -i, neweffort); //prints the current time, speed in RPM, input effort and adjusted effort
            oldPosition = newPosition;
            prevtime = curtime;
        }
    }
    
    else if (keyPress == 18)
    {
        runpid = false;
        blueMotor.reset();
        if ((now = millis()) > timeToPrint)
        {
            timeToPrint = now + sampleTime;
        }
    }
    else if (keyPress == 21)
{
    setpoint = 0; //sets the target for PID to the position of the staging platform
}
    if (runpid) {        
        // printf("%d\n", blueMotor.setEffortWithoutDB(outputValue));
        neweffort = blueMotor.setEffortWithoutDB(outputValue);
        
    }
}