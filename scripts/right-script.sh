#!/bin/bash
while true;
do

	cd /home/ubuntu/scootplayer
	echo Ajustando IP !!!
	$(ifconfig eth0 10.0.1.2 netmask 255.255.255.0)
	n=false
	x=0
	ping 10.0.1.1 | while read line; do echo $line | grep "Destination Host Unreachable" && n=true && echo Sem rede && break; done
	ping 10.0.1.1 | while read line; do echo $line | grep "time" && echo Iniciando Scootplayer && $(python scootplayer.py -m http://10.0.1.1/dash/bunny/BigBuckBunny_4s_isoffmain_DIS_23009_1_v_2_1c2_2011_08_30.mpd) && nome=$(($x + 1)) && $(mv /home/ubuntu/scootplayer/out/* /home/ubuntu/$nome) && sleep 390 && break; done
done

