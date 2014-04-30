/*
 * Socket.h
 *
 *  Created on: Oct 20, 2013
 *      Author: rodrigo
 */

#ifndef CSOCKET_H_
#define CSOCKET_H_

#include "../General/Thread.h"
#include "pthread.h"
#include "socket.h"

class CSocket : public Thread
{
public:
	CSocket();
	virtual ~CSocket();

	CSocket* GetInstance();

private:
	virtual void* Run();

	CSocket* m_pThis;
	CPPSocket::Socket* m_pSocket;

};

#endif /* CSOCKET_H_ */
