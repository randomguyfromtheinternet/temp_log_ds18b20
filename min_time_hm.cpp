/*! @file min_time_hm.cpp
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

#include "min_time_hm.h"

namespace min_time
{
    TimeHM::TimeHM()
    {
        overflow = true;
        hour = _FLAG_TIME_INVALID;
        minute = 0;
        return;
    }

    TimeHM::TimeHM(const unsigned char& h, const unsigned char& m)
    {
        overflow = true;
        minute = m % 60;
        hour = h + (m / 60) % 24;
        
        return;
    }

    // GET Functions
    //   - for hours
    char TimeHM::get_hours() const
    {
        // check if value is invalid
        if (!is_valid())
            return 0;
        
        // valid value: return variable
        return hour;
    }

    //   - for minutes
    char TimeHM::get_minutes() const
    {
        // check if value is invalid
        if (!is_valid())
            return 0;

        // valid value: return variable
        return minute;
    }

    // GET Functions
    //   - for hours
    void TimeHM::set_hours(const unsigned char& h)
    {
        if(overflow)
            hour = h % 24;
        else
        {
            if (h < 24)
                hour = h;
            else
                set_invalid();
        }
        return;
    }

    //   - for minutes
    void TimeHM::set_minutes(const unsigned char& m)
    {
        if(overflow)
        {
            minute = m % 60;
            if(m >= 60)
                set_hours(hour + (m / 60));
        }
        else
        {
            if(m < 60)
                minute = m;
            else
                set_invalid();
        }
        return;
    }

    void TimeHM::set_overflow(const bool& o)
    {
        overflow = o;
        return;
    }

    // Add two time objects.
    // Overflow = true (standard) will calculate further from 00:00, when reaching 24:00
    // Overflow = false will limit to 00:00 / 23:59
    void TimeHM::add(const TimeHM& diff)
    {
        short sum_m = minute + diff.get_minutes();
        short sum_h = hour + diff.get_hours();

        if(overflow)
        {
            minute = sum_m % 60;
            hour = (sum_h + (sum_m / 60)) % 24;
        }
        else
        {
            if(sum_h + (sum_m / 60) <= 23)
            {
                minute = sum_m % 60;
                hour = (sum_h + (sum_m / 60));
            }
            else
            {
                minute = 59;
                hour = 23;
            }
        }
        return;
    }

    // Substract two time objects.
    // Overflow = true (standard) will calculate further from 00:00, when reaching 24:00
    // Overflow = false will limit to 00:00 / 23:59
    void TimeHM::substract(const TimeHM& diff)
    {
        short dif_m = minute - diff.get_minutes();
        short dif_h = hour - diff.get_hours();

        if(overflow)
        {
            if (dif_m >= 0)
            {
                minute = dif_m % 60;
            }
            else
            {
                minute = dif_m % 60 + 60;
                dif_h -= 1;
            }
                
            if (dif_h >= 0)
            {
                hour = dif_h % 24;
            }
            else
            {
                hour = dif_h % 24 + 24;
            }
        }
        else
        {
            if(dif_h + (dif_m / 60) >= 0)
            {
                minute = dif_m % 60;
                hour = (dif_h + (dif_m / 60));
            }
            else
            {
                minute = 0;
                hour = 0;
            }
        }
        return;
    }

    // Compare with another time object
    bool TimeHM::equals(const TimeHM& comp) const
    {
        if(hour == comp.get_hours() && minute == comp.get_minutes())
            return true;
        return false;
    }

    bool TimeHM::is_valid() const
    {
        return (hour < _FLAG_TIME_INVALID);
    }

    void TimeHM::set_invalid()
    {
        hour = _FLAG_TIME_INVALID;
        minute = 0;
        return;
    }

    TimeHM TimeHM::operator+(const TimeHM& other) const
    {
        TimeHM out(hour, minute);
        out.add(other);
        return out;
    }

    TimeHM TimeHM::operator-(const TimeHM& other) const
    {
        TimeHM out(hour, minute);
        out.substract(other);
        return out;
    }
 
    bool TimeHM::operator==(const TimeHM& other) const
    {
        return equals(other);
    }

    bool TimeHM::operator<(const TimeHM& other) const
    {
        if(!other.is_valid() || !this->is_valid())
            return false;
            
        if(other.hour < hour)
            return true;
        if(other.hour == hour && other.minute < minute)
            return true;
        return false;
    }
}