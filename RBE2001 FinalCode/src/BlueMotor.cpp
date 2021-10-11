#include "BlueMotor.h"

#include <RBE1001Lib.h>

long count = 0;  // encoder counter
long errorCount = 0;
// Mutex for the count critical variable
portMUX_TYPE mux = portMUX_INITIALIZER_UNLOCKED;


const int X = 5;
int encoderArray[4][4] = {
  {0, -1, 1, X},
  {1, 0, X, -1},
  {-1, X, 0, 1},
  {X, 1, -1, 0}};

int newValue, oldValue;

/**
 * Interrupt service routine (ISR) for one of the encoder
 * channels. It simply counts how many interrupts occured
 */
void IRAM_ATTR isr() {

  newValue = ((digitalRead(21) << 1) | digitalRead(18));
  int value = encoderArray[oldValue][newValue];
  if (value == X){
      errorCount++;
  }
     else{
     portENTER_CRITICAL_ISR(&mux);
      count -= value;
    portEXIT_CRITICAL_ISR(&mux);
     }
  
  // else{
  //   count++;
  // }
  oldValue = newValue;
  //  count++;
}

/**
 * Set up all the variables for the blue motor
 * This is not the same as the ESP32 setup() function, although
 * that would be a good place to call this setup function
 */
void BlueMotor::setup() {
  pinMode(ENCA, INPUT);
  pinMode(ENCB, INPUT);
  pinMode(AIN1, OUTPUT);
  pinMode(AIN2, OUTPUT);
  digitalWrite(AIN1, LOW);
  digitalWrite(AIN2, HIGH);
  pinMode(PWM, OUTPUT);
  attachInterrupt(ENCA, isr, CHANGE);
  attachInterrupt(ENCA, isr, CHANGE);
}

/**
 * Get the current encoder count
 * Returns the encoder count value
 */
long BlueMotor::getPosition() {
  long value;
  portENTER_CRITICAL(&mux);
  value = count;
  portEXIT_CRITICAL(&mux);
  return value;
}

/**
 * Reset the encoder count to zero
 */
void BlueMotor::reset() {
  portENTER_CRITICAL(&mux);
  count = 0;
  portEXIT_CRITICAL(&mux);
}

/**
 * Set the motor effort
 * effort value ranges from -255 to +255
 * Negative values turn one way, positive values turn the other way
 */
void BlueMotor::setEffort(int effort) {
  if (effort < 0) {
    setEffort(-effort, true);
  } else {
    setEffort(effort, false);
  }
}

void BlueMotor::stop(){
  setEffort(0);
}

/**
 * Set the motor effort
 * effort values range from 0-255
 * clockwise is true for one direction, false for the other
 */
void BlueMotor::setEffort(int effort, bool clockwise) {
  if (clockwise) {
    digitalWrite(AIN1, HIGH);
    digitalWrite(AIN2, LOW);
  } else {
    digitalWrite(AIN1, LOW);
    digitalWrite(AIN2, HIGH);
  }
  int value = constrain(effort, 0, 255);
  analogWrite(PWM, value);
}

int BlueMotor::setEffortWithoutDB(int effort){
  int neweffort = 0;
  if(effort > 0){
    neweffort = map(effort, 0, 255, 134, 255); //scales the range of 0-255 to 134-255 to remove the deadband on the motor. 
                                               //the measured deadband going up was around 170 but was adjusted to account for excessive oscillations with the PID calculations
  }
  else if(effort < 0){
    neweffort = map(effort, -255, 0, -255, -134); //scales the range of 0 - -255 to -134 - -255 to remove the deadband on the motor 
  } else {
    neweffort = 0;
  }
  setEffort(neweffort);
  return neweffort;
}
enum PLACING {Roof25Degrees, Roof45Degrees, StagingPlatform};
