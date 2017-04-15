#!/usr/bin/env python
# -*- coding: utf-8 -*-

import RPi.GPIO as GPIO
import time

# Inputs
#-------------------------
firmware = retrieveInput(0, "ycopter")


# Start script
#-------------------------
pwmPins = []
uartPins = []
i2cPins = []

appStart = time.time()
lastChange = appStart

print("Setup GPIO for pwm, uart and i2c\n")
GPIO.setmode(GPIO.BOARD)
#GPIO.setmode(GPIO.BCM)
# To retrieve the current configuration: configuration = GPIO.getmode() 

GPIO.setup(16, GPIO.OUT) 

pwm = GPIO.PWM(16, 50)

pwm.start(7.5)

# Retrieve input parameter
def retrieveInput(pArgvIdx, pDefaultValue):
	if len(sys.argv) > pArgvIdx + 1:
		return sys.argv[pArgvIdx]
	else:
		return pDefaultValue

def initGPIO():
	initPwm(firmware)
	# initUart()
	# initI2c()

def initPwm(pFirmware):
	print("Firmware: "  + pFirmware)


# Servo
# 0° => 1ms width => 0.5ms/20ms * 100 => 2.5%
# 90° => 1.5ms width => 1.5ms/20ms * 100 => 7.5%
# 180° => 1.5ms width => 2.5ms/20ms * 100 => 12.5%


while time.time() - appStart < 300:

	pwm.ChangeDutyCycle(7.5)  # turn towards 90 degree
	time.sleep(1) # sleep 1 second
	pwm.ChangeDutyCycle(2.5)  # turn towards 0 degree
	time.sleep(1) # sleep 1 second
	pwm.ChangeDutyCycle(12.5) # turn towards 180 degree
	time.sleep(1) # sleep 1 second 
	
