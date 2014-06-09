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
	altitude_setpoint(0.0F),
	is_landing_(false),
	is_flying_(false),
	landed_(false)
{
	attitude = new Attitude();
	barometer = new Barometer();

	esc = ESC::GetInstance();

	esc->OpenFiles();

	esc->Idle();

	float interval = get_pitch_roll_interval();

	printf("Interval: %f\n", interval);

	pitch_pid = new PID(P_PITCH, I_PITCH, D_PITCH, interval);
	roll_pid = new PID(P_ROLL, I_ROLL, D_ROLL, interval);
	//altitude_pid = new PID(P_ALTITUDE, I_ALTITUDE, D_ALTITUDE, );

	pitch_pid->setInputLimits(MIN_DEGREE_LIMIT, MAX_DEGREE_LIMIT);
	roll_pid->setInputLimits(MIN_DEGREE_LIMIT, MAX_DEGREE_LIMIT);
	//altitude_pid->setInputLimits(MIN_ALTITUDE, MAX_ALTITUDE);

	pitch_pid->setMode(AUTO_MODE);
	roll_pid->setMode(AUTO_MODE);
	//altitude_pid->setMode(AUTO_MODE);

	pitch_pid->setOutputLimits(PITCH_MIN_OUT, PITCH_MAX_OUT);
	roll_pid->setOutputLimits(ROLL_MIN_OUT, ROLL_MAX_OUT);
	//altitude_pid->setOutputLimits(ALTITUDE_MIN_OUT, ALTITUDE_MAX_OUT);

	polling_interval = static_cast<int>(ceilf(interval*100000));
	polling_interval *= 10;

	Start();

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
	float altitude_pwm = 30.0F;

	int counter = 0;

	int i;

	for(i=0; i < 12000; i++)
	{
		attitude->getEstimatedAttitude();

		usleep(polling_interval);
	}


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

			//printf("pitch: %2.2f pitch_pwm: %2.2f\n", pitch, pitch_pwm);
			//printf("roll: %2.2f\n", roll);
/*
			if(counter==3000 && altitude_pwm <= 55.0F)
			{
				altitude_pwm++;
				counter = 0;
			}
			else if(altitude_pwm <= 55.0F)
			{
				counter++;
			}
*/

			//altitude_pwm = altitude_pid->compute();

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
				altitude_pwm -= LANDING_RATE;

				if(altitude_pwm < 0.0F)
				{
					altitude_pwm = 0.0F;
					is_landing_ = false;
					landed_ = true;
				}
			}
			else
			{
				--counter;
			}

			refresh_pwm(pitch_pwm, roll_pwm, altitude_pwm);
		}
		else if(landed_)
		{
			esc->Idle();
			counter = LANDING_REFRESH_RATE;

			landed_= false;
			is_flying_ = false;
			is_landing_ = false;
			//desliga luz de pouso
			//manda para idle
		}
		else
		{
			attitude->getEstimatedAttitude();
		}

		usleep(polling_interval);
	}

	ESC::GetInstance()->Idle();
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
		half_pitch = fabs(pitch_pwm)/2.0F;

		front = offset_pwm + half_pitch;
		back = offset_pwm - half_pitch;
	}

	if(roll_pwm > 0.0F)
	{
		half_roll = fabs(roll_pwm)/2.0F;

		left = offset_pwm - half_roll;
		right = offset_pwm + half_roll;
	}
	else
	{
		half_roll = fabs(roll_pwm)/2.0F;

		left = offset_pwm + half_roll;
		right = offset_pwm - half_roll;
	}

	//system("clear");
	//printf("F: %2.2F\nT: %2.2F\nE: %2.2F\nD: %2.2F\n", front, back, left, right);

	esc->front(front);
	esc->back(back);
	esc->left(left);
	esc->right(right);
}

void Control::set_landing()
{
	is_flying_ = false;
	is_landing_ = true;
	landed_ = false;
}

void Control::turn_on_engines()
{
	esc->turn_on_engines();
}

void Control::set_flying()
{
	is_flying_ = true;
	is_landing_ = false;
	landed_ = false;
}

bool Control::is_flying()
{
	return is_flying_;
}

bool Control::is_idle()
{
	return !(is_flying_ || is_landing_ || landed_);
}

void Control::setPitchValue(float value)
{
	Lock lock(positions_mutex);

	pitch_setpoint = value;
}

void Control::setRollValue(float value)
{
	Lock lock(positions_mutex);

	roll_setpoint = value;
}

void Control::setAltitude(float value)
{
	Lock lock(positions_mutex);

	altitude_setpoint = value;
}
