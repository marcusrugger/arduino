
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
  const int PIN_LED_MAIN    = 5;

  static Board *_pInstance;

  Led _mainLed;

  const Button  _buttonUp;
  const Button  _buttonDown;

  Board(void)
  : _mainLed(PIN_LED_MAIN),
    _buttonUp(PIN_BUTTON_UP),
    _buttonDown(PIN_BUTTON_DOWN)
  {
    LedBrightnessControl::setOff().onLed(_mainLed);
  }

public:

  static Board &getInstance(void)
  {
    if (_pInstance == NULL)
      _pInstance = new Board();
    return (*_pInstance);
  }

  void loop(void)
  {
    auto control = LedBrightnessControl::setBrightness(_mainLed);

    if (_buttonUp.isButtonPressed())
      control.increment().onLed(_mainLed);
    else if (_buttonDown.isButtonPressed())
      control.decrement().onLed(_mainLed);

    delay(100);
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
