/*
 * main.c
 *
 *  Created on: Jul 28, 2013
 *      Author: rodrigo
 */

#include <stdlib.h>
#include <stdio.h>
#include <Defs.h>
//#include "Comunication/Socket/SocketInterface.h"
#include "Hardware/ESC.h"
#include "Hardware/I2CSensors.h"
int main()
{
//	ESC esc;
	char c;
	int x;
	ACC_RAW acc;
	GYRO_RAW gyro;

	I2CSensors* sensors = new I2CSensors();

	sensors->Start();

//	printf("Iniciando socket...\n\n");
//
//	std::string data;
//
//	CSocketInterface* m_socketserver = new CSocketInterface();
//
//	m_socketserver->StartSocket();
//
//	while(true)
//	{
//		if(m_socketserver->GetMessage(data))
//		{
//			m_socketserver->SendMessage(data);
//		}
//	}

	do
	{
		usleep(10000);
		//system("clear");
		sensors->get_acc(&acc);
		sensors->get_gyro(&gyro);

		printf("\nAccelerometer\nX: %d\nY: %d\nZ: %d\n\nGyro\nX: %d\nY: %d\nZ: %d\n", acc.X.data, acc.Y.data, acc.Z.data, gyro.X.data, gyro.Y.data, gyro.Z.data);

	}while(false);

	sensors->Stop();

	sleep(1);

	delete sensors;

	return 0;
}


