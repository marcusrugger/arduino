#include <Arduino.h>


class BaseI2C
{
protected:

    uint8_t _device_address;


protected:

    BaseI2C(uint8_t device_address);

    void write8(uint8_t reg, uint8_t value);
    uint8_t read8(uint8_t reg);

};
