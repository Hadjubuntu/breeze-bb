/*
 * standalonetest.cpp
 *
 *  Created on: Aug 19, 2017
 *      Author: adrien
 */




// PWM example, based on code from http://elinux.org/RPi_Low-level_peripherals for the mmap part
// and http://www.raspberrypi.org/phpBB3/viewtopic.php?t=8467&p=124620 for PWM initialization
//
// compile with "gcc pwm.c -o pwm", test with "./pwm" (needs to be root for /dev/mem access)
//
// Frank Buss, 2012

#define BCM2708_PERI_BASE	0x20000000
#define GPIO_BASE		(BCM2708_PERI_BASE + 0x200000) /* GPIO controller */
#define PWM_BASE		(BCM2708_PERI_BASE + 0x20C000) /* PWM controller */
#define CLOCK_BASE		(BCM2708_PERI_BASE + 0x101000)

#define	PWM_CTL  0
#define	PWM_RNG1 4
#define	PWM_DAT1 5

#define	PWMCLK_CNTL 40
#define	PWMCLK_DIV  41
#define COUNTS 3600

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <dirent.h>
#include <fcntl.h>
#include <assert.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>

#include <unistd.h>

#define PAGE_SIZE (4*1024)
#define BLOCK_SIZE (4*1024)

// I/O access
volatile unsigned *gpio;
volatile unsigned *pwm;
volatile unsigned *clk;

// GPIO setup macros. Always use INP_GPIO(x) before using OUT_GPIO(x) or SET_GPIO_ALT(x,y)
#define INP_GPIO(g) *(gpio+((g)/10)) &= ~(7<<(((g)%10)*3))
#define OUT_GPIO(g) *(gpio+((g)/10)) |=  (1<<(((g)%10)*3))
#define SET_GPIO_ALT(g,a) *(gpio+(((g)/10))) |= (((a)<=3?(a)+4:(a)==4?3:2)<<(((g)%10)*3))

#define GPIO_SET *(gpio+7)  // sets   bits which are 1 ignores bits which are 0
#define GPIO_CLR *(gpio+10) // clears bits which are 1 ignores bits which are 0

// map 4k register memory for direct access from user space and return a user space pointer to it
volatile unsigned *mapRegisterMemory(unsigned long base)
{
	static int mem_fd = 0;
	//	char *mem, *map;
	void *map;

	/* open /dev/mem */
	if (!mem_fd) {
		if ((mem_fd = open("/dev/mem", O_RDWR|O_SYNC) ) < 0) {
			printf("can't open /dev/mem \n");
			exit (-1);
		}
	}

	/* mmap register */

	//	// Allocate MAP block
	//	if ((mem = (char *)malloc(BLOCK_SIZE + (PAGE_SIZE-1))) == NULL) {
	//		printf("allocation error \n");
	//		exit (-1);
	//	}
	//
	//	// Make sure pointer is on 4K boundary
	//	if ((unsigned long)mem % PAGE_SIZE)
	//		mem += PAGE_SIZE - ((unsigned long)mem % PAGE_SIZE);


	// TODO
	// Added: MAP_LOCKED insted of MAP_FIXED, PROT_EXEC

	// Now map it
	map = (char *)mmap(
			NULL , // (caddr_t)mem replaced by NULL
			BLOCK_SIZE,
			PROT_READ|PROT_WRITE|PROT_EXEC,
			MAP_SHARED|MAP_LOCKED,
			mem_fd,
			base
	);

	if (map == MAP_FAILED) {
		perror("mmap error >> ");
		close(mem_fd);
		exit (1);
	}


	if(close(mem_fd) < 0){ //No need to keep mem_fd open after mmap
		//i.e. we can close /dev/mem
		perror("couldn't close /dev/mem file descriptor");
		exit(1);
	}

	// Always use volatile pointer!
	return (volatile unsigned *)map;
}

// set up a memory regions to access GPIO, PWM and the clock manager
void setupRegisterMemoryMappings()
{
	clk = mapRegisterMemory(CLOCK_BASE);
	gpio = mapRegisterMemory(GPIO_BASE);
	pwm = mapRegisterMemory(PWM_BASE);
}

void setServo(const unsigned int &percent)
{
	// TODO replace by percent directly
	//	int bitCount;
	//	unsigned int bits = 0;
	//
	//	// 32 bits = 2 milliseconds
	//	bitCount = 16 + 16 * percent / 100;
	//	if (bitCount > 32) bitCount = 32;
	//	if (bitCount < 1) bitCount = 1;
	//	bits = 0;
	//	while (bitCount) {
	//		bits <<= 1;
	//		bits |= 1;
	//		bitCount--;
	//	}
	*(pwm + PWM_DAT1) = percent;
}

