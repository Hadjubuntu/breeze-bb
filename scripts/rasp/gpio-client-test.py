#!/usr/bin/env python
# coding: utf-8

import socket
import time


hote = "localhost"
port = 1111

socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
socket.connect((hote, port))
print "Connection on {}".format(port)

start = time.time()
socket.send(u"Hey my name is Adrien!")
msg = socket.recv(99);
print("Received: " + str(msg))
print("Delay: " + str((time.time()-start)/1000.0) + " ms")

print "Close"
socket.close()