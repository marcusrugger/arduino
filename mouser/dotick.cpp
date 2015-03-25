#include "dotick.h"
#include "movement.h"
#include "datalog.h"
#include <math.h>
#include <Wire.h>


DoTick *DoTick::_pInstance = NULL;


DoTick *DoTick::instance(void)
{
    if (_pInstance == NULL)
        _pInstance = new DoTick();

    return _pInstance;
}


void DoTick::tick(unsigned long dt)
{
    static int mode = 0;

    gscope->readGyroscope();
    ameter->readAccelerometer();

    switch (mode)
    {
        case 0:
            mode = mode0(dt);
            break;

        case 1:
            mode = mode1(dt);
            break;

        case 2:
            mode = mode2(dt);
            break;
    }
}


int DoTick::mode0(unsigned long dt)
{
    Serial.print("accelerometer min: ");
    Serial.print(ameter->_min.x);   Serial.print(", ");
    Serial.print(ameter->_min.y);   Serial.print(", ");
    Serial.print(ameter->_min.z);
    Serial.println();

    Serial.print("accelerometer max: ");
    Serial.print(ameter->_max.x);   Serial.print(", ");
    Serial.print(ameter->_max.y);   Serial.print(", ");
    Serial.print(ameter->_max.z);
    Serial.println();

    Serial.print("accelerometer difference: ");
    Serial.print(ameter->_difference.x);   Serial.print(", ");
    Serial.print(ameter->_difference.y);   Serial.print(", ");
    Serial.print(ameter->_difference.z);
    Serial.println();

    //Movement::instance()->goForward(255);
    return 1;
}


int DoTick::mode1(unsigned long dt)
{
    int mode = 1;
    float t = ((float) dt) / 1000.0;
    Geometry::Point gravity(0.0, 0.0, -980.67 * t);

    Geometry::EulerAngles so = gscope->a * t;
    ea = ea + so * 57.2957795;

    Geometry::RotationMatrix yaw    = Geometry::RotationMatrix::createYaw(so.c);
    Geometry::RotationMatrix pitch  = Geometry::RotationMatrix::createPitch(so.a);
    Geometry::RotationMatrix roll   = Geometry::RotationMatrix::createRoll(so.b);

    robot_matrix = roll * pitch * yaw * robot_matrix;
    Geometry::Point da = ameter->a;// - ameter->d;//robot_matrix.rotate(ameter->a);// - ameter->d;

    current_velocity = current_velocity + da * t;

    current_position = current_position + current_velocity * t;

    float cv = sqrt(current_velocity.x * current_velocity.x +
                    current_velocity.y * current_velocity.y +
                    current_velocity.z * current_velocity.z);

    if (current_distance >= 1000.0)
    {
        Movement::instance()->stop();
        mode = 2;
    }

    return mode;
}


int DoTick::mode2(unsigned long dt)
{
    Movement::instance()->stop();
    return 2;
}


void DoTick::print(void)
{
    // Serial.print("gyroscope: ");
    // Serial.print(Gyroscope::instance()->a.a);   Serial.print(", ");
    // Serial.print(Gyroscope::instance()->a.b);   Serial.print(", ");
    // Serial.print(Gyroscope::instance()->a.c);
    // Serial.println();

    Serial.print("accelerometer: ");
    Serial.print(Accelerometer::instance()->rx);   Serial.print(", ");
    Serial.print(Accelerometer::instance()->ry);   Serial.print(", ");
    Serial.print(Accelerometer::instance()->rz);
    Serial.println();

    // Serial.print("angle: ");
    // Serial.print(ea.a);   Serial.print(", ");
    // Serial.print(ea.b);   Serial.print(", ");
    // Serial.print(ea.c);
    // Serial.println();

    // Serial.print("current_velocity: ");
    // Serial.print(current_velocity.x);   Serial.print(", ");
    // Serial.print(current_velocity.y);   Serial.print(", ");
    // Serial.print(current_velocity.z);
    // Serial.println();

    // Serial.print("current_position: ");
    // Serial.print(current_position.x);   Serial.print(", ");
    // Serial.print(current_position.y);   Serial.print(", ");
    // Serial.print(current_position.z);
    // Serial.println();
}
