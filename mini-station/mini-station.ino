
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


Button button(2);


void setup()
{
  Serial.begin(9600);
}

void loop()
{
    if (button.isButtonPressed())
        Serial.println("The button is pressed!");
    else
        Serial.println("No button press");

    delay(1000);
}
