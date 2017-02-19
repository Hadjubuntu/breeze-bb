#!/bin/bash

dirPath="/etc/init.d/"
scriptName="bb-superscript"
fullpath="${dirPath}${scriptName}"

echo "fullpath: ${fullpath}"

sudo mv ${scriptName} ${fullpath}
sudo chmod 755 ${fullpath}
sudo update-rc.d ${scriptName} defaults
