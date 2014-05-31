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
#include


#define SAMPLES_QTD		20

#define MAX_PWM			30.0F


#define PITCH_MAX_OUT	100.0F
#define PITCH_MIN_OUT	-100.0F

#define ROLL_MAX_OUT	100.0F
#define ROLL_MIN_OUT	-100.0F

#define ALTITUDE_MAX_OUT	100.0F
#define ALTITUDE_MIN_OUT	0.0F

#if defined(MAX_PWM)
	#undef	ALTITUDE_MAX_OUT
	#define ALTITUDE_MAX_OUT	MAX_PWM

	#undef	PITCH_MAX_OUT
	#define PITCH_MAX_OUT	MAX_PWM
	#undef 	PITCH_MIN_OUT
	#define PITCH_MIN_OUT	-MAX_PWM

	#undef	ROLL_MAX_OUT
	#define ROLL_MAX_OUT	MAX_PWM
	#undef 	ROLL_MIN_OUT
	#define ROLL_MIN_OUT	-MAX_PWM
#endif


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
#define MIN_ALTITUDE	-1.0F

#define MAX_DEGREE_LIMIT		180.0F
#define MIN_DEGREE_LIMIT		-180.0F

#define MAX_PWM			30.0F

#define LANDING_RATE
#define LANDING_REFRESH_RATE

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

	void set_landing();
	void turn_on_engines();
	void set_flying();

	void is_flying();
	void is_idle();

	void propelers();

	~Control();

private:
	Control();
	static Control* instance;

	float get_pitch_roll_interval();
	float get_baro_interval();

	void* Run();

	void refresh_set_point();

	float compute_offset_for_landing(float B);

	void refresh_pwm(float pitch_pwm, float roll_pwm, float offset_pwm);

	Attitude* attitude;
	Barometer* barometer;

	ESC* esc;

	int polling_interval;

	PID* pitch_pid;
	PID* roll_pid;
	PID* altitude_pid;

	volatile bool is_landing_;
	volatile bool is_flying_;
	volatile bool landed_;

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
