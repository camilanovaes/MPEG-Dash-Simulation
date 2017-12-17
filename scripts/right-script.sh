#!/bin/bash

cd /home/ubuntu/scootplayer 
while true;
do
	n=false
	ping 10.0.1.1 | while read line; do echo $line | grep "Destination Host Unreachable" && n=true && echo Sem rede && break; done
	ping 10.0.1.1 | while read line; do echo $line | grep "time" && echo Com rede && $(python scootplayer.py -m http://10.0.0.1/dash/bunny/BigBuckBunny_4s_isoffmain_DIS_23009_1_v_2_1c2_2011_08_30.mpd) && break; done
done

