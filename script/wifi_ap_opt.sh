# opt wifi ap start or stop
#!/bin/sh

wifi_ap_opt=""
wifi_dev="wlan0"
wifi_ap_ssid="Device_AP"
wifi_ap_passwd=""

echo "start para_receive"
if [ "" != "$1" ];then
    wifi_ap_opt=$1
    echo "opt is $wifi_ap_opt"
fi
if [ "" != "$2" ];then 
    wifi_dev=$2
    echo "wifi_dev is $wifi_dev"
fi
if [ "" != "$3" ];then
    wifi_ap_ssid=$3
    echo "wifi_ap_ssid is $wifi_ap_ssid"
fi
if [ "" != "$4" ];then
    wifi_ap_passwd=$4
    echo "wifi_ap_passwd is $wifi_ap_passwd"
fi


wifi_ap_start()
{

# sudo apt-get update
# sudo apt-get install apt-utils hostapd isc-dhcp-server iw udhcpd wpasupplicant

sudo ifconfig $wifi_dev down
# ps -ef | grep hostapd | grep -v grep | awk '{print $1}' | xargs kill -9
# ps -ef | grep wpa_supplicant | grep -v grep | awk '{print $1}' | xargs kill -9
# ps -ef | grep udhcpd | grep -v grep | awk '{print $1}' | xargs kill -9
sudo killall -9 wpa_supplicant
sudo killall -9 hostapd
sudo killall -9 udhcpc
sudo killall -9 udhcpd
sudo killall -9 dhclient

sudo iwconfig $wifi_dev mode master
sudo ifconfig $wifi_dev up


echo "driver=nl80211
ieee80211n=1
hw_mode=g
ssid=$wifi_ap_ssid
channel=6
interface=$wifi_dev" >  hostapd.conf

if [ "" != "$wifi_ap_passwd" ];then
echo "wpa=2
wpa_key_mgmt=WPA-PSK
wpa_passphrase=$wifi_ap_passwd
rsn_pairwise=CCMP TKIP
wpa_pairwise=TKIP CCMP" >>  hostapd.conf
fi

sudo mv hostapd.conf /etc/hostapd/hostapd.conf

# echo "option domain-name 'example.org';
# option domain-name-servers ns1.example.org, ns2.example.org;

# default-lease-time 600;
# max-lease-time 7200;

# ddns-update-style none;

# subnet 192.168.175.0 netmask 255.255.255.0 {
#    range 192.168.175.2 192.168.175.232;
#    option subnet-mask 255.255.255.0;
#    option broadcast-address 192.168.175.255;
#    option routers 192.168.175.1;
#    option domain-name-servers home;
# }" >> dhcpd.conf

# sudo mv dhcpd.conf /etc/dhcp/dhcpd.conf

# echo "
# INTERFACES='"$wifi_dev"'">> isc-dhcp-server

# sudo mv isc-dhcp-server /etc/default/isc-dhcp-server

# sudo /etc/init.d/isc-dhcp-server restart
echo "start 192.168.175.20
end  192.168.175.254
interface $wifi_dev" > udhcpd.conf
sudo mv udhcpd.conf /etc/udhcpd.conf
sudo udhcpd -f /etc/udhcpd.conf &
# sudo ifconfig $wifi_dev 192.168.175.0

sudo ifconfig $wifi_dev down
sudo iwconfig $wifi_dev mode master
sudo ifconfig $wifi_dev up
sudo hostapd -B /etc/hostapd/hostapd.conf
sudo ifconfig $wifi_dev 192.168.175.1

}


wifi_ap_stop()
{


sudo killall -9 udhcpd
sudo killall -9 hostapd
sudo ifconfig $wifi_dev down
# ps -ef | grep hostapd | grep -v grep | awk '{print $1}' | xargs sudo kill -9

echo "stop AP..."
# sudo iwconfig $wifi_dev mode managed
# sudo ifconfig $wifi_dev up
# nmcli radio all on
# sudo service network-manager restart

}



if [ "start" = "$wifi_ap_opt" ];then
    wifi_ap_start
elif [ "stop" = "$wifi_ap_opt" ];then
    wifi_ap_stop
else
    echo "wifi ap opt err!"
fi