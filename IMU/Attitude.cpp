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
	acc_1G(4096),
	acc_1G_square(acc_1G*acc_1G),
	pitch_(0),
	roll_(0)
{
	sensors = new I2CSensors();

	sensors->Start();
}

Attitude::~Attitude()
{

}

void Attitude::get_pitch_and_roll(float& pitch, float& roll)
{
	pitch = pitch_;
	roll = roll_;
}

void Attitude::getEstimatedAttitude()
{
	float pitchAcc, rollAcc;

	sensors->get_acc(&acc_);
	sensors->get_gyro(&gyro_);

	float dT = static_cast<float>(gyro_.time_in_micros - previousTimeinMicros);

	dT /= 1000000.0F;

	previousTimeinMicros = gyro_.time_in_micros;

	// Integrate the gyroscope data -> int(angularSpeed) = angle
	pitch_ += (static_cast<float>(gyro_.X.data) / 16.4F) * dT; // Angle around the X-axis
	roll_ -= (static_cast<float>(gyro_.Y.data) / 16.4F) * dT;    // Angle around the Y-axis

	// Compensate for drift with accelerometer data if !bullshit
	// Sensitivity = -2 to 2 G at 16Bit -> 2G = 32768 && 0.5G = 8192
	int forceMagnitudeApprox = abs(acc_.X.data) + abs(acc_.Y.data) + abs(acc_.Z.data);

	if (forceMagnitudeApprox > 2048 && forceMagnitudeApprox < 32768)
	{
		// Turning around the X axis results in a vector on the Y-axis
		pitchAcc = atan2f(static_cast<float>(acc_.Y.data), static_cast<float>(acc_.Z.data)) * 180.0F / M_PI;
		pitch_ = (pitch_ * 0.98F) + (pitchAcc * 0.02F);

		// Turning around the Y axis results in a vector on the X-axis
		rollAcc = atan2f(static_cast<float>(acc_.X.data), static_cast<float>(acc_.Z.data)) * 180.0F / M_PI;
		roll_ = roll_ * 0.98 + rollAcc * 0.02;
	}
}

















































//static int32_t accLPF32[3] = {0, 0, 1};
//static int32_t accSmooth[3];
//
//static t_fp_vector EstG;
//static t_int32_t_vector EstG32;
//
//void rotateV(struct fp_vector *v, float* delta)
//{
//	fp_vector v_tmp = *v;
//	v->Z -= delta[ROLL]  * v_tmp.X + delta[PITCH] * v_tmp.Y;
//	v->X += delta[ROLL]  * v_tmp.Z - delta[YAW]   * v_tmp.Y;
//	v->Y += delta[PITCH] * v_tmp.Z + delta[YAW]   * v_tmp.X;
//}
//
//float InvSqrt (float x){
//  union{
//    int32_t i;
//    float   f;
//  } conv;
//  conv.f = x;
//  conv.i = 0x5f3759df - (conv.i >> 1);
//  return 0.5f * conv.f * (3.0f - x * conv.f * conv.f);
//}
//
//int16_t _atan2(int32_t y, int32_t x){
//  float z = (float)y / x;
//  int16_t a;
//  if ( abs(y) < abs(x) ){
//     a = 573 * z / (1.0f + 0.28f * z * z);
//   if (x<0) {
//     if (y<0) a -= 1800;
//     else a += 1800;
//   }
//  } else {
//   a = 900 - 573 * z / (z * z + 0.28f);
//   if (y<0) a -= 1800;
//  }
//  return a;
//}
//
//
//void Attitude::getEstimatedAttitude()
//{
//	int32_t accMag = 0;
//	float scale;
//	float deltaGyroAngle[3];
//
//	sensors->get_acc(&acc_);
//	sensors->get_gyro(&gyro_);
//
//	scale = static_cast<float>(gyro_.time_in_micros - previousTimeinMicros) * static_cast<float>(GYRO_SCALE);
//	previousTimeinMicros = gyro_.time_in_micros;
//
//	// Initialization
//	deltaGyroAngle[ROLL] = static_cast<float>(gyro_.X.data) * scale;
//	deltaGyroAngle[PITCH] = static_cast<float>(gyro_.Y.data) * scale;
//	deltaGyroAngle[YAW] = static_cast<float>(gyro_.Z.data) * scale;
//
//	accLPF32[X] -= accLPF32[X]>>ACC_LPF_FACTOR;
//	accLPF32[Y] -= accLPF32[Y]>>ACC_LPF_FACTOR;
//	accLPF32[Z] -= accLPF32[Z]>>ACC_LPF_FACTOR;
//
//	accLPF32[X] += static_cast<int32_t>(acc_.X.data);
//	accLPF32[Y] += static_cast<int32_t>(acc_.Y.data);
//	accLPF32[Z] += static_cast<int32_t>(acc_.Z.data);
//
//	accSmooth[X] = accLPF32[X]>>ACC_LPF_FACTOR;
//	accSmooth[Y] = accLPF32[Y]>>ACC_LPF_FACTOR;
//	accSmooth[Z] = accLPF32[Z]>>ACC_LPF_FACTOR;
//
//	accMag += accSmooth[X]*accSmooth[X];
//	accMag += accSmooth[Y]*accSmooth[Y] ;
//	accMag += accSmooth[Z]*accSmooth[Z] ;
//	//End Inicialization
//
//	accMag = (accMag*100)/(acc_1G_square);
//
//	rotateV(&EstG.V,deltaGyroAngle);
///*
//	if ( abs(accSmooth[ROLL])<acc_25deg && abs(accSmooth[PITCH])<acc_25deg && accSmooth[YAW]>0)
//	{
//		f.SMALL_ANGLES_25 = 1;
//	} else {
//		f.SMALL_ANGLES_25 = 0;
//	}
//*/
//	// Apply complimentary filter (Gyro drift correction)
//	// If accel magnitude >1.15G or <0.85G and ACC vector outside of the limit range => we neutralize the effect of accelerometers in the angle estimation.
//	// To do that, we just skip filter, as EstV already rotated by Gyro
//	//if (  (72 < accMag) && (accMag < 133) )
//	//{
//		EstG.A[ROLL] = (EstG.A[ROLL] * GYR_CMPF_FACTOR + accSmooth[ROLL]) * INV_GYR_CMPF_FACTOR;
//		EstG.A[PITCH] = (EstG.A[PITCH] * GYR_CMPF_FACTOR + accSmooth[PITCH]) * INV_GYR_CMPF_FACTOR;
//		EstG.A[YAW] = (EstG.A[YAW] * GYR_CMPF_FACTOR + accSmooth[YAW]) * INV_GYR_CMPF_FACTOR;
//	//}
//
//	EstG32.A[ROLL] = EstG.A[ROLL];
//	EstG32.A[PITCH] = EstG.A[PITCH];
//	EstG32.A[YAW] = EstG.A[YAW];
//
//	// Attitude of the estimated vector
//	int32_t sqGZ = EstG32.V.Z * EstG32.V.Z;
//	int32_t sqGX = EstG32.V.X * EstG32.V.X;
//	int32_t sqGY = EstG32.V.Y * EstG32.V.Y;
//	int32_t sqGX_sqGZ = sqGX + sqGZ;
//
//	float invmagXZ  = InvSqrt(static_cast<float>(sqGX_sqGZ));
//
//	//invG = InvSqrt(sqGX_sqGZ + sqGY);
//
//	roll_  = _atan2(EstG32.V.X , EstG32.V.Z);
//	pitch_ = _atan2(EstG32.V.Y , (invmagXZ*sqGX_sqGZ));
//}


