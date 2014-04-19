// Spo2Module.cpp: implementation of the CAmSpo2Module class.
//
//////////////////////////////////////////////////////////////////////

#include "SerialCommandModule.h"



//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CSerialCommandModule::CSerialCommandModule ( ) :
	CSysAbstractProcessUnit( (CSysCompositeProcessUnit *)NULL, SERIAL_ACQUIS_PRIORITY, 1024, NAME_SERIAL_PROCESS, NAME_SERIAL_QUEUE, 7500, SCHED_RR ),
	CSysTimerClient(100)
{
	if( (serialDriver = open(SERIALDRIVER_PATH, O_RDWR )) == NULL)
	{
		printf("Falha ao abrir driver serial\n");
	}
}

CSerialCommandModule::~CSerialCommandModule()
{
	if(serialDriver != NULL)
	{
		close(serialDriver);
	}
}


/*
* Funções para controle de start e stop do modulo
*/

void CSerialCommandModule::StartTask()
{
	CSysAbstractProcessUnit::StartTask();
}


/*
* Funções para controle das filas de envio e recepção
*/

void CSerialCommandModule::FlushEmergencyQueue()
{
	Lock myLock(m_emergencyQueueMutex, true);

	while(m_emergencyMessageQueue.size() > 0)
	{
		delete m_emergencyMessageQueue.front();
		m_emergencyMessageQueue.pop_front();
	}

	m_emergencyMessageQueue.clear();
}

void CSerialCommandModule::FlushReceiveQueue()
{
	Lock myLock(m_receiveQueueMutex, true);

	while(m_messageReceiveQueue.size() > 0)
	{
		delete m_messageReceiveQueue.front();
		m_messageReceiveQueue.pop_front();
	}

	m_messageReceiveQueue.clear();
}


/*
* Funções para acesso a fila de menssagens de emergencia
*/

bool CSerialCommandModule::HasEmergencyMessages()
{
	Lock myLock(m_emergencyQueueMutex, true);
	return (m_emergencyMessageQueue.size() > 0);
}

void CSerialCommandModule::GetEmergencyMessage( std::vector<UCHAR> &aMessage )
{
	Lock myLock(m_emergencyQueueMutex, true);

	std::vector<UCHAR>* message = m_emergencyMessageQueue.front();
	aMessage.clear();
	aMessage.insert(aMessage.begin(), message->begin(), message->end());

	delete m_emergencyMessageQueue.front();

	m_emergencyMessageQueue.pop_front();
}


/*
* Funções para enviar e receber comandos durante o teste
*/

bool CSerialCommandModule::SendMessage( UCHAR aProcessorSelect, UCHAR* aData, UCHAR aSize)
{
	UCHAR ucBuf[MAX_MESSAGE_SIZE];
	USHORT usCrc;

	usCrc = CalcCRC16(aData, aSize);

	ucBuf[FF_START_BYTE] = aProcessorSelect;
	ucBuf[FF_CRC_MSB] = (UCHAR) (0xFFFF & (usCrc<<8));
	ucBuf[FF_CRC_LSB] = (UCHAR) (0x00FF & usCrc);
	ucBuf[FF_SIZE] = aSize;

	memcpy(&ucBuf[FF_COMMAND], aData, aSize);

	return SendMessageToDriver(ucBuf, aSize+HEADER_SIZE);
}

int CSerialCommandModule::WaitMessage( UCHAR aCommandToWait, std::vector<UCHAR> &aMessage, ULONG aTimeout )
{
	int iSize;
	std::vector<UCHAR>* message;
	bool bKeepSearching = true;

	Lock myLock(m_receiveQueueMutex, true);

	iSize = m_messageReceiveQueue.size();

	while( (iSize > 0) && bKeepSearching && (aTimeout > 0) )
	{
		myLock.LockAgain(true);

		message = m_messageReceiveQueue.front();

		myLock.Unlock();

		if(message->at(0) == aCommandToWait)
		{
			aMessage.insert(aMessage.begin(), message->begin(), message->end());
			bKeepSearching = false;
		}

		delete message;

		myLock.LockAgain(true);

		m_messageReceiveQueue.pop_front();
		iSize = m_messageReceiveQueue.size();

		myLock.Unlock();

		if( bKeepSearching )
		{
			aTimeout--;
			usleep(1000);
		}
	}

	if( iSize > 0 )
	{
		FlushReceiveQueue();
	}
}

/*
* Funções que fazem interface com o Driver
*/

void CSerialCommandModule::FlushDriverReceiveQueue()
{

}

void CSerialCommandModule::FlushDriverSendQueue()
{

}


void CSerialCommandModule::GetMessagesFromDriver()
{
	UCHAR ucBuf[MAX_MESSAGE_SIZE];
	int iSize;
	USHORT checksum;
	bool bIsEmergency;

	memset(ucBuf, 0, MAX_MESSAGE_SIZE);

	iSize = read(serialDriver, ucBuf, MAX_MESSAGE_SIZE);

	bIsEmergency = ucBuf[FF_COMMAND] == M_EMERGENCY_CMD;

	if( (iSize > HEADER_SIZE) || bIsEmergency )
	{
		checksum = (0xFFFF & (ucBuf[FF_CRC_MSB]<<8));
		checksum |= ucBuf[FF_CRC_LSB];

		if( checksum == CalcCRC16(&ucBuf[FF_COMMAND], ucBuf[FF_SIZE] ))
		{
			std::vector<UCHAR>* message = new std::vector<UCHAR>(&ucBuf[FF_COMMAND], &ucBuf[FF_COMMAND]+ucBuf[FF_SIZE]);

			if(bIsEmergency)
			{
				Lock myLock(m_emergencyQueueMutex, true);
				m_emergencyMessageQueue.push_back(message);
				myLock.Unlock();
			}
			else
			{
				Lock myLock(m_receiveQueueMutex, true);
				m_messageReceiveQueue.push_back(message);
				myLock.Unlock();
			}
		}
		else
		{
			//TODO: send nack
		}
	}
}


USHORT CSerialCommandModule::CalcCRC16(UCHAR *aData, UCHAR aSize)
{
	register unsigned int Crc = 0;
	UCHAR *pChar = aData;
	register unsigned int i, j, Aux;

	for(j = aSize; j > 0; j--)
	{
		Aux = Crc ^ ((unsigned int) (*pChar));
		pChar++;
		for(i=0; i < 8; i++)
		{
			if( (Aux & ((unsigned int) 0x0001)) )
			{
				Aux = ((Aux >> 1) ^ 0xA001);
			}
			else
			{
				Aux >>= 1;
			}
		}
		Crc ^= Aux;
	}
	return (USHORT) Crc;
}


bool CSerialCommandModule::SendMessageToDriver(UCHAR *aMessage, UCHAR aSize)
{
	int i;

	i = write(serialDriver, aMessage, aSize);

	return (i==aSize);
}


//////////////////////////////////////////////////////////////////////
// Interrupt :
//////////////////////////////////////////////////////////////////////
void CSerialCommandModule::Interrupt()
{
	EventPost( TIMER_EVENT_ID, NULL );
}


//////////////////////////////////////////////////////////////////////
// Event :
//////////////////////////////////////////////////////////////////////
LONG CSerialCommandModule::Event(EVENT_ID aEvent, void * aData)
{
	switch( aEvent )
	{
		case TIMER_EVENT_ID:
        {
        	GetMessagesFromDriver();
            return 1;
        }
        break;
	}

	return 0;
}




