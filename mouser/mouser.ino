#include "seeed-motorshield.h"
#include "dfrobot-shield-l298p.h"
#include "movement.h"
#include "gyroscope.h"
#include "accelerometer.h"
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
    Accelerometer::instance();

    delay(5000);
}


void logGyroscope(void)
{
    Gyroscope *gyro = Gyroscope::instance();
    gyro->readGyro();
    datalog.add(gyro->x, gyro->y, gyro->z);
}


void logAccelerator(void)
{
    Accelerometer *accel = Accelerometer::instance();
    accel->readAccelerometer();
    datalog.add(accel->x, accel->y, accel->z);
}


void logDelay(int d)
{
    for (int a = 0; a < d; a++)
    {
        logAccelerator();
        delay(100);
    }
}


void loop()
{
    if (isActive())
    {
        delay(10000);

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
