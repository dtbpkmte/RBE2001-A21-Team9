
int servoPin = 33;
int linearPotPin = 34;
int servoStop = 95;
int servoJawDown = 50;
int servoJawUp = 130;
int printDelay = 500;
float linearPotVoltage = 2.0;
float jawOpenPotVoltage = 1.0;
float jawClosedPotVoltage = 3.05;

ESP32AnalogRead linearPot;
Servo jawServo;

Timer printTimer(printDelay);


void setup()
{
  Serial.begin(9600);
  linearPot.attach(linearPotPin);
  jawServo.attach(servoPin);
}

void loop()
{
  // Stop servo
  jawServo.write(servoStop);
  delay(500);
  // Get Pot Value
  linearPotVoltage = linearPot.readVoltage();
  printf("Initial linearPotVoltage:  %f\n", linearPotVoltage);
  
  // Move Jaw Down
  jawServo.write(servoJawDown);

  while (linearPotVoltage > jawOpenPotVoltage)
  {
    linearPotVoltage = linearPot.readVoltage();
    if (printTimer.isExpired()){
      printf("linearPotVoltage:  %f\n", linearPotVoltage);
    }
  }

  // Stop servo
  jawServo.write(servoStop);

  linearPotVoltage = linearPot.readVoltage();
  printf("Bottom linearPotVoltage Before Delay:  %f\n", linearPotVoltage);
  delay(5000);
  linearPotVoltage = linearPot.readVoltage();
  printf("Bottom linearPotVoltage After Delay:  %f\n", linearPotVoltage);
  delay(5000);


  // Move Jaw Up
  jawServo.write(servoJawUp);

  while (linearPotVoltage < jawClosedPotVoltage)
  {
    linearPotVoltage = linearPot.readVoltage();
    if (printTimer.isExpired()){
      printf("linearPotVoltage:  %f\n", linearPotVoltage);
    }
  }
  
  // Stop servo
  jawServo.write(servoStop);

  linearPotVoltage = linearPot.readVoltage();
  printf("Final linearPotVoltage Before Delay:  %f\n", linearPotVoltage);
  delay(5000);
  linearPotVoltage = linearPot.readVoltage();
  printf("Final linearPotVoltage After Delay:  %f\n", linearPotVoltage);
  delay(5000);
  
}