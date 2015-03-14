#include <Arduino.h>
#include "interfaces.h"


namespace Seeed
{

class Motor : public IMotor
{
private:

    enum Pin
    {
        MOTORSHIELD_IN1 = 8,
        MOTORSHIELD_IN2 = 11,
        MOTORSHIELD_IN3 = 12,
        MOTORSHIELD_IN4 = 13,
        SPEEDPIN_A      = 9,
        SPEEDPIN_B      = 10
    };

    enum Direction
    {
        FORWARD,
        BACKWARD
    };


private:

    Direction _direction;

    Pin _out_pin_1;
    Pin _out_pin_2;
    Pin _speed_pin;


private:

    Motor(Pin out_pin_1, Pin out_pin_2, Pin speed_pin);


public:

    static IMotor *createLeftSide(void);
    static IMotor *createRightSide(void);

    virtual void stop(void);
    virtual void setSpeed(uint8_t speed);
    virtual void goForward(void);
    virtual void goForward(uint8_t speed);
    virtual void goBackward(void);
    virtual void goBackward(uint8_t speed);

};

}