// init hardware
void initHardware(double pFrequencyHz)
{
	// mmap register space
	setupRegisterMemoryMappings();

	// Set input
	INP_GPIO(18);

	// set PWM alternate function for GPIO18
	SET_GPIO_ALT(18, 5);

	/*GPIO 18 in ALT5 mode for PWM0 */
	// Let's first set pin 18 to input
	//taken from #define INP_GPIO(g) *(gpio+((g)/10)) &= ~(7<<(((g)%10)*3))
	*(gpio+1) &= ~(7 << 24);
	//then set it to ALT5 function PWM0
	//taken from #define SET_GPIO_ALT(g,a) *(gpio+(((g)/10))) |= (((a)<=3?(a)+4:(a)==4?3:2)<<(((g)%10)*3))
	*(gpio+1) |= (2<<24);

	// ------------------------

	// stop clock and waiting for busy flag doesn't work, so kill clock
	*(clk + PWMCLK_CNTL) = 0x5A000000 | (1 << 5);
	usleep(10);

	// wait until busy flag is set
	while ( (*(clk + PWMCLK_CNTL)) & 0x00000080){}

	//calculate divisor value for PWM1 clock...base frequency is 19.2MHz
	double period = 1.0 / pFrequencyHz;
	double countDuration = period / (COUNTS*1.0f);
	int divisor = (int)(19200000.0f / (1.0/countDuration));

	if( divisor < 0 || divisor > 4095 ) {
		printf("divisor value must be between 0-4095\n");
		exit(-1);
	}

	*(clk + PWMCLK_DIV)  = 0x5A000000 | (divisor << 12);

	// source=osc and enable clock
	*(clk + PWMCLK_CNTL) = 0x5A000011;

	// disable PWM
	*(pwm + PWM_CTL) = 0;

	// needs some time until the PWM module gets disabled, without the delay the PWM module crashs
	usleep(10);

	// filled with 0 for 20 milliseconds = 320 bits
	// TODO 320 replaced by 3600
	*(pwm + PWM_RNG1) = COUNTS; usleep(10);

	// 32 bits = 2 milliseconds, init with 1 millisecond
	//	setServo(0); TODO replaced by
	*(pwm + PWM_DAT1) = (int) ((10.0/100.0) * COUNTS); usleep(10);

	// start PWM1 in serializer mode
	//	*(pwm + PWM_CTL) = 3; TODO previous
	// Replace with

	//	printf("Value of pwm_ctl: %d\n\n", ( (1 << 7) | (1 << 0) ))
	*(pwm + PWM_CTL) |= ( (1 << 7) | (1 << 0) );
}

void close()
{

	//lets put the PWM peripheral registers in their original state
	*(pwm + PWM_CTL) = 0;
	*(pwm + PWM_RNG1) = 0x20;
	*(pwm + PWM_DAT1) = 0;
	// unmap the memory block containing PWM registers
	if(munmap((void*)pwm, BLOCK_SIZE) < 0){
		perror("munmap (pwm) failed");
		exit(1);
	}
	//lets put the PWM Clock peripheral registers in their original state
	//kill PWM clock
	*(clk + PWMCLK_CNTL) = 0x5A000000 | (1 << 5);
	usleep(10);

	// wait until busy flag is set
	while ( (*(clk + PWMCLK_CNTL)) & 0x00000080){}

	//reset divisor
	*(clk + PWMCLK_DIV) = 0x5A000000;
	usleep(10);

	// source=osc and enable clock
	*(clk + PWMCLK_CNTL) = 0x5A000011;

	// unmap the memory block containing PWM Clock registers
	if(munmap((void*)clk, BLOCK_SIZE) < 0){
		perror("munmap (clk) failed");
		exit(1);
	}

	//lets put the GPIO peripheral registers in their original state
	//first put it in input mode (default)
	//taken from #define INP_GPIO(g) *(gpio+((g)/10)) &= ~(7<<(((g)%10)*3))
	*(gpio+1) &= ~(7 << 24);
	//then munmap
	if(munmap((void*)gpio, BLOCK_SIZE) < 0){
		perror("munmap (gpio) failed");
		exit(1);
	}
}

int main(int argc, char **argv)
{
	// init PWM module for GPIO pin 18 with 50 Hz frequency
	initHardware(50.0);

	// servo test, position in percent: 0 % = 1 ms, 100 % = 2 ms

	printf("Position: 90\n");
	setServo(90);
	sleep(1);


	printf("Position: 100\n");
	setServo(100);
	sleep(1);


	close();

	return 0;
}
