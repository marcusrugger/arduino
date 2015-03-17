#ifndef __MOUSER_GEOMETRY__
#define __MOUSER_GEOMETRY__
#include <Arduino.h>

namespace Geometry
{

class Point;
class RotationMatrix;

class Point
{
public:

    float x, y, z;

    Point(float nx, float ny, float nz) : x(nx), y(ny), z(nz)
    {}

    Point(const RotationMatrix &m, const Point &p);

};

class RotationMatrix
{
public:

    float matrix[3][3];

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
