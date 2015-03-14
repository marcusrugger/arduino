#include "movement.h"


Movement *Movement::_pInstance = NULL;


Movement::Movement(IMotor *pLeftMotor, IMotor *pRightMotor)
:   _pLeftMotor(pLeftMotor),
    _pRightMotor(pRightMotor)
{
}


void Movement::initialize(IMotor *pLeftMotor, IMotor *pRightMotor)
{
    _pInstance = new Movement(pLeftMotor, pRightMotor);
}


IMovement *Movement::instance(void)
{
    return _pInstance;
}


void Movement::stop(void)
{
    _pLeftMotor->stop();
    _pRightMotor->stop();
}


void Movement::setSpeed(uint8_t speed)
{
    _pLeftMotor->setSpeed(speed);
    _pRightMotor->setSpeed(speed);
}


void Movement::goForward(void)
{
    _pLeftMotor->goForward();
    _pRightMotor->goForward();
}


void Movement::goBackward(void)
{
    _pLeftMotor->goBackward();
    _pRightMotor->goBackward();
}


void Movement::turnLeft(void)
{
    _pLeftMotor->goBackward();
    _pRightMotor->goForward();
}


void Movement::turnRight(void)
{
    _pLeftMotor->goForward();
    _pRightMotor->goBackward();
}


void Movement::goForward(uint8_t speed)
{
    setSpeed(speed);
    goForward();
}


void Movement::goBackward(uint8_t speed)
{
    setSpeed(speed);
    goBackward();
}


void Movement::turnLeft(uint8_t speed)
{
    setSpeed(speed);
    turnLeft();
}


void Movement::turnRight(uint8_t speed)
{
    setSpeed(speed);
    turnRight();
}
