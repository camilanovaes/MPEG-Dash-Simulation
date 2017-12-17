// Network topology
//
//
//			            n4                                   n5                     n6          
//			       +------------+                      +------------+         +------------+   
//			       |            |                      |  Left Cont |         | Right Cont |   
//			       |            |                      |   Server   |         |    Client  |   
//			       +------------+    point-to-point    +------------+         +------------+   
//			       | CSMA | P2P | -------------------  | P2P | Wifi |     	  |    Wifi    | 
//			       +-----+------+       10.0.2.0       +------+-----+         +------------+   
//		                  |                                          |                |      
//		                  |                                        ((*))            ((*))    
//		                  |               				                                
//		n0  n1  n2   n3   |                               		    wifi
//		|   |   |    |    |                                               10.0.1.0
//	        ===================                     
//		 CSMA LAN 10.0.3.0          
//
//
//
//
// The CSMA device on node 0 is:  10.0.3.1
// The CSMA device on node 1 is:  10.0.3.2
// The CSMA device on node 2 is:  10.0.3.3
// The CSMA device on node 3 is:  10.0.3.4
// The CSMA device on node 4 is:  10.0.3.5
// The  P2P device on node 4 is:  10.0.2.1
// The  P2P device on node 5 is:  10.0.2.2
// The WIFI device on node 5 is:  10.0.1.1
// The WIFI device on node 6 is:  10.0.1.2



#include <iostream>
#include <fstream> 
#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "ns3/mobility-module.h"
#include "ns3/point-to-point-module.h"
#include "ns3/wifi-module.h"
#include "ns3/internet-module.h"
#include "ns3/csma-module.h"
#include "ns3/applications-module.h"
#include "ns3/ipv4-global-routing-helper.h"
#include "ns3/tap-bridge-module.h"
 
using namespace ns3;
 
NS_LOG_COMPONENT_DEFINE ("mpeg-dash-final-network");
 
