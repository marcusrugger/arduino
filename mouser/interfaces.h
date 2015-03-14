#ifndef __MOUSER_INTERFACES__
#define __MOUSER_INTERFACES__

class IMotor
{
public:

    virtual void stop(void) = 0;

    virtual void setSpeed(uint8_t speed) = 0;
    virtual void goForward(void) = 0;
    virtual void goBackward(void) = 0;

    virtual void goForward(uint8_t speed) = 0;
    virtual void goBackward(uint8_t speed) = 0;

};


class IMovement
{
public:

    virtual void stop(void) = 0;

    virtual void setSpeed(uint8_t speed) = 0;
    virtual void goForward(void) = 0;
    virtual void goBackward(void) = 0;
    virtual void turnLeft(void) = 0;
    virtual void turnRight(void) = 0;

    virtual void goForward(uint8_t speed) = 0;
    virtual void goBackward(uint8_t speed) = 0;
    virtual void turnLeft(uint8_t speed) = 0;
    virtual void turnRight(uint8_t speed) = 0;

};

#endif
