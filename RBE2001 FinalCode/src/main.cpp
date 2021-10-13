#include <main.h>
#include "Scheduler.h"


Scheduler sched;
int16_t keyPress;

bool turnAnglePID(float angle) {
  return chassis.turnAnglePID(angle);
}

bool driveDistance(float inches) {
  return chassis.driveDistance(inches);
}

bool stopAll() {
  chassis.stop();
  blueMotor.setEffort(0);
  return true;
}

bool setSetpoint(float angle) {
  setpointMotor = angle;
  pidMotor.resetIntegral();
  return true;
}

// blue motor
bool moveTo(float angle) {
  if (PIDTimer.isExpired()) {
    inputValueMotor = blueMotor.getPosition();
    // float outBM = -KpMotor * (angle - inputValueMotor);
    // printf("%f\n", inputValueMotor = blueMotor.getPosition());
  } 
  if (!withinEpsilon(angle - inputValueMotor, 10)) {
    int neweffort = blueMotor.setEffortWithoutDB(outputValueMotor);
    // printf("%f %f %d\n", inputValueMotor, setpointMotor, neweffort);
    return false;
  }
  blueMotor.setEffortWithoutDB(0);
  return true;
}

bool lowerFourbar(float f) {
  blueMotor.setEffortWithoutDB(255);
  return false;
}

bool openGripper(float f) {
  lifter.attach(SERVO_PIN);
  static long start_time = millis();
  lifter.write(120);
  if (millis() - start_time <= 500)
    return false;
 //lifter.detach();
  return true;
}

bool closeGripper(float f) {
  lifter.attach(SERVO_PIN);
  static long start_time = millis();
  lifter.write(170);
  if (millis() - start_time <= 500)
    return false;
 // lifter.detach();
  return true;
}

bool waitForInput(float key) {
  // printf("Input: %d\n", (int16_t) key);
  if(keyPress == PLAY){
  return true;
  }
return false;
}

bool printTest(float f) {
  printf("123\n");
  return true;
}

