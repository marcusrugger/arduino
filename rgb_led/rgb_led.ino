
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
  const int PIN_LED_MAIN    = 3;

  const int PIN_TRI_RED     = 9;
  const int PIN_TRI_GREEN   = 10;
  const int PIN_TRI_BLUE    = 11;

  static Board *_pInstance;

  Led _mainLed;
  Led _triRed;
  Led _triGreen;
  Led _triBlue;

  const Button  _buttonUp;
  const Button  _buttonDown;

  Board(void)
  : _mainLed(PIN_LED_MAIN),
    _buttonUp(PIN_BUTTON_UP),
    _buttonDown(PIN_BUTTON_DOWN),
    _triRed(PIN_TRI_RED),
    _triGreen(PIN_TRI_GREEN),
    _triBlue(PIN_TRI_BLUE)
  {
    auto off = LedBrightnessControl::setOff();
    off.onLed(_mainLed);
    off.onLed(_triRed);
    off.onLed(_triGreen);
    off.onLed(_triBlue);
  }

public:

  static Board &getInstance(void)
  {
    if (_pInstance == NULL)
      _pInstance = new Board();
    return (*_pInstance);
  }

  // void loop(void)
  // {
  //   auto control = LedBrightnessControl::setBrightness(_mainLed);

  //   if (_buttonUp.isButtonPressed())
  //     control.increment().onLed(_mainLed);
  //   else if (_buttonDown.isButtonPressed())
  //     control.decrement().onLed(_mainLed);

  //   delay(100);
  // }

  void loop(void)
  {
    auto control = LedBrightnessControl::setBrightness(_mainLed);

    for (int r = 0; r < 2; r++)
    {
//      _triRed.writeBrightness(255 * r);
      digitalWrite(PIN_TRI_RED, r == 1 ? HIGH : LOW);
      for (int g = 0; g < 2; g++)
      {
//        _triGreen.writeBrightness(255 * g);
        digitalWrite(PIN_TRI_GREEN, g == 1 ? HIGH : LOW);
        for (int b = 0; b < 2; b++)
        {
//          _triBlue.writeBrightness(255 * b);
          digitalWrite(PIN_TRI_BLUE, b == 1 ? HIGH : LOW);
          for (int brightness = 0; brightness < 16; brightness++)
          {
            LedBrightnessControl::setBrightness(brightness).onLed(_mainLed);
            delay(100);
          }
          for (int brightness = 0; brightness < 16; brightness++)
          {
            LedBrightnessControl::setBrightness(15 - brightness).onLed(_mainLed);
            delay(100);
          }
        }
      }
    }
  }
};

Board *Board::_pInstance = NULL;


void setup()
{
  Serial.begin(9600);
}

void loop()
{
  Board::getInstance().loop();
}
