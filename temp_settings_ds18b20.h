/*! @file temp_settings_ds18b20.h
 *! @author Tobias Rolke (github.com/randomguyfromtheinternet/)
 *! @version 1.1
 *! @date 2022-11-19 
 *! @brief Contains the settings of the Arduino sketch
 *! @copyright GPLv3
    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */


#ifndef _TEMP_DS18B20_SETTINGS_H_
#define _TEMP_DS18B20_SETTINGS_H_

#include "Arduino.h"

namespace temp_log
{
    constexpr bool _SERIAL_LOGGING{true};
    constexpr bool _SD_LOGGING{true};
    constexpr unsigned int _NUM_SENSORS_MAX{3u};
    constexpr unsigned char _LCD_ROWS{20};
    constexpr unsigned char _LCD_LINES{4};

    namespace pin
    {
        constexpr uint8_t _TEMP_SENSOR{2};
        constexpr unsigned char _LED_ONBOARD{13};
        constexpr unsigned char _SD{10};
    }

    // I2C adresses
    namespace i2c
    {
        constexpr unsigned char _LCD{0x27};
    }

    namespace lcd_char
    {
        constexpr unsigned char _CELSIUS[8] = {0b00100, 0b01010, 0b00100, 0b00000, 0b00000};
    }
}

#endif