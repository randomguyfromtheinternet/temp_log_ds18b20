#ifndef _TEMP_DS18B20_SETTINGS_H_
#define _TEMP_DS18B20_SETTINGS_H_

#include "Arduino.h"

namespace temp_log
{
    constexpr bool _SERIAL_LOGGING{true};
    constexpr bool _SD_LOGGING{true};
    constexpr unsigned int _NUM_SENSORS_MAX{3u};

    namespace pin
    {
        constexpr uint8_t _TEMP_SENSOR{5};
        constexpr unsigned char _LED_ONBOARD{13};
        constexpr unsigned char _SD{10};
    }
}

#endif