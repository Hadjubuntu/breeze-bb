#!/bin/bash

USERNAME="adrien"

echo "Run script at startup to launch breeze-bb application"

# Rebuild app
cd /home/adrien/breeze-bb/
make

# Execute app
sudo /home/adrien/breeze-bb/build/breezebb
