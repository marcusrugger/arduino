#include "seeed-motorshield.h"
#include "dfrobot-shield-l298p.h"
#include "movement.h"
#include "gyroscope.h"
#include <Wire.h>


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


void setup()
{
    Serial.begin(9600);
    createSeeedMotors();
    Wire.begin();

    delay(2000);

    Gyroscope::instance();

    delay(5000);
}


void loop()
{
    Gyroscope::instance()->readGyro();
    delay(1000);

    if (isActive())
    {
        Movement::instance()->goForward(192);
        delay(800);

        Movement::instance()->goForward(255);
        delay(5000);

        Movement::instance()->stop();
        delay(2000);

        Movement::instance()->turnLeft(255);
        delay(2000);

        Movement::instance()->stop();
        delay(2000);

        Movement::instance()->turnRight(255);
        delay(2000);

        Movement::instance()->stop();
        delay(2000);

        Movement::instance()->goBackward(192);
        delay(800);

        Movement::instance()->goBackward(255);
        delay(5000);

        Movement::instance()->stop();
        delay(2000);
    }
}


#if 0
Seeed::MotorShield motor_left(Seeed::MotorShield::MOTORSHIELD_IN1, Seeed::MotorShield::MOTORSHIELD_IN2, Seeed::MotorShield::SPEEDPIN_A);
Seeed::MotorShield motor_right(Seeed::MotorShield::MOTORSHIELD_IN4, Seeed::MotorShield::MOTORSHIELD_IN3, Seeed::MotorShield::SPEEDPIN_B);

int count = Seeed::MotorShield::MAX_SPEEDS;
bool flag = false;

bool isActive(void)
{
  return false;
}

void setup()
{
  Serial.begin(9600);
  if (isActive())
  {
    motor_left.setSpeed(Seeed::MotorShield::MAX_SPEEDS - 1);
    motor_right.setSpeed(Seeed::MotorShield::MAX_SPEEDS - 1);
    motor_left.setDirection(Seeed::MotorShield::FORWARD);
    motor_right.setDirection(Seeed::MotorShield::FORWARD);
  }
}

void loop()
{
  if (isActive())
  {
    if (--count == 0)
    {
      count = Seeed::MotorShield::MAX_SPEEDS;

      if (flag)
      {
        motor_left.setSpeed(Seeed::MotorShield::MAX_SPEEDS - 1);
        motor_right.setSpeed(Seeed::MotorShield::MAX_SPEEDS - 1);
        flag = false;
      }
      else
      {
        motor_left.setSpeed(0);
        motor_right.setSpeed(0);
        flag = true;
      }

      delay(5000);
    }


    motor_left.tick();
    motor_right.tick();
    delay(100);
  }
}
#endif
