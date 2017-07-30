#!/usr/bin/env python
# -*- coding: utf-8 -*-
#----------------------------
# setup-gpio.py
# Example of command:
# setup-gpio.py setup fixed_wing

import RPi.GPIO as GPIO
import time
import Tools
import serial
import sys

# Inputs
#-------------------------
action = Tools.retrieveInput(1, "None")


# Start script
#-------------------------
minDutyPercent = 2.5 # = 0%
maxDutyPercent = 12.5 # = 100%
pwmPins = [11, 12, 13, 15, 16, 18]
uartPins = [{'rx': 10, 'tx': 8}]
i2cPins = [{'sda': 3, 'scl': 5}]
pwmObj = []

appStart = time.time()
lastChange = appStart


print("Setup GPIO for pwm, uart and i2c\n")
GPIO.setmode(GPIO.BOARD)


#GPIO.setmode(GPIO.BCM)
# To retrieve the current configuration: configuration = GPIO.getmode() 

# Initialize pwm pins regarding firwmare
def initPwm(pFirmware):
	firstEl = True

	for pwmPin in pwmPins:
		initDutyCycle = minDutyPercent
		freq = 490

		# Specific configuration for servos
		if not firstEl and pFirmware == 'fixed_wing':
			freq = 50
			initDutyCycle = (minDutyPercent + maxDutyPercent) / 2.0

		GPIO.setup(pwmPin, GPIO.OUT)
		cPwm = GPIO.PWM(pwmPin, freq)
		cPwm.start(initDutyCycle)

		print("GPIO " + str(pwmPin) + " initialized at " + str(freq) + "Hz setted at " + str(initDutyCycle) +  " of duty cycle.")

		pwmObj.append(cPwm)
		firstEl = False




# Initialize GPIO pwm, uart and i2c
def initGPIO(pFirmware):
	print("Firmware: " + pFirmware)	
	initPwm(pFirmware)
	
def setup():	
	firmware = "fixed_wing" # Tools.retrieveInput(2, "fixed_wing")
	initGPIO(firmware)
	
	# Servo
	# 0° => 1ms width => 0.5ms/20ms * 100 => 2.5%
	# 90° => 1.5ms width => 1.5ms/20ms * 100 => 7.5%
	# 180° => 1.5ms width => 2.5ms/20ms * 100 => 12.5%	
# 	while time.time() - appStart < 300:
# 		pwmObj[1].ChangeDutyCycle(7.5)  # turn towards 90 degree
# 		time.sleep(1) # sleep 1 second
# 		pwmObj[1].ChangeDutyCycle(2.5)  # turn towards 0 degree
# 		time.sleep(1) # sleep 1 second
# 		pwmObj[1].ChangeDutyCycle(12.5) # turn towards 180 degree
# 		time.sleep(1) # sleep 1 second 
		


# Control a pwm
# -------------------------
def control():
	pwmIdx = int(Tools.retrieveInput(2, 0));
	pinNumber = pwmPins[pwmIdx]
	dutyCycle = float(Tools.retrieveInput(3, 2.5))
	print("Command: pin[" + str(pwmIdx) +"] = " + str(pinNumber) + " setted to " + str(dutyCycle) + " duty cycle")
	
	setup();
	
	pwmObj[pwmIdx].ChangeDutyCycle(dutyCycle)
	
	
	

# Route action
# -------------------------
if action == "setup":
	setup();
if action == "control":
	control();
		

	
	
	