/*
 * Control.h
 *
 *  Created on: May 20, 2014
 *      Author: rodrigo
 */

#ifndef CONTROL_H_
#define CONTROL_H_

#include "ProjDefs.h"
#include "PID.h"


#define SAMPLES_QTD		20

#define PITCH_MAX_OUT	100.0F
#define PITCH_MIN_OUT	-100.0F

#define ROLL_MAX_OUT	100.0F
#define ROLL_MIN_OUT	-100.0F

#define ALTITUDE_MAX_OUT	100.0F
#define ALTITUDE_MIN_OUT	0.0F

#define P_PITCH			2.357142857F
#define I_PITCH			2.5F
#define D_PITCH			2.875F

#define P_ROLL			P_PITCH
#define I_ROLL			I_PITCH
#define D_ROLL			D_PITCH

#define P_ALTITUDE		4.571428571F
#define I_ALTITUDE		2.083333333F
#define D_ALTITUDE		3.0F

#define MAX_ALTITUDE	5.0F

#define MAX_PWM			30.0F

//Forward declaration
class Attitude;
class Barometer;
class ESC;

class Control: public Thread
{
public:
	static Control* GetInstance();

	void setPitchValue(float value);
	void setRollValue(float value);
	void setAltitude(float value);

	void propelers();

	~Control();

private:
	Control();
	static Control* instance;

	float get_pitch_roll_interval();
	float get_baro_interval();

	void* Run();

	void refresh_set_point();

	Attitude* attitude;
	Barometer* barometer;

	ESC* esc;

	PID* pitch_pid;
	PID* roll_pid;
	PID* altitude_pid;

	volatile bool is_landing_;
	volatile bool is_flying_;

	int counter_cycles;

	Mutex positions_mutex;
	float pitch_setpoint;
	float roll_setpoint;
	float altitude_setpoint;
};

inline void Control::refresh_set_point()
{
	Lock lock(positions_mutex);

	pitch_pid->setSetPoint(pitch_setpoint);
	roll_pid->setSetPoint(roll_setpoint);
	altitude_pid->setSetPoint(altitude_setpoint);
}

#endif /* CONTROL_H_ */