void setup()
{
  Serial.begin(9600);
  blueMotor.setup();
  decoder.init();
  ultra.attach(SIDE_ULTRASONIC_TRIG, SIDE_ULTRASONIC_ECHO);
  pidUltra.SetMode(AUTOMATIC);
  pidUltra.SetOutputLimits(-1.0, 1.0);
  pidMotor.SetMode(AUTOMATIC);
  pidMotor.SetOutputLimits(-255.0, 255.0);
  setpointMotor = POSITION_0DEG;
  // lifter.attach(SERVO_PIN);
  servoPositionFeedback.attach(SERVO_FEEDBACK_SENSOR);
  // lifter.write(170);
  closeGripper(1.0);

  
  sched.addPauseTask(*stopAll);
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
  ultra.getDistanceCM();
  //
  switch (turning) // might want to turn over the intersection (course is tight)
  {
  case TURN_LEFT:
    chassis.turnAnglePID(90);
    break;
    //
  case TURN_RIGHT:
    chassis.turnAnglePID(-90);
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
    chassis.turnAnglePID(180);
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
  keyPress = decoder.getKeyCode();


  pidMotor.Compute();
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
      keyPress = decoder.getKeyCode();
      if (keyPress == KEY4) {
        sched.terminateCurrentTask();
        blueMotor.reset(); //this is not gud
        printf("Setup terminated\n");
      } else if (keyPress == PLAY)
      {
        sched.resume();
        break;
      }
      // chassis.stop();
      // blueMotor.stop();
      // printf("key: %d\n", keyPress = decoder.getKeyCode());
      
      


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
    if (keyPress == SETUP) {
      printf("SETUP pressed\n");
      sched.addTask(*lowerFourbar);
      sched.resume();
    }
    if (keyPress == ENTER)
    {
      // Notify us that we're switching to ACTIVE
      Serial.println("Button press -> ACTIVE\n");
      robotState = ROBOT_ACTIVE;
      sched.resume(); // dangerous here
    }
  }
  //
  //ACTIVE STATE
  if (robotState == ROBOT_ACTIVE)
  {
    // printf("Left sense %d\n",analogRead(LEFT_LINE_SENSE));
    // printf("Right sense %d\n",analogRead(RIGHT_LINE_SENSE));
    // delay(50);
      // printf("dist=%f\n", ultra.getDistanceCM());
    
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
      // Serial.printf("Going to the Platform2 \n");
      // navigator.updateDestination(keyPress);
      // robotState = ROBOT_LINING;
      sched.addTask(*openGripper);
    }
    
    else if (keyPress == KEY4)
    {
      // sched.addTask(*followLineUntilIntersection);
      // sched.addTask(*turnAnglePID, 180);
      // sched.addTask(*followLineUntilIntersection);
      // sched.addTask(*turnAnglePID, 180);
      // sched.addTask(*followLineUntilIntersection);
      // sched.addTask(*closeGripper);
      sched.addTask(*driveUntilLine);
    }
    else if (keyPress == KEY5) {
      // sched.addTask(*setSetpoint, POSITION_45DEG);
      // sched.addTask(*moveTo, POSITION_45DEG);
      sched.addTask(*turnAnglePID, 180);
      sched.addTask(*turnAnglePID, 90);
      sched.addTask(*turnAnglePID, -90);
    }
    else if (keyPress == KEY6) {
      sched.addTask(*openGripper);
      sched.addTask(*setSetpoint, POSITION_45DEG-1240);
      sched.addTask(*moveTo, POSITION_45DEG-1240);
      sched.addTask(*followLineUntilDistance, 10.0);
      sched.addTask(*closeGripper);
      sched.addTask(*waitForInput, PLAY);
      sched.addTask(*setSetpoint, 6789.0);
      sched.addTask(*moveTo, 6789.0);
      // sched.addTask(*setSetpoint, POSITION_25DEG - 300);
      // sched.addTask(*moveTo, POSITION_25DEG - 300);
      // // sched.addTask(*followLineUntilIntersection, -1.0);      
      // sched.addTask(*turnAnglePID, 180);
      // sched.addTask(*followLineUntilDistance, 6.0);
      // sched.addTask(*driveDistance, -10);
      sched.addTask(*turnAnglePID, 180);
      // sched.addTask(*driveDistance, -4);
      sched.addTask(*followLineUntilIntersection);
      sched.addTask(*driveDistance, 3.0);
      sched.addTask(*turnAnglePID, 90);
      sched.addTask(*followLineUntilDistance, 5.0);
      sched.addTask(*setSetpoint, 0.0);
      sched.addTask(*moveTo, 0.0);
      sched.addTask(*waitForInput, PLAY);
      sched.addTask(*openGripper);
      // sched.addTask(*setSetpoint, POSITION_25DEG - 100);
      // sched.addTask(*moveTo, POSITION_25DEG - 100);
      //servo
      sched.addTask(*waitForInput);
      sched.addTask(*closeGripper);
      sched.addTask(*setSetpoint, 6789.0);
      sched.addTask(*moveTo, 6789.0);
      sched.addTask(*turnAnglePID, -180);
      sched.addTask(*followLineUntilIntersection);
      sched.addTask(*driveDistance, 3.0);
      sched.addTask(*turnAnglePID, -90);
      sched.addTask(*followLineUntilDistance, 8.0);
      sched.addTask(*setSetpoint, POSITION_45DEG-100);
      sched.addTask(*moveTo, POSITION_45DEG-100);
      sched.addTask(*waitForInput, PLAY);
      sched.addTask(*openGripper);
      sched.addTask(*driveDistance, -3);
      sched.addTask(*turnAnglePID, 180);
      sched.addTask(*closeGripper);
      sched.addTask(*followLineUntilIntersection);
      sched.addTask(*driveDistance, 3.0);
      sched.addTask(*turnAnglePID, 90);
      sched.addTask(*followLineUntilDistance, 7.0);
      sched.addTask(*turnAnglePID, 90);
      sched.addTask(*driveUntilLine);
      sched.addTask(*driveDistance, 3);
      sched.addTask(*turnAnglePID, 90);
      sched.addTask(*followLineUntilIntersection);
    } else if (keyPress == KEY7) {
      sched.addTask(*openGripper);
      sched.addTask(*setSetpoint, POSITION_25DEG + 250);
      sched.addTask(*moveTo, POSITION_25DEG + 250);
      sched.addTask(*followLineUntilDistance, 2.0);
      sched.addTask(*closeGripper);
      sched.addTask(*waitForInput, PLAY);
      sched.addTask(*setSetpoint, POSITION_25DEG - 600);
      sched.addTask(*moveTo, POSITION_25DEG - 600);

      sched.addTask(*driveDistance, -7);
      sched.addTask(*turnAnglePID, 180);
      sched.addTask(*driveDistance, -5);
      sched.addTask(*followLineUntilIntersection);
      sched.addTask(*driveDistance, 3.0);
      sched.addTask(*turnAnglePID, -90);
      sched.addTask(*followLineUntilDistance, 5.0);
      sched.addTask(*setSetpoint, 0.0);
      sched.addTask(*moveTo, 0.0);
      sched.addTask(*waitForInput, PLAY);
      sched.addTask(*openGripper);
      // sched.addTask(*setSetpoint, POSITION_25DEG - 100);
      // sched.addTask(*moveTo, POSITION_25DEG - 100);
      //servo
      sched.addTask(*waitForInput);
      sched.addTask(*closeGripper);
      sched.addTask(*setSetpoint, 6789.0);
      sched.addTask(*moveTo, 6789.0);
      sched.addTask(*turnAnglePID, -180);
      sched.addTask(*followLineUntilIntersection);
      sched.addTask(*driveDistance, 3.0);
      sched.addTask(*turnAnglePID, 90);
      sched.addTask(*followLineUntilDistance, 4.0);
      sched.addTask(*setSetpoint, POSITION_25DEG - 200);
      sched.addTask(*moveTo, POSITION_25DEG - 200);
      sched.addTask(*waitForInput, PLAY);
      sched.addTask(*openGripper);
      sched.addTask(*driveDistance, -3);

      sched.addTask(*turnAnglePID, 180);
      sched.addTask(*closeGripper);
      sched.addTask(*followLineUntilIntersection);
      sched.addTask(*driveDistance, 3.0);
      sched.addTask(*turnAnglePID, -90);
      sched.addTask(*followLineUntilDistance, 7.0);
      sched.addTask(*turnAnglePID, -90);
      sched.addTask(*driveUntilLine);
      sched.addTask(*driveDistance, 3);
      sched.addTask(*turnAnglePID, -90);
      sched.addTask(*followLineUntilIntersection);
    }
    
    else if(keyPress == VOL_PLUS){
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
    sched.addTask(*followLineUntilIntersection);
    if (followLineUntilIntersection(80))
    {
      delay(1000);
      if (navigator.ReachDestination())
      {
        navigator.updateDestination(keyPress);
        printf("Reach destination");

        //Vol_minus  -> place the collector
        //Vol_plus -> grab the collector
        if(keyPress == VOL_MINUS){
          printf("Placing the collector");
          robotState = ROBOT_PLACING;
        } else if (keyPress == VOL_PLUS){
          printf("Lifting the collector");
        }
        //Wait for human signal to know if the mission is to
        //place to lift the collector
      }
    }
  }
  if (robotState == ROBOT_LIFTING){

    bool doneLifting = false;
    //Turn the robot around
    //Adjusting the distance
    //Lift the fourbar to the angles
    
    //on road B-> in front of the 45 degree roof
    //on road C-> in front of the 25 degree roof
    //ROADA/D-> in front of the box -> Bottom position (0 degree)
    printf("Robot is now in Lifting mode\n");
    if(navigator.isOnRoadB()){
      printf("Lifting the collector from 45 degree \n");
      //Grip
      


    } else if (navigator.isOnRoadC()){
      //when done lifting, change to robot gripping
      printf("Lifting the collector from 25 degree \n");
    }
      else if(navigator.isOnRoadA() || navigator.isOnRoadD()){
      //when done lifting, change to robot gripping
      printf("Lifting the collector from the box\n");
    }

    
    
    if(doneLifting){
      chassis.stop();
      printf("Is it safe to lift is up? - Press PLAY if safe\n");
      if(keyPress == KEY7 ){
      printf(" User agree, lifting up the collector \n");
      }
    }
    //
  }
  if (robotState == ROBOT_PLACING)
  {
    bool donePLacing = false;
  //TODO : Adjusting the position before placing
}

}
