/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2015 Universita' degli Studi di Napoli "Federico II"
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation;
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 * Author: Pasquale Imputato <p.imp...@gmail.com>
 * Author: Stefano Avallone <stefano....@unina.it>
 */

#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "ns3/internet-module.h"
#include "ns3/point-to-point-module.h"
#include "ns3/applications-module.h"
#include "ns3/traffic-control-module.h"
#include "ns3/flow-monitor-module.h"

using namespace ns3;

NS_LOG_COMPONENT_DEFINE ("TrafficControlExample");

int
main (int argc, char *argv[])
{
  double simulationTime = 10; //seconds
  std::string transportProt = "Udp";
  std::string socketType;

  CommandLine cmd;
  cmd.Parse (argc, argv);

  if (transportProt.compare ("Tcp") == 0)
    {
      socketType = "ns3::TcpSocketFactory";
    }
  else
    {
      socketType = "ns3::UdpSocketFactory";
    }

  NodeContainer nodes;
  nodes.Create (4);

  PointToPointHelper pointToPoint;
  pointToPoint.SetDeviceAttribute ("DataRate", StringValue ("5Mbps"));
  pointToPoint.SetChannelAttribute ("Delay", StringValue ("2ms"));
  pointToPoint.SetQueue ("ns3::DropTailQueue", "Mode", StringValue ("QUEUE_MODE_PACKETS"), "MaxPackets", UintegerValue (1));

  NetDeviceContainer devices1,devices2,devices3;
  devices1 = pointToPoint.Install (nodes.Get(0),nodes.Get(2));
  devices3 = pointToPoint.Install (nodes.Get(2),nodes.Get(3));
  devices2 = pointToPoint.Install (nodes.Get(1),nodes.Get(2)); 

  InternetStackHelper stack;
  stack.Install (nodes);

  Ipv4AddressHelper address1;
  address1.SetBase ("10.1.1.0", "255.255.255.0");

  Ipv4InterfaceContainer interfaces1 = address1.Assign (devices1);

  Ipv4AddressHelper address2;
  address2.SetBase ("10.1.2.0", "255.255.255.0");

  Ipv4InterfaceContainer interfaces2 = address2.Assign (devices2); 

  Ipv4AddressHelper address3;
  address3.SetBase ("10.1.3.0", "255.255.255.0");

  Ipv4InterfaceContainer interfaces3 = address3.Assign (devices3);

  Ipv4GlobalRoutingHelper::PopulateRoutingTables();

  //UDP Flow
  uint16_t port = 7;
  Address localAddress (InetSocketAddress (Ipv4Address::GetAny (), port));
  PacketSinkHelper packetSinkHelper (socketType, localAddress);
  ApplicationContainer sinkApp = packetSinkHelper.Install (nodes.Get (3));

  sinkApp.Start (Seconds (0.0));
  sinkApp.Stop (Seconds (simulationTime + 0.1));

  uint32_t payloadSize = 1448;

  OnOffHelper onoff (socketType, Ipv4Address::GetAny ());
  onoff.SetAttribute ("OnTime",  StringValue ("ns3::ConstantRandomVariable[Constant=1]"));
  onoff.SetAttribute ("OffTime", StringValue ("ns3::ConstantRandomVariable[Constant=0]"));
  onoff.SetAttribute ("PacketSize", UintegerValue (payloadSize));
  onoff.SetAttribute ("DataRate", StringValue ("50Mbps")); //bit/s
  ApplicationContainer apps;

  AddressValue remoteAddress (InetSocketAddress (interfaces3.GetAddress (1), port));
  onoff.SetAttribute ("Remote", remoteAddress);
  apps.Add (onoff.Install (nodes.Get (1)));
  apps.Start (Seconds (1.0));
  apps.Stop (Seconds (simulationTime + 0.1));

  //TCP FLOW
  uint16_t port_tcp = 9;
  socketType = "ns3::TcpSocketFactory";
  Address localAddress_tcp (InetSocketAddress (Ipv4Address::GetAny (), port_tcp));
  PacketSinkHelper packetSinkHelper_tcp (socketType, localAddress_tcp );
  ApplicationContainer sinkApp_tcp = packetSinkHelper_tcp.Install (nodes.Get (3));

  sinkApp_tcp.Start (Seconds (0.5));
  sinkApp_tcp.Stop (Seconds (simulationTime + 0.1));

  OnOffHelper onoff_tcp (socketType, Ipv4Address::GetAny ());
  onoff_tcp.SetAttribute ("OnTime",  StringValue ("ns3::ConstantRandomVariable[Constant=1]"));
  onoff_tcp.SetAttribute ("OffTime", StringValue ("ns3::ConstantRandomVariable[Constant=0]"));
  onoff_tcp.SetAttribute ("PacketSize", UintegerValue (payloadSize));
  onoff_tcp.SetAttribute ("DataRate", StringValue ("50Mbps")); //bit/s
  ApplicationContainer apps_tcp;

  AddressValue remoteAddress_tcp (InetSocketAddress (interfaces3.GetAddress (1), port_tcp));
  onoff_tcp .SetAttribute ("Remote", remoteAddress_tcp );
  apps_tcp.Add (onoff_tcp.Install (nodes.Get (0)));
  apps_tcp.Start (Seconds (1.5));
  apps_tcp.Stop (Seconds (simulationTime + 0.1));

  FlowMonitorHelper flowmon;
  Ptr<FlowMonitor> monitor = flowmon.InstallAll();

  Simulator::Stop (Seconds (simulationTime + 5));
  Simulator::Run ();

  Ptr<Ipv4FlowClassifier> classifier = DynamicCast<Ipv4FlowClassifier> (flowmon.GetClassifier ());
  std::map<FlowId, FlowMonitor::FlowStats> stats = monitor->GetFlowStats ();
  std::cout << std::endl << "*** Flow monitor statistics ***" << std::endl;
 // std::cout << "  Lost Packets:   " << stats[1].lostPackets << std::endl;
  for(std::map<FlowId, FlowMonitor::FlowStats>::const_iterator iter = stats.begin();iter!=stats.end();++iter)
   {
    Ipv4FlowClassifier::FiveTuple t = classifier->FindFlow(iter->first);
    NS_LOG_UNCOND("Flow ID: " << iter->first << "Src Addr " << t.sourceAddress << "Dest Addr " << t.destinationAddress);
    NS_LOG_UNCOND("Number of packets transmitted: " << iter->second.txPackets);
   }

  Simulator::Destroy ();

  return 0;
}
