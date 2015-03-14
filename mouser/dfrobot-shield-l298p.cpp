#include "dfrobot-shield-l298p.h"


using namespace DFRobot;


void log(char *s, unsigned int v, unsigned int format = DEC)
{
    Serial.print(s);
    Serial.println(v, format);
}


IMotor *Motor::createLeftSide(void)
{
    return new Motor(DIRECTION_MOTOR_1, PWM_MOTOR_1, HIGH, LOW);
}


IMotor *Motor::createRightSide(void)
{
    return new Motor(DIRECTION_MOTOR_2, PWM_MOTOR_2, LOW, HIGH);
}


Motor::Motor(Pin direction_pin, Pin pwm_pin, uint8_t forward_value, uint8_t backward_value)
:   _direction_pin(direction_pin),
    _pwm_pin(pwm_pin),
    _forward_value(forward_value),
    _backward_value(backward_value)
{
    log("direction pin: ", _direction_pin);
    log("pwm_pin:       ", _pwm_pin);
    pinMode(_direction_pin, OUTPUT);
    pinMode(_pwm_pin, OUTPUT);
}


void Motor::stop(void)
{
    analogWrite(_pwm_pin, 0);
}


void Motor::setSpeed(uint8_t speed)
{
    analogWrite(_pwm_pin, speed);
}


void Motor::goForward(void)
{
    digitalWrite(_direction_pin, _forward_value);
}


void Motor::goForward(uint8_t speed)
{
    goForward();
    setSpeed(speed);
}


void Motor::goBackward(void)
{
    digitalWrite(_direction_pin, _backward_value);
}


void Motor::goBackward(uint8_t speed)
{
    goBackward();
    setSpeed(speed);
}
