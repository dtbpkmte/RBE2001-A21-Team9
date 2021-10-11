#pragma once

class BlueMotor {
 public:
  void setEffort(int effort);
  int setEffortWithoutDB(int effort);
  long getPosition();
  void reset();
  void setup();
  void stop();
  //    static portMUX_TYPE mux;

 private:
  void setEffort(int effort, bool clockwise);
  const int PWM = 5;
  const int AIN2 = 23;
  const int AIN1 = 27;
  const int ENCA = 21;
  const int ENCB = 18;
};