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

    Point(void) : x(0.0), y(0.0), z(0.0)
    {}

    Point(float nx, float ny, float nz) : x(nx), y(ny), z(nz)
    {}

    Point(const Point &p);
    Point(const RotationMatrix &m, const Point &p);

};


class EulerAngles
{
public:

    float a, b, c;  // Alpha, Beta, Gamma;

    EulerAngles(void) : a(0.0), b(0.0), c(0.0)
    {}

    EulerAngles(float na, float nb, float nc) : a(DegToRad(na)), b(DegToRad(nb)), c(DegToRad(nc))
    {}

    EulerAngles(const EulerAngles &m);
};


class RotationMatrix
{
public:

    float matrix[3][3];

    static RotationMatrix createLeftHanded(EulerAngles o);
    static RotationMatrix createLeftHanded(float a, float b, float c);
    static RotationMatrix createRightHanded(float a, float b, float c);

    RotationMatrix(void)
    {
        matrix[0][0] = 1.0; matrix[0][1] = 0.0; matrix[0][2] = 0.0;
        matrix[1][0] = 0.0; matrix[1][1] = 1.0; matrix[1][2] = 0.0;
        matrix[2][0] = 0.0; matrix[2][1] = 0.0; matrix[2][2] = 1.0;
    }
};

}

#endif
