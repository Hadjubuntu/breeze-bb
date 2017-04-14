#!/bin/bash
# Installation procedure on linux

# Create autorun of application at startup
dirPath="/etc/init.d/"
scriptName="breeze-superscript.sh"
fullpath="${dirPath}${scriptName}"

echo "fullpath: ${fullpath}"

sudo mv ${scriptName} ${fullpath}
sudo chmod 755 ${fullpath}
sudo update-rc.d ${scriptName} defaults
