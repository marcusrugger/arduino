#include <Arduino.h>
#include "geometry.h"
#include "gyroscope.h"
#include "accelerometer.h"


class DoTick
{
private:

    Geometry::EulerAngles ea;

    float current_distance;
    Geometry::Point current_velocity;
    Geometry::Point current_position;

    Geometry::RotationMatrix robot_matrix;

    Accelerometer   *ameter;
    Gyroscope       *gscope;

    DoTick(void)
    : ameter(Accelerometer::instance()),
      gscope(Gyroscope::instance())
    {};

    static DoTick *_pInstance;


public:

    static DoTick *instance(void);

    void tick(unsigned long dt);

    int mode0(unsigned long dt);
    int mode1(unsigned long dt);
    int mode2(unsigned long dt);

    void print(void);
};
