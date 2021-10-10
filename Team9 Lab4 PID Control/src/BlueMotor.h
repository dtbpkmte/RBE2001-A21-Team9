#pragma once

#include <RBE1001Lib.h>

class BlueMotor {
  // friend void IRAM_ATTR isr();
  
 public:
  void setEffort(int effort);
  long getPosition();
  void reset();
  void setup();
  int setEffortWithoutDB(int effort);
  void moveToPosition(int position);
  //    static portMUX_TYPE mux;

 private:
  void setEffort(int effort, bool clockwise);
  const int PWM = 5;
  const int AIN2 = 23;
  const int AIN1 = 27;
  const static int ENCA = 21;
  const static int ENCB = 18;
};