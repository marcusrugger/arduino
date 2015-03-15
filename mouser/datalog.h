#include <Arduino.h>


class DataLog
{
public:

    struct data3d
    {
        int16_t x, y, z;
    };

    static const int MAX_SAMPLES = (10 * 12);
    data3d _datalog[MAX_SAMPLES];
    int16_t _current_index;

    DataLog(void) : _current_index(0)
    {}

    void add(int16_t x, int16_t y, int16_t z)
    {
        if (_current_index < MAX_SAMPLES)
        {
            _datalog[_current_index].x = x;
            _datalog[_current_index].y = y;
            _datalog[_current_index].z = z;
            ++_current_index;
        }
    }

    void dump(void)
    {
        Serial.println("{");
        for (int i = 0; i < _current_index; ++i)
        {
            Serial.print("{ \"X\": ");
            Serial.print(_datalog[i].x);
            Serial.print(", \"Y\": ");
            Serial.print(_datalog[i].y);
            Serial.print(", \"Z\": ");
            Serial.print(_datalog[i].z);
            Serial.println(" },");
        }
        Serial.println("}");
        _current_index = 0;
    }
};
