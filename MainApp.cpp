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

int main()
{
	ESC esc;
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


