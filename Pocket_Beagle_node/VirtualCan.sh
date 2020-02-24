#!/bin/bash

#initialisation of Virtual CAN interface and CANopen nodes for 
#testing of Node program.
# Set up virtual can link
sudo modprobe vcan
sudo ip link add dev vcan0 type vcan
sudo ip link set up vcan0

