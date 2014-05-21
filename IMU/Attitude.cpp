/*
 * Attitude.cpp
 *
 *  Created on: May 14, 2014
 *      Author: rodrigo
 */

#include "Attitude.h"
#include <cmath>

Attitude::Attitude() :
	previousTimeinMicros(0),
	pitch_(0.0F),
	roll_(0.0F),
	interval_(0.0F)
{
	sensors = I2CSensors::GetInstance();
}

Attitude::~Attitude()
{

}

void Attitude::get_pitch_and_roll(float& pitch, float& roll)
{
	pitch = pitch_;
	roll = roll_;
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

	if (forceMagnitudeApprox > 2048 && forceMagnitudeApprox < 32768)
	{
		// Turning around the X axis results in a vector on the Y-axis
		pitchAcc = atan2f(static_cast<float>(acc_.Y.data), static_cast<float>(acc_.Z.data)) * 180.0F / M_PI;
		pitch_ = (pitch_ * 0.98F) + (pitchAcc * 0.02F);

		// Turning around the Y axis results in a vector on the X-axis
		rollAcc = atan2f(static_cast<float>(acc_.X.data), static_cast<float>(acc_.Z.data)) * 180.0F / M_PI;
		roll_ = (roll_ * 0.98F) + (rollAcc * 0.02F);
	}
}


