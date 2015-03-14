#include "motor.h"

Motor motor_left(Motor::MOTORSHIELD_IN1, Motor::MOTORSHIELD_IN2, Motor::SPEEDPIN_A);
Motor motor_right(Motor::MOTORSHIELD_IN4, Motor::MOTORSHIELD_IN3, Motor::SPEEDPIN_B);

int count = Motor::MAX_SPEEDS;
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
        motor_left.setSpeed(Motor::MAX_SPEEDS-1);
        motor_right.setSpeed(Motor::MAX_SPEEDS-1);
        motor_left.setDirection(Motor::FORWARD);
        motor_right.setDirection(Motor::FORWARD);
    }
}

void loop()
{
    if (isActive())
    {
        if (--count == 0)
        {
            count = Motor::MAX_SPEEDS;

            if (flag)
            {
                motor_left.setSpeed(Motor::MAX_SPEEDS-1);
                motor_right.setSpeed(Motor::MAX_SPEEDS-1);
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
