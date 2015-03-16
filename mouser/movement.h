#include <Arduino.h>
#include "interfaces.h"


class Movement : public IMovement
{
private:

    static Movement *_pInstance;

    IMotor *_pLeftMotor;
    IMotor *_pRightMotor;

    Movement(IMotor *pLeftMotor, IMotor *pRightMotor);


public:

    static void initialize(IMotor *pLeftMotor, IMotor *pRightMotor);
    static IMovement *instance(void);

    virtual void stop(void);

    virtual void setSpeed(uint8_t speed);
    virtual void goForward(void);
    virtual void goBackward(void);
    virtual void turnLeft(void);
    virtual void turnRight(void);

    virtual void goForward(uint8_t speed);
    virtual void goBackward(uint8_t speed);
    virtual void turnLeft(uint8_t speed);
    virtual void turnRight(uint8_t speed);

    virtual void adjustForYaw(int16_t yaw);

};
