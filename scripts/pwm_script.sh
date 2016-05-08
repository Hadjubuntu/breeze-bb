#!/bin/bash

config-pin overlay cape-universaln

PIN=0
SUBPATH=0

config-pin 9.42 pwm

cd /sys/class/pwm/pwmchip${PIN}
chmod +rw *
chown adrien *

echo ${SUBPATH} > /sys/class/pwm/pwmchip${PIN}/unexport
echo ${SUBPATH} > /sys/class/pwm/pwmchip${PIN}/export

cd pwm${SUBPATH}
chmod +rw *
chown adrien *

echo 0 > enable
echo 20000000 > period
echo 1500000 > duty_cycle
echo 1 > enable


