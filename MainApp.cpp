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
#include "Hardware/PWM.h"

int main()
{
	PWM pwm;
	char c;
	int x;
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
	pwm.Initialize();

	do
	{

	printf("PWM Test\n\n");
	printf("1- frente\n");
	printf("2- traz\n");
	printf("3- esquerda\n");
	printf("4- direita\n");
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
				pwm.front(x);
			}
			break;

			case '2':
			{
				pwm.back(x);
			}
			break;

			case '3':
			{
				pwm.left(x);
			}
			break;

			case '4':
			{
				pwm.right(x);
			}
			break;
		}
		//system("clear");
	}while(c!='0');

	return 0;
}


