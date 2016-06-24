#!/bin/sh
#
# This script creates interfaces (wwan0 and wwan1) for LT100x modems
# in /etc/config/network and add those interfaces to "wan" firewall zone

. /lib/functions.sh
. /lib/ramips.sh
. /lib/functions/uci-defaults-new.sh
. /lib/functions/system.sh

# LT1000
uci set network.wwan0=interface
uci set network.wwan0.ifname=wwan0
uci set network.wwan0.device=/dev/cdc-wdm0
uci set network.wwan0.proto=dhcp
uci set network.wwan0.apn=nxtgenphone
uci commit

# LT1001/LT1002
uci set network.wwan1=interface
uci set network.wwan1.ifname=eth1
uci set network.wwan1.proto=dhcp
uci commit

# firewall
i=0
ret=`uci get firewall.@zone[$i].name`
while [ -n "$ret" ]  ; do
	if [ "$ret" == "wan" ]; then
		nw=`uci get firewall.@zone[$i].network`	
		nw="$nw"" wwan0 wwan1"
		uci set firewall.@zone[$i].network="$nw"
		uci commit
		exit 0
	fi	
	i=`expr $i + 1`
	ret=`uci get firewall.@zone[$i].name`
done 

