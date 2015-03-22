#include "seeed-motorshield.h"
#include "dfrobot-shield-l298p.h"
#include "movement.h"
#include "gyroscope.h"
#include "accelerometer.h"
#include "datalog.h"
#include "geometry.h"
#include <math.h>
#include <Wire.h>


DataLog datalog;
int countdown;


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


void normalizeSensors(int d)
{
    for (int a = 0; a < d; a++)
    {
        Gyroscope::instance()->normalize();
        delay(100);
    }
}


void setup()
{
    countdown = 24;
    Serial.begin(9600);
    createSeeedMotors();

    Wire.begin();
    Gyroscope::instance();
    Accelerometer::instance();

    delay(5000);
    normalizeSensors(50);
}


void loop()
{
    const unsigned long TICK = 100;
    static unsigned long last_tick = 0;
    unsigned long current_tick = millis();

    if (current_tick - last_tick >= TICK)
    {
        last_tick = current_tick;
    }
}
