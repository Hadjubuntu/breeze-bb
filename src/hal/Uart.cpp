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

// TODO see : https://github.com/yigityuce/BlackLib/tree/master/v3_0/BlackUART

#define READ_BUFFER_SIZE 1024

Uart::Uart(int pUartPin)
{
	uartPin = pUartPin;
	devicePath = "/dev/ttyO1";
	fd = -1;
	scriptDirectory = findScriptDirectory();
}

std::string Uart::findScriptDirectory()
{
	std::string searchResult = FileTools::searchDirectory(".","scripts");
	return searchResult;
}

void Uart::open()
{
	std::string paramStr = "P9.26";
	std::string scriptSetup = "sudo ./" + scriptDirectory + "/uart-setup.sh " + paramStr;
	int result = system(scriptSetup.c_str());
	printf("Executed command setup uart %s\n", scriptSetup.c_str());

	/**
	 * TODO FIXME
	 * ---------------------------------
	 * See code on beaglebone black : test.cpp
	 */


	fd = ::open(devicePath.c_str(), O_RDWR | O_CLOEXEC);



	if (fd < 0) {
		printf("Failed to open UART device %s\n", devicePath.c_str());
	}

	//	disableCrlf();
	setProperties();
}

std::string Uart::read()
{
	std::string tempReadBuffer;
	tempReadBuffer.resize(READ_BUFFER_SIZE);

	int readSize = ::read(fd, &tempReadBuffer[0], tempReadBuffer.size());

	if(readSize > 0)
	{
		tempReadBuffer.resize(readSize);
		return tempReadBuffer;
	}
	else
	{
		return std::string("");
	}
}


int Uart::write(const uint8_t *buf, uint16_t n)
{
	struct pollfd fds;
	fds.fd = fd;
	fds.events = POLLOUT;
	fds.revents = 0;

	int ret = 0;

	if (poll(&fds, 1, 0) == 1) {
		ret = ::write(fd, buf, n);
	}

	return ret;
}

void Uart::setBlocking(bool blocking)
{
	int flags = fcntl(fd, F_GETFL, 0);

	if (blocking) {
		flags = flags & ~O_NONBLOCK;
	} else {
		flags = flags | O_NONBLOCK;
	}

	if (fcntl(fd, F_SETFL, flags) < 0) {
		printf("Failed to make UART nonblocking");
	}

}

void Uart::writeStr(std::string pData)
{
	const char *data = pData.c_str();
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

void Uart::setProperties()
{
	termios tempProperties;
	tcgetattr(fd, &tempProperties);

	cfsetispeed(&tempProperties, 100000);
	cfsetospeed(&tempProperties, 100000);


	tempProperties.c_cflag &= ~(PARENB);
	tempProperties.c_cflag &= ~(CSTOPB);
	tempProperties.c_cflag &= ~(CSIZE);
	tempProperties.c_cflag |= CS8;

	tempProperties.c_cflag |= CLOCAL;
	tempProperties.c_cflag |= CREAD;

	tempProperties.c_iflag |= ICRNL;
	tempProperties.c_oflag = 0;
	tempProperties.c_lflag = 0;

	tempProperties.c_cc[VTIME] = 0;
	tempProperties.c_cc[VMIN]  = 1;

	if( tcsetattr(fd, TCSANOW, &tempProperties) == 0 )
	{
		printf("Properties setted for uart\n");
	}
	else
	{
		printf("Error while initializing properties for UART\n");
	}
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

