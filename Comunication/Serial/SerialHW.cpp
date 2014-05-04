#include "SerialHW.h"
#include <cstdlib>
#include <cstring>
#include <cstdio>
#include <errno.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>


//int SerialHW::set_interface_attribs (int fd, int speed, int parity)
bool SerialHW::set_interface_attribs(int baudrate, int parity)
{
	(void)get_attributes();

	cfsetospeed(&tty_, baudrate);
	cfsetispeed(&tty_, baudrate);

	tty_.c_cflag = (tty_.c_cflag & ~CSIZE) | CS8;     // 8-bit chars
	// disable IGNBRK for mismatched speed tests; otherwise receive break
	// as \000 chars
	tty_.c_iflag &= ~IGNBRK;         // ignore break signal
	tty_.c_lflag = 0;                // no signaling chars, no echo,
									// no canonical processing
	tty_.c_oflag = 0;                // no remapping, no delays
	tty_.c_cc[VMIN]  = 0;            // read doesn't block
	tty_.c_cc[VTIME] = 5;            // 0.5 seconds read timeout

	tty_.c_iflag &= ~(IXON | IXOFF | IXANY); // shut off xon/xoff ctrl

	tty_.c_cflag |= (CLOCAL | CREAD);// ignore modem controls,
									// enable reading
	tty_.c_cflag &= ~(PARENB | PARODD);      // shut off parity
	tty_.c_cflag |= parity;
	tty_.c_cflag &= ~CSTOPB;
	tty_.c_cflag &= ~CRTSCTS;

	return set_attributes();
}

void SerialHW::set_blocking(bool should_block, int timeout_100ms)
{
	(void)get_attributes();

	tty_.c_cc[VMIN]  = (should_block ? 1 : 0);
	tty_.c_cc[VTIME] = timeout_100ms;// 0.5 seconds read timeout

	(void)set_attributes();
}

bool SerialHW::get_attributes()
{
	memset(&tty_, 0, sizeof(tty_));

	if ( (tcgetattr(serial_pointer_, &tty_)) != 0)
	{
		printf("SerialHW: error %d from tcgetattr", errno);
		return true;
	}
	else
	{
		return false;
	}
}

bool SerialHW::set_attributes()
{
	if ( (tcsetattr(serial_pointer_, TCSANOW, &tty_)) != 0)
	{
		printf("SerialHW: error %d from tcsetattr", errno);
		return true;
	}
	else
	{
		return false;
	}
}

SerialHW::SerialHW(const char* serial_port, BaudRate baud, int parity) :
		port_name_(serial_port)
{
	initialize(baud);
}

SerialHW::~SerialHW()
{
	close_port();
}

void SerialHW::initialize(BaudRate baud)
{
	serial_pointer_ = open (port_name_, O_RDWR | O_NOCTTY | O_SYNC);
	if (serial_pointer_ < 0)
	{
		printf("SerialHW: error %d opening %s: %s", errno, port_name_, strerror(errno));
	}
	else
	{
		set_interface_attribs(baud);  // set speed to 115,200 bps, 8n1 (no parity)
		set_blocking(false);
	}
}

void SerialHW::close_port()
{
	if(serial_pointer_ != NULL)
	{
		close(serial_pointer_);
	}
}


bool SerialHW::write_data(void* buf, int size)
{
	return (write(serial_pointer_, buf, size) == size);
}

bool SerialHW::read_data(void* buf, int size)
{
	return (read(serial_pointer_, buf, size) == size);
}

