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
  multiplier(0.00875),
  time_interval(0.1),
  ax(0), ay(0), az(0),
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


void Gyroscope::readGyroscope(void)
{
    unadjustedReadGyro();

    ax  = _uax - _ax;
    ay  = _uay - _ay;
    az  = _uaz - _az;

    Geometry::EulerAngles m(multiplier * (float) ax,
                            multiplier * (float) ay,
                            multiplier * (float) az);

    a = m;
}
