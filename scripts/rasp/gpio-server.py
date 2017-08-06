#!/usr/bin/env python
# coding: utf-8

PORT=1111

import socket
import threading


serverShallRun = True

class ClientThread(threading.Thread):

    shallRunClient = True
    
    def __init__(self, ip, port, clientsocket):
        threading.Thread.__init__(self)
        self.ip = ip
        self.port = port
        self.clientsocket = clientsocket
        print("[+] Nouveau thread pour %s %s" % (self.ip, self.port, ))

    def run(self): 
   
        print("Connection de %s %s" % (self.ip, self.port, ))
        while self.shallRunClient:
            r = self.clientsocket.recv(128)            
            print("Message received: " + r)
                
            if r == 'quit':
                self.shallRunClient = False
                global serverShallRun
                serverShallRun = False
                print("status: " + str(serverShallRun))
            else:
                self.clientsocket.send("ACQ")
        
        print("Client déconnecté...")
        
        
# Server part
# ---------------------------------
tcpsock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
tcpsock.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
tcpsock.bind(("",PORT))


while serverShallRun:
    tcpsock.listen(10)
    print( "En écoute...")
    (clientsocket, (ip, port)) = tcpsock.accept()
    newthread = ClientThread(ip, port, clientsocket)
    newthread.start()