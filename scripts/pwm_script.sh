#!/bin/bash

# config-pin overlay cape-universaln
sudo su -c "echo am33xx_pwm > /sys/devices/platform/bone_capemgr/slots"


PIN=$1 # 0 => PWM0 | 1 => ? | 2 => P9.22/21 SPIO  |  4 => PWM1A/B | 6 => PWM2A/B
PWM_ELEMENT=$2 # 0 for 1A | 1 for 1B

#config-pin 9.14 pwm

cd /sys/class/pwm/pwmchip${PIN}
chmod +rw *
chown adrien *

echo ${PWM_ELEMENT} > /sys/class/pwm/pwmchip${PIN}/unexport
echo ${PWM_ELEMENT} > /sys/class/pwm/pwmchip${PIN}/export

cd pwm${PWM_ELEMENT}
chmod +rw *
chown adrien *

echo 0 > enable
echo 20000000 > period
echo 1500000 > duty_cycle
echo 1 > enable


