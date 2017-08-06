#include "rpiPWM1.h"


unsigned int  setAngle(rpiPWM1 *el, unsigned int degrees){
    unsigned int retVal = 0;

    if((degrees < 0) || (degrees > 180 ))
        retVal = 1;
    else
    	el->setDutyCycleCount(90+ degrees); // call the necessary rpiPWM1 method

    return retVal;
}

int main (void){

	rpiPWM1 pwm(50.0, 3600, 2.5, rpiPWM1::MSMODE);

	// 20ms = 3600 counts (Period)
	// 2ms = 360 counts (10% duty cycle)  => angle 180
	// 1ms = 180 counts (5% duty cycle) => angle 0
	// 1.5ms = 180+90 = 270 (7.5%  duty cycle ) => angle = 90 //servo centered

	for (int i=0; i < 270; i +=10)
	{
		setAngle(&pwm, i);
		printf("Current angle: %d\n", i);
		usleep(250000);
	}
	setAngle(&pwm, 270);

	return 0;
}