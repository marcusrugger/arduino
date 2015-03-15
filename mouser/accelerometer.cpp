#include <Wire.h>
#include "accelerometer.h"


Accelerometer *Accelerometer::_pInstance = NULL;


Accelerometer *Accelerometer::instance(void)
{
    if (_pInstance == NULL)
        _pInstance = new Accelerometer();

    return _pInstance;
}


Accelerometer::Accelerometer(void)
: BaseI2C(DEVICE_ADDRESS),
  x(0), y(0), z(0)
{
    write8(CTRL_REG1_A, 0x47);
}


void Accelerometer::readAccelerometer(void)
{
    Serial.print("Gyro: ");

    Wire.beginTransmission(DEVICE_ADDRESS);
    Wire.write(OUT_X_L_A | 0x80);
    Wire.endTransmission();

    Wire.requestFrom(DEVICE_ADDRESS, (uint8_t) 6);

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
