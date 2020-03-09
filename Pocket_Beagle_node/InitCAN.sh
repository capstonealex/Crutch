#!/bin/bash
 
sudo config-pin -a p2.2 in+
sudo config-pin -a p2.4 in+
sudo config-pin -a p2.6 in+ 
 
sudo config-pin p1_28 can   # configure pins to CAN
sudo config-pin p1_26 can

sudo ifconfig can0 down
sudo ip link set can0 up type can bitrate 125000 loopback off
sudo ifconfig can0 up
ifconfig
candump can0
