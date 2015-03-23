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


Point Point::operator +(const Point &p) const
{
    return Point(x + p.x, y + p.y, z + p.z);
}


Point Point::operator -(const Point &p) const
{
    return Point(x - p.x, y - p.y, z - p.z);
}


Point Point::operator *(float t) const
{
    return Point(x * t, y * t, z * t);
}


EulerAngles EulerAngles::fromDegrees(float a, float b, float c)
{
    return EulerAngles(DegToRad(a), DegToRad(b), DegToRad(c));
}


EulerAngles::EulerAngles(const EulerAngles &m)
: a(m.a), b(m.b), c(m.c)
{}


EulerAngles EulerAngles::operator *(float t) const
{
    return EulerAngles(a * t, b * t, c * t);
}


EulerAngles EulerAngles::operator +(const EulerAngles &e) const
{
    return EulerAngles(a + e.a, b + e.b, c + e.c);
}


RotationMatrix RotationMatrix::createLeftHanded(const EulerAngles &o)
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

    m.matrix[1][0] = cb * cc;
    m.matrix[1][1] = cb * sc;
    m.matrix[1][2] = -sb;

    m.matrix[0][0] = ca * sc + sa * sb * cc;
    m.matrix[0][1] = ca * cc - sa * sb * sc;
    m.matrix[0][2] = sa * cb;

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


RotationMatrix RotationMatrix::createYaw(float a)
{
    RotationMatrix m;

    float ca = cos(a);
    float sa = sin(a);

    m.matrix[0][0] = ca;
    m.matrix[0][1] = -sa;
    m.matrix[0][2] = 0.0;

    m.matrix[1][0] = sa;
    m.matrix[1][1] = ca;
    m.matrix[1][2] = 0.0;

    m.matrix[2][0] = 0.0;
    m.matrix[2][1] = 0.0;
    m.matrix[2][2] = 1.0;

    return m;
}


RotationMatrix RotationMatrix::createPitch(float a)
{
    RotationMatrix m;

    float ca = cos(a);
    float sa = sin(a);

    m.matrix[0][0] = 1.0;
    m.matrix[0][1] = 0.0;
    m.matrix[0][2] = 0.0;

    m.matrix[1][0] = 0.0;
    m.matrix[1][1] = ca;
    m.matrix[1][2] = -sa;

    m.matrix[2][0] = 0.0;
    m.matrix[2][1] = sa;
    m.matrix[2][2] = ca;

    return m;
}


RotationMatrix RotationMatrix::createRoll(float a)
{
    RotationMatrix m;

    float ca = cos(a);
    float sa = sin(a);

    m.matrix[0][0] = ca;
    m.matrix[0][1] = 0.0;
    m.matrix[0][2] = sa;

    m.matrix[1][0] = 0.0;
    m.matrix[1][1] = 1.0;
    m.matrix[1][2] = 0.0;

    m.matrix[2][0] = -sa;
    m.matrix[2][1] = 0.0;
    m.matrix[2][2] = ca;

    return m;
}


RotationMatrix RotationMatrix::operator *(const RotationMatrix &m) const
{
    RotationMatrix r;

    r.matrix[0][0] = matrix[0][0] * m.matrix[0][0] +
                     matrix[0][1] * m.matrix[1][0] +
                     matrix[0][2] * m.matrix[2][0];

    r.matrix[0][1] = matrix[0][0] * m.matrix[0][1] +
                     matrix[0][1] * m.matrix[1][1] +
                     matrix[0][2] * m.matrix[2][1];

    r.matrix[0][2] = matrix[0][0] * m.matrix[0][2] +
                     matrix[0][1] * m.matrix[1][2] +
                     matrix[0][2] * m.matrix[2][2];


    r.matrix[1][0] = matrix[1][0] * m.matrix[0][0] +
                     matrix[1][1] * m.matrix[1][0] +
                     matrix[1][2] * m.matrix[2][0];

    r.matrix[1][1] = matrix[1][0] * m.matrix[0][1] +
                     matrix[1][1] * m.matrix[1][1] +
                     matrix[1][2] * m.matrix[2][1];

    r.matrix[1][2] = matrix[1][0] * m.matrix[0][2] +
                     matrix[1][1] * m.matrix[1][2] +
                     matrix[1][2] * m.matrix[2][2];


    r.matrix[2][0] = matrix[2][0] * m.matrix[0][0] +
                     matrix[2][1] * m.matrix[1][0] +
                     matrix[2][2] * m.matrix[2][0];

    r.matrix[2][1] = matrix[2][0] * m.matrix[0][1] +
                     matrix[2][1] * m.matrix[1][1] +
                     matrix[2][2] * m.matrix[2][1];

    r.matrix[2][2] = matrix[2][0] * m.matrix[0][2] +
                     matrix[2][1] * m.matrix[1][2] +
                     matrix[2][2] * m.matrix[2][2];

    return r;
}


Point RotationMatrix::rotate(const Point &p) const
{
    return Point(p.x * matrix[0][0] + p.y * matrix[0][1] + p.z * matrix[0][2],
                 p.x * matrix[1][0] + p.y * matrix[1][1] + p.z * matrix[1][2],
                 p.x * matrix[2][0] + p.y * matrix[2][1] + p.z * matrix[2][2]);
}


Matrix44::Matrix44(void)
{
    matrix[0][0] = 1.0; matrix[0][1] = 0.0; matrix[0][2] = 0.0; matrix[0][3] = 0.0;
    matrix[1][0] = 0.0; matrix[1][1] = 1.0; matrix[1][2] = 0.0; matrix[1][3] = 0.0;
    matrix[2][0] = 0.0; matrix[2][1] = 0.0; matrix[2][2] = 1.0; matrix[2][3] = 0.0;
    matrix[3][0] = 0.0; matrix[3][1] = 0.0; matrix[3][2] = 0.0; matrix[3][3] = 1.0;
}


Matrix44::Matrix44(const Matrix44 &m)
{
    matrix[0][0] = m.matrix[0][0]; matrix[0][1] = m.matrix[0][1]; matrix[0][2] = m.matrix[0][2]; matrix[0][3] = m.matrix[0][3];
    matrix[1][0] = m.matrix[1][0]; matrix[1][1] = m.matrix[1][1]; matrix[1][2] = m.matrix[1][2]; matrix[1][3] = m.matrix[1][3];
    matrix[2][0] = m.matrix[2][0]; matrix[2][1] = m.matrix[2][1]; matrix[2][2] = m.matrix[2][2]; matrix[2][3] = m.matrix[2][3];
    matrix[3][0] = m.matrix[3][0]; matrix[3][1] = m.matrix[3][1]; matrix[3][2] = m.matrix[3][2]; matrix[3][3] = m.matrix[3][3];
}


Matrix44::Matrix44(const EulerAngles &o, const Point &p)
{
    RotationMatrix r = RotationMatrix::createLeftHanded(o);

    matrix[0][0] = r.matrix[0][0]; matrix[0][1] = r.matrix[0][1]; matrix[0][2] = r.matrix[0][2]; matrix[0][3] = p.x;
    matrix[1][0] = r.matrix[1][0]; matrix[1][1] = r.matrix[1][1]; matrix[1][2] = r.matrix[1][2]; matrix[1][3] = p.y;
    matrix[2][0] = r.matrix[2][0]; matrix[2][1] = r.matrix[2][1]; matrix[2][2] = r.matrix[2][2]; matrix[2][3] = p.z;

    matrix[3][0] = 0.0; matrix[3][1] = 0.0; matrix[3][2] = 0.0; matrix[3][3] = 1.0;
}


