/*
 * GPIO.cpp
 *
 *  Created on: Apr 27, 2016
 *      Author: adrien-i3
 */

#include "GPIO.h"
#include "HAL.h"


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <fcntl.h>
#include <poll.h>
#include <sys/mman.h>
#include <sys/stat.h>


GPIO::GPIO()
{}

void GPIO::init()
{
#if LINUX_GPIO_NUM_BANKS == 4
	int mem_fd;
	// Enable all GPIO banks
	// Without this, access to deactivated banks (i.e. those with no clock source set up) will (logically) fail with SIGBUS
	// Idea taken from https://groups.google.com/forum/#!msg/beagleboard/OYFp4EXawiI/Mq6s3sg14HoJ

	short bank_enable[3] = { 5, 65, 105 };
	int export_fd = open("/sys/class/gpio/export", O_WRONLY);
	if (export_fd == -1) {
		printf("unable to open /sys/class/gpio/export");
	}
	for (short i=0; i<3; i++) {
		dprintf(export_fd, "%u\n", (unsigned)bank_enable[i]);
	}
	close(export_fd);


	/* open /dev/mem */
	if ((mem_fd = open("/dev/mem", O_RDWR|O_SYNC) ) < 0) {
		printf("can't open /dev/mem \n");
		exit (-1);
	}

	/* mmap GPIO */
	off_t offsets[LINUX_GPIO_NUM_BANKS] = { GPIO0_BASE, GPIO1_BASE, GPIO2_BASE, GPIO3_BASE };
	for (short i=0; i<LINUX_GPIO_NUM_BANKS; i++) {
		gpio_bank[i].base = (volatile unsigned *)mmap(0, GPIO_SIZE, PROT_READ|PROT_WRITE, MAP_SHARED, mem_fd, offsets[i]);
		if ((char *)gpio_bank[i].base == MAP_FAILED) {
			printf("unable to map GPIO bank\n");
		}
		gpio_bank[i].oe = gpio_bank[i].base + GPIO_OE;
		gpio_bank[i].in = gpio_bank[i].base + GPIO_IN;
		gpio_bank[i].out = gpio_bank[i].base + GPIO_OUT;
	}

	close(mem_fd);
#endif // LINUX_GPIO_NUM_BANKS
}

void GPIO::pinMode(short pin, short output)
{
	short bank = pin/32;
	short bankpin = pin & 0x1F;
	if (bank >= LINUX_GPIO_NUM_BANKS) {
		return;
	}
	if (output == HAL_GPIO_INPUT) {
		*gpio_bank[bank].oe |= (1U<<bankpin);
	} else {
		*gpio_bank[bank].oe &= ~(1U<<bankpin);
	}
}

short GPIO::analogPinToDigitalPin(short pin)
{
	return -1;
}


short GPIO::read(short pin) {

	short bank = pin/32;
	short bankpin = pin & 0x1F;
	if (bank >= LINUX_GPIO_NUM_BANKS) {
		return 0;
	}
	return *gpio_bank[bank].in & (1U<<bankpin) ? HIGH : LOW;

}

void GPIO::write(short pin, short value)
{
	short bank = pin/32;
	short bankpin = pin & 0x1F;
	if (bank >= LINUX_GPIO_NUM_BANKS) {
		return;
	}
	if (value == LOW) {
		*gpio_bank[bank].out &= ~(1U<<bankpin);
	} else {
		*gpio_bank[bank].out |= 1U<<bankpin;
	}
}

void GPIO::toggle(short pin)
{
	write(pin, !read(pin));
}

/* Alternative interface: */
//AP_HAL::DigitalSource* GPIO::channel(uint16_t n) {
//	return new DigitalSource(n);
//}

/* Interrupt interface: */
//bool GPIO::attach_interrupt(short interrupt_num, AP_HAL::Proc p, short mode)
//{
//	return true;
//}

bool GPIO::usb_connected(void)
{
	return false;
}
