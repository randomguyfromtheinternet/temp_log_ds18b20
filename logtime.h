/*! @file logtime.h
 *! @author Tobias Rolke (github.com/randomguyfromtheinternet/)
 *! @version 1.1
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


#ifndef _LOGTIME_H_
#define _LOGTIME_H_

#include <RTClib.h> // RTCLib by Adafruit

namespace sdlog
{
    class LogTime : public RTC_DS1307{
        public:
            LogTime();
            ~LogTime();
            String iso_now(bool filesys = false, bool brackets = false) const;
            String current_filename() const;
            String year_month() const;
            String year() const;
            String zerofill(int value, int numZero = 1) const;
            void set_prefix(char prefix);
            void append_separator(String& text) const;
        private:
            char prefix;
    };

}

#endif