/*
 * I2CSensors.cpp
 *
 *  Created on: Feb 23, 2014
 *      Author: rodrigo
 */

#include "I2CSensors.h"

I2CSensors::I2CSensors() :
	Thread(SENSORS_PRIORITY, SCHED_RR),
	i2c_driver(0)
{
	memset(MS561101_prom, 0, sizeof(MS561101_prom));
}

I2CSensors::~I2CSensors()
{
}

bool I2CSensors::open_and_configure()
{
	i2c_driver = open(I2C_INTERFACE, O_RDWR);

	if(i2c_driver < 0)
	{
		printf("Falha ao abrir interface de i2c: %s\n", I2C_INTERFACE);
		return false;
	}
	else
	{
		printf("Interface de i2c %s aberta.\n", I2C_INTERFACE);

		if( configure_mpu6050() )
		{
			printf("MPU6050 configurado.\n");

			if( configure_MS561101BA() )
			{
				printf("MS561101BA configurado.\n");
			}
			else
			{
				printf("Falha ao configurar MPU6050.\n");
				return false;
			}
		}
		else
		{
			printf("Falha ao configurar MPU6050.\n");
			return false;
		}
	}
}

bool I2CSensors::configure_mpu6050()
{
	uint8_t buffer[4];
	size_t espected_number, do_number;
	bool result = true;

	buffer[0] = 0x6B;
	buffer[1] = 0x80; //Reset device

	result &= set_i2c_address(MPU6050_ADDRESS);

	espected_number = 2;
	do_number = write(i2c_driver, buffer, espected_number); //Reset device
	usleep(10000);

	result &= espected_number == do_number;

	buffer[0] = 0x6B;
	buffer[1] = 0x0B; // Reset = 0 Cycle = 0 Temp_dis = 1  PLL with gyroscope Z reference
	do_number = write(i2c_driver, buffer, espected_number);
	usleep(1000);

	result &= espected_number == do_number;

	espected_number = 4;
	buffer[0] = 0x1A;
	buffer[1] = 0x00;
	buffer[2] = 0x18;
	buffer[3] = 0x10;
	write(i2c_driver, buffer, espected_number);
	usleep(1000);

	result &= espected_number == do_number;

	return result;
}

bool I2CSensors::configure_MS561101BA()
{
	uint8_t buffer[2];
	size_t i, espected_number, do_number;
	uint8_t prom_command;
	bool result = true;

	memset(buffer, 0, sizeof(buffer));

	set_i2c_address(MS561101BA_ADDESS);

	buffer[0] = MS561101BA_RESET;

	espected_number = 1;

	do_number = write(i2c_driver, buffer, espected_number);

	result &= espected_number == do_number;

	usleep(10000);

	for(i = 0; i < sizeof(MS561101_prom); ++i)
	{
		prom_command = static_cast<uint8_t>(MS561101BA_PROM_MASK + (i << 1));

		espected_number = 1;

		do_number = write(i2c_driver, &prom_command, espected_number);
		usleep(10000);

		result &= espected_number == do_number;

		espected_number = 2;

		do_number = read(i2c_driver, &buffer, espected_number);

		result &= espected_number == do_number;

		MS561101_prom[i] = (static_cast<uint16_t>(buffer[0]) << 8) +
							static_cast<uint16_t>(buffer[1]);
		usleep(10000);
	}
}

bool I2CSensors::set_i2c_address(int address)
{
	return static_cast<bool>(ioctl(i2c_driver, I2C_SLAVE, address) > 0);
}

void* I2CSensors::Run()
{
	while(!Should_Stop())
	{

	}
}
