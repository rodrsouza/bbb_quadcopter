/*
 * Socket.cpp
 *
 *  Created on: Oct 20, 2013
 *      Author: rodrigo
 */

#include "Socket.h"

//CSocket::m_pThis = NULL;

CSocket::CSocket() :
	Thread(SOCKET_PRIORITY, SCHED_RR)
{
	// TODO Auto-generated constructor stub

}

CSocket::~CSocket() {
	// TODO Auto-generated destructor stub
}

CSocket* CSocket::GetInstance()
{

}



void* CSocket::Run()
{

}
