/*! @file temp_ds18b20_log.ino
 *! @author Tobias Rolke (github.com/chillerwal/)
 *! @version 0.1
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

#include "temp_ds18b20_settings.h" // <-- PIN setup etc here 

#include <RTClib.h> // RTCLib by Adafruit
#include <dhtnew.h> // DHTNEW by Rob Tilaart
#include <OneWire.h> // OneWire by Jim Studt
#include <DallasTemperature.h> // DallasTemperature by Miles Burton


#include "temp_ds18b20_sdlog.h" // using: SD by Arduino
#include "logtime.h" // using: RTCLib by Adafruit

#include "min_time_hm.h"
#include "min_time_conversion.h"

//////////////////////////////////////////////////////////////////////////
// LOOP STATES
//
enum LoopState {
  initializing,
  idle,
  check_measure,
  measuring,
  fatalerror
};

//////////////////////////////////////////////////////////////////////////
// GLOBAL VARIABLES

// LogTime clock / SD object
sdlog::LogTime logtime;

// Temperature sensors and bus the sensors are connected to
OneWire ow(temp_log::pin::_TEMP_SENSOR);
DallasTemperature temp_sensors(&ow);
unsigned char num_connected_sensors{0};
unsigned char temp_sensor_address[temp_log::_NUM_SENSORS_MAX][8]{0};

// Measure interval control
min_time::TimeHM measuring_time{00,00}; // will be overwritten by current time + 1 minute in setup
min_time::TimeHM measuring_cycle{00,01}; //h,m

// Program loop control
LoopState state{LoopState::initializing};
LoopState last_loop_state{LoopState::initializing};

// Temperature value buffer
float current_temperature[temp_log::_NUM_SENSORS_MAX]{0.f};

// For Serial logging
char loop_state_map[]{"     "};

// status of status led
bool status_led{false};

//////////////////////////////////////////////////////////////////////////
// FUNCTIONS IN THIS FILE

void setup();
bool setup_clock();
bool setup_sd();
void setup_temp_sensors();

void loop();
bool check_measure_time();
void fill_loop_state_map();
bool check_measure_time();
void set_next_measure_time();
void set_onboard_led(bool state);

/// @fn setup
/// @brief setup routine before running loop function
/// @see loop
void setup()
{
  Serial.begin(9600);
  delay(500);

  // setup onboard LED
  pinMode(temp_log::pin::_LED_ONBOARD, OUTPUT);

  // fill serial log state acronym lookup table
  fill_loop_state_map();

  // Setup real time clock on SD shield
  if(!setup_clock())
  {
    Serial.println(F("E: Fatal!"));
    state = LoopState::fatalerror;
    return;
  }
  logtime.set_prefix('t');

  // setup SD card interface on SD shield
  if(!setup_sd())
  {
    Serial.println(F("E: Fatal!"));
    state = LoopState::fatalerror;
    return;
  }

  // give the system some time to set up...
  delay(500);

  // set up temperature sensor
  setup_temp_sensors();

  // set current time
  DateTime now{logtime.now()};

  // set up next measuring time in one minute
  measuring_time.set_overflow(true);
  measuring_time.set_hours(now.hour());
  measuring_time.set_minutes(now.minute());
  measuring_time = measuring_time + min_time::TimeHM{00,01};

  if(temp_log::_SERIAL_LOGGING)
  {
    Serial.println(min_time::to_string(min_time::TimeHM{now.hour(), now.minute()}));
    Serial.println("next: " + min_time::to_string(measuring_time));
  }

  measuring_cycle.set_overflow(true);

  state = LoopState::check_measure;
  return;
}

///
/// Helper functions
///

//! @brief setup clock on SD shield
bool setup_clock()
{
  // Detect and Start Real Time Clock (RTC)
  if(logtime.begin()){
    Serial.println(F("i: Clock OK"));
  }
  else{
    Serial.println(F("E: Clock fail"));
    return false;
  }

  // Initialize time if RTC is not running
  if(!logtime.isrunning())
    logtime.adjust(DateTime(F(__DATE__), F(__TIME__)));

  return true;
}

//! @brief setup SD interface
bool setup_sd()
{
  // SD Logger initialisieren
  if(!temp_log::init_sd_logging(temp_log::pin::_SD))
    return false;
  
  // Append "System boot" to file named as current month
  temp_log::log_boot(logtime);
  return true;
}

//! @fn setup_temp_sensors
//! @brief sets up DS18B20 temperature sensors using OneWire bus
void setup_temp_sensors()
{
  temp_sensors.begin();
  num_connected_sensors = (unsigned char) temp_sensors.getDeviceCount();
  for(unsigned char i = 0; i < temp_log::_NUM_SENSORS_MAX; ++i)
  {
    if(i <= num_connected_sensors)
      ow.search(temp_sensor_address[i]);
    else
    {
      for(unsigned char j{0}; j < 8; ++j)
      {
        temp_sensor_address[i][j] = 0;
      }
    }
  }

  if(temp_log::_SD_LOGGING)
  {
    temp_log::log_sensors(logtime, temp_sensor_address);
  }

  return;
}

/// @brief helper function to fill loop_state_map arrays with acronyms for terminal debugging
void fill_loop_state_map()
{
  loop_state_map[LoopState::initializing]   = '#';
  loop_state_map[LoopState::idle]           = '~';
  loop_state_map[LoopState::check_measure]  = 'c';
  loop_state_map[LoopState::measuring]      = 'm';
  loop_state_map[LoopState::fatalerror]     = '!';
}

/// @brief checks if set measuring_time matches current time (by minute)
/// @return true = measuring_time is current time, false = measuring_time is not the current time
bool check_measure_time()
{
  // objects to compare with each other
  DateTime now{logtime.now()};
    
  if((now.hour() == measuring_time.get_hours()) && (now.minute() == measuring_time.get_minutes()))
    return true;

  return false;
}

/// @brief calculates measuring_time: adds measuring_cycle to current time
void set_next_measure_time()
{
  DateTime now{logtime.now()};
  measuring_time = min_time::TimeHM(now.hour(), now.minute()) + measuring_cycle;
  return;
}

/// @brief switches the onboard led to given state
/// @param state true = on, false = off
void set_onboard_led(bool state)
{
  status_led = state;
  digitalWrite(temp_log::pin::_LED_ONBOARD, status_led);
  return;
}


///
/// @fn loop
/// @brief executes the main loop
void loop() {

  delay(10);
  
  switch(state)
  {
    case LoopState::initializing:
    {
      break;
    }

    case LoopState::idle:
    {
      delay(5000);
      state = LoopState::check_measure;

      break;
    }

    case LoopState::check_measure:
    {
      state = LoopState::idle;

      if(check_measure_time())
        state = LoopState::measuring;

      if(temp_log::_SERIAL_LOGGING)
      {
        DateTime now{logtime.now()};
        Serial.println(min_time::to_string(min_time::TimeHM{now.hour(), now.minute()}));
        Serial.println("next: " + min_time::to_string(measuring_time));
      }

      break;
    }

    case LoopState::measuring:
    {
        // Poll all sensors for current temperature
        for(unsigned char i{0}; i < num_connected_sensors; ++i)
        {
            current_temperature[i] = temp_sensors.getTempC(temp_sensor_address[i]);
        }
    
        /* TODO: Replace the code from the one DHT11 sensor
         * with the code for the multiple DS18B20 sensors
         */
      if(temp_log::_SD_LOGGING)
      {
        // log data to SD Card
        temp_log::log_temperature(logtime, current_temperature);
      }

      if(temp_log::_SERIAL_LOGGING)
      {
        for(unsigned char i{0}; i < temp_log::_NUM_SENSORS_MAX; ++i)
        {
            Serial.println("T" + (String)i + ": " + (String)current_temperature[i] + "°C");
        }
      }

      // set next measuring time
      set_next_measure_time();

      if(temp_log::_SERIAL_LOGGING)
      {
        Serial.println("next: " + min_time::to_string(measuring_time));
      }

      state = LoopState::idle;

      break;
    }

    case LoopState::fatalerror:
    {
      delay(600);
      set_onboard_led(!status_led);
      state = LoopState::fatalerror;

      break;
    }
  }

  // If loop state changes, log it!
  if(state != last_loop_state)
  {
    if(temp_log::_SERIAL_LOGGING)
    {
      Serial.println(loop_state_map[state]);        
    }

    last_loop_state = state;
  }
}
