/*
 * PWM.h
 *
 *  Created on: Dec 10, 2013
 *      Author: rodrigo
 */

#ifndef PWM_H_
#define PWM_H_

//#define ESC_FREQUENCY	50 //Hz
#define ESC_FREQUENCY	480 //Hz
//#define ENGINE_FREQUENCY	8000 //Hz
#define ENGINE_PERIOD(freq)		1000000000/freq
#define ENGINE_PERIOD_F		((float) ENGINE_PERIOD)

#define MINIMUM_PERIOD		1000000 //1 ms
#define MINIMUM_PERIOD_F	1000000.0F //1 ms

#include "ProjDefs.h"
#include <cstdio>

class PWM {
public:
	/**
	 * Constructor
	 */
	PWM(int frequency);

	/**
	 * Destructor
	 */
	~PWM();

	/**
	 * Used to initialize the PWM peripheral
	 */
	void Initialize();

	void OpenFiles();

	void CloseFiles();

	void front(uint32_t duty_period);
	void left(uint32_t duty_period);
	void right(uint32_t duty_period);
	void back(uint32_t duty_period);

private:
	int frequency_;

	FILE* front_pwm;
	FILE* back_pwm;
	FILE* left_pwm;
	FILE* right_pwm;
};

#endif /* PWM_H_ */
