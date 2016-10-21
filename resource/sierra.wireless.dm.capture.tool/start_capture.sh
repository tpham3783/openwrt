#!/bin/sh
# Author: Toan Pham
# Purpose: simplied method to capture debug logs for Sierra Wireless
# Loging procedure
#
#a       AT+CFUN=0
#b.       Start logging with attached filter
#c.       AT+CFUN=1
#d.       AT!GSTATUS? and verify attach and registration
#e.       Reproduce issue…
#f.       Stop log.

file=${1-dm.raw}
system_status="ATI9 AT!GSTATUS? AT+COPS? AT+CREG? AT!SELRAT? AT!BAND?"
echo "" > $file.system.txt
for i in $system_status; do
    {
    echo "Output of AT command: $i";
    echo "-------------------------------------------"
    at.sh "$i" 
    echo "" 
    echo ""
    } >> $file.system.txt
done;

at.sh "AT+CFUN=0"
./dmcapture.sh  -l -d /dev/ttyUSB0 -f filters/MC7xxx_With-Low-Level-Debug.sqf  -o $file &
pid=$!
sleep 3;
at.sh "AT+CFUN=1"
at.sh "AT!GSTATUS"

echo "Capturing data to $file"
while [ 0 ]; do
   read -p "Type stop to abort capture: " cmd
   if [ "$cmd" = "stop" ]; then
	echo "Stoping capture service";
	killall dmcapture.sh &> /dev/null
	kill -p $pid &> /dev/null
	exit 0;
   elif [ -z "$cmd" ]; then
	ls -lah $file
   else
	eval $cmd
   fi
done;
