/*
 * Attitude.h
 *
 *  Created on: May 14, 2014
 *      Author: rodrigo
 */

#ifndef ATTITUDE_H_
#define ATTITUDE_H_

#include "../Hardware/I2CSensors.h"

#define GYRO_DEGREES_PER_SECOND		16.4F

class Attitude {
public:
	Attitude();
	~Attitude();

	void get_pitch_and_roll(float& pitch, float& roll);

	void getEstimatedAttitude();

	float get_interval_seconds();

private:
	void get_acc_and_gyro_values();

private:
	uint64_t previousTimeinMicros;

	float interval_;

	I2CSensors* sensors;

	ACC_RAW acc_;
	GYRO_RAW gyro_;

	float pitch_;
	float roll_;
};

#endif /* ATTITUDE_H_ */