int main (int argc, char *argv[]) {
   // We are interacting with the outside, real, world.  This means we have to 
   // interact in real-time and therefore means we have to use the real-time
   // simulator and take the time to calculate checksums.
   GlobalValue::Bind ("SimulatorImplementationType", StringValue ("ns3::RealtimeSimulatorImpl"));
   GlobalValue::Bind ("ChecksumEnabled", BooleanValue (true));
   
   //===== CSMA CONFIG =====
   uint32_t nNodes = 5;
   NodeContainer csmaNodes;
   csmaNodes.Create (5);
   
   CsmaHelper csma;
   csma.SetChannelAttribute ("DataRate", DataRateValue (5000000));
   csma.SetChannelAttribute ("Delay", TimeValue (MilliSeconds (2)));

   NetDeviceContainer csmaDevices = csma.Install (csmaNodes);

   InternetStackHelper stack;
   stack.Install (csmaNodes);

   Ipv4AddressHelper address;
   address.SetBase ("10.0.3.0", "255.255.255.0");
   Ipv4InterfaceContainer csmaInterfaces = address.Assign (csmaDevices);
   //=======================
      
   NodeContainer virtualNodes;
   virtualNodes.Create(2); // virtualNodes.Get(0) --> Left Container | virtualNodes.Get(1) ---> Right Container

   //===== P2P CONFIG ======
   PointToPointHelper p2p;
   p2p.SetDeviceAttribute ("DataRate", StringValue ("512kbps"));
   p2p.SetChannelAttribute ("Delay", StringValue ("10ms"));

   NodeContainer p2pNodes = NodeContainer (csmaNodes.Get(4), virtualNodes.Get(0));
   NetDeviceContainer p2pDevices = p2p.Install (p2pNodes);
    
   stack.Install (virtualNodes);
 
   address.SetBase ("10.0.2.0", "255.255.255.0");
   Ipv4InterfaceContainer p2pInterfaces = address.Assign (p2pDevices);
   //=======================

   //===== WIFI CONFIG =====
   WifiHelper wifi;
   wifi.SetStandard (WIFI_PHY_STANDARD_80211a);
   wifi.SetRemoteStationManager ("ns3::ConstantRateWifiManager", "DataMode", StringValue ("OfdmRate54Mbps")); 

   WifiMacHelper wifiMac;
   wifiMac.SetType ("ns3::AdhocWifiMac");

   YansWifiChannelHelper wifiChannel = YansWifiChannelHelper::Default ();
   YansWifiPhyHelper wifiPhy = YansWifiPhyHelper::Default ();
   wifiPhy.SetChannel (wifiChannel.Create ());

   NetDeviceContainer wifiDevices = wifi.Install (wifiPhy, wifiMac, virtualNodes);

   MobilityHelper mobility;
   Ptr<ListPositionAllocator> positionAlloc = CreateObject<ListPositionAllocator> ();
   positionAlloc->Add (Vector (0.0, 0.0, 0.0));
   positionAlloc->Add (Vector (5.0, 0.0, 0.0));
   mobility.SetPositionAllocator (positionAlloc);
   mobility.SetMobilityModel ("ns3::ConstantPositionMobilityModel");
   mobility.Install (virtualNodes);

   address.SetBase ("10.0.1.0", "255.255.255.0");
   Ipv4InterfaceContainer wifiInterfaces = address.Assign (wifiDevices);
   //============================

   // ====== BRIDGE CONFIG =============
   TapBridgeHelper tapBridge;
   tapBridge.SetAttribute ("Mode", StringValue ("UseLocal"));
   tapBridge.SetAttribute ("DeviceName", StringValue ("tap-left"));
   tapBridge.Install (virtualNodes.Get (0), wifiDevices.Get (0));

   tapBridge.SetAttribute ("DeviceName", StringValue ("tap-right"));
   tapBridge.Install (virtualNodes.Get (1), wifiDevices.Get (1));
   //===================================

   //=== APLICATION FOR CSMA NODES =====
   Ipv4Address Server_Address(wifiInterfaces.GetAddress(0));
   uint16_t Server_port = 4500;

   UdpEchoServerHelper echoServer (Server_port);   //creating the echo server on port 9
   ApplicationContainer serverApps = echoServer.Install (virtualNodes.Get(0));  //install server application on the node 1
   serverApps.Start (Seconds (1.0));
   serverApps.Stop (Seconds (600.0));

   //We are going to create echo client applications, which would send packets to the echo server.
   // Thefore, we need to use random numbers to send packets in random time to bring closer network model to real life.
   RngSeedManager::SetSeed (3);  // Changes seed from default of 1 to 3
   RngSeedManager::SetRun (7);   // Changes run numberfrom default of 1 to 7
   Ptr<UniformRandomVariable> rand = CreateObject<UniformRandomVariable>();
   rand->SetAttribute ("Min", DoubleValue(10));
   rand->SetAttribute ("Max", DoubleValue(100));

   UdpEchoClientHelper echoClient (Server_Address, Server_port); //Echo client application will send packets to node 1 port 4500
   ApplicationContainer clientApps;
   for (uint32_t i = 0; i < nNodes; i++) {
     echoClient.SetAttribute ("MaxPackets", UintegerValue (50000));
     echoClient.SetAttribute ("Interval", TimeValue (MilliSeconds (rand->GetInteger()))); //random interval
     echoClient.SetAttribute ("PacketSize", UintegerValue(1024));
     clientApps = (echoClient.Install (csmaNodes.Get(i)));
   }
   //clientApps = (echoClient.Install (virtualNodes.Get(1)));
   clientApps.Start (Seconds (2.0));
   clientApps.Stop (Seconds (600.0));    
   //===================================

   Ipv4GlobalRoutingHelper::PopulateRoutingTables();

   Time::SetResolution (Time::NS);
   LogComponentEnable ("UdpEchoClientApplication", LOG_LEVEL_INFO);
   LogComponentEnable ("UdpEchoServerApplication", LOG_LEVEL_INFO);

   //Analise do trafego na rede 
   AsciiTraceHelper ascii;
   csma.EnableAsciiAll (ascii.CreateFileStream ("csma-dash.tr"));
   csma.EnablePcapAll ("csma-dash");
   wifiPhy.EnableAsciiAll (ascii.CreateFileStream ("wifi-dash.tr"));
   wifiPhy.EnablePcapAll ("wifi-dash");

   // Run the simulation for ten minutes to give the user time to play around
   Simulator::Stop (Seconds (600.0));
   Simulator::Run ();
   Simulator::Destroy ();
}
