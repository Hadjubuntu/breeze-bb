/*
 * GPIO.h
 *
 *  Created on: Apr 27, 2016
 *      Author: adrien-i3
 */

#ifndef SRC_HAL_GPIO_H_
#define SRC_HAL_GPIO_H_

#define SYSFS_GPIO_DIR "/sys/class/gpio"

#define GPIO0_BASE 0x44E07000
#define GPIO1_BASE 0x4804C000
#define GPIO2_BASE 0x481AC000
#define GPIO3_BASE 0x481AE000

#define GPIO_SIZE  0x00000FFF

// OE: 0 is output, 1 is input
#define GPIO_OE    0x14d
#define GPIO_IN    0x14e
#define GPIO_OUT   0x14f

#define LED_AMBER       117
#define LED_BLUE        48
#define LED_SAFETY      61
#define SAFETY_SWITCH   116
#define LOW             0
#define HIGH            1
#define LINUX_GPIO_NUM_BANKS 4


// BeagleBone Black GPIO mappings
#define BBB_USR0 53
#define BBB_USR1 54
#define BBB_USR2 55
#define BBB_USR3 56
#define BBB_P8_3 38
#define BBB_P8_4 39
#define BBB_P8_5 34
#define BBB_P8_6 35
#define BBB_P8_7 66
#define BBB_P8_8 67
#define BBB_P8_9 69
#define BBB_P8_10 68
#define BBB_P8_11 45
#define BBB_P8_12 44
#define BBB_P8_13 23
#define BBB_P8_14 26
#define BBB_P8_15 47
#define BBB_P8_16 46
#define BBB_P8_17 27
#define BBB_P8_18 65
#define BBB_P8_19 22
#define BBB_P8_20 63
#define BBB_P8_21 62
#define BBB_P8_22 37
#define BBB_P8_23 36
#define BBB_P8_24 33
#define BBB_P8_25 32
#define BBB_P8_26 61
#define BBB_P8_27 86
#define BBB_P8_28 88
#define BBB_P8_29 87
#define BBB_P8_30 89
#define BBB_P8_31 10
#define BBB_P8_32 11
#define BBB_P8_33 9
#define BBB_P8_34 81
#define BBB_P8_35 8
#define BBB_P8_36 80
#define BBB_P8_37 78
#define BBB_P8_38 79
#define BBB_P8_39 76
#define BBB_P8_40 77
#define BBB_P8_41 74
#define BBB_P8_42 75
#define BBB_P8_43 72
#define BBB_P8_44 73
#define BBB_P8_45 70
#define BBB_P8_46 71
#define BBB_P9_11 30
#define BBB_P9_12 60
#define BBB_P9_13 31
#define BBB_P9_14 50
#define BBB_P9_15 48
#define BBB_P9_16 51
#define BBB_P9_17 5
#define BBB_P9_18 4
#define BBB_P9_19 13
#define BBB_P9_20 12
#define BBB_P9_21 3
#define BBB_P9_22 2
#define BBB_P9_23 49
#define BBB_P9_24 15
#define BBB_P9_25 117
#define BBB_P9_26 14
#define BBB_P9_27 115
#define BBB_P9_28 113
#define BBB_P9_29 111
#define BBB_P9_30 112
#define BBB_P9_31 110
#define BBB_P9_41 20
#define BBB_P9_42 7

class GPIO {
private:
	struct gpioContainer {
		volatile unsigned int *base;
		volatile unsigned int *oe;
		volatile unsigned int *in;
		volatile unsigned int *out;
	} gpio_bank[LINUX_GPIO_NUM_BANKS];

public:
	GPIO();
	void    init();
	void    pinMode(short pin, short output);
	short  analogPinToDigitalPin(short pin);
	short read(short pin);
	void    write(short pin, short value);
	void    toggle(short pin);

	/* Alternative interface: */
//	AP_HAL::DigitalSource* channel(uint16_t n);

	/* Interrupt interface: */
	//    bool    attach_interrupt(short interrupt_num, AP_HAL::Proc p,
	//            short mode);

	/* return true if USB cable is connected */
	bool    usb_connected(void);
};
#endif /* SRC_HAL_GPIO_H_ */
