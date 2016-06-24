# This script handles the issue where wwan0 is not 
# brought up after the modem resets with Telit modems. 
# This also happens to the Sierra module, so handle reset.

h=$(cat /proc/sys/kernel/hostname)
if [ "$h" == "AP102B" ]; then
	# This happens because openwrt is still busy formating JFFS right after sys-upgrade

	[ -f /tmp/.quirk_sysupgrade ] && exit 0;

	# This must be first boot, mount service maybe running and block LED config. Start LED task
	[ -f /etc/init.d/led ] && /etc/init.d/led boot

	if lsusb | grep "1199:68c0"; then
		# Special treatment for Sierra Wireless module
		at.sh "at!reset"
	fi
	touch /tmp/.quirk_sysupgrade
	# wman failover seems to have a problem tracking wan right after sysupgrade
	mwan3 restart
fi


modem=$(uci get modem.@modem[0].module)

# This fixup is only for Sierra Wireless Cell Modem
if [ "$modem" == "MC7350" ]; then
	# Allow enough time for mwan3 to check interface status, dont do anything
	if [ ! -f /tmp/.lock_first_modem_check ]; then
		touch /tmp/.lock_first_modem_check
		exit 0;
	fi
	if mwan3 interfaces | grep "interface wwan1 is online" ; then
		[ -f /tmp/.lock_sierra_issued_reset ] && rm /tmp/.lock_sierra_issued_reset
		exit 0;
	else
		touch /tmp/.lock_sierra_issued_reset
		at.sh "at!reset"	
		exit 0;
	fi 
	exit 0;
fi 


if [ -c /dev/cdc-wdm0 ] && ! ifconfig | grep wwan0 > /dev/null; then
	/etc/init.d/network restart 
fi

exit 0
