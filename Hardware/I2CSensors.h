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



class I2CSensors : public Thread
{
public:
	I2CSensors();
	~I2CSensors();

	bool open_and_configure();

private:
	void* Run();

	bool set_i2c_address(int address);

	bool configure_mpu6050();

	bool configure_MS561101BA();

	int i2c_driver;

	uint16_t accelX;
	uint16_t accelY;
	uint16_t accelZ;

	uint16_t gyroX;
	uint16_t gyroY;
	uint16_t gyroZ;

	uint16_t MS561101_prom[8];
};

#endif /* I2CSENSORS_H_ */
