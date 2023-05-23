#!/bin/bash
####!/usr/bin/env bash

pushd /home/vaitl/IPC_PoC_BS
if [ ! -L "./setup_wifi.sh" ];then
ln -s ./script/setup_wifi.sh ./setup_wifi.sh
fi
if [ ! -L "./wifi_ap_opt.sh" ];then
ln -s ./script/wifi_ap_opt.sh ./wifi_ap_opt.sh
fi
unset DISPLAY
mkdir /mntssd1/tuya

echo "[Unit]
Description=Tuya streaming service
Wants=network.target
After=NetworkManager-wait-online.service

[Service]
Type=simple
User=root
Group=root
ExecStartPre=/bin/sleep 3
ExecStart=/home/vaitl/IPC_PoC_BS/script/exec_bin.sh
ExecStartPost=/bin/sleep 3
StandardOutput=journal
KillMode=mixed
Restart=always
RestartSec=8s

[Install]
WantedBy=multi-user.target
" > /etc/systemd/system/tuya_streaming.service

sudo systemctl daemon-relaod
sudo systemctl enable tuya_streaming.service
