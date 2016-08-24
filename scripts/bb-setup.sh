#!/bin/bash
# -----------------------------------------
# BreezeUav project
# -----------------------------------------
# breeze setup
# -----------------------------------------
# @author Adrien Hadj-Salah
# -----------------------------------------

user="adrien"

# Define beaglebone tree-overlay
config-pin overlay cape-universaln

# Initialize pin (uart, pwm, i2c ?..)
# ---------------------------------
pwm_pin_list=(9.14 9.16 9.42 8.13 8.19);
pwm_freq_list=(9.14=50 9.16=50 59.42=0 8.13=50 8.19=50);
uart_pin_list=(9.24 9.26);

for i in "${pwm_pin_list[@]}"
do
	config-pin $i pwm

	# Prepare file access mode
	DIRECTORY="/sys/class/pwm/pwm${i}"
	echo "Directory ${DIRECTORY}"

	if [ -d "$DIRECTORY" ]; then
		# Control will enter here if $DIRECTORY exists.

		cd ${DIRECTORY}
		chmod +rw *
		chown ${user} *

		PERIOD=${pwm_freq_list[$i]} # eg. 20000000 means 50Hz
		DUTY_CYCLE=800000 # min duty cycle =  800 us | eg. 1500000 ns

		echo ${PERIOD} > period_ns
		echo ${DUTY_CYCLE} > duty_ns
		echo 1 > run
	fi
done
for i in "${uart_pin_list[@]}"
do
	config-pin $i uart
done