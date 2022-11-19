/*! @file temp_sdlog_ds18b20.cpp
 *! @author Tobias Rolke (github.com/chillerwal/)
 *! @version 1.0
 *! @date 2022-11-18 
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

#include "temp_sdlog_ds18b20.h"

namespace temp_log
{    
    bool init_sd_logging(const unsigned char& sd_pin){
        if(!SD.begin(sd_pin)){
            Serial.println(F("E: SD fail"));
            return false;
        }
        
        Serial.println(F("i: SD OK"));
        return true;
    }

    bool fileExists(const String& filename){
        return SD.exists(filename);
    }

    bool deleteFile(const String& filename){
        return SD.remove(filename);
    }

    bool appendToFile(const String& text, const String& filename){
        File currentfile = SD.open(filename, FILE_WRITE);
        
        if(!currentfile){
            Serial.println("E: " + filename + " no access");
            return false;
        }

        currentfile.println(text);
        currentfile.close();
        return true;
    }

    String readFile(const String& filename)
    {
        String out = "";

        // check if file exists
        if(!SD.exists(filename))
        {
            Serial.println("E: \"" + filename + "\" no exist");
            return "";
        }
        
        // open file
        File openfile = SD.open(filename);
        Serial.println("i: \"" + filename + "\" opened.");

        // read content and return into pointer
        out = openfile.read();
        
        // close file
        openfile.close();

        return out;
    }

    void log_boot(const sdlog::LogTime& lt)
    {
        String out {""};
        
        // Generate Output for logging
        out += lt.iso_now(false, false);
        lt.append_separator(out);
        out += _LOG_BOOT;
        
        appendToFile(out, lt.current_filename());
    
        return;
    }

    void log_temperature(const sdlog::LogTime& lt, float current_temperature[temp_log::_NUM_SENSORS_MAX])
    {
        String out {""};
        out.reserve(temp_log::_NUM_SENSORS_MAX * 5 + 13);
        
        out += lt.iso_now(false, false);
        lt.append_separator(out);
        out += _LOG_TEMP;
        lt.append_separator(out);
        for(unsigned char i{0}; i < temp_log::_NUM_SENSORS_MAX; ++i)
        {
            out += (String) current_temperature[i];

            if(i < temp_log::_NUM_SENSORS_MAX - 1)
            {
                lt.append_separator(out);
            }
        }
        appendToFile(out, lt.current_filename());

        return;
    }

    void log_sensors(const sdlog::LogTime& lt, unsigned char addresses[temp_log::_NUM_SENSORS_MAX][8])
    {
        String out {""};
        out.reserve(temp_log::_NUM_SENSORS_MAX * 17 + 13);
        
        out += lt.iso_now(false, false);
        lt.append_separator(out);
        out += _LOG_SENSORS;
        lt.append_separator(out);

        for(unsigned char i{0}; i < temp_log::_NUM_SENSORS_MAX; ++i)
        {
            for(unsigned char j{0}; j < 8; ++j)
            {
                out += byte_to_hex(addresses[i][j]);
            }

            if(i < temp_log::_NUM_SENSORS_MAX - 1)
            {
                lt.append_separator(out);
            }
        }

        appendToFile(out, lt.current_filename());

        return;
    }

    String sensor_address_to_string(unsigned char address[8])
    {
        String out("");
        out.reserve(17);
        for(unsigned char i{0}; i < 8; ++i)
            {
                out += byte_to_hex(address[i]);
            }
        return out;
    }
    
    String byte_to_hex(const unsigned char& input)
    {
        String out{ "00" };
        constexpr char mask{ 0x0f };
        out[0] = subbyte_to_hex((input >> 4) & mask);
        out[1] = subbyte_to_hex(input & mask);
        
        return out;
    }

    char subbyte_to_hex(const unsigned char& slice)
    {
        if (slice < 10)
            return (slice + '0');
        else if (slice < 16)
            return((slice - 10) + 'A');
        else
            return '#';
    }

}