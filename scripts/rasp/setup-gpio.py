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

pwm.start(5)

i=0

while time.time() - appStart < 300:

	duty = i * 10
	print("Duty: " + str(duty))

	pwm.ChangeDutyCycle(duty)
	time.sleep(2)
	
	i+=1

	if i == 10:
		i = 0
