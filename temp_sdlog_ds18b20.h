/*! @file temp_sdlog_ds18b20.h
 *! @author Tobias Rolke (github.com/chillerwal/)
 *! @version 1.1
 *! @date 2022-11-19 
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


#ifndef _TEMP_DS18B20_SDLOG_H_
#define _TEMP_DS18B20_SDLOG_H_

#include <SPI.h> // SD by Arduino
#include <SD.h> // SD by Arduino
#include "logtime.h"
#include "temp_settings_ds18b20.h"

namespace temp_log
{
    constexpr char _LOG_BOOT{'b'};
    constexpr char _LOG_SENSORS{'s'};
    constexpr char _LOG_TEMP{'t'};

    bool init_sd_logging(const unsigned char& sd_pin);
    bool fileExists(const String& filename);
    bool deleteFile(const String& filename);
    bool appendToFile(const String& text, const String& filename);
    String readFile(const String& filename);
    String sensor_address_to_string(unsigned char address[8]);
    String byte_to_hex(const unsigned char& input);
    char subbyte_to_hex(const unsigned char& slice);

    void log_boot(const sdlog::LogTime& lt);
    void log_temperature(const sdlog::LogTime& lt, float current_temperature[temp_log::_NUM_SENSORS_MAX]);
    void log_sensors(const sdlog::LogTime& lt, unsigned char addresses[temp_log::_NUM_SENSORS_MAX][8]);
}

#endif