Matrix44 Matrix44::operator *(const Matrix44 &m) const
{
    Matrix44 r;

    r.matrix[0][0] = matrix[0][0] * m.matrix[0][0] +
                     matrix[0][1] * m.matrix[1][0] +
                     matrix[0][2] * m.matrix[2][0] +
                     matrix[0][3] * m.matrix[3][0];

    r.matrix[0][1] = matrix[0][0] * m.matrix[0][1] +
                     matrix[0][1] * m.matrix[1][1] +
                     matrix[0][2] * m.matrix[2][1] +
                     matrix[0][3] * m.matrix[3][1];

    r.matrix[0][2] = matrix[0][0] * m.matrix[0][2] +
                     matrix[0][1] * m.matrix[1][2] +
                     matrix[0][2] * m.matrix[2][2] +
                     matrix[0][3] * m.matrix[3][2];

    r.matrix[0][3] = matrix[0][0] * m.matrix[0][3] +
                     matrix[0][1] * m.matrix[1][3] +
                     matrix[0][2] * m.matrix[2][3] +
                     matrix[0][3] * m.matrix[3][3];


    r.matrix[1][0] = matrix[1][0] * m.matrix[0][0] +
                     matrix[1][1] * m.matrix[1][0] +
                     matrix[1][2] * m.matrix[2][0] +
                     matrix[1][3] * m.matrix[3][0];

    r.matrix[1][1] = matrix[1][0] * m.matrix[0][1] +
                     matrix[1][1] * m.matrix[1][1] +
                     matrix[1][2] * m.matrix[2][1] +
                     matrix[1][3] * m.matrix[3][1];

    r.matrix[1][2] = matrix[1][0] * m.matrix[0][2] +
                     matrix[1][1] * m.matrix[1][2] +
                     matrix[1][2] * m.matrix[2][2] +
                     matrix[1][3] * m.matrix[3][2];

    r.matrix[1][3] = matrix[1][0] * m.matrix[0][3] +
                     matrix[1][1] * m.matrix[1][3] +
                     matrix[1][2] * m.matrix[2][3] +
                     matrix[1][3] * m.matrix[3][3];


    r.matrix[2][0] = matrix[2][0] * m.matrix[0][0] +
                     matrix[2][1] * m.matrix[1][0] +
                     matrix[2][2] * m.matrix[2][0] +
                     matrix[2][3] * m.matrix[3][0];

    r.matrix[2][1] = matrix[2][0] * m.matrix[0][1] +
                     matrix[2][1] * m.matrix[1][1] +
                     matrix[2][2] * m.matrix[2][1] +
                     matrix[2][3] * m.matrix[3][1];

    r.matrix[2][2] = matrix[2][0] * m.matrix[0][2] +
                     matrix[2][1] * m.matrix[1][2] +
                     matrix[2][2] * m.matrix[2][2] +
                     matrix[2][3] * m.matrix[3][2];

    r.matrix[2][3] = matrix[2][0] * m.matrix[0][3] +
                     matrix[2][1] * m.matrix[1][3] +
                     matrix[2][2] * m.matrix[2][3] +
                     matrix[2][3] * m.matrix[3][3];


    r.matrix[3][0] = matrix[3][0] * m.matrix[0][0] +
                     matrix[3][1] * m.matrix[1][0] +
                     matrix[3][2] * m.matrix[2][0] +
                     matrix[3][3] * m.matrix[3][0];

    r.matrix[3][1] = matrix[3][0] * m.matrix[0][1] +
                     matrix[3][1] * m.matrix[1][1] +
                     matrix[3][2] * m.matrix[2][1] +
                     matrix[3][3] * m.matrix[3][1];

    r.matrix[3][2] = matrix[3][0] * m.matrix[0][2] +
                     matrix[3][1] * m.matrix[1][2] +
                     matrix[3][2] * m.matrix[2][2] +
                     matrix[3][3] * m.matrix[3][2];

    r.matrix[3][3] = matrix[3][0] * m.matrix[0][3] +
                     matrix[3][1] * m.matrix[1][3] +
                     matrix[3][2] * m.matrix[2][3] +
                     matrix[3][3] * m.matrix[3][3];

    return r;
}


Point Matrix44::translate(const Point &p) const
{
    return Point(p.x * matrix[0][0] + p.x * matrix[0][1] + p.x * matrix[0][2] + matrix[0][3],
                 p.y * matrix[1][0] + p.y * matrix[1][1] + p.y * matrix[1][2] + matrix[1][3],
                 p.z * matrix[2][0] + p.z * matrix[2][1] + p.z * matrix[2][2] + matrix[2][3]);
}


Point Matrix44::rotate(const Point &p) const
{
    return Point(p.x * matrix[0][0] + p.x * matrix[0][1] + p.x * matrix[0][2],
                 p.y * matrix[1][0] + p.y * matrix[1][1] + p.y * matrix[1][2],
                 p.z * matrix[2][0] + p.z * matrix[2][1] + p.z * matrix[2][2]);
}


Matrix44 Matrix44::rotate(const EulerAngles &o) const
{
    RotationMatrix m = RotationMatrix::createLeftHanded(o);
    Matrix44 r;

    r.matrix[0][0] = matrix[0][0] * m.matrix[0][0] +
                     matrix[0][1] * m.matrix[1][0] +
                     matrix[0][2] * m.matrix[2][0];

    r.matrix[0][1] = matrix[0][0] * m.matrix[0][1] +
                     matrix[0][1] * m.matrix[1][1] +
                     matrix[0][2] * m.matrix[2][1];

    r.matrix[0][2] = matrix[0][0] * m.matrix[0][2] +
                     matrix[0][1] * m.matrix[1][2] +
                     matrix[0][2] * m.matrix[2][2];


    r.matrix[1][0] = matrix[1][0] * m.matrix[0][0] +
                     matrix[1][1] * m.matrix[1][0] +
                     matrix[1][2] * m.matrix[2][0];

    r.matrix[1][1] = matrix[1][0] * m.matrix[0][1] +
                     matrix[1][1] * m.matrix[1][1] +
                     matrix[1][2] * m.matrix[2][1];

    r.matrix[1][2] = matrix[1][0] * m.matrix[0][2] +
                     matrix[1][1] * m.matrix[1][2] +
                     matrix[1][2] * m.matrix[2][2];


    r.matrix[2][0] = matrix[2][0] * m.matrix[0][0] +
                     matrix[2][1] * m.matrix[1][0] +
                     matrix[2][2] * m.matrix[2][0];

    r.matrix[2][1] = matrix[2][0] * m.matrix[0][1] +
                     matrix[2][1] * m.matrix[1][1] +
                     matrix[2][2] * m.matrix[2][1];

    r.matrix[2][2] = matrix[2][0] * m.matrix[0][2] +
                     matrix[2][1] * m.matrix[1][2] +
                     matrix[2][2] * m.matrix[2][2];

    return r;
}
