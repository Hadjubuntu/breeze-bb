#!/bin/bash
# -----------------------------------------
# BreezeUav project
# -----------------------------------------
# Pwm setup
# Example of use
# sudo ./pwm-setup 0 9.42 20000000 1500000
# -----------------------------------------
# @author Adrien Hadj-Salah
# -----------------------------------------

config-pin overlay cape-universaln

# Initialize pin
# ---------------------------------
pin_list=(9.14 9.16 9.42 8.13 8.19);

for i in "${pin_list[@]}"
do
	echo "Config pin $i"
	config-pin $i pwm
done


# Setup variables
# ---------------------------------
PIN=$1 # 0 => PWM0 | 1 => ? | 2 => P9.22/21 SPIO  |  4 => PWM1A/B | 6 => PWM2A/B
SUBPATH=$2 # 0 for 1A | 1 for 1B
PERIOD=$3 # eg. 20000000 means 50Hz
DUTY_CYCLE=$4 # eg. 1500000 ns

if [ -z "$2" ];	then
	echo "No argument supplied for sub-pin (0 or 1), exit.."
	exit
fi

DIRECTORY="/sys/class/pwm/pwmchip${PIN}"
echo "Directory ${DIRECTORY}"

if [ -d "$DIRECTORY" ]; then
	# Control will enter here if $DIRECTORY exists.

	cd /sys/class/pwm/pwmchip${PIN}
	chmod +rw *
	chown adrien *

	echo ${SUBPATH} > /sys/class/pwm/pwmchip${PIN}/unexport
	echo ${SUBPATH} > /sys/class/pwm/pwmchip${PIN}/export

	cd pwm${SUBPATH}
	chmod +rw *
	chown adrien *

	echo 0 > enable
	echo ${PERIOD} > period
	echo ${DUTY_CYCLE} > duty_cycle
	echo 1 > enable
else
	echo "Unknow directory"
	echo $DIRECTORY
fi


