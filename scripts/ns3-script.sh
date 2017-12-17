#!/bin/bash

n=0

while true
do
	n=$(($n + 1))
	cd /home/ns3/Documents/ns-allinone-3.26/ns-3.26/
	$(./waf --run mpeg-dash-network)
	sleep 1
	echo Simulação finalizada
        cd /home/ns3/Documents/
	nome="simu_$n" 
	mkdir $nome
        mv /home/ns3/Documents/ns-allinone-3.26/ns-3.26/csma-dash* /home/ns3/Documents/$nome/	
        mv /home/ns3/Documents/ns-allinone-3.26/ns-3.26/wifi-dash* /home/ns3/Documents/$nome/
        sleep 60
done

