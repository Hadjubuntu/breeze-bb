#!/usr/bin/env python
# -*- coding: utf-8 -*-

import RPi.GPIO as GPIO
import time

appStart = time.time()
lastChange = appStart

print("Setup GPIO for pwm, uart and i2c\n")
GPIO.setmode(GPIO.BOARD)
#GPIO.setmode(GPIO.BCM)
# To retrieve the current configuration: configuration = GPIO.getmode() 

GPIO.setup(12, GPIO.OUT) 

pwm = GPIO.PWM(12, 50)

pwm.start(7.5)


while time.time() - appStart < 300:

	p.ChangeDutyCycle(7.5)  # turn towards 90 degree
	time.sleep(1) # sleep 1 second
	p.ChangeDutyCycle(2.5)  # turn towards 0 degree
	time.sleep(1) # sleep 1 second
	p.ChangeDutyCycle(12.5) # turn towards 180 degree
	time.sleep(1) # sleep 1 second 
	
