#!/usr/bin/env python
# -*- coding: utf-8 -*-

import RPi.GPIO as GPIO
import time
import Tools

# Inputs
#-------------------------
firmware = Tools.retrieveInput(0, "fixed_wing")


# Start script
#-------------------------
minDutyPercent = 2.5 # = 0%
maxDutyPercent = 12.5 # = 100%
pwmPins = [11, 12, 13, 15, 16, 18]
uartPins = [{'rx':10, 'tx':8}]
i2cPins = [{'sda': 3, 'scl': 5}]
pwmObj = []

appStart = time.time()
lastChange = appStart

print("Setup GPIO for pwm, uart and i2c\n")
GPIO.setmode(GPIO.BOARD)
#GPIO.setmode(GPIO.BCM)
# To retrieve the current configuration: configuration = GPIO.getmode() 

GPIO.setup(16, GPIO.OUT) 

pwm = GPIO.PWM(16, 50)

pwm.start(7.5)

# Initialize pwm pins regarding firwmare
def initPwm(pFirmware):
	firstEl = True

	for pwmPin in pwmPins:
		initDutyCycle = minDutyPercent
		freq = 490

		# Specific configuration for servos
		if not firstEl and pFirmware == 'fixed_wing':
			freq = 50
			initDutyCycle = (minDutyPercent + maxDutyPercent / 2.0)

		GPIO.setup(pwmPin, GPIO.OUT)
		cPwm = GPIO.PWM(pwmPin, freq)
		cPwm.start(initDutyCycle)

		print("GPIO " + str(pwmPin) + " initialized at " + str(freq) + "Hz setted at " + str(initDutyCycle) +  " of duty cycle.")

		pwmObj.append(cPwm)
		firstEl = False

def initGPIO(pFirmware):
	print("Firmware: " + pFirmware)	
	initPwm(pFirmware)
	# initUart()
	# initI2c()


initGPIO(firmware)

# Servo
# 0° => 1ms width => 0.5ms/20ms * 100 => 2.5%
# 90° => 1.5ms width => 1.5ms/20ms * 100 => 7.5%
# 180° => 1.5ms width => 2.5ms/20ms * 100 => 12.5%


# while time.time() - appStart < 300:

# 	pwm.ChangeDutyCycle(7.5)  # turn towards 90 degree
# 	time.sleep(1) # sleep 1 second
# 	pwm.ChangeDutyCycle(2.5)  # turn towards 0 degree
# 	time.sleep(1) # sleep 1 second
# 	pwm.ChangeDutyCycle(12.5) # turn towards 180 degree
# 	time.sleep(1) # sleep 1 second 
	
