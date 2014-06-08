/*
 * ESC.cpp
 *
 *  Created on: Apr 24, 2014
 *      Author: rodrigo
 */

#include "ProjDefs.h"
#include "ESC.h"
#include "PWM.h"

#include <math.h>

ESC* ESC::instance = NULL;

ESC::ESC() :
	pwm_(NULL)
{
	pwm_ = new PWM(ESC_FREQUENCY);
}


ESC* ESC::GetInstance()
{
	if(!instance)
	{
		instance = new ESC();
	}

	return instance;
}

ESC::~ESC()
{
}

void ESC::InitializePWM()
{
	pwm_->Initialize();
}

void ESC::InitializeESC()
{
	Idle();

	sleep(2);
}

void ESC::Idle()
{
	front(0.0F);
	back(0.0F);
	left(0.0F);
	right(0.0F);
}

void ESC::turn_on_engines()
{
	pwm_->front(11.0F);
	pwm_->back(11.0F);
	pwm_->left(11.0F);
	pwm_->right(11.0F);
}

void ESC::OpenFiles()
{
	pwm_->OpenFiles();
}

void ESC::CloseFiles()
{
	pwm_->CloseFiles();
}

void ESC::front(float duty_cycle)
{
	uint32_t duty_period = static_cast<uint32_t>(roundf(((duty_cycle/100.0F)*MINIMUM_PERIOD_F) + MINIMUM_PERIOD_F));

	pwm_->front(duty_period);
}

void ESC::back(float duty_cycle)
{
	uint32_t duty_period = static_cast<uint32_t>(roundf(((duty_cycle/100.0F)*MINIMUM_PERIOD_F) + MINIMUM_PERIOD_F));

	pwm_->back(duty_period);
}

void ESC::left(float duty_cycle)
{
	uint32_t duty_period = static_cast<uint32_t>(roundf(((duty_cycle/100.0F)*MINIMUM_PERIOD_F) + MINIMUM_PERIOD_F));

	pwm_->left(duty_period);
}

void ESC::right(float duty_cycle)
{
	uint32_t duty_period = static_cast<uint32_t>(roundf(((duty_cycle/100.0F)*MINIMUM_PERIOD_F) + MINIMUM_PERIOD_F));

	pwm_->right(duty_period);
}

