#include <stdio.h>
#include <unistd.h>
#include <stdint.h>
#include <fcntl.h>    
#include <cstring>
#include <stdio.h>
#include <unistd.h>			//Used for UART
#include <fcntl.h>			//Used for UART
#include <termios.h>		//Used for UART


#define BAUD_RATE 100000

// TODO READ https://developer.mbed.org/users/Digixx/code/SBUS-Library_16channel/file/83e415034198/FutabaSBUS/FutabaSBUS.cpp

//At bootup, pins 8 and 10 are already set to UART0_TXD, UART0_RXD (ie the alt0 function) respectively
int uart0_filestream = -1;
uint8_t inData;
int toChannels = 0;
uint8_t sbusData[25];
int16_t channelsCalib[18];
int16_t channels[18];
int feedState = 0;
int bufferIndex=  0;
uint8_t inBuffer[25];
uint8_t rx_buffer[2];

int iter = 0;



void feedLine()
{

	int rxLength = read(uart0_filestream, rx_buffer, 1);

	if (rxLength  > 0) {
		inData = rx_buffer[0];

		switch (feedState){
		case 0:

			// printf("[%d] data=%u | buffer index: %d | rxLength=%d\n", feedState, inData, bufferIndex, rxLength);
			if (inData != 0x0f) {
				return;
			}
			else{
				// printf("Reset buffer =======> feedstate 1\n");
				bufferIndex = 0;
				inBuffer[bufferIndex] = inData;
				inBuffer[24] = 0xff;
				feedState = 1;
			}
			break;
		case 1:

			bufferIndex ++;
			// printf("f[%d] data=%u | buffer index: %d | rxLength=%d\n", feedState, inData, bufferIndex, rxLength);

			inBuffer[bufferIndex] = inData;

			if (bufferIndex < 24 && rxLength  == 0) {
				feedState = 0;
			}
			if (bufferIndex == 24) {
				feedState = 0;
				if (inBuffer[0]==0x0f ) { // && inBuffer[24] == 0x00
					// Copy data
					memcpy(sbusData,inBuffer,25);
					toChannels = 1;
				}
			}
			break;
		}
	}
}


void setUartBaudRate(uint32_t baudrate)
{
    struct termios t;
    memset(&t, 0, sizeof(t));

    tcgetattr(uart0_filestream, &t);
    cfsetspeed(&t, baudrate);
    tcsetattr(uart0_filestream, TCSANOW, &t);
}

int main() {



	//OPEN THE UART
	//The flags (defined in fcntl.h):
	//	Access modes (use 1 of these):
	//		O_RDONLY - Open for reading only.
	//		O_RDWR - Open for reading and writing.
	//		O_WRONLY - Open for writing only.
	//
	//	O_NDELAY / O_NONBLOCK (same function) - Enables nonblocking mode. When set read requests on the file can return immediately with a failure status
	//											if there is no input immediately available (instead of blocking). Likewise, write requests can also return
	//											immediately with a failure status if the output can't be written immediately.
	//
	//	O_NOCTTY - When set and path identifies a terminal device, open() shall not cause the terminal device to become the controlling terminal for the process.
	uart0_filestream = open("/dev/ttyO1", O_RDWR | O_NOCTTY);		//Open in non blocking read/write mode
	if (uart0_filestream == -1)
	{
		//ERROR - CAN'T OPEN SERIAL PORT
		printf("Error - Unable to open UART.  Ensure it is not in use by another application\n");
	}

	//CONFIGURE THE UART
	//The flags (defined in /usr/include/termios.h - see http://pubs.opengroup.org/onlinepubs/007908799/xsh/termios.h.html):
	//	Baud rate:- B1200, B2400, B4800, B9600, B19200, B38400, B57600, B115200, B230400, B460800, B500000, B576000, B921600, B1000000, B1152000, B1500000, B2000000, B2500000, B3000000, B3500000, B4000000
	//	CSIZE:- CS5, CS6, CS7, CS8
	//	CLOCAL - Ignore modem status lines
	//	CREAD - Enable receiver
	//	IGNPAR = Ignore characters with parity errors
	//	ICRNL - Map CR to NL on input (Use for ASCII comms where you want to auto correct end of line characters - don't use for bianry comms!)
	//	PARENB - Parity enable
	//	PARODD - Odd parity (else even)
	struct termios options;
	tcgetattr(uart0_filestream, &options);
	options.c_cflag = B115200 | CS8 | CLOCAL | CREAD;		//<Set baud rate
	options.c_iflag = IGNPAR;
	options.c_oflag = 0;
	options.c_lflag = 0;
	tcflush(uart0_filestream, TCIFLUSH);
	tcsetattr(uart0_filestream, TCSANOW, &options);

	// Finally set baud rate
	setUartBaudRate(BAUD_RATE);


	//----- CHECK FOR ANY RX BYTES -----
	if (uart0_filestream != -1)
	{

		uint8_t loc_sbusData[25] = {0x0f,0x01,0x04,0x20,0x00,0xff,0x07,0x40,0x00,0x02,0x10,0x80,0x2c,0x64,0x21,0x0b,0x59,0x08,0x40,0x00,0x02,0x10,0x80,0x00,0x00};
		int16_t loc_channels[18]  = {1023,1023,1023,1023,1023,1023,1023,1023,1023,1023,1023,1023,1023,1023,1023,1023,0,0};

		std::memcpy(sbusData,loc_sbusData,25);
		std::memcpy(channels,loc_channels,18);
		std::memcpy(channelsCalib, loc_channels, 18);

		while (iter < 250) {
			feedLine();

			if (toChannels == 1) {
				channels[0]  = ((sbusData[1]|sbusData[2]<< 8) & 0x07FF);
				channels[1]  = ((sbusData[2]>>3|sbusData[3]<<5) & 0x07FF);
				channels[2]  = ((sbusData[3]>>6|sbusData[4]<<2|sbusData[5]<<10) & 0x07FF);
				channels[3]  = ((sbusData[5]>>1|sbusData[6]<<7) & 0x07FF);
				channels[4]  = ((sbusData[6]>>4|sbusData[7]<<4) & 0x07FF);
				channels[5]  = ((sbusData[7]>>7|sbusData[8]<<1|sbusData[9]<<9) & 0x07FF);
				channels[6]  = ((sbusData[9]>>2|sbusData[10]<<6) & 0x07FF);
				channels[7]  = ((sbusData[10]>>5|sbusData[11]<<3) & 0x07FF);

				for (int i = 0; i < 5; i ++) {
					printf("ch[%d]=%d | ", i, channels[i]);
				}
				printf("\n");

				toChannels = 0;
				iter ++;
			}
		}


		

	}

	return 0;
}
