/*
 * SerialArduino.cpp
 *
 *  Created on: May 1, 2014
 *      Author: rodrigo
 */

#include "SerialArduino.h"
#include "../Serial/SerialHW.h"
#include <cstring>

SerialArduino::SerialArduino()	:
	Thread(SERIAL_ARDUINO_PRIORITY, SCHED_RR),
	send_buffer(new std::deque<COMMAND_DATA>()),
	receive_buffer(new std::deque<COMMAND_DATA>())
{
	serial = new SerialHW(SERIAL_ARDUINO_PATH, Baud19200);

	Start();
}

SerialArduino::~SerialArduino()
{
	Stop();

	if(serial != NULL)
	{
		delete serial;
	}

	if(send_buffer != NULL)
	{
		delete send_buffer;
	}

	if(receive_buffer != NULL)
	{
		delete receive_buffer;
	}
}

void* SerialArduino::Run()
{
	COMMAND_DATA message;

	while(!Should_Stop())
	{
		if(serial->read_data(&message, sizeof(message)))
		{
			put_rcv_msg_in_buffer(message);
		}

		if(has_message_to_send())
		{
			get_msg_to_send(&message);
			serial->write_data(&message, sizeof(message));
		}

		usleep(500000);
	}
}

bool SerialArduino::has_message_to_send()
{
	Lock lock(send_mutex);

	return !send_buffer->empty();
}

void SerialArduino::put_rcv_msg_in_buffer(COMMAND_DATA& message)
{
	Lock lock(receive_mutex);

	receive_buffer->push_back(message);
}

void SerialArduino::get_msg_to_send(COMMAND_DATA* message)
{
	Lock lock(send_mutex);

	memcpy(message, &(send_buffer->front()), sizeof(COMMAND_DATA));
	receive_buffer->pop_front();
}

void SerialArduino::put_data(COMMAND_DATA& message)
{
	Lock lock(send_mutex);

	send_buffer->push_back(message);
}

void SerialArduino::get_data(COMMAND_DATA* message)
{
	Lock lock(receive_mutex);

	memcpy(message, &(receive_buffer->front()), sizeof(COMMAND_DATA));
	receive_buffer->pop_front();
}
