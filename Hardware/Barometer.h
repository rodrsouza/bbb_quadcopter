/*
 * Control.h
 *
 *  Created on: May 9, 2014
 *      Author: rodrigo
 */

#ifndef BAROMETER_H_
#define BAROMETER_H_

#include "../Hardware/I2CSensors.h"

struct BAROMETER_DATA
{
	float pressure;
	float temperature;
};

class Barometer
{
public:
	Barometer();
	~Barometer();

	float get_temperature_ts();

	float get_pressure_ts();

	float get_altitude_ts();

	float get_filtered_altitude_ts();

	void clean_current_altitude();

	void zero_altitude_ts();

private:
	void compute_temp_press(BAROMETER_DATA* bar_data);

	float dead_band(float value);

private:
	PROM_COEF prom_coef_;
	I2CSensors* sensors;

	Mutex current_bar_mutex;
	BAROMETER_DATA current_bar_data;

	Mutex zero_alt_mutex;
	BAROMETER_DATA zero_altitude_data;

	Mutex current_alt_mutex;
	float current_altitude;
};

#endif /* BAROMETER_H_ */
