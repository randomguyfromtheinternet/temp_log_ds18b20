/*! @file min_time_conversion.cpp
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

#include "min_time_conversion.h"

namespace min_time
{
    String to_string(const TimeHM& origin)
    {
        String out{""};
		if(origin.is_valid())
		{
			unsigned char time[2]{ origin.get_hours(), origin.get_minutes() };
			out = String((time[0] % 100) / 10)
				+ String((time[0] % 10))
				+ String(_TIME_SEPARATOR)
				+ String((time[1] % 100) / 10)
				+ String((time[1] % 10));
		}
		else
		{
			out = "--" + String(_TIME_SEPARATOR) + "--";
		}

        return out;
    }

}