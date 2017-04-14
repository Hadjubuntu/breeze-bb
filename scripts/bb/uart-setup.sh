#!/bin/bash
# -----------------------------------------
# BreezeUav project
# -----------------------------------------
# @author Adrien Hadj-Salah
# -----------------------------------------


# Initialize pin
# ---------------------------------
pin_list=(9.26);

for i in "${pin_list[@]}"
do
	echo "Config pin $i"
	config-pin $i uart
done
