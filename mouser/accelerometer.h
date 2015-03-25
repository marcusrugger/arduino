#ifndef __MOUSER_ACCELEROMETER
#define __MOUSER_ACCELEROMETER

#include <Arduino.h>
#include "base-i2c.h"
#include "geometry.h"

template<class T>
class point_t
{
public:

    T x, y, z;

    point_t(T a, T b, T c) : x(a), y(b), z(c)
    {}

    void minumum(T a, T b, T c)
    {
        if (a < x) x = a;
        if (b < y) y = b;
        if (c < z) z = c;
    }

    void maximum(T a, T b, T c)
    {
        if (a > x) x = a;
        if (b > y) y = b;
        if (c > z) z = c;
    }

    point_t<T> operator -(const point_t<T> d) const
    {
        return point_t<T>(x - d.x, y - d.y, z - d.z);
    }
};

template<class T>
class point_avg_t
{
public:

    T x, y, z;
    T sx, sy, sz;
    uint16_t number_of_samples;

    point_avg_t(T a, T b, T c)
    : x(a), y(b), z(c),
      sx(0), sy(0), sz(0),
      number_of_samples(0)
    {}

    void sum(T a, T b, T c)
    {
        sx += a;
        sy += b;
        sz += c;

        ++number_of_samples;

        x = sx / number_of_samples;
        y = sy / number_of_samples;
        z = sz / number_of_samples;
    }
};

class Accelerometer : public BaseI2C
{
private:

    const uint8_t DEVICE_ADDRESS    = (0x32 >> 1);
    const uint8_t CTRL_REG1_A       = 0x20;
    const uint8_t CTRL_REG2_A       = 0x21;
    const uint8_t CTRL_REG3_A       = 0x22;
    const uint8_t CTRL_REG4_A       = 0x23;
    const uint8_t CTRL_REG5_A       = 0x24;
    const uint8_t CTRL_REG6_A       = 0x25;
    const uint8_t OUT_X_L_A         = 0x28;

    const uint8_t REG1_XEN          = 0x01;     // X-axis enable (1=enable)
    const uint8_t REG1_YEN          = 0x02;     // Y-axis enable (1=enable)
    const uint8_t REG1_ZEN          = 0x04;     // Z-axis enable (1=enable)
    const uint8_t REG1_LPEN         = 0x08;     // Low-power enable (1=enable)
    const uint8_t REG1_10HZ         = 0x20;
    const uint8_t REG1_100HZ        = 0x50;
    const uint8_t REG1_1KHZ         = 0x90;

    const uint8_t REG4_HR           = 0x08;     // High-resolution (1=enable)
    const uint8_t REG4_FS0          = 0x10;     // Full-scale selection
    const uint8_t REG4_FS1          = 0x20;     // 00=2g, 01=4g, 10=8g, 11=16g
    const uint8_t REG4_BLE          = 0x40;     // Big/little endian (0: LSB MSB, 1: MSB LSB)

    const uint8_t REG4_FS_2         = 0x00;
    const uint8_t REG4_FS_4         = (REG4_FS0);
    const uint8_t REG4_FS_8         = (REG4_FS1);
    const uint8_t REG4_FS_16        = (REG4_FS0 | REG4_FS1);

    const float ax = 0.9847;    const float bx =  23.56;
    const float ay = 0.9485;    const float by =   0.12;
    const float az = 0.9825;    const float bz = -85.45;

    static Accelerometer *_pInstance;

    Accelerometer(void);

public:

    // Used during sampling to calculate adjustments
    point_t<int16_t> _min;
    point_t<int16_t> _max;
    point_t<int16_t> _difference;
    int32_t _sx, _sy, _sz;

    int  _number_of_samples;

    void unadjustedReadAccelerometer(void);
    Geometry::Point calibrate(const Geometry::Point & p) const;

public:

    // Unadjusted values from sensor
    int16_t rx, ry, rz;

    Geometry::Point d;

    // Calibrated values
    Geometry::Point a;

    static Accelerometer *instance(void);

    void normalize(void);
    void readAccelerometer(void);

};

#endif
