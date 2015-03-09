#!/bin/sh
Connect() {
	local	
	#sudo iw dev
	read -p "Which interface would you like to use:" inter
	sudo wpa_supplicant -i $inter -c ~/WPA2_CONF/$conf &
	sleep 10
	sudo dhcpcd $inter
}
Create() {
	local
	read -p "Name of config file:" conf
	read -p "SSID:" ssid
	read -p "Username:" usr
	echo -n "Password:"
	read -s pas
	cd ~/WPA2_CONF
	touch $conf
	echo "network={" >> $conf
	echo "  ssid=\"$ssid\"" >> $conf
	echo "  scan_ssid=1" >> $conf
	echo "  key_mgmt=WPA-EAP" >> $conf
	echo "  identity=\"$usr\"" >> $conf
	echo "  password=\"$pas\"" >> $conf
	echo "  eap=PEAP" >> $conf
	echo "  phase1=\"peaplabel=0\"" >> $conf
	echo "  phase2=\"auth=MSCHAPV2\"" >> $conf
	echo "}" >> $conf
	echo "" 
	cat $conf #| grep -v "password="
	exit
}

Configcheck() {
	if [ -d ~/WPA2_CONF ]; then
		Create
	else
		mkdir ~/WPA2_CONF
		Create
	fi
}


read -p "Creat config or connect?(cr/co)" menu
if [ "$menu" == "cr" ]; then
	Configcheck	
elif [ "$menu" == "co" ]; then
	read -p "Name of config(and/or path):" conf
	if [ -f ~/WPA2_CONF/$conf ]; then
		Connect
	elif [ -r ~/WPA2_CONF/$conf ]; then
		read -p "Config does not exist, create one?(y/n):" a
		if [ "$a" == "y" ]; then
			Configcheck
		elif [ "$a" == "n" ]; then
			echo "------"
			exit
		else
			echo "Invalid input"
			exit
		fi
	else
		echo "Invalid input"
		exit
	fi			
else 
	echo "Invalid input"
	exit
fi
