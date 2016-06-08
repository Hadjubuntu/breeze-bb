/*
 * Uart.h
 *
 *  Created on: Jun 7, 2016
 *      Author: adrien
 */

#ifndef SRC_HAL_UART_H_
#define SRC_HAL_UART_H_


#include <string>
#include <stdint.h>
/**
 * Manage UART serial communication
 */
class Uart {
private:
	/** P9.24/26 : UART1 */
	int uartPin;
	/** Device path */
	std::string devicePath;
	/** File status */
	int fd;

	void disableCrlf();
public:
	/**
	 * Create a new instance of uart
	 *
	 * @param pUartPin Pin UART
	 */
	Uart(int pUartPin);

	/** Open serial port */
	void open();

	/** Set baud rate */
	void setSpeed(unsigned int pBaudRate);

	/**
	 * Write to the serial
	 */
	int write(const uint8_t *buf, uint16_t n);
	void writeStr(std::string pData);

	int read(uint8_t *buf, uint16_t n);

	void setBlocking(bool blocking);

	virtual ~Uart();
};

#endif /* SRC_HAL_UART_H_ */
