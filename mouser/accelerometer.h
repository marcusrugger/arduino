#include <Arduino.h>
#include "base-i2c.h"
#include "geometry.h"

class Accelerometer : public BaseI2C
{
private:

    const uint8_t DEVICE_ADDRESS    = (0x32 >> 1);
    const uint8_t CTRL_REG1_A       = 0x20;
    const uint8_t CTRL_REG4_A       = 0x23;
    const uint8_t OUT_X_L_A         = 0x28;

    const float ax = 0.9847;    const float bx =  23.56;
    const float ay = 0.9485;    const float by =   0.12;
    const float az = 0.9825;    const float bz = -85.45;

    static Accelerometer *_pInstance;

    Accelerometer(void);


public:

    // Values from sensor
    int16_t rx, ry, rz;

    // Adjusted values
    Geometry::Point a;

    static Accelerometer *instance(void);

    void readAccelerometer(void);

};
