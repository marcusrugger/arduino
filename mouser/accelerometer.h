#include <Arduino.h>
#include "base-i2c.h"

class Accelerometer : public BaseI2C
{
private:

    const uint8_t DEVICE_ADDRESS    = (0x32 >> 1);
    const uint8_t CTRL_REG1_A       = 0x20;
    const uint8_t CTRL_REG4_A       = 0x23;
    const uint8_t OUT_X_L_A         = 0x28;

    static Accelerometer *_pInstance;

    Accelerometer(void);


public:

    int16_t x, y, z;

    static Accelerometer *instance(void);

    void readAccelerometer(void);

};
