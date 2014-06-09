/*
 * Attitude.cpp
 *
 *  Created on: May 14, 2014
 *      Author: rodrigo
 */

#include "Attitude.h"
#include "ProjDefs.h"
#include <sys/time.h>
#include <cmath>

Attitude::Attitude() :
	pitch_(0.0F),
	roll_(0.0F),
	pitch_cal_(0.0F),
	roll_cal_(0.0F),
	interval_(0.0F)
{
	struct timeval time;

	sensors = I2CSensors::GetInstance();

	restore_calibration();

	gettimeofday(&time, NULL);

	previousTimeinMicros = static_cast<uint64_t>( ( static_cast<uint64_t>(time.tv_sec * 1000000)) + time.tv_usec );
}

Attitude::~Attitude()
{

}

bool Attitude::save_cal(float pitch, float roll)
{
	FILE* cal_file;
	ATTIUDE_CAL attiude_cal;
	bool result;

	cal_file = fopen(CAL_FILE_PATH, "wb+");
	if( cal_file == NULL )
	{
		printf("Attitude: Error to create/open calibration file.");
		return false;
	}
	else
	{
		attiude_cal.pitch = pitch;
		attiude_cal.roll = roll;

		result = (fwrite(&attiude_cal, sizeof(ATTIUDE_CAL), 1, cal_file) == 1 );

		fclose(cal_file);
	}

	return result;
}

void Attitude::restore_calibration()
{
	FILE* cal_file;
	ATTIUDE_CAL attiude_cal;
	bool result;

	cal_file = fopen(CAL_FILE_PATH, "rb");
	if( cal_file == NULL )
	{
		printf("Attitude: Error to open calibration file.\n");
		printf("Attitude: Using 0 value.\n");

		pitch_cal_ = 0.0F;
		roll_cal_ = 0.0F;
	}
	else
	{
		if(fread(&attiude_cal, sizeof(ATTIUDE_CAL), 1, cal_file) == 1 )
		{
			pitch_cal_ = attiude_cal.pitch;
			roll_cal_ = attiude_cal.roll;

			printf("Attitude: Calibration file restored.\n");
			printf("Attitude: Pitch: %f\nAttitude:  Roll: %f\n", attiude_cal.pitch, attiude_cal.roll);
		}
		else
		{
			printf("Attitude: Error to read calibration file.\n");
			printf("Attitude: Using 0 value.\n");

			pitch_cal_ = 0.0F;
			roll_cal_ = 0.0F;
		}

		fclose(cal_file);
	}
}

void Attitude::get_pitch_and_roll_no_cal(float& pitch, float& roll)
{
	pitch = pitch_;
	roll = roll_;
}

void Attitude::get_pitch_and_roll(float& pitch, float& roll)
{
	pitch = (pitch_ - pitch_cal_);
	roll = (roll_ - roll_cal_);
}

float Attitude::get_interval_seconds()
{
	return interval_;
}

void Attitude::getEstimatedAttitude()
{
	float pitchAcc, rollAcc;

	sensors->get_acc(&acc_);
	sensors->get_gyro(&gyro_);

	float dT = static_cast<float>(gyro_.time_in_micros - previousTimeinMicros);

	dT /= 1000000.0F;
	interval_ = dT;

	previousTimeinMicros = gyro_.time_in_micros;

	// Integrate the gyroscope data -> int(angularSpeed) = angle
	pitch_ += (static_cast<float>(gyro_.X.data) / GYRO_DEGREES_PER_SECOND) * dT; // Angle around the X-axis
	roll_ -= (static_cast<float>(gyro_.Y.data) / GYRO_DEGREES_PER_SECOND) * dT;    // Angle around the Y-axis

	// Compensate for drift with accelerometer data if !bullshit
	// Sensitivity = -8 to 8 G at 16Bit -> 8G = 32768 && 0.5G = 2048
	int forceMagnitudeApprox = abs(acc_.X.data) + abs(acc_.Y.data) + abs(acc_.Z.data);

	if( (forceMagnitudeApprox > 2048) && (forceMagnitudeApprox < 32768) )
	{
		// Turning around the X axis results in a vector on the Y-axis
		pitchAcc = atan2f(static_cast<float>(acc_.Y.data), static_cast<float>(acc_.Z.data)) * 180.0F / M_PI;
		pitch_ = (pitch_ * 0.98F) + (pitchAcc * 0.02F);

		// Turning around the Y axis results in a vector on the X-axis
		rollAcc = atan2f(static_cast<float>(acc_.X.data), static_cast<float>(acc_.Z.data)) * 180.0F / M_PI;
		roll_ = (roll_ * 0.98F) + (rollAcc * 0.02F);
	}

	if(pitch_ > 180.0F)
	{
		pitch_ = 180.0F;
	}
	else if(pitch_ < -180.0F)
	{
		pitch_ = -180.0F;
	}

	if(roll_ > 180.0F)
	{
		roll_ = 180.0F;
	}
	else if(roll_ < -180.0F)
	{
		roll_ = -180.0F;
	}
}

