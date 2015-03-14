#include "seeed-motorshield.h"

using namespace Seeed;


IMotor *Motor::createLeftSide(void)
{
    return new Motor(MOTORSHIELD_IN1, MOTORSHIELD_IN2, SPEEDPIN_A);
}


IMotor *Motor::createRightSide(void)
{
    return new Motor(MOTORSHIELD_IN4, MOTORSHIELD_IN3, SPEEDPIN_B);
}


Motor::Motor(Pin out_pin_1, Pin out_pin_2, Pin speed_pin)
:   _out_pin_1(out_pin_1),
    _out_pin_2(out_pin_2),
    _speed_pin(speed_pin),
    _direction(FORWARD)
{
    pinMode(_out_pin_1, OUTPUT);
    pinMode(_out_pin_2, OUTPUT);
    pinMode(_speed_pin, OUTPUT);
}


void Motor::stop(void)
{
    analogWrite(_speed_pin, 0);
}


void Motor::setSpeed(uint8_t speed)
{
    analogWrite(_speed_pin, speed);
}


void Motor::goForward(void)
{
    digitalWrite(_out_pin_1, HIGH);
    digitalWrite(_out_pin_2, LOW);
}


void Motor::goForward(uint8_t speed)
{
    goForward();
    setSpeed(speed);
}


void Motor::goBackward(void)
{
    digitalWrite(_out_pin_1, LOW);
    digitalWrite(_out_pin_2, HIGH);
}


void Motor::goBackward(uint8_t speed)
{
    goBackward();
    setSpeed(speed);
}
