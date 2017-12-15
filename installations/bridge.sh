#!/bin/bash

#Add bridge to transmit packets
brctl addbr br-left
brctl addbr br-right

#Create tap devices that will be used to get packet from the bridges into its process
tunctl -t tap-left
tunctl -t tap-right

#Set IP addresses of the tap devices and turn them on
ifconfig tap-left 0.0.0.0 promisc up
ifconfig tap-right 0.0.0.0 promisc up

#Add the tap devices to bridges and bring up
brctl addif br-left tap-left
ifconfig br-left up 
brctl addif br-right tap-right
ifconfig br-right up

brctl show
lxc-ls -f
