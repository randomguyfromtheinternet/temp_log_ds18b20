/*! @file min_time_hm.h
 *! @author Tobias Rolke (github.com/randomguyfromtheinternet/)
 *! @version 1.0
 *! @date 2022-11-18
 *! @brief Only stores hours and minutes in a small memory footprint.
    Can add and substract times with or without rolling over midnight.
    There is also a flag for detected invalid times, when overflow is
    not active.
  
    Made for use with Arduino, no dependencies.
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

#ifndef _MIN_TIME_HM_H_
#define _MIN_TIME_HM_H_

namespace min_time
{
    constexpr unsigned char _FLAG_TIME_INVALID{ 0b10000000 };

    class TimeHM
    {
        protected:
        bool overflow;
        unsigned char hour;
        unsigned char minute;

        public:
        TimeHM();
        TimeHM(const unsigned char& h, const unsigned char& m);
        ~TimeHM() = default;

        char get_hours() const;
        char get_minutes() const;
        void set_hours(const unsigned char& h);
        void set_minutes(const unsigned char& m);
        void set_overflow(const bool& o);
        void add(const TimeHM& diff);
        void substract(const TimeHM& diff);
        bool equals(const TimeHM& comp) const;
        bool is_valid() const;
        void set_invalid();
        TimeHM operator+(const TimeHM& other) const;
        TimeHM operator-(const TimeHM& other) const;
        bool operator==(const TimeHM& other) const;
        bool operator<(const TimeHM& other) const;
    };
}

#endif