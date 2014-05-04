/*
 * SerialArduino.h
 *
 *  Created on: May 1, 2014
 *      Author: rodrigo
 */

#ifndef SERIALARDUINO_H_
#define SERIALARDUINO_H_

#include "../../General/Thread.h"
#include <deque>
#include "ProjDefs.h"
#include "ArduinoDefs.h"

#define SERIAL_ARDUINO_PATH "/dev/ttyO2"

//forward declaration of SerialHW class
class SerialHW;

class SerialArduino : public Thread
{
public:
	SerialArduino();
	~SerialArduino();

	void has_message();

	void put_data(COMMAND_DATA& message);

	void get_data(COMMAND_DATA* message);

private:
	void* Run();

	bool has_message_to_send();

	void put_rcv_msg_in_buffer(COMMAND_DATA& message);

	void get_msg_to_send(COMMAND_DATA* message);

	SerialHW* serial;

	std::deque<COMMAND_DATA>* send_buffer;
	std::deque<COMMAND_DATA>* receive_buffer;

	Mutex send_mutex;
	Mutex receive_mutex;
};

#endif /* COMMUNICATIONINTERFACE_H_ */
