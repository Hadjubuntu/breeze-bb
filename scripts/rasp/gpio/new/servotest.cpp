/*
 * servotest.cpp
 *
 *  Created on: Aug 19, 2017
 *      Author: adrien
 */





#define NAVIO_RCOUTPUT_1 1
#define SERVO_MIN 1.250 /*mS*/
#define SERVO_MAX 1.750 /*mS*/

#include "gpio.h"
#include "PCA9685.h"


using namespace Navio;

int main()
{
    static const uint8_t outputEnablePin = 12;


    Pin pin(outputEnablePin);

    if (pin.init()) {
        pin.setMode(Pin::GpioModeOutput);
        pin.write(0); /* drive Output Enable low */
    } else {
        fprintf(stderr, "Output Enable not set. Are you root?\n");
        return 1;
    }

    PCA9685 pwm;

    pwm.initialize();
    pwm.setFrequency(50);

    while (true) {
        pwm.setPWMmS(NAVIO_RCOUTPUT_1, SERVO_MIN);
        sleep(1);
        pwm.setPWMmS(NAVIO_RCOUTPUT_1, SERVO_MAX);
        sleep(1);
    }

    return 0;
}
