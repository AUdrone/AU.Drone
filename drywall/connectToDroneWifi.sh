#!/bin/sh

if [ -z "$1" ] ; then
	sudo iwconfig wlan0 essid "ardrone2_005449"
else
	sudo iwconfig wlan0 essid "$1"
fi

sudo dhclient wlan0
sudo ip route del default
sudo ip route add default via 131.204.22.220
