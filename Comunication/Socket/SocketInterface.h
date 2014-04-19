/*
 * SocketInterface.h
 *
 *  Created on: Oct 20, 2013
 *      Author: rodrigo
 */

#ifndef CSOCKETINTERFACE_H_
#define CSOCKETINTERFACE_H_

#include "Defs.h"
#include "ServerSocket.h"
#include "SocketException.h"
#include <deque>
#include <algorithm>

class CSocketInterface: public Thread {
public:
	CSocketInterface();
	virtual ~CSocketInterface();

	bool StartSocket();

	void SendMessage(std::string &aMessage);

	bool GetMessage(std::string &aMessage);

private:
	void* Run();
	bool HasMessageToSend();
	void GetMessageToSend(std::string &aMessage);
	void StoreReceivedMessage(std::string &aMessage);

	ServerSocket* m_server;

	Mutex m_receiveBufMutex;
	std::deque<std::string> m_receiveBuffer;

	Mutex m_sendBufMutex;
	std::deque<std::string> m_sendBuffer;
};

#endif /* CSOCKETINTERFACE_H_ */
