/*
 * Uart.cpp
 *
 *  Created on: Jun 7, 2016
 *      Author: adrien
 */

#include <termios.h>
#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <fcntl.h>
#include <poll.h>
#include <termios.h>
#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <fcntl.h>
#include <poll.h>
#include <string.h> // for memset function
#include "Uart.h"

Uart::Uart(int pUartPin)
{
	uartPin = pUartPin;
	devicePath = "/dev/ttyO1";
}

void Uart::open()
{
	fd = ::open(devicePath.c_str(), O_RDWR | O_CLOEXEC);

	if (fd < 0) {
		printf("Failed to open UART device %s\n", devicePath.c_str());
	}
}

void Uart::writeByte(uint8_t pData)
{

}

void Uart::writeStr(std::string pData)
{
	const char *data = pData.c_str();
}

uint8_t Uart::read()
{
//	::write();
}

void Uart::disableCrlf()
{
    struct termios t;
    memset(&t, 0, sizeof(t));

    tcgetattr(fd, &t);

    // disable LF -> CR/LF
    t.c_iflag &= ~(BRKINT | ICRNL | IMAXBEL | IXON | IXOFF);
    t.c_oflag &= ~(OPOST | ONLCR);
    t.c_lflag &= ~(ISIG | ICANON | IEXTEN | ECHO | ECHOE | ECHOK | ECHOCTL | ECHOKE);
    t.c_cc[VMIN] = 0;

    tcsetattr(fd, TCSANOW, &t);
}

void Uart::setSpeed(unsigned int baudrate)
{
    struct termios t;
    memset(&t, 0, sizeof(t));

    tcgetattr(fd, &t);
    cfsetspeed(&t, baudrate);
    tcsetattr(fd, TCSANOW, &t);
}


Uart::~Uart()
{

}

