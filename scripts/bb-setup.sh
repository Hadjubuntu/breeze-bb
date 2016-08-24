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
pwm_freq_list=(50 50 0 50 50);
uart_pin_list=(9.24 9.26);

pwmIdx=(0 1 2 3 4 5 6 7);
for j in "${pwmIdx}"
do
	sudo su -c "echo $j > /sys/class/pwm/export"
done

idx=0
for i in "${pwm_pin_list[@]}"
do
	config-pin $i pwm

	# Prepare file access mode
	DIRECTORY="/sys/class/pwm/pwm${idx}"
	echo "Directory ${DIRECTORY}"

	if [ -d "$DIRECTORY" ]; then
		# Control will enter here if $DIRECTORY exists.

		cd ${DIRECTORY}
		chmod +rw *
		chown ${user} *

		PERIOD=${pwm_freq_list[$idx]} # eg. 20000000 means 50Hz TODO operation division and *10^9
		DUTY_CYCLE="800000" # min duty cycle =  800 us | eg. 1500000 ns

		echo "Duty cycle="
		echo ${DUTY_CYCLE}

		echo ${PERIOD} > period_ns
		echo ${DUTY_CYCLE} > duty_ns
		echo 1 > run

		idx=$((idx+1))
	else
		echo "Unknow directory"
	fi
done
for i in "${uart_pin_list[@]}"
do
	config-pin $i uart
done
