#include "seeed-motorshield.h"
#include "dfrobot-shield-l298p.h"
#include "movement.h"
#include "gyroscope.h"
#include "datalog.h"
#include <Wire.h>


DataLog datalog;


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


void logDelay(int d)
{
    Gyroscope *gyro = Gyroscope::instance();
    for (int a = 0; a < d; a++)
    {
        gyro->readGyro();
        datalog.add(gyro->x, gyro->y, gyro->z);
        delay(100);
    }
}


void loop()
{
    if (isActive())
    {
        delay(10000);
        Gyroscope::instance()->readGyro();
        datalog.add(Gyroscope::instance()->x, Gyroscope::instance()->y, Gyroscope::instance()->z);

        // Movement::instance()->goForward(192);
        // logDelay(8);

        Movement::instance()->goForward(255);
        logDelay(50);

        Movement::instance()->stop();
        logDelay(20);

        // Movement::instance()->turnLeft(255);
        // logDelay(20);

        // Movement::instance()->stop();
        // logDelay(20);

        // Movement::instance()->turnRight(255);
        // logDelay(20);

        // Movement::instance()->stop();
        // logDelay(20);

        // Movement::instance()->goBackward(192);
        // logDelay(8);

        // Movement::instance()->goBackward(255);
        // logDelay(50);

        // Movement::instance()->stop();
        // logDelay(20);

        delay(30000);
        datalog.dump();
    }
}
