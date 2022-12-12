/*! @file logtime.cpp
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

#include "logtime.h"

namespace sdlog
{
  LogTime::LogTime(){
      RTC_DS1307();
      this->prefix = ' ';
      return;
  }

  LogTime::~LogTime(){
      return;
  }

  String LogTime::iso_now(bool filesys, bool brackets) const
  {
    DateTime currentTime {now()};
    String out {""};
    char timeseparator {':'};

    if(filesys)
      timeseparator = '-';

    if(brackets)
      out += ("[");

    out += zerofill(currentTime.year(), 4);
    out += "-";
    out += zerofill(currentTime.month(), 2);
    out += "-";
    out += zerofill(currentTime.day(), 2);
    out += " ";
    out += zerofill(currentTime.hour(), 2);
    out += timeseparator;
    out += zerofill(currentTime.minute(), 2);
    out += timeseparator;
    out += zerofill(currentTime.second(), 2);
    
    if(brackets)
      out += "[";

    return out;
  }

  String LogTime::current_filename() const
  {
    DateTime currentTime {now()};
    String out{};

    if(this->prefix != ' ')
      out += prefix;

    out += zerofill(currentTime.year(), 4) + "-" + zerofill(currentTime.month(), 2);
    out += ".csv";

    return out;
  }

  String LogTime::year_month() const{
    DateTime currentTime = now();
    String out = "";

    out += zerofill(currentTime.year(), 4);
    out += "-";
    out += zerofill(currentTime.month(), 2);

    return out;
  }

  String LogTime::year() const
  {
    DateTime currentTime = now();
    String out = "";

    out = zerofill(currentTime.year(), 4);

    return out;
  }

  String LogTime::zerofill(int value, int numZero) const
  {
    String textOut = "";
    int multi = 1;
    
    for(int i = 0; i < (numZero-1); i++)
    {
      multi *= 10;
    }
    
    while(multi > value)
    {
      textOut.concat("0");
      if(multi > 10)
        multi /= 10;
      else
        break;
    }
    textOut.concat(value);
    return textOut;
  }

  void LogTime::set_prefix(char prefix)
  {
    this->prefix = prefix;
    return;
  }

  void LogTime::append_separator(String& text) const
  {
    text.concat(';');
    return;
  }
}