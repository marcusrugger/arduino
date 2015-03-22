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
        Accelerometer::instance()->normalize();
        delay(100);
    }
}


void setup()
{
    countdown = 24;
    Serial.begin(9600);
    createSeeedMotors();

    delay(1000);

    Wire.begin();
    Gyroscope::instance();
    Accelerometer::instance();

    delay(1000);
    normalizeSensors(50);
}


void loop()
{
    const unsigned long TICK = 100;
    static unsigned long last_tick = 0;
    unsigned long current_tick = millis();
    unsigned long dt = current_tick - last_tick;

    if (dt >= TICK)
    {
        if (isActive())
            tick(dt);
        else
            Movement::instance()->stop();

        last_tick = current_tick;
    }
}


void tick(unsigned long dt)
{
    static int mode = 0;

    static float current_distance;
    static Geometry::Point current_velocity;
    static Geometry::Point current_position;

    static Geometry::Matrix44 robot_matrix;

    Accelerometer   *ameter = Accelerometer::instance();
    Gyroscope       *gscope = Gyroscope::instance();

    ameter->readAccelerometer();
    gscope->readGyroscope();

    if (mode == 0)
    {
        //Movement::instance()->goForward(255);
        mode = 1;

        Serial.print("accelerometer adjustments: ");
        Serial.print(ameter->d.x); Serial.print(", ");
        Serial.print(ameter->d.y); Serial.print(", ");
        Serial.println(ameter->d.z);

        float a = 30.0;
        Geometry::Point p(0.0, 0.0, 1.0);

        {
            Geometry::RotationMatrix m = Geometry::RotationMatrix::createYaw(Geometry::DegToRad(a));
            Geometry::Point r(m.rotate(p));

            Serial.print("rotate yaw:   ");
            Serial.print(r.x); Serial.print(", ");
            Serial.print(r.y); Serial.print(", ");
            Serial.println(r.z);
        }

        {
            Geometry::RotationMatrix m = Geometry::RotationMatrix::createPitch(Geometry::DegToRad(a));
            Geometry::Point r(m.rotate(p));

            Serial.print("rotate pitch: ");
            Serial.print(r.x); Serial.print(", ");
            Serial.print(r.y); Serial.print(", ");
            Serial.println(r.z);
        }

        {
            Geometry::RotationMatrix m = Geometry::RotationMatrix::createRoll(Geometry::DegToRad(a));
            Geometry::Point r(m.rotate(p));

            Serial.print("rotate roll:  ");
            Serial.print(r.x); Serial.print(", ");
            Serial.print(r.y); Serial.print(", ");
            Serial.println(r.z);
        }

        {
            Geometry::RotationMatrix yaw    = Geometry::RotationMatrix::createYaw(Geometry::DegToRad(a));
            Geometry::RotationMatrix pitch  = Geometry::RotationMatrix::createPitch(Geometry::DegToRad(a));
            Geometry::RotationMatrix roll   = Geometry::RotationMatrix::createRoll(Geometry::DegToRad(a));
            Geometry::Point r;

            r = yaw.rotate(p);
            r = pitch.rotate(r);
            r = roll.rotate(r);

            Serial.print("all three:  ");
            Serial.print(r.x); Serial.print(", ");
            Serial.print(r.y); Serial.print(", ");
            Serial.println(r.z);
        }

        {
            Geometry::RotationMatrix yaw    = Geometry::RotationMatrix::createYaw(Geometry::DegToRad(a));
            Geometry::RotationMatrix pitch  = Geometry::RotationMatrix::createPitch(Geometry::DegToRad(a));
            Geometry::RotationMatrix roll   = Geometry::RotationMatrix::createRoll(Geometry::DegToRad(a));

            Geometry::RotationMatrix matrix = roll * pitch * yaw;

            Geometry::Point r;
            r = matrix.rotate(p);

            Serial.print("big matrix:  ");
            Serial.print(r.x); Serial.print(", ");
            Serial.print(r.y); Serial.print(", ");
            Serial.println(r.z);
        }

        mode = 3;        
    }
    else if (mode == 1)
    {
        float t = ((float) dt) / 1000.0;
        Geometry::Point gravity(0.0, 0.0, -980.67 * t);

        Serial.print("dt: "); Serial.println(t);

        // Serial.print("gscope: ");
        // Serial.print(gscope->a.a); Serial.print(", ");
        // Serial.print(gscope->a.b); Serial.print(", ");
        // Serial.println(gscope->a.c);

        // Serial.print("ameter: ");
        // Serial.print(ameter->a.x); Serial.print(", ");
        // Serial.print(ameter->a.y); Serial.print(", ");
        // Serial.println(ameter->a.z);

        Geometry::EulerAngles so(gscope->a.a * t,
                                 gscope->a.b * t,
                                 gscope->a.c * t);

        // Serial.print("so: ");
        // Serial.print(so.a); Serial.print(", ");
        // Serial.print(so.b); Serial.print(", ");
        // Serial.println(so.c);

        Geometry::EulerAngles rdo(so.a * t, so.b * t, so.c * t);
        Geometry::RotationMatrix dom = Geometry::RotationMatrix::createLeftHanded(rdo);

        robot_matrix = robot_matrix.rotate(rdo);
        Geometry::Point da = robot_matrix.rotate(ameter->a) - ameter->d;

        Geometry::Point dv = da * t;

        // Serial.print("dv: ");
        // Serial.print(dv.x); Serial.print(", ");
        // Serial.print(dv.y); Serial.print(", ");
        // Serial.println(dv.z);

        // Serial.print("dv: ");
        // Serial.print(dv.x); Serial.print(", ");
        // Serial.print(dv.y); Serial.print(", ");
        // Serial.println(dv.z);

        current_velocity = current_velocity + dv;

        current_position = current_position + current_velocity * t;

        // Serial.print("dv: ");
        // Serial.print(dv.x); Serial.print(", ");
        // Serial.print(dv.y); Serial.print(", ");
        // Serial.println(dv.z);

        float cv = sqrt(current_velocity.x * current_velocity.x +
                        current_velocity.y * current_velocity.y +
                        current_velocity.z * current_velocity.z);

        Serial.print("current acceleration: ");
        Serial.print(da.x); Serial.print(", ");
        Serial.print(da.y); Serial.print(", ");
        Serial.println(da.z);

        // Serial.print("current velocity: ");
        // Serial.print(current_velocity.x); Serial.print(", ");
        // Serial.print(current_velocity.y); Serial.print(", ");
        // Serial.println(current_velocity.z);

        // Serial.print("current position: ");
        // Serial.print(current_position.x); Serial.print(", ");
        // Serial.print(current_position.y); Serial.print(", ");
        // Serial.println(current_position.z);

        // Geometry::Matrix44 drobot(da, dv);

        // robot_matrix = robot_matrix * drobot;
        // current_position = robot_matrix.translate(Geometry::Point(0.0, 0.0, 0.0));

        // Serial.print("current_position: ");
        // Serial.print(current_position.x); Serial.print(", ");
        // Serial.print(current_position.y); Serial.print(", ");
        // Serial.println(current_position.z);

        // current_distance = sqrt(current_position.x * current_position.x +
        //                         current_position.y * current_position.y +
        //                         current_position.z * current_position.z);

        // Serial.print("Distance: ");
        // Serial.println(current_distance);

        if (current_distance >= 1000.0)
        {
            Movement::instance()->stop();
            mode = 2;
        }
    }
    else
    {}
}
