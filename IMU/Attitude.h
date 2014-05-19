/*
 * Attitude.h
 *
 *  Created on: May 14, 2014
 *      Author: rodrigo
 */

#ifndef ATTITUDE_H_
#define ATTITUDE_H_

#include "../Hardware/I2CSensors.h"

enum
{
	ROLL,
	PITCH,
	YAW
};

enum
{
	X,
	Y,
	Z
};

// MultiWii definitions
typedef struct fp_vector {
  float X,Y,Z;
} t_fp_vector_def;

typedef union {
  float A[3];
  t_fp_vector_def V;
} t_fp_vector;

typedef struct int32_t_vector {
  int32_t X,Y,Z;
} t_int32_t_vector_def;

typedef union {
  int32_t A[3];
  t_int32_t_vector_def V;
} t_int32_t_vector;

/* Set the Low Pass Filter factor for ACC
   Increasing this value would reduce ACC noise (visible in GUI), but would increase ACC lag time
   Comment this if  you do not want filter at all.
   unit = n power of 2 */
// this one is also used for ALT HOLD calculation, should not be changed
#ifndef ACC_LPF_FACTOR
  #define ACC_LPF_FACTOR 4 // that means a LPF of 16
#endif

/* Set the Gyro Weight for Gyro/Acc complementary filter
   Increasing this value would reduce and delay Acc influence on the output of the filter*/
#ifndef GYR_CMPF_FACTOR
  #define GYR_CMPF_FACTOR 600
#endif

/* Set the Gyro Weight for Gyro/Magnetometer complementary filter
   Increasing this value would reduce and delay Magnetometer influence on the output of the filter*/
#define GYR_CMPFM_FACTOR 250

//****** end of advanced users settings *************
#define INV_GYR_CMPF_FACTOR   (1.0f / (GYR_CMPF_FACTOR  + 1.0f))
//#define INV_GYR_CMPFM_FACTOR  (1.0f / (GYR_CMPFM_FACTOR + 1.0f))

// End MultiWii definitions

//#define GYRO_SCALE ((2279.0F * M_PI)/((32767.0f / 4.0f ) * 180.0f * 1000000.0f)) //(ITG3200 and MPU6050)
#define GYRO_SCALE 157322200.79416546919548882335141F



class Attitude {
public:
	Attitude();
	~Attitude();

	void get_pitch_and_roll(float& pitch, float& roll);

	void getEstimatedAttitude();

private:
	void get_acc_and_gyro_values();

private:
	uint64_t previousTimeinMicros;

	I2CSensors* sensors;

	ACC_RAW acc_;
	GYRO_RAW gyro_;

	float pitch_;
	float roll_;

	const int32_t acc_1G;
	const int32_t acc_1G_square;
};

#endif /* ATTITUDE_H_ */
