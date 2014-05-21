/*
 * Control.h
 *
 *  Created on: May 20, 2014
 *      Author: rodrigo
 */

#ifndef CONTROL_H_
#define CONTROL_H_

#include "ProjDefs.h"

#define SAMPLES_QTD		20

#define PITCH_MAX_OUT	100.0F
#define PITCH_MIN_OUT	-100.0F

#define ROLL_MAX_OUT	100.0F

//Forward declaration
class Attitude;
class Barometer;

class Control: public Thread
{
public:
	static Control* GetInstance();

	~Control();

private:
	Control();
	static Control* instance;

	float get_pitch_roll_interval();
	float get_baro_interval();

	void* Run();

	Attitude* attitude;
	Barometer* barometer;

	PID* pitch_pid;
	PID* roll_pid;
	PID* altitude_pid;
};

#endif /* CONTROL_H_ */
