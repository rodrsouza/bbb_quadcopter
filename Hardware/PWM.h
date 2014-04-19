/*
 * PWM.h
 *
 *  Created on: Dec 10, 2013
 *      Author: rodrigo
 */

#ifndef PWM_H_
#define PWM_H_

#define ENGINE_FREQUENCY	8000 //Hz
#define ENGINE_PERIOD		1000000000/ENGINE_FREQUENCY

class PWM {
public:
	/**
	 * Constructor
	 */
	PWM();

	/**
	 * Destructor
	 */
	~PWM();

	/**
	 * Used to initialize the PWM peripheral
	 */
	bool Initialize();

	void front(int duty_cycle);
	void left(int duty_cycle);
	void right(int duty_cycle);
	void back(int duty_cycle);
};

#endif /* PWM_H_ */
