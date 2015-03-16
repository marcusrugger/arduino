#include <Wire.h>
#include "gyroscope.h"


Gyroscope *Gyroscope::_pInstance = NULL;


Gyroscope *Gyroscope::instance(void)
{
    if (_pInstance == NULL)
        _pInstance = new Gyroscope();

    return _pInstance;
}


Gyroscope::Gyroscope(void)
: BaseI2C(GYROSCOPE_ADDRESS),
  x(0), y(0), z(0),
  _ax(0), _ay(0), _az(0),
  _sx(0), _sy(0), _sz(0),
  _number_of_samples(0)
{
    uint8_t id = read8(GYRO_REGISTER_WHO_AM_I);
    Serial.println(id, HEX);
    write8(GYRO_REGISTER_CTRL_REG1, 0x00);
    write8(GYRO_REGISTER_CTRL_REG1, 0x0F);
    write8(GYRO_REGISTER_CTRL_REG4, 0x00);
}


void Gyroscope::normalize(void)
{
    unadjustedReadGyro();

    _sx += (int32_t) _uax;
    _sy += (int32_t) _uay;
    _sz += (int32_t) _uaz;

    _number_of_samples++;

    _ax = _sx / _number_of_samples;
    _ay = _sy / _number_of_samples;
    _az = _sz / _number_of_samples;
}


void Gyroscope::unadjustedReadGyro(void)
{
    Wire.beginTransmission(GYROSCOPE_ADDRESS);
    Wire.write(GYRO_REGISTER_OUT_X_L | 0x80);
    Wire.endTransmission();

    Wire.requestFrom(GYROSCOPE_ADDRESS, (uint8_t) 6);

    /* Wait around until enough data is available */
    while (Wire.available() < 6);
    uint8_t xlo = Wire.read();
    uint8_t xhi = Wire.read();
    uint8_t ylo = Wire.read();
    uint8_t yhi = Wire.read();
    uint8_t zlo = Wire.read();
    uint8_t zhi = Wire.read();

    _uax = (int) ((xhi << 8) | xlo);
    _uay = (int) ((yhi << 8) | ylo);
    _uaz = (int) ((zhi << 8) | zlo);
}


void Gyroscope::readGyro(void)
{
    // Serial.print("Gyro: ");

    unadjustedReadGyro();

    x = _uax - _ax;
    y = _uay - _ay;
    z = _uaz - _az;

    // Serial.print("X: ");
    // Serial.print(x);
    // Serial.print(", Y: ");
    // Serial.print(y);
    // Serial.print(", Z: ");
    // Serial.println(z);
}
