/*
 * ESC.h
 *
 *  Created on: Apr 24, 2014
 *      Author: rodrigo
 */

#ifndef ESC_H_
#define ESC_H_

//Forward declaration of PWM class
class PWM;

#define MINIMUM_PERIOD		1000000 //1 ms
#define MINIMUM_PERIOD_F	1000000.0F //1 ms

class ESC
{
public:
	ESC();
	~ESC();

	void InitializePWM();

	void InitializeESC();

	void Idle();

	void front(float duty_cycle);
	void back(float duty_cycle);
	void left(float duty_cycle);
	void right(float duty_cycle);

private:
	PWM* pwm_;
};

#endif /* ESC_H_ */
