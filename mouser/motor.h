#include <Arduino.h>

class Motor
{
public:

    enum Pin
    {
        MOTORSHIELD_IN1 = 8,
        MOTORSHIELD_IN2 = 11,
        MOTORSHIELD_IN3 = 12,
        MOTORSHIELD_IN4 = 13,
        SPEEDPIN_A      = 9,
        SPEEDPIN_B      = 10
    };

    enum Direction
    {
        FORWARD,
        BACKWARD
    };

    static const int    MAX_SPEEDS = 18;


private:

    static const uint8_t    _speeds[MAX_SPEEDS];

    uint8_t  _current_speed_index;
    uint8_t  _expected_speed_index;

    Direction _direction;

    Pin _out_pin_1;
    Pin _out_pin_2;
    Pin _speed_pin;


public:

    Motor(Pin out_pin_1, Pin out_pin_2, Pin speed_pin)
    :   _out_pin_1(out_pin_1),
        _out_pin_2(out_pin_2),
        _speed_pin(speed_pin),
        _direction(FORWARD),
        _current_speed_index(0),
        _expected_speed_index(0)
    {
        pinMode(MOTORSHIELD_IN1, OUTPUT);
        pinMode(MOTORSHIELD_IN2, OUTPUT);
        pinMode(MOTORSHIELD_IN3, OUTPUT);
        pinMode(MOTORSHIELD_IN4, OUTPUT);
        pinMode(SPEEDPIN_A, OUTPUT);
        pinMode(SPEEDPIN_B, OUTPUT);
    }

    void tick(void)
    {
        adjustSpeed();
    }

    void setDirection(Direction direction)
    {
        _direction = direction;
        if (_direction == FORWARD)
        {
            digitalWrite(_out_pin_1, HIGH);
            digitalWrite(_out_pin_2, LOW);
        }
        else
        {
            digitalWrite(_out_pin_1, LOW);
            digitalWrite(_out_pin_2, HIGH);
        }
    }

    void setSpeed(uint8_t speed)
    {
        if (speed >= MAX_SPEEDS)
            speed = MAX_SPEEDS - 1;
\
        _expected_speed_index = speed;
    }

    void stop(void)
    {
        _current_speed_index = 0;
        _expected_speed_index = 0;
        analogWrite(_speed_pin, _speeds[_current_speed_index]);
    }


private:

    void log(char *s, unsigned int v, unsigned int format = DEC) const
    {
        Serial.print(s);
        Serial.println(v, format);
    }

    void adjustSpeed(void)
    {
        if (_current_speed_index < _expected_speed_index)
            _current_speed_index++;
        else if (_current_speed_index > _expected_speed_index)
            _current_speed_index--;

        log("Current speed: ", _current_speed_index);
        analogWrite(_speed_pin, _speeds[_current_speed_index]);
    }
};
