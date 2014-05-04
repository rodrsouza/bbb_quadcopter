/*
 * SerialHW.h
 *
 *  Created on: Apr 29, 2014
 *      Author: rodrigo
 */

#ifndef SERIALHW_H_
#define SERIALHW_H_

#include <termios.h>


typedef enum BaudRate
{
	Baud9600	= B9600,
	Baud19200	= B19200,
	Baud115200	= B115200
};

class SerialHW
{
public:
	SerialHW(const char* serial_port, BaudRate baud, int parity = 0);

	~SerialHW();

	bool write_data(void* buf, int size);

	bool read_data(void* buf, int size);

	void set_blocking(bool should_block, int timeout_100ms = 5);

	void close_port();

private:
	bool set_interface_attribs(int baudrate, int parity = 0);
	void initialize(BaudRate baud);

	bool get_attributes();
	bool set_attributes();

	int serial_pointer_;
	struct termios tty_;

	const char* port_name_;
};


#endif /* SERIALHW_H_ */
