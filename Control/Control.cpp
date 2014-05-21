/*
 * Control.cpp
 *
 *  Created on: May 20, 2014
 *      Author: rodrigo
 */

#include "Control.h"
#include "../IMU/Attitude.h"
#include "../Hardware/Barometer.h"
#include "PID.h"

Control* Control::instance = NULL;

Control::Control() :
	Thread(CONTROL_PRIORITY, SCHED_RR)
{
	attitude = new Attitude();
	barometer = new Barometer();
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
}


void* Control::Run()
{
	while(!Should_Stop())
	{

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
