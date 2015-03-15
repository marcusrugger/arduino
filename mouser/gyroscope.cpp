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
  x(0), y(0), z(0)
{
    uint8_t id = read8(GYRO_REGISTER_WHO_AM_I);
    Serial.println(id, HEX);
    write8(GYRO_REGISTER_CTRL_REG1, 0x00);
    write8(GYRO_REGISTER_CTRL_REG1, 0x0F);
    write8(GYRO_REGISTER_CTRL_REG4, 0x20);
}


void Gyroscope::readGyro(void)
{
    Serial.print("Gyro: ");

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

    x = (int) ((xhi << 8) | xlo);
    y = (int) ((yhi << 8) | ylo);
    z = (int) ((zhi << 8) | zlo);

    Serial.print("X: ");
    Serial.print(x);
    Serial.print(", Y: ");
    Serial.print(y);
    Serial.print(", Z: ");
    Serial.println(z);
}
