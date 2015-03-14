#include "seeed-motorshield.h"
#include "dfrobot-shield-l298p.h"
#include "movement.h"
#include <Wire.h>


const uint8_t GYROSCOPE_ADDRESS         = 0x6b; //0x35;
const uint8_t GYRO_REGISTER_WHO_AM_I    = 0x0f;
const uint8_t GYRO_REGISTER_CTRL_REG1   = 0x20;
const uint8_t GYRO_REGISTER_CTRL_REG2   = 0x21;
const uint8_t GYRO_REGISTER_CTRL_REG3   = 0x22;
const uint8_t GYRO_REGISTER_CTRL_REG4   = 0x23;
const uint8_t GYRO_REGISTER_OUT_X_L     = 0x28;


bool isActive(void)
{
    const uint8_t pin = 2;
    pinMode(pin, INPUT_PULLUP);
    return digitalRead(pin) == HIGH;
}


void createDFRobotMotors(void)
{
    IMotor *pMotorLeft  = DFRobot::Motor::createLeftSide();
    IMotor *pMotorRight = DFRobot::Motor::createRightSide();
    Movement::initialize(pMotorLeft, pMotorRight);
}


void createSeeedMotors(void)
{
    IMotor *pMotorLeft  = Seeed::Motor::createLeftSide();
    IMotor *pMotorRight = Seeed::Motor::createRightSide();
    Movement::initialize(pMotorLeft, pMotorRight);
}


void write8(uint8_t reg, uint8_t value)
{
    Wire.beginTransmission(GYROSCOPE_ADDRESS);
    Wire.write(reg);
    Wire.write(value);
    Wire.endTransmission();
}


uint8_t read8(uint8_t reg)
{
    uint8_t value;

    Wire.beginTransmission((uint8_t) GYROSCOPE_ADDRESS);
    Wire.write(reg);
    Wire.endTransmission();

    Wire.requestFrom(GYROSCOPE_ADDRESS, (uint8_t) 1);
    while (!Wire.available()); // Wait for data to arrive.
    value = Wire.read();
    Wire.endTransmission();

    return value;
}


void setup()
{
    Serial.begin(9600);
    createSeeedMotors();

    delay(2000);

    Wire.begin();
    uint8_t id = read8(GYRO_REGISTER_WHO_AM_I);
    Serial.println(id, HEX);
    write8(GYRO_REGISTER_CTRL_REG1, 0x00);
    write8(GYRO_REGISTER_CTRL_REG1, 0x0F);
    write8(GYRO_REGISTER_CTRL_REG4, 0x20);

    delay(5000);
}


void loop()
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

    int x = (int) ((xhi << 8) | xlo);
    int y = (int) ((yhi << 8) | ylo);
    int z = (int) ((zhi << 8) | zlo);

    Serial.print("X: ");
    Serial.print(x);
    Serial.print(", Y: ");
    Serial.print(y);
    Serial.print(", Z: ");
    Serial.println(z);

    delay(1000);

    if (isActive())
    {
        Movement::instance()->goForward(192);
        delay(800);

        Movement::instance()->goForward(255);
        delay(5000);

        Movement::instance()->stop();
        delay(2000);

        Movement::instance()->turnLeft(255);
        delay(2000);

        Movement::instance()->stop();
        delay(2000);

        Movement::instance()->turnRight(255);
        delay(2000);

        Movement::instance()->stop();
        delay(2000);

        Movement::instance()->goBackward(192);
        delay(800);

        Movement::instance()->goBackward(255);
        delay(5000);

        Movement::instance()->stop();
        delay(2000);
    }
}


#if 0
Seeed::MotorShield motor_left(Seeed::MotorShield::MOTORSHIELD_IN1, Seeed::MotorShield::MOTORSHIELD_IN2, Seeed::MotorShield::SPEEDPIN_A);
Seeed::MotorShield motor_right(Seeed::MotorShield::MOTORSHIELD_IN4, Seeed::MotorShield::MOTORSHIELD_IN3, Seeed::MotorShield::SPEEDPIN_B);

int count = Seeed::MotorShield::MAX_SPEEDS;
bool flag = false;

bool isActive(void)
{
  return false;
}

void setup()
{
  Serial.begin(9600);
  if (isActive())
  {
    motor_left.setSpeed(Seeed::MotorShield::MAX_SPEEDS - 1);
    motor_right.setSpeed(Seeed::MotorShield::MAX_SPEEDS - 1);
    motor_left.setDirection(Seeed::MotorShield::FORWARD);
    motor_right.setDirection(Seeed::MotorShield::FORWARD);
  }
}

void loop()
{
  if (isActive())
  {
    if (--count == 0)
    {
      count = Seeed::MotorShield::MAX_SPEEDS;

      if (flag)
      {
        motor_left.setSpeed(Seeed::MotorShield::MAX_SPEEDS - 1);
        motor_right.setSpeed(Seeed::MotorShield::MAX_SPEEDS - 1);
        flag = false;
      }
      else
      {
        motor_left.setSpeed(0);
        motor_right.setSpeed(0);
        flag = true;
      }

      delay(5000);
    }


    motor_left.tick();
    motor_right.tick();
    delay(100);
  }
}
#endif
