/*
 * SocketInterface.cpp
 *
 *  Created on: Oct 20, 2013
 *      Author: rodrigo
 */

#include "SocketInterface.h"

CSocketInterface::CSocketInterface() :
	Thread(SOCKET_PRIORITY, SCHED_RR)
{
}

CSocketInterface::~CSocketInterface()
{
	delete m_server;
}

bool CSocketInterface::StartSocket()
{
	try
	{
		m_server = new ServerSocket(SOCKET_PORT);
	}
	catch(SocketException& e)
	{
		printf("Exception: %s\n", e.description());
		return false;
	}

	this->Thread::Start();

	return true;
}


void* CSocketInterface::Run()
{
	ServerSocket new_sock;
	std::string data;

	while(!this->Should_Stop())
	{
		try
		{
			m_server->accept(new_sock);

			new_sock << "Hi. I'm alive.";

			try
			{
				while( !this->Should_Stop())
				{
					new_sock >> data;
					if(!data.empty())
					{
						StoreReceivedMessage(data);
						data.clear();
					}

					if(HasMessageToSend())
					{
						GetMessageToSend(data);
						new_sock << data;
						data.clear();
					}
				}
			}
			catch ( SocketException& ) {}

		}
		catch (SocketException& e)
		{
			printf("Exception: %s\n", e.description());
		}
	}
}


bool CSocketInterface::HasMessageToSend()
{
	Lock myLock(m_sendBufMutex, true);

	return (m_sendBuffer.size() > 0);
}

void CSocketInterface::GetMessageToSend(std::string &aMessage)
{
	Lock myLock(m_sendBufMutex, true);

	aMessage = m_sendBuffer.front();

	m_sendBuffer.pop_front();
}

void CSocketInterface::StoreReceivedMessage(std::string &aMessage)
{
	std::string message(aMessage.begin(), aMessage.end());

	Lock myLock(m_receiveBufMutex, true);

	m_receiveBuffer.push_back(message);
}


void CSocketInterface::SendMessage(std::string &aMessage)
{
	Lock myLock(m_sendBufMutex, true);

	m_sendBuffer.push_back(aMessage);
}


bool CSocketInterface::GetMessage(std::string &aMessage)
{
	Lock myLock(m_receiveBufMutex, true);

	if(m_receiveBuffer.size() > 0)
	{
		aMessage = m_receiveBuffer.front();

		m_receiveBuffer.pop_front();

		return true;
	}
	else
	{
		return false;
	}
}
