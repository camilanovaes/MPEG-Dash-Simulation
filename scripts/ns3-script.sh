#/bin/bash

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
        sleep 1
	
	echo "Separando arquivos WiFi"
	cd /home/ns3/Documents/$nome/
	mkdir wifi
	mkdir wifi/Trace
	mkdir wifi/Wire
	mv /home/ns3/Documents/$nome/wifi-dash* /home/ns3/Documents/$nome/wifi/
	mv /home/ns3/Documents/$nome/wifi/*.pcap /home/ns3/Documents/$nome/wifi/Wire/
	mv /home/ns3/Documents/$nome/wifi/*.tr /home/ns3/Documents/$nome/wifi/Trace/
	sleep 1

	echo "Separando arquivos CSMA"
	mkdir csma
	mkdir csma/Trace
	mkdir csma/Wire
	mv /home/ns3/Documents/$nome/csma-dash* /home/ns3/Documents/$nome/csma/
	mv /home/ns3/Documents/$nome/csma/*.pcap /home/ns3/Documents/$nome/csma/Wire/
	mv /home/ns3/Documents/$nome/csma/*.tr /home/ns3/Documents/$nome/csma/Trace/
	sleep 1
done

