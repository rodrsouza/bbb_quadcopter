// Spo2Module.h: interface for the CAmSpo2Module class.
//
//////////////////////////////////////////////////////////////////////

#ifndef _SERIALCOMMANDMODULE_H_
#define _SERIALCOMMANDMODULE_H_

#include "../ProjDefs.h"
#include <deque>
#include <vector>
#include <algorithm>


typedef struct
{
	UCHAR StartByte;
	USHORT Checksum;
	UCHAR Size;
	UCHAR Command;
	UCHAR Data[255];

} LPCMESSAGE;


class CSerialCommandModule : public CSysAbstractProcessUnit,
                      	  	 public CSysTimerClient
{
public:
    /**
    * Construtor do Modulo
    */
	CSerialCommandModule();
    virtual ~CSerialCommandModule();

    /**
     * Retorna o nome da classe especializada.
     */
    virtual const char * GetClassName() { return "CSerialCommandModule"; }

    /**
     * Interpreta eventos desta unidade de processamento
     */
	virtual LONG Event( EVENT_ID aEvent, void *aData=NULL );

    /**
	* Funções utilizadas para tratamento da fila de mensagens recebidas e enviadas
	*/

	void FlushDriverReceiveQueue();

	void FlushDriverSendQueue();

	//Limpa fila de recepção de mensagens
    void FlushReceiveQueue();

    //Limpa fila de mensagens de emergencia
    void FlushEmergencyQueue();

    //Pega mensagem no topo da fila de menssagens de emergencia
    void GetEmergencyMessage( std::vector<UCHAR> &aMessage );

    //Verifica se existem mensagens na fila de mensagens de emergencia
    bool HasEmergencyMessages();

    bool SendMessage( UCHAR aProcessorSelect, UCHAR* aData, UCHAR aSize);

    int WaitMessage( UCHAR aCommandToWait, std::vector<UCHAR> &aMessage, ULONG aTimeout );

    /**
	 * Inicializa a execucao da Task.
	 */
	virtual void StartTask();


protected:
    virtual void Interrupt();

private:

    int serialDriver;

    Mutex m_emergencyQueueMutex;
    std::deque< std::vector<UCHAR>* > m_emergencyMessageQueue;

    Mutex m_receiveQueueMutex;
    std::deque< std::vector<UCHAR>* > m_messageReceiveQueue;

    //Pega mensagens recebidas no driver e insere no buffer apropriado
    void GetMessagesFromDriver();

    //Calcula o CRC16 do buffer
    USHORT CalcCRC16(UCHAR *aData, UCHAR aSize);

    bool SendMessageToDriver(UCHAR *aMessage, UCHAR aSize);
};

#endif //_SERIALCOMMANDMODULE_H_
