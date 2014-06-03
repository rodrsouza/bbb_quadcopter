/*
 * Control.cpp
 *
 *  Created on: May 20, 2014
 *      Author: rodrigo
 */

#include "Control.h"
#include "../IMU/Attitude.h"
#include "../Hardware/Barometer.h"
#include "../Hardware/ESC.h"


Control* Control::instance = NULL;

Control::Control() :
	Thread(CONTROL_PRIORITY, SCHED_RR),
	pitch_pid(NULL),
	roll_pid(NULL),
	altitude_pid(NULL),
	pitch_setpoint(0.0F),
	roll_setpoint(0.0F),
	altitude_setpoint(0.0F)
{
	attitude = new Attitude();
	barometer = new Barometer();

	esc = new ESC();

	float interval = get_pitch_roll_interval();

	pitch_pid = new PID(P_PITCH, I_PITCH, D_PITCH, interval);
	roll_pid = new PID(P_ROLL, I_ROLL, D_ROLL, interval);
	//altitude_pid = new PID(P_ALTITUDE, I_ALTITUDE, D_ALTITUDE, );

	pitch_pid->setInputLimits(MIN_DEGREE_LIMIT, MAX_DEGREE_LIMIT);
	roll_pid->setInputLimits(MIN_DEGREE_LIMIT, MAX_DEGREE_LIMIT);
	altitude_pid->setInputLimits(MIN_ALTITUDE, MAX_ALTITUDE);

	pitch_pid->setMode(AUTO_MODE);
	roll_pid->setMode(AUTO_MODE);
	altitude_pid->setMode(AUTO_MODE);

	pitch_pid->setOutputLimits(PITCH_MIN_OUT, PITCH_MAX_OUT);
	roll_pid->setOutputLimits(ROLL_MIN_OUT, ROLL_MAX_OUT);
	altitude_pid->setOutputLimits(ALTITUDE_MIN_OUT, ALTITUDE_MAX_OUT);

	polling_interval = static_cast<int>(ceilf(interval/1000000.0F));
}

Control* Control::GetInstance()
{
	if(!instance)
	{
		instance = new Control();
	}

	return instance;
}

Control::~Control()
{
	if(attitude != NULL)
	{
		delete attitude;
	}

	if(barometer != NULL)
	{
		delete barometer;
	}

	if(esc != NULL)
	{
		delete esc;
	}

	if(pitch_pid != NULL)
	{
		delete pitch_pid;
	}

	if(roll_pid != NULL)
	{
		delete roll_pid;
	}

	if(altitude_pid != NULL)
	{
		delete altitude_pid;
	}
}


void* Control::Run()
{
	float pitch = 0.0F;
	float roll = 0.0F;

	float pitch_pwm = 0.0F;
	float roll_pwm = 0.0F;
	float altitude_pwm = 0.0F;

	int counter = 0;

	while(!Should_Stop())
	{
		if(is_flying_)
		{
			refresh_set_point();
			attitude->getEstimatedAttitude();
			attitude->get_pitch_and_roll(pitch, roll);

			pitch_pid->setProcessValue(pitch);
			roll_pid->setProcessValue(roll);

			pitch_pwm = pitch_pid->compute();
			roll_pwm = roll_pid->compute();

			altitude_pwm = altitude_pid->compute();

			refresh_pwm(pitch_pwm, roll_pwm, altitude_pwm);
		}
		else if(is_landing_)
		{
			refresh_set_point();
			attitude->getEstimatedAttitude();
			attitude->get_pitch_and_roll(pitch, roll);

			pitch_pid->setProcessValue(pitch);
			roll_pid->setProcessValue(roll);

			pitch_pwm = pitch_pid->compute();
			roll_pwm = roll_pid->compute();

			if(!counter)
			{
				//altitude_pwm = compute_offset_for_landing(altitude_pwm);
				//counter = LANDING_REFRESH_RATE;
			}
			else
			{
				--counter;
			}

			refresh_pwm(pitch_pwm, roll_pwm, altitude_pwm);
		}
		else if(landed_)
		{
			//desliga motor
			//desliga luz de pouso
			//manda para idle
		}
		else
		{
			//idle state;
		}

		usleep(polling_interval);
	}
}

float Control::get_pitch_roll_interval()
{
	float interval = 0;
	int i;

	for(i=0; i<SAMPLES_QTD; ++i)
	{
		attitude->getEstimatedAttitude();
		interval += attitude->get_interval_seconds();
	}

	return interval/static_cast<float>(SAMPLES_QTD);
}

inline void Control::refresh_pwm(float pitch_pwm, float roll_pwm, float offset_pwm)
{
	float front;
	float back;
	float left;
	float right;

	float half_pitch;
	float half_roll;

	if(pitch_pwm < 0.0F)
	{
		half_pitch = fabs(pitch_pwm)/2.0F;

		front = offset_pwm - half_pitch;
		back = offset_pwm + half_pitch;
	}
	else
	{
		half_pitch = pitch_pwm/2.0F;

		front = offset_pwm + half_pitch;
		back = offset_pwm - half_pitch;
	}

	if(roll_pwm < 0.0F)
	{
		half_roll = fabs(roll_pwm)/2.0F;

		left = offset_pwm - half_roll;
		right = offset_pwm + half_roll;
	}
	else
	{
		half_roll = roll_pwm/2.0F;

		left = offset_pwm + half_roll;
		right = offset_pwm - half_roll;
	}

	esc->front(front);
	esc->back(back);
	esc->left(left);
	esc->right(right);
}
