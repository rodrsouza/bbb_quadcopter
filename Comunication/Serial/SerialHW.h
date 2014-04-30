/*
 * SerialHW.h
 *
 *  Created on: Apr 29, 2014
 *      Author: rodrigo
 */

#ifndef SERIALHW_H_
#define SERIALHW_H_

#include <termios.h>

class SerialHW
{
public:
	SerialHW(const char* serial_port, int baudrate, int parity = 0);

	~SerialHW();

	int write_data(void* buf, int size);

	int read_data(void* buf, int size);

	void set_blocking(bool should_block, int timeout_100ms = 5);

private:
	bool set_interface_attribs(int baudrate, int parity = 0);
	void initialize();

	bool get_attributes();
	bool set_attributes();

	int serial_pointer_;
	struct termios tty_;

	const char* port_name_;
};


#endif /* SERIALHW_H_ */
