#include <Wire.h>

class Led
{
  private:

    const int _pin;
    unsigned int _state;

  public:

    Led(int pin) : _pin(pin)
    {
      pinMode(pin, OUTPUT);
      writeBrightness(0);
    }

    void writeBrightness(unsigned int value)
    {
      _state = value;
      analogWrite(_pin, value);
    }

    unsigned int readBrightness(void) const
    {
      return _state;
    }
};

class Button
{
private:

  const int _pin;

public:

  Button(int pin) : _pin(pin)
  {
    pinMode(pin, INPUT_PULLUP);
  }

  bool isButtonPressed(void) const
  {
    return digitalRead(_pin) == LOW;
  }
};

class LedBrightnessControl
{
  private:
  
    const unsigned int _index;

    static const int MAX_VALUES = 16;
    static const int _values[MAX_VALUES];


  public:
  
    LedBrightnessControl(void)
    : _index(0)
    {}
  
    LedBrightnessControl(unsigned int brightness)
    : _index(brightness < MAX_VALUES ? brightness : MAX_VALUES-1)
    {}

    static LedBrightnessControl setBrightness(unsigned int value)
    {
      return LedBrightnessControl(value);
    }

    static LedBrightnessControl setBrightness(Led &led)
    {
      unsigned int current = led.readBrightness();
      unsigned int value;
      for (value = 0; current > _values[value]; value++);
      return LedBrightnessControl(value);
    }

    static LedBrightnessControl setOff(void)
    {
      return LedBrightnessControl(0);
    }
    
    static LedBrightnessControl setMax(void)
    {
      return LedBrightnessControl(MAX_VALUES-1);
    }

    bool canIncrement(void) const
    {
      return _index < MAX_VALUES-1;
    }

    LedBrightnessControl increment(void) const
    {
      if (canIncrement())
        return LedBrightnessControl(_index+1);
      else
        return (*this);
    }

    bool canDecrement(void) const
    {
      return _index > 0;
    }

    LedBrightnessControl decrement(void) const
    {
      if (canDecrement())
        return LedBrightnessControl(_index-1);
      else
        return (*this);
    }

    unsigned int getPinValue(void) const
    {
      return _values[_index];
    }

    void onLed(Led &led) const
    {
      led.writeBrightness(getPinValue());
    }
};

const int LedBrightnessControl::_values[MAX_VALUES] = { 0, 1, 2, 4, 6, 8, 11, 16, 23, 32, 45, 64, 91, 128, 181, 255 };


class Board
{
private:

  const int PIN_BUTTON_UP   = 2;
  const int PIN_BUTTON_DOWN = 4;

  const int PIN_LED_TEMPERATURE = 3;
  const int PIN_LED_HUMIDITY    = 5;
  const int PIN_LED_PRESSURE    = 6;
  const int PIN_LED_TIME        = 9;

  static Board *_pInstance;

  const Button  _buttonUp;
  const Button  _buttonDown;

  Led _ledTemperature;
  Led _ledHumidity;
  Led _ledPressure;
  Led _ledTime;

  Board(void)
  : _buttonUp(PIN_BUTTON_UP),
    _buttonDown(PIN_BUTTON_DOWN),
    _ledTemperature(PIN_LED_TEMPERATURE),
    _ledHumidity(PIN_LED_HUMIDITY),
    _ledPressure(PIN_LED_PRESSURE),
    _ledTime(PIN_LED_TIME)
  {
    auto off = LedBrightnessControl::setOff();

    off.onLed(_ledTemperature);
    off.onLed(_ledHumidity);
    off.onLed(_ledPressure);
    off.onLed(_ledTime);
  }

  static void staticOnReceive(int count)
  {
    getInstance().onReceive(count);
  }

public:

  static Board &getInstance(void)
  {
    if (_pInstance == NULL)
      _pInstance = new Board();
    return (*_pInstance);
  }

  void initialize(unsigned char i2c_address)
  {
    Wire.begin(i2c_address);
    Wire.onReceive(Board::staticOnReceive);
  }

  void loop(void)
  {
    delay(100);
  }

  void printinfo(char *s, unsigned int v)
  {
    Serial.print(s);
    Serial.println(v);
  }

  void onReceive(int count)
  {
    Serial.print("count: ");
    Serial.println(count);
    unsigned char c;
    unsigned long l = 0;
    while (Wire.available() > 0)
    {
      c = Wire.read();
      l = (l << 8) | c;
      Serial.print(c);
      Serial.print(", l: ");
      Serial.print(l);
      Serial.print(", Available: ");
      Serial.println(Wire.available());
    }

    Serial.print(l);
    Serial.println();

    unsigned int led1 = (l & 0xff000000) >> 24;
    unsigned int led2 = (l & 0x00ff0000) >> 16;
    unsigned int led3 = (l & 0x0000ff00) >> 8;
    unsigned int led4 = (l & 0x000000ff);

    printinfo("led1: ", led1);
    printinfo("led2: ", led2);
    printinfo("led3: ", led3);
    printinfo("led4: ", led4);

    _ledTemperature.writeBrightness(led1);
    _ledHumidity.writeBrightness(led2);
    _ledPressure.writeBrightness(led3);
    _ledTime.writeBrightness(led4);
  }
};

Board *Board::_pInstance = NULL;


void setup()
{
  Serial.begin(115200);
  Board::getInstance().initialize(0x77);
}

void loop()
{
  Board::getInstance().loop();
}
