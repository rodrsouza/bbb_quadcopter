/*
 * PWM.cpp
 *
 *  Created on: Dec 10, 2013
 *      Author: rodrigo
 */

#include "PWM.h"
#include <cstdlib>
#include <cstring>
#include <cstdio>
#include <unistd.h>

PWM::PWM()
{
}

bool PWM::Initialize()
{
	int enginePeriod = ENGINE_PERIOD;

	char buf[70];

	//Initializing the pwm module
	system("echo am33xx_pwm > /sys/devices/bone_capemgr.8/slots");
	sleep(2);

	//Configure pins to be used like pwm pins
	system("echo bone_pwm_P8_34 > /sys/devices/bone_capemgr.8/slots");
	sleep(2);

	system("echo bone_pwm_P8_36 > /sys/devices/bone_capemgr.8/slots");
	sleep(2);

	system("echo bone_pwm_P8_45 > /sys/devices/bone_capemgr.8/slots");
	sleep(2);

	system("echo bone_pwm_P8_46 > /sys/devices/bone_capemgr.8/slots");
	sleep(2);

	//Configure frequency for each pin
	memset(buf, '\0', sizeof(buf));

	sprintf(buf, "echo %d > /sys/devices/ocp.2/pwm_test_P8_34.11/period", enginePeriod);
	printf(buf);
	system(buf);

	memset(buf, '\0', sizeof(buf));

	sprintf(buf, "echo %d > /sys/devices/ocp.2/pwm_test_P8_36.12/period", enginePeriod);
	printf(buf);
	system(buf);

	memset(buf, '\0', sizeof(buf));

	sprintf(buf, "echo %d > /sys/devices/ocp.2/pwm_test_P8_45.13/period", enginePeriod);
	printf(buf);
	system(buf);

	memset(buf, '\0', sizeof(buf));

	sprintf(buf, "echo %d > /sys/devices/ocp.2/pwm_test_P8_46.14/period", enginePeriod);
	printf(buf);
	system(buf);
	sleep(1);
	//Force turn off
	/*
	system("echo 0 > /sys/devices/ocp.2/pwm_test_P8_34.11/duty");
	usleep(1000);
	system("echo 0 > /sys/devices/ocp.2/pwm_test_P8_36.12/duty");
	system("echo 0 > /sys/devices/ocp.2/pwm_test_P8_45.13/duty");
	system("echo 0 > /sys/devices/ocp.2/pwm_test_P8_46.14/duty");
*/
}

PWM::~PWM() {
	// TODO Auto-generated destructor stub
}

void PWM::front(int duty_cycle)
{
	char buf[70];
	unsigned int duty_period = (ENGINE_PERIOD*duty_cycle)/100;

	sprintf(buf, "echo %d > /sys/devices/ocp.2/pwm_test_P8_34.11/duty", duty_period);
}

void PWM::left(int duty_cycle)
{
	char buf[70];
	unsigned int duty_period = (ENGINE_PERIOD*duty_cycle)/100;

	sprintf(buf, "echo %d > /sys/devices/ocp.2/pwm_test_P8_36.12/duty", duty_period);
}

void PWM::right(int duty_cycle)
{
	char buf[70];
	unsigned int duty_period = (ENGINE_PERIOD*duty_cycle)/100;

	sprintf(buf, "echo %d > /sys/devices/ocp.2/pwm_test_P8_45.13/duty", duty_period);
}

void PWM::back(int duty_cycle)
{
	char buf[70];
	unsigned int duty_period = (ENGINE_PERIOD*duty_cycle)/100;

	sprintf(buf, "echo %d > /sys/devices/ocp.2/pwm_test_P8_46.14/duty", duty_period);
}



