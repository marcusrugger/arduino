#include <Arduino.h>
#include "base-i2c.h"


class Gyroscope : public BaseI2C
{
private:

    const uint8_t GYROSCOPE_ADDRESS         = 0x6b; //0x35;
    const uint8_t GYRO_REGISTER_WHO_AM_I    = 0x0f;
    const uint8_t GYRO_REGISTER_CTRL_REG1   = 0x20;
    const uint8_t GYRO_REGISTER_CTRL_REG2   = 0x21;
    const uint8_t GYRO_REGISTER_CTRL_REG3   = 0x22;
    const uint8_t GYRO_REGISTER_CTRL_REG4   = 0x23;
    const uint8_t GYRO_REGISTER_OUT_X_L     = 0x28;

    static Gyroscope *_pInstance;

    Gyroscope(void);


public:

    int16_t x;
    int16_t y;
    int16_t z;

    static Gyroscope *instance(void);

    void readGyro(void);

};
