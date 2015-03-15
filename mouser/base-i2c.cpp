#include <Wire.h>
#include "base-i2c.h"


BaseI2C::BaseI2C(uint8_t device_address) : _device_address(device_address)
{}


void BaseI2C::write8(uint8_t reg, uint8_t value)
{
    Wire.beginTransmission(_device_address);
    Wire.write(reg);
    Wire.write(value);
    Wire.endTransmission();
}


uint8_t BaseI2C::read8(uint8_t reg)
{
    uint8_t value;

    Wire.beginTransmission((uint8_t) _device_address);
    Wire.write(reg);
    Wire.endTransmission();

    Wire.requestFrom(_device_address, (uint8_t) 1);
    while (!Wire.available()); // Wait for data to arrive.
    value = Wire.read();

    return value;
}
