/*
 * I2CSensors.h
 *
 *  Created on: Feb 23, 2014
 *      Author: rodrigo
 */

#ifndef I2CSENSORS_H_
#define I2CSENSORS_H_

#include "Defs.h"
#include "../General/Thread.h"

#include <sys/ioctl.h>
#include <linux/i2c.h>
#include <linux/i2c-dev.h>

#define I2C_INTERFACE "/dev/i2c-1"

/**
 *  MPU-6050 defines
 */
#define MPU6050_ADDRESS		0x68

#define MPU6050_ACCELX_MSB	0x3B

#define MPU6050_GYROX_MSB	0x43


/**
 *  Barometer defines
 */

#define MS561101BA_ADDESS	0b1110111 //0x77

// registers of the device
#define MS561101BA_PRESSURE    0x40
#define MS561101BA_TEMPERATURE 0x50
#define MS561101BA_RESET       0x1E

#define MS561101BA_PROM_MASK   0b10100000

// OSR (Over Sampling Ratio) constants
#define MS561101BA_OSR_256  0x00
#define MS561101BA_OSR_512  0x02
#define MS561101BA_OSR_1024 0x04
#define MS561101BA_OSR_2048 0x06
#define MS561101BA_OSR_4096 0x08

#define OSR MS561101BA_OSR_4096

#pragma pack(1)

union data16
{
	uint8_t lsb;
	uint8_t msb;
};


struct ACC_RAW
{
	union data16 X;
	union data16 Y;
	union data16 Z;
};

struct GYRO_RAW
{
	union data16 X;
	union data16 Y;
	union data16 Z;
};


#pragma pack()

class I2CSensors : public Thread
{
public:
	I2CSensors();
	~I2CSensors();

	bool open_and_configure();

	void get_acc(void* acc_struct);

	void get_gyro(void* gyro_struct);

private:
	void* Run();

	bool read_and_store_acc_gyro();

	void store_acc(uint8_t* buf_w_readings);

	void store_gyro(uint8_t* buf_w_readings);

	bool read_temp_pressure();

	void clean_all();

	bool set_i2c_address(int address);

	bool configure_mpu6050();

	bool configure_MS561101BA();

	int i2c_driver;

	Mutex acc_mutex;
	ACC_RAW accelerometer_;

	Mutex gyro_mutex;
	GYRO_RAW gyroscope_;

	uint16_t MS561101_prom[8];
};


inline bool I2CSensors::set_i2c_address(int address)
{
	return (ioctl(i2c_driver, I2C_SLAVE, address) > 0);
}

inline bool I2CSensors::read_and_store_acc_gyro()
{
	uint8_t temp_buf[6];
	uint8_t command;
	int done;

	bool result = true;

	command = MPU6050_ACCELX_MSB; //First data read + 6 to read all axes
	done = write(i2c_driver, &command, sizeof(command));
	result &= (done == sizeof(command));

	done = read(i2c_driver, temp_buf, sizeof(temp_buf));
	result &= (done == sizeof(temp_buf));

	store_acc(temp_buf);

	command = MPU6050_GYROX_MSB; //First data read + 6 to read all axes
	done = write(i2c_driver, &command, sizeof(command));
	result &= (done == sizeof(command));

	done = read(i2c_driver, temp_buf, sizeof(temp_buf));
	result &= (done == sizeof(temp_buf));

	store_gyro(temp_buf);

	return result;
}

inline void I2CSensors::store_acc(uint8_t* buf_w_readings)
{
	Lock lock(acc_mutex);

	accelerometer_.X.msb = buf_w_readings[0];
	accelerometer_.X.lsb = buf_w_readings[1];

	accelerometer_.Y.msb = buf_w_readings[2];
	accelerometer_.Y.lsb = buf_w_readings[3];

	accelerometer_.Z.msb = buf_w_readings[4];
	accelerometer_.Z.lsb = buf_w_readings[5];
}

inline void I2CSensors::store_gyro(uint8_t* buf_w_readings)
{
	Lock lock(gyro_mutex);

	gyroscope_.X.msb = buf_w_readings[0];
	gyroscope_.X.lsb = buf_w_readings[1];

	gyroscope_.Y.msb = buf_w_readings[2];
	gyroscope_.Y.lsb = buf_w_readings[3];

	gyroscope_.Z.msb = buf_w_readings[4];
	gyroscope_.Z.lsb = buf_w_readings[5];
}

inline void I2CSensors::get_acc(void* acc_struct)
{
	Lock lock(acc_mutex);

	memcpy(acc_struct, &accelerometer_, sizeof(accelerometer_));
}

inline void I2CSensors::get_gyro(void* gyro_struct)
{
	Lock lock(gyro_mutex);

	memcpy(gyro_struct, &gyroscope_, sizeof(gyroscope_));
}


inline bool I2CSensors::read_temp_pressure()
{

}

#endif /* I2CSENSORS_H_ */
