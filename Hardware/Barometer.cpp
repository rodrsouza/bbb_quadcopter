/*
 * Barometer.cpp
 *
 *  Created on: May 9, 2014
 *      Author: rodrigo
 */

#include "Barometer.h"
#include <cstdlib>
#include <cstdio>
#include <cmath>

#define BARO_TAB_SIZE   21

Barometer::Barometer() :
	current_altitude(0.0)
{
	sensors = new I2CSensors();

	sensors->get_prom_coef(&prom_coef_);

	sensors->Start();
}

Barometer::~Barometer()
{
	if(sensors!=NULL)
	{
		sensors->Stop();

		sleep(1);

		delete sensors;
	}
}

void Barometer::compute_temp_press(BAROMETER_DATA* bar_data)
{
	double D1 = static_cast<double>(sensors->get_pressure());
	float D2 = static_cast<float>(sensors->get_temperature());
	float dT, temp, press;
	double off, sens;

	dT = (D2 - static_cast<float>(prom_coef_.C5 * 256));
	temp = 2000.0F + (dT * static_cast<float>(static_cast<float>(prom_coef_.C6)/8388608.0F));

	bar_data->temperature = roundf(temp)/100.0F;

	off = static_cast<double>(((static_cast<double>(prom_coef_.C2) * static_cast<double>(65536.0))) + static_cast<double>((static_cast<double>(prom_coef_.C4) * static_cast<double>(dT))/ static_cast<double>(128.0)) );
	sens = static_cast<double>( (static_cast<double>(prom_coef_.C1) * static_cast<double>(32768.0)) + ((static_cast<double>(prom_coef_.C3) * static_cast<double>(dT))/static_cast<double>(256.0)));

	press = static_cast<float>( ( ((D1*sens)/static_cast<double>(2097152.0)) - off) / static_cast<double>(32768.0));

	bar_data->pressure = roundf(press)/100.0F;
}


float Barometer::get_temperature_ts()
{
	Lock lock(current_bar_mutex);

	return current_bar_data.temperature;
}

float Barometer::get_pressure_ts()
{
	Lock lock(current_bar_mutex);

	compute_temp_press(&current_bar_data);

	return current_bar_data.pressure;
}

float Barometer::get_altitude_ts()
{
	Lock lock1(current_bar_mutex);
	Lock lock2(zero_alt_mutex);

	compute_temp_press(&current_bar_data);

	// on faster CPUs use a more exact calculation
	float scaling = current_bar_data.pressure / zero_altitude_data.pressure;
	float temp    = zero_altitude_data.temperature + 273.15F;

	// This is an exact calculation that is within +-2.5m of the standard atmosphere tables
	// in the troposphere (up to 11,000 m amsl).
	float ret = (153.8462F * temp * (1.0F - expf(0.190259F * logf(fabs(scaling)))));

	ret = ( (scaling<0) ? ((-1.0F)*ret) : ret );

	return ret;
}

void Barometer::zero_altitude_ts()
{
	float sum_press;
	float sum_temp;
	int i;
	Lock lock(zero_alt_mutex);

	for(i=0, sum_press=sum_temp=0.0; i<10; ++i)
	{
		compute_temp_press(&zero_altitude_data);

		sum_press += zero_altitude_data.pressure;
		sum_temp += zero_altitude_data.temperature;

		usleep(30000);
	}

	zero_altitude_data.pressure = sum_press/10.0;
	zero_altitude_data.temperature = sum_temp/10.0;
}

float Barometer::get_filtered_altitude_ts()
{
	Lock lock(current_alt_mutex);

	current_altitude =  dead_band((((current_altitude * 6.0F) + (get_altitude_ts() * 2.0F))  / 8.0F));

	return current_altitude;
}

void Barometer::clean_current_altitude()
{
	Lock lock(current_alt_mutex);

	current_altitude = 0.0;
}

float Barometer::dead_band(float value)
{
	if(fabs(value) < 0.1F)
	{
	value = 0;
	}
	else if(value > 0)
	{
	  value -= 0.1F;
	}
	else if(value < 0)
	{
	  value += 0.1F;
	}

	return value;
}


/*
The formula can also be expressed in metric units, as shown below:

Z = C ( Bo - B ) / ( Bo + B )
C = 16000 ( 1 + 2 ( To + T ) ) / 1000

Z = The difference in altitude in meters.
Bo and B = Barometric pressures in mm of mercury at two altitudes.
To and T = Centigrade temperatures at the two altitudes.

1 millibar =
0.750061683 mmhg


EstAlt = (EstAlt * 6 + BaroAlt * 2) >> 3; // additional LPF to reduce baro noise (faster by 30 µs)
 */
