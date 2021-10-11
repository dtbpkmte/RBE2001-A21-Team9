#include <main.h>
#include "Scheduler.h"


Scheduler sched;

bool turnAnglePID(float angle) {
  return chassis.turnAnglePID(angle);
}

void setup()
{
  Serial.begin(9600);
  blueMotor.setup();
  decoder.init();
  ultrasonic.attach(SIDE_ULTRASONIC_TRIG, SIDE_ULTRASONIC_ECHO);
  pidUltra.SetMode(AUTOMATIC);
  pidUltra.SetOutputLimits(-1.0, 1.0);
  pidMotor.SetMode(AUTOMATIC);
  pidMotor.SetOutputLimits(-255.0, 255.0);
  lifter.attach(SERVO_PIN);
  servoPositionFeedback.attach(SERVO_FEEDBACK_SENSOR);
  lifter.write(170);
  // Sched test
  sched.addSequentialTask(*turnAnglePID, 90);
  sched.addSequentialTask(*turnAnglePID, -180);
  sched.addSequentialTask(*turnAnglePID, -90);

}
//
void pidDriving(double targetDistance)
{
  setpointUltra = targetDistance;
  if (inputValue > 0)
  {

    chassis.drive(outputValue);
  }
  if (printTimer.isExpired())
  {
    printf("i = %lf o = %lf\n", inputValue, outputValue);
  }
}
//
void pidPlacing(double targetPosition)
{
  setpointMotor = targetPosition;
  if (inputValue >= 0)
  {
    pidMotor.Compute();
    blueMotor.setEffortWithoutDB(outputValue);
  }
  //
}
void HandleIntersection(void)
{
  TURN turning = navigator.CalcTurn();
  ultrasonic.getDistanceCM();
  //
  switch (turning) // might want to turn over the intersection (course is tight)
  {
  case TURN_LEFT:
    chassis.turnLeft();
    break;
    //
  case TURN_RIGHT:
    chassis.turnRight();
    break;
    //
  case TURN_STRAIGHT:
    chassis.drive(100);
    delay(500);
    chassis.stop();
    delay(800);
    Serial.println("Passed Intersection");
    break;
    //
  case TURN_UTURN:
    chassis.turnAround();
    break;
  }
  //
  //
}
//
//
void loop()
{
  //
  int16_t keyPress = decoder.getKeyCode();

  sched.loop();

  //Serial.printf("Ultrasonic reading :  %f", ultrasonic.getDistanceCM());
  inputValueUltra = ultra.getDistanceCM();
  //Pid Distance for Ultrasonic to get to the right position
  pidUltra.Compute();
  //Emergency stop situation
  //Break and resume the previous task if hit play button
  if (keyPress == STOP_BUTTON)
  {
    sched.pause();
    Serial.println("Emergency STOP");
    while (true)
    {
      chassis.stop();
      blueMotor.stop();
      if ((keyPress = decoder.getKeyCode()) == PLAY)
      {
        sched.resume();
        break;
      }
    }
  }

  //
  // If a valid key is pressed, print out its value
  if (keyPress >= 0)
  {
    Serial.println("Key: " + String(keyPress));
  }
  //
  //IDLE STATE
  if (robotState == ROBOT_IDLE)
  {
    if (keyPress == ENTER)
    {
      // Notify us that we're switching to ACTIVE
      Serial.println("Button press -> ACTIVE\n");
      robotState = ROBOT_ACTIVE;
    }
  }
  //
  //ACTIVE STATE
  if (robotState == ROBOT_ACTIVE)
  {
    if (keyPress == KEY1)
    {
      Serial.printf("Going to the House\n");
      navigator.updateDestination(keyPress);
      updateKey = keyPress;
      robotState = ROBOT_LINING;
    }
    else if (keyPress == KEY2)
    {
      Serial.printf("Going to the Platform1 \n");
      navigator.updateDestination(keyPress);
      updateKey = keyPress;
      robotState = ROBOT_LINING;
    }
    else if (keyPress == KEY3)
    {
      Serial.printf("Going to the Platform2 \n");
      navigator.updateDestination(keyPress);
      robotState = ROBOT_LINING;
    }
    else if (keyPress == PLAY)
    {
      printf("Testing distance");
      pidDriving(20);
    }
    else if (keyPress == KEY5)
    {
      turn180();
      robotState = ROBOT_TURN180;
      printf("Turn Around test");
    }
    else if (keyPress == VOL_PLUS)
    {
      printf("Turn right");
      // chassis.turnAnglePID(90);
      sched.resume();
    }
    else if (keyPress == VOL_MINUS)
    {
      printf("Turn left");
      chassis.turnLeft();
    }
    else if (keyPress == KEY4)
    {
      float degree = chassis.left_motor.getCurrentDegrees();
      printf("Degree at leftMotor :  %f\n", degree);
      printf("Degree at rightMotor : %f\n", chassis.right_motor.getCurrentDegrees());
    }
    else if(keyPress == KEY7){
      robotState = ROBOT_GRIPPING;
    }
  }
  if (robotState == ROBOT_TURN180)
  {
    if (CheckDistanceTravelled())
    {
      chassis.stop();
      robotState = ROBOT_ACTIVE;
    }
  }
  //
  //LINING STATE
  if (robotState == ROBOT_LINING)
  {
    printf("Robot is now in Lining mode \n");
    lineFollow(40);
    if (meetIntersection())
    {
      chassis.stop();
      delay(1000);
      if (navigator.ReachDestination())
      {
        navigator.updateDestination(keyPress);
        printf("Reach destination");
        //Wait for human signal to know if the mission is to
        //place to lift the collector
      }
    }
  }
  if (robotState == ROBOT_LIFTING)
  {
    //Turn the robot around
    //Adjusting the distance
    //Lift the fourbar to the angles

    //on road B-> in front of the 45 degree roof
    //on road C-> in front of the 25 degree roof
    //ROADA/D-> in front of the box -> Bottom position (0 degree)
    printf("Robot is now in Lifting mode\n");
    // if(navigator.isOnRoadB()){
    //   printf("Lifting the collector from 45 degree \n");
    //   //when done lifting, change to robot gripping

    // } else if (navigator.isOnRoadC()){
    //   //when done lifting, change to robot gripping
    //   printf("Lifting the collector from 25 degree \n");
    // }
    //   else if(navigator.isOnRoadA() || navigator.isOnRoadD()){
    //   //when done lifting, change to robot gripping
    //   printf("Lifting the collector from the box\n");
    // }
    //
  }
  if (robotState == ROBOT_PLACING)
  {
    bool donePLacing = false;
    //TODO : Adjusting the position before placing
    //Placing the fourbar to the angles to place the collector
    //on road B-> in front of the 45 degree roof
    //on road C-> in front of the 25 degree roof
    //ROADA/D-> in front of the box -> Bottom position (0 degree)

    printf("Robot is now in Placing mode\n");
    // if(navigator.isOnRoadA()|| navigator.isOnRoadD()){
    //   setpoint= 0;
    //   pidPlacing(0);
    //   donePLacing = true;
    // }
    // else if(navigator.isOnRoadC()){
    //   setpoint = 9362;
    //   pidPlacing(9362);
    //   donePLacing = true;
    // }
    // else if(navigator.isOnRoadB()){
    //   setpoint = 4803;
    //   pidPlacing(4802.7);
    //   bool donePLacing = true;
    // } 
  }

  if (robotState == ROBOT_GRIPPING)
  {
    if(keyPress == 25){
        lifter.write(120);
    }
    else if(keyPress == 26){
      lifter.write(170);
      }
    //Key Vol- -> Lower the jaw
    //Key Vol+ -> Gripping (move up the lower jaw)

    //After done gripping, stop and wait for the human signal to continue
  }
  //
}
