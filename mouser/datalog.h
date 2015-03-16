#include <Arduino.h>


class DataLog
{
public:

    struct dataset
    {
        int16_t gyrox;
        int16_t gyroz;
        uint8_t speed;
    };

    static const int MAX_SAMPLES = (900/sizeof(dataset));
    dataset _datalog[MAX_SAMPLES];
    int16_t _current_index;

    DataLog(void) : _current_index(0)
    {}

    void add(int16_t gyrox, int16_t gyroz, uint8_t speed)
    {
        if (_current_index < MAX_SAMPLES)
        {
            _datalog[_current_index].gyrox = gyrox;
            _datalog[_current_index].gyroz = gyroz;
            _datalog[_current_index].speed = speed;
            ++_current_index;
        }
    }

    void dump(void)
    {
        for (int i = 0; i < _current_index; ++i)
        {
            Serial.print(_datalog[i].gyrox);
            Serial.print(", ");
            Serial.print(_datalog[i].gyroz);
            Serial.print(", ");
            Serial.println(_datalog[i].speed);
        }
        _current_index = 0;
    }
};
