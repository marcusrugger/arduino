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

    // Unadjusted gyro values
    int16_t _uax, _uay, _uaz;

    // Adjustments to calculated adjusted values
    int16_t _ax, _ay, _az;

    // Used during sampling to calculate adjustments
    int32_t _sx, _sy, _sz;
    int8_t _number_of_samples;

    void unadjustedReadGyro(void);


public:

    int16_t x, y, z;

    static Gyroscope *instance(void);

    void normalize(void);
    void readGyro(void);

};
