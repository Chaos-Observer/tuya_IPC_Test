#!/bin/bash
####!/usr/bin/env bash
# exec bin
# ./output/tuya_ipc_demo -m 2 -p yarklzchdy89vgil -u uuid8525dc9334ab0e72 -a VIz7xVzlJGbFZOGwarjMgRbgVYFV0PvX -r "." -t "TOKEN"
pushd /home/vaitl/IPC_PoC_BS
if [ ! -L "./setup_wifi.sh" ];then
ln -s ./script/setup_wifi.sh ./setup_wifi.sh
fi
if [ ! -L "./wifi_ap_opt.sh" ];then
ln -s ./script/wifi_ap_opt.sh ./wifi_ap_opt.sh
fi
unset DISPLAY
mkdir /mntssd1/tuya
./output/tuya_ipc_demo -m 1 -p yarklzchdy89vgil -u uuid8525dc9334ab0e72 -a VIz7xVzlJGbFZOGwarjMgRbgVYFV0PvX -r "/mntssd1/tuya"
