/*
 * PWM.cpp
 *
 *  Created on: Dec 10, 2013
 *      Author: rodrigo
 */

#include "PWM.h"
#include <cstdlib>
#include <cstring>

#include <unistd.h>
#include <stdint.h>

PWM::PWM(int frequency) :
	front_pwm(NULL),
	back_pwm(NULL),
	left_pwm(NULL),
	right_pwm(NULL)
{
	frequency_ = frequency;
}

void PWM::Initialize()
{
	uint32_t enginePeriod = ENGINE_PERIOD(frequency_);

	char buf[70];

	//Initializing the pwm module
	system("echo am33xx_pwm > /sys/devices/bone_capemgr.8/slots");
	sleep(1);

	//Configure pins to be used like pwm pins
	system("echo sc_pwm_P8_34 > /sys/devices/bone_capemgr.8/slots");
	sleep(1);

	system("echo sc_pwm_P8_36 > /sys/devices/bone_capemgr.8/slots");
	sleep(1);

	system("echo sc_pwm_P8_45 > /sys/devices/bone_capemgr.8/slots");
	sleep(1);

	system("echo sc_pwm_P8_46 > /sys/devices/bone_capemgr.8/slots");
	sleep(1);

	//Configure frequency for each pin
	memset(buf, '\0', sizeof(buf));

	sprintf(buf, "echo %d > /sys/devices/ocp.2/pwm_test_P8_34.11/period", enginePeriod);
	system(buf);

	memset(buf, '\0', sizeof(buf));

	sprintf(buf, "echo %d > /sys/devices/ocp.2/pwm_test_P8_36.12/period", enginePeriod);
	system(buf);

	memset(buf, '\0', sizeof(buf));

	sprintf(buf, "echo %d > /sys/devices/ocp.2/pwm_test_P8_45.13/period", enginePeriod);
	system(buf);

	memset(buf, '\0', sizeof(buf));

	sprintf(buf, "echo %d > /sys/devices/ocp.2/pwm_test_P8_46.14/period", enginePeriod);
	system(buf);

	//Adjusting polarity
	system("echo 0 > /sys/devices/ocp.2/pwm_test_P8_34.11/polarity");
	system("echo 0 > /sys/devices/ocp.2/pwm_test_P8_36.12/polarity");
	system("echo 0 > /sys/devices/ocp.2/pwm_test_P8_45.13/polarity");
	system("echo 0 > /sys/devices/ocp.2/pwm_test_P8_46.14/polarity");

	//Turn on individual modules
	system("echo 1 > /sys/devices/ocp.2/pwm_test_P8_34.11/run");
	system("echo 1 > /sys/devices/ocp.2/pwm_test_P8_36.12/run");
	system("echo 1 > /sys/devices/ocp.2/pwm_test_P8_45.13/run");
	system("echo 1 > /sys/devices/ocp.2/pwm_test_P8_46.14/run");
}

PWM::~PWM()
{
}

void PWM::OpenFiles()
{

}

void PWM::CloseFiles()
{

}

void PWM::front(uint32_t duty_period)
{
	front_pwm = fopen("/sys/devices/ocp.2/pwm_test_P8_34.11/duty", "w");
	fprintf(front_pwm, "%d", duty_period);
	fclose(front_pwm);
}

void PWM::back(uint32_t duty_period)
{
	back_pwm = fopen("/sys/devices/ocp.2/pwm_test_P8_36.12/duty", "w");
	fprintf(back_pwm, "%d", duty_period);
	fclose(front_pwm);
}

void PWM::left(uint32_t duty_period)
{
	left_pwm = fopen("/sys/devices/ocp.2/pwm_test_P8_45.13/duty", "w");
	fprintf(left_pwm, "%d", duty_period);
	fclose(left_pwm);
}

void PWM::right(uint32_t duty_period)
{
	right_pwm = fopen("/sys/devices/ocp.2/pwm_test_P8_46.14/duty", "w");
	fprintf(right_pwm, "%d", duty_period);
	fclose(right_pwm);
}



