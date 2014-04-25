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

ESC::ESC() :
	pwm_(NULL)
{
}

ESC::~ESC()
{
}

void ESC::InitializePWM()
{
	pwm_ = new PWM(ESC_FREQUENCY);

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

