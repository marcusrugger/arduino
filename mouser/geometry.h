#ifndef __MOUSER_GEOMETRY__
#define __MOUSER_GEOMETRY__
#include <Arduino.h>

namespace Geometry
{

class Point;
class EulerAngles;
class RotationMatrix;


const float pi = 3.1415926536;


inline float DegToRad(float a)
{
    return a * pi / 180;
}


inline float RadToDeg(float a)
{
    return a * 180 / pi;
}


class Point
{
public:

    float x, y, z;

    Point(float nx, float ny, float nz) : x(nx), y(ny), z(nz)
    {}

    Point(const RotationMatrix &m, const Point &p);

};


class EulerAngles
{
public:

    float a, b, c;  // Alpha, Beta, Gamma;

    EulerAngles(float na, float nb, float nc) : a(DegToRad(na)), b(DegToRad(nb)), c(DegToRad(nc))
    {}
};


class RotationMatrix
{
public:

    float matrix[3][3];

    static RotationMatrix createLeftHanded(EulerAngles o);
    static RotationMatrix createLeftHanded(float a, float b, float c);
    static RotationMatrix createRightHanded(float a, float b, float c);
    static RotationMatrix createYaw(float a);
    static RotationMatrix createPitch(float a);
    static RotationMatrix createRoll(float a);

    RotationMatrix(void)
    {
        matrix[0][0] = 1.0; matrix[0][1] = 0.0; matrix[0][2] = 0.0;
        matrix[1][0] = 0.0; matrix[1][1] = 1.0; matrix[1][2] = 0.0;
        matrix[2][0] = 0.0; matrix[2][1] = 0.0; matrix[2][2] = 1.0;
    }
};

}

#endif
