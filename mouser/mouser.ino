#include "seeed-motorshield.h"
#include "dfrobot-shield-l298p.h"
#include "movement.h"
#include "gyroscope.h"
#include "accelerometer.h"
#include "datalog.h"
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
    countdown = 100;
    Serial.begin(9600);
    createSeeedMotors();

    Wire.begin();
    Gyroscope::instance();
    Accelerometer::instance();

    delay(5000);
    normalizeSensors(50);
}


// void logGyroscope(void)
// {
//     Gyroscope *gyro = Gyroscope::instance();
//     gyro->readGyro();
//     datalog.add(gyro->x, gyro->y, gyro->z);
// }


// void logAccelerator(void)
// {
//     Accelerometer *accel = Accelerometer::instance();
//     accel->readAccelerometer();
//     datalog.add(accel->x, accel->y, accel->z);
// }


// void logDelay(int d)
// {
//     for (int a = 0; a < d; a++)
//     {
//         logAccelerator();
//         delay(100);
//     }
// }


void strategy1()
{
    const int GYRO_MAX_VALUE    = 1024;
    const int GYRO_MAX_X        = (GYRO_MAX_VALUE - 1);
    const int GYRO_DIVIDER_X    = (GYRO_MAX_VALUE / 128);
    const int GYRO_FLOOR        = 128;

    Gyroscope *gyro = Gyroscope::instance();
    gyro->readGyro();

    int gx = gyro->x - GYRO_FLOOR;
    if (gx < 0)
        gx = 0;
    else if (gx > GYRO_MAX_X)
        gx = GYRO_MAX_X;

    int speed = (GYRO_MAX_X - gx) / GYRO_DIVIDER_X + 128;
    Movement::instance()->goForward((uint8_t) speed);

    datalog.add(gx, 0, speed);
}


void strategy2()
{
    static int mode = 0;
    const uint8_t max_speed         = 255;
    const uint8_t base_speed        = 127;
    const uint8_t add_speed         = 32;
    static uint8_t current_speed    = base_speed;

    if (current_speed == max_speed)
        return;

    if (mode == 0)
    {
        mode += 1;
        Movement::instance()->goForward(current_speed);
    }
    else if (mode == 1)
    {
        Gyroscope *gyro = Gyroscope::instance();
        gyro->readGyro();
        if (gyro->x > 16)
            mode += 1;
        datalog.add(gyro->x, gyro->z, current_speed);
    }
    else if (mode == 2)
    {
        Gyroscope *gyro = Gyroscope::instance();
        gyro->readGyro();
        if (gyro->x < 0)
        {
            current_speed += add_speed;
            Movement::instance()->goForward(current_speed);
        }
        datalog.add(gyro->x, gyro->z, current_speed);
    }
}


void spin_it()
{
    static int mode = 0;
    static float degreesTurned = 0.0;

    if (mode == 0)
    {
        Movement::instance()->turnLeft(255);
        mode += 1;
    }
    else if (mode == 1)
    {
        Gyroscope *gyro = Gyroscope::instance();
        gyro->readGyro();
        float dps = ((float) abs(gyro->z)) * 0.00875;
        degreesTurned += 0.1 * dps;
        if (degreesTurned >= (360.0 - dps / 10.0))
        {
            Movement::instance()->stop();
            degreesTurned = 0.0;
            mode += 1;
            Movement::instance()->turnRight(255);
        }
    }
    else if (mode == 2)
    {
        Gyroscope *gyro = Gyroscope::instance();
        gyro->readGyro();
        float dps = ((float) abs(gyro->z)) * 0.00875;
        degreesTurned += 0.1 * dps;
        if (degreesTurned >= (360.0 - dps / 4.0))
        {
            Movement::instance()->stop();
            mode += 1;
        }
    }
}


void loop()
{
    // Gyroscope::instance()->readGyro();
    // delay(1000);
    if (isActive() && countdown > 0)
    {
        //countdown--;
        spin_it();
        delay(100);
    }
    else if (countdown == 0)
    {
        Movement::instance()->stop();
        delay(30000);
        datalog.dump();
        countdown = -1;
    }
}
