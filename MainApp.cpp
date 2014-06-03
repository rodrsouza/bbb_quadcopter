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
#include "Hardware/Barometer.h"
#include "IMU/Attitude.h"
#include "General/kbhit.h"

#define ALTITUDE_TEST

#if defined(ATTITUDE_CALIBRATION)
int main()
{
	Attitude* attitude = new Attitude();
	float pitch, roll;
	float pitch_raw_sum, roll_raw_sum;
	float pitch_sum, roll_sum;
	int i;

	nonblock(NB_ENABLE);

	do
	{
		usleep(1000);
		system("clear");

		attitude->getEstimatedAttitude();
		attitude->get_pitch_and_roll_no_cal(pitch, roll);

		printf("\npitch: %2.4f\nroll: %2.4f", pitch, roll);
	}while(!kbhit());

	printf("\n\nEfetuando aquisicao de dados brutos...\n");

	pitch_raw_sum = 0;
	roll_raw_sum = 0;

	for(i = 0; i < 2000;  ++i)
	{
		usleep(200);

		attitude->getEstimatedAttitude();
		attitude->get_pitch_and_roll_no_cal(pitch, roll);

		pitch_raw_sum += pitch;
		roll_raw_sum += roll;
	}

	printf("Calculando media sem calibracao...\n");

	pitch_raw_sum /= 2000.0F;
	roll_raw_sum /= 2000.0F;
	printf("Pitch: %f\nRoll: %f\n", static_cast<float>(pitch_raw_sum), static_cast<float>(roll_raw_sum));

	printf("Salvando calibracao...\n");
	if( attitude->save_cal(static_cast<float>(pitch_raw_sum), static_cast<float>(roll_raw_sum)) )
	{
		printf("Calibracao salva com sucesso...\n");
	}
	else
	{
		printf("Falha ao salvar calibracao...\n");
	}

	sleep(1);

	delete attitude;

	return 0;
}
#endif




#if defined(PITCH_AND_ROLL_TEST)
int main()
{
	Attitude* attitude = new Attitude();
	float pitch, roll;

	sleep(2);

	do
	{
		usleep(5000);
		system("clear");
		attitude->getEstimatedAttitude();
		attitude->get_pitch_and_roll(pitch, roll);
		printf("\npitch: %02.4f\nroll: %2.2f", pitch, roll);
		//printf("\npitch: %d\nroll: %d", pitch, roll);
	}while(true);


	sleep(1);

	return 0;
}
#endif



#if defined(ALTITUDE_TEST)
int main()
{
	Barometer* barometer = new Barometer();

	sleep(2);

	barometer->zero_altitude_ts();

	do
	{
		usleep(20000);
		system("clear");
		printf("\nAltitude: %02.1f m\nTemperature: %2.2f C\nPressure: %2.2F\n", barometer->get_filtered_altitude_ts(), barometer->get_temperature_ts(), barometer->get_pressure_ts());
	}while(true);

	delete barometer;

	sleep(1);

	return 0;
}
#endif



#if defined(SENSORS_RAW_TEST)
int main()
{
	ACC_RAW acc;
	GYRO_RAW gyro;
	uint32_t pressure, temperature;

	I2CSensors* sensors = new I2CSensors();

	sensors->Start();

	do
	{
		usleep(20000);
		system("clear");
		sensors->get_acc(&acc);
		sensors->get_gyro(&gyro);
		pressure = sensors->get_pressure();
		temperature = sensors->get_temperature();
		printf("\nAccelerometer\nX: %d\nY: %d\nZ: %d\n\nGyro\nX: %d\nY: %d\nZ: %d\n\nBarometer\nPressure: %d\nTemperature: %d\n", acc.X.data, acc.Y.data, acc.Z.data, gyro.X.data, gyro.Y.data, gyro.Z.data, pressure, temperature);

	}while(true);

	sensors->Stop();

	sleep(1);

	delete sensors;

	return 0;
}
#endif



#if defined(SOCKET_TEST)
int main()
{
	ESC esc;
	char c;
	int x;

	printf("Iniciando socket...\n\n");

	std::string data;

	CSocketInterface* m_socketserver = new CSocketInterface();

	m_socketserver->StartSocket();

	while(true)
	{
		if(m_socketserver->GetMessage(data))
		{
			m_socketserver->SendMessage(data);
		}
	}

	return 0;
}
#endif




#if defined(ESC_TEST)
int main()
{
	ESC esc;
	char c;
	int x;

	esc.InitializePWM();

	do
	{

		printf("ESC Test\n\n");
		printf("1- frente\n");
		printf("2- traz\n");
		printf("3- esquerda\n");
		printf("4- direita\n");
		printf("5- Fade test\n");
		printf("6- Initialize\n");
		printf("0- Sair\n");


		c = getchar();

		if(c>='1' && c<='4')
		{
			printf("Digite duty cycle: ");
			scanf("%d", &x);
		}

		switch(c)
		{
			case '1':
			{
				esc.front(x);
			}
			break;

			case '2':
			{
				esc.back(x);
			}
			break;

			case '3':
			{
				//pwm.left(5.263F);
				esc.left(x);
			}
			break;

			case '4':
			{
				esc.right(x);
			}
			break;

			case '5':
			{
				float i=0;

				while(1)
				{
					esc.left(i);
					esc.right(100-i);

					i+=10;
					if(i>=100)
						i = 0;

					usleep(10000);
				}
			}
			break;

			case '6':
			{
				esc.InitializeESC();
			}
			break;
		}
		//system("clear");
	}while(c!='0');

	return 0;
}
#endif
