#!/bin/bash

function setup_AP() {
  systemctl stop wpa_supplicant
  res=$(systemctl is-enabled network-manager)
  if [ "$res" == 'enabled' ]; then
    echo " systemctl stop network-manager ..."
    systemctl stop network-manager
    echo " systemctl disable network-manager ..."
    systemctl disable network-manager
    echo " stop network-manager sucessfully."
  fi
  systemctl start isc-dhcp-server
  if [ $? -eq 0 ]; then
    echo " systemctl start hostapd"
    systemctl start hostapd
    res=$(systemctl is-active hostapd)
    if [ "$res" != "active" ]; then
      echo " hostapd is not started"
    fi

    if [ $? -eq 0 ]; then
      echo "start Wifi Ap Mode sucessfully."
      echo "ifconfig wlan0 192.168.8.1 netmask 255.255.255.0 ..."
      ifconfig wlan0 192.168.8.1 netmask 255.255.255.0
      echo "setup ip address for wifi_ap ret = $?"
      return 0
    fi
  #	sudo /sbin/route del default gw 192.168.8.1 wlan0
  fi
  return -1
}

function cancel_AP() {
  echo "start cancel_AP()"
  systemctl stop hostapd
  systemctl stop isc-dhcp-server
  systemctl start wpa_supplicant
  echo "cancel_AP()"
  return $?
}

function connect_wifi() {
  echo "start connect_wifi()"
  res=$(systemctl is-active wpa_supplicant)
  if [ "$res" == 'inactive' ]; then
    systemctl start wpa_supplicant
  fi
  /bin/echo "" >/var/log/wpa_supplicant
  ip address flush dev $1
  wpa_cli -i $1 disconnect
  wpa_cli -i $1 remove_network 0
  wpa_cli -i $1 add_network
  wpa_cli -i $1 set_network 0 ssid "\"$2\""
  wpa_cli -i $1 set_network 0 psk "\"$3\""
  wpa_cli -i $1 enable_network 0
  wpa_cli -i $1 save_config
  wpa_cli -i $1 reconnect
  ifconfig $1 down
  ifconfig $1 up &
  echo "connect wifi $2"
  return 0
}

function start_dhcp() {
  echo "check the status of isc-dhcp-server"
  systemctl status isc-dhcp-server | grep Active: | awk '{print $2}' | while read line; do
    echo "line = $line"
    if [ "$line" != "active" ]; then
      echo "restart isc-dhcp-server"
      systemctl restart isc-dhcp-server
      echo "restart isc-dhcp-server ret = $?"
      return 0
    fi
  done
  return 0
}

echo "setup Wifi(1: setup_AP; 2:cancel_AP; 3: connect_wifi)"
ret=0
if [ $1 -eq 1 ]; then
  setup_AP
  echo "setup_AP ret = $?"
  start_dhcp
  echo "start_dhcp ret = $?"
elif [ $1 -eq 2 ]; then
  cancel_AP
  ret=$?
  echo "cancel_AP ret = $ret"
elif [ $1 -eq 3 ]; then
  connect_wifi $2 $3 $4
  ret=$?
  echo "connect_wifi: $2, account: $3, password: $4, ret = $ret"
else
  echo "Input is error."
fi
exit $ret
