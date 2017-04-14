#!/bin/bash

echo "screen cmd\n"
sudo screen -dm /dev/ttyO1 57600
sudo killall screen
