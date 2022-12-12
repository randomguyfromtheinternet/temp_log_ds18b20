/*! @file min_time_conversion.h
 *! @author Tobias Rolke (github.com/randomguyfromtheinternet/)
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

#ifndef _MIN_TIME_CONVERSION_H_
#define _MIN_TIME_CONVERSION_H_

#include <Arduino.h>
#include "min_time_hm.h"

namespace min_time
{
    constexpr char _TIME_SEPARATOR = ':';
    constexpr char _DURATION_SEPARATOR = '!';

    String to_string(const TimeHM& origin);
}


#endif