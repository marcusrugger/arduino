#include <Arduino.h>
#include "interfaces.h"


namespace DFRobot
{

class Motor : public IMotor
{
private:

    enum Pin
    {
        DIRECTION_MOTOR_1   = 4,
        DIRECTION_MOTOR_2   = 7,
        PWM_MOTOR_1         = 5,
        PWM_MOTOR_2         = 6
    };

    Pin _direction_pin;
    Pin _pwm_pin;

    uint8_t _forward_value;
    uint8_t _backward_value;


private:

    Motor(Pin direction_pin, Pin pwm_pin, uint8_t forward_value, uint8_t backward_value);


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