#include "geometry.h"
#include <math.h>

using namespace Geometry;


Point::Point(const Point &p)
: x(p.x), y(p.y), z(p.z)
{}


Point::Point(const RotationMatrix &m, const Point &p)
: x(m.matrix[0][0] * p.x + m.matrix[0][1] * p.y + m.matrix[0][2] * p.z),
  y(m.matrix[1][0] * p.x + m.matrix[1][1] * p.y + m.matrix[1][2] * p.z),
  z(m.matrix[2][0] * p.x + m.matrix[2][1] * p.y + m.matrix[2][2] * p.z)
{}


EulerAngles::EulerAngles(const EulerAngles &m)
: a(m.a), b(m.b), c(m.c)
{}


RotationMatrix RotationMatrix::createLeftHanded(EulerAngles o)
{
    return createLeftHanded(o.a, o.b, o.c);
}


RotationMatrix RotationMatrix::createLeftHanded(float a, float b, float c)
{
    RotationMatrix m;

    float ca = cos(a);
    float cb = cos(b);
    float cc = cos(c);

    float sa = sin(a);
    float sb = sin(b);
    float sc = sin(c);

    m.matrix[0][0] = cb * cc;
    m.matrix[0][1] = -cb * sc;
    m.matrix[0][2] = sb;

    m.matrix[1][0] = ca * sc + sa * sb * cc;
    m.matrix[1][1] = ca * cc - sa * sb * sc;
    m.matrix[1][2] = -sa * cb;

    m.matrix[2][0] = sa * sc - ca * sb * cc;
    m.matrix[2][1] = sa * cc + ca * sb * sc;
    m.matrix[2][2] = ca * cb;

    return m;
}


RotationMatrix RotationMatrix::createRightHanded(float a, float b, float c)
{
    RotationMatrix m;

    float ca = cos(a);
    float cb = cos(b);
    float cc = cos(c);

    float sa = sin(a);
    float sb = sin(b);
    float sc = sin(c);

    m.matrix[0][0] = ca * cb;
    m.matrix[0][1] = ca * sb * sc - sa * cc;
    m.matrix[0][2] = ca * sb * cc + sa * sc;

    m.matrix[1][0] = sa * cb;
    m.matrix[1][1] = sa * sb * sc + ca * cc;
    m.matrix[1][2] = sa * sb * cc - ca * sc;

    m.matrix[2][0] = -sb;
    m.matrix[2][1] = cb * sc;
    m.matrix[2][2] = cb * cc;

    return m;
}
