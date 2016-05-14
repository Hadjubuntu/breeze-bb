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

PIN=$1 # eg. 0
PIN_NAME=$2 # eg. 9.42
PERIOD=$3 # eg. 20000000 means 50Hz
DUTY_CYCLE=$4 # eg. 1500000 ns
SUBPATH=1

if [ -z "$2" ];	then
	echo "No argument supplied for PIN_NAME, exit.."
	exit
fi

config-pin ${PIN_NAME} pwm

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
fi

