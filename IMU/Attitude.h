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

struct ATTIUDE_CAL
{
	float pitch;
	float roll;
};

class Attitude {
public:
	Attitude();
	~Attitude();

	void get_pitch_and_roll(float& pitch, float& roll);
	void get_pitch_and_roll_no_cal(float& pitch, float& roll);

	void getEstimatedAttitude();

	float get_interval_seconds();

	bool save_cal(float pitch, float roll);

	void restore_calibration();

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

	float pitch_cal_;
	float roll_cal_;
};

#endif /* ATTITUDE_H_ */
