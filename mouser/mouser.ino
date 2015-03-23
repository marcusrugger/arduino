#include "seeed-motorshield.h"
#include "dfrobot-shield-l298p.h"
#include "movement.h"
#include "gyroscope.h"
#include "accelerometer.h"
#include "datalog.h"
#include "geometry.h"
#include "dotick.h"
#include <math.h>
#include <Wire.h>


DataLog datalog;
int countdown;


bool isActive(void)
{
  const uint8_t pin = 2;
  pinMode(pin, INPUT_PULLUP);
  return digitalRead(pin) == HIGH;
}


void createDFRobotMotors(void)
{
  IMotor *pMotorLeft  = DFRobot::Motor::createLeftSide();
  IMotor *pMotorRight = DFRobot::Motor::createRightSide();
  Movement::initialize(pMotorLeft, pMotorRight);
}


void createSeeedMotors(void)
{
  IMotor *pMotorLeft  = Seeed::Motor::createLeftSide();
  IMotor *pMotorRight = Seeed::Motor::createRightSide();
  Movement::initialize(pMotorLeft, pMotorRight);
}


void normalizeSensors(int d)
{
  for (int a = 0; a < d; a++)
  {
    Gyroscope::instance()->normalize();
    Accelerometer::instance()->normalize();
    delay(100);
  }
}


void setup()
{
  countdown = 24;
  Serial.begin(9600);
  createSeeedMotors();

  delay(1000);

  Wire.begin();
  Gyroscope::instance();
  Accelerometer::instance();

  delay(1000);
  normalizeSensors(50);
}


void loop()
{
  const unsigned long TICK = 100;
  const unsigned long PRINT = 1000;

  static unsigned long last_tick = 0;
  static unsigned long last_print = 0;

  unsigned long current_tick = millis();
  unsigned long dt = current_tick - last_tick;
  unsigned long dp = current_tick - last_print;

  if (dt >= TICK)
  {
    if (isActive())
      DoTick::instance()->tick(dt);
    else
      Movement::instance()->stop();

    last_tick = current_tick;
  }

  if (dp >= PRINT)
  {
    if (isActive())
      DoTick::instance()->print();

    last_print = current_tick;
  }
}
