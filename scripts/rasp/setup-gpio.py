#!/usr/bin/env python
# -*- coding: utf-8 -*-

import RPi.GPIO as GPIO
import time

print("Setup GPIO for pwm, uart and i2c\n")
GPIO.setmode(GPIO.BOARD)
#GPIO.setmode(GPIO.BCM)
# To retrieve the current configuration: configuration = GPIO.getmode() 

GPIO.setup(12, GPIO.OUT) 

pwm = GPIO.PWM(12, 100)

pwm.start(5)

for i in range(1, 9):
	print("Change duty cycle and sleep")
	duty = i * 10
	pwm.ChangeDutyCycle(duty)
	time.sleep(2)