# Allow enough time for USB bus to settle
sleep 3 

# Bus 001 Device 002: ID 1199:68c0 Sierra Wireless, Inc.
# Bus 001 Device 002: ID 1bc7:1201 Telit

if lsusb | grep 1199; then
	# No neccessary bc Gobi will bind before cdc
	rmmod cdc_wdm
	rmmod qmi_wwan

	# Do not start AvageAgent, otherwise will crash connectionManager
	export  AVA_PATH=NO_AVA 
	/sdk/connectionmgrmipsel /sdk/slqssdk 0 a || /sdk/slqssdk 0 &
else
	# Telit module will auto connect via qmi kernel interface
	echo -n "";
	rmmod GobiNet
	rmmod GobiSerial
fi

exit 0
