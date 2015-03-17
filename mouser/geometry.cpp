#include "geometry.h"
#include <math.h>

using namespace Geometry;


Point::Point(const RotationMatrix &m, const Point &p)
: x(m.matrix[0][0] * p.x + m.matrix[0][1] * p.y + m.matrix[0][2] * p.z),
  y(m.matrix[1][0] * p.x + m.matrix[1][1] * p.y + m.matrix[1][2] * p.z),
  z(m.matrix[2][0] * p.x + m.matrix[2][1] * p.y + m.matrix[2][2] * p.z)
{}


RotationMatrix RotationMatrix::createYaw(float a)
{
    RotationMatrix m;
    m.matrix[0][0] = cos(a);  m.matrix[0][1] = -sin(a);
    m.matrix[1][0] = sin(a);  m.matrix[1][1] = cos(a);
    return m;
}


RotationMatrix RotationMatrix::createPitch(float a)
{
    RotationMatrix m;
    m.matrix[0][0] = cos(a);  m.matrix[0][2] = sin(a);
    m.matrix[1][0] = -sin(a); m.matrix[1][2] = cos(a);
    return m;
}


RotationMatrix RotationMatrix::createRoll(float a)
{
    RotationMatrix m;
    m.matrix[1][1] = cos(a);  m.matrix[1][2] = -sin(a);
    m.matrix[2][1] = sin(a);  m.matrix[2][2] = cos(a);
    return m;
}
