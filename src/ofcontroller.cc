//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
// 
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
// 
// You should have received a copy of the GNU Lesser General Public License
// along with this program.  If not, see http://www.gnu.org/licenses/.
// 

// Author: A S M Asadujjaman (asadujjaman@gmail.com)


#include "ofcontroller.h"

Define_Module(ofcontroller);

ofcontroller::ofcontroller() {
    // TODO Auto-generated constructor stub

}

ofcontroller::~ofcontroller() {
    // TODO Auto-generated destructor stub
}

void ofcontroller::initialize(){
    numPktIn=0;
    hasC=false;

    ofs.open("con.txt");
    ofs<<"Controller Id="<<getId()<<endl;


}

void ofcontroller::finish(){
    EV_INFO<<"Received "<<numPktIn<<" Packet-In messages. Congestion="<<hasC<<endl;
    ofs.close();
}

void ofcontroller::handleMessage(cMessage *msg)
{
    EV_INFO<<"Message arrived. Node:  "<<getName()<<" Message Type: "<<msg->getName()<<std::endl;

    if(msg->isSelfMessage()){
        /*for(std::vector<rtentry>::iterator it=routingTable.begin(); it!=routingTable.end(); it++)
        {
            generatePacketOut(it->destination);
        }
        scheduleAt(simTime()+0.0001, msg);*/
    }
    else{
        if(dynamic_cast<tpacket*>(msg)){
            tpacket * tpkt = (tpacket*) msg;
            EV_INFO<<"Received packet type "<<(tpkt->getType())<<endl;
            if(tpkt->getType()==PACKET_IN){
                numPktIn++;
                addRoute(getId(), tpkt->getSource(), tpkt->getArrivalGate()->getIndex());
                handlePacketIn(tpkt);
            }
            delete msg;
        }
    }
}

void ofcontroller::handlePacketIn(tpacket *tpkt)
{
    //TODO: Write install flow rule
    //generatePacketOut()
}

void ofcontroller::addRoute(int source, int destination, int gateIndex)
{
    struct rtentry rte;
    rte.source=source;
    rte.destination=destination;
    rte.port=gateIndex;
    bool duplicate=false;
    std::vector<rtentry>::iterator it;
    for(it=routingTable.begin(); it!=routingTable.end(); it++)
    {
        if(it->source==source && it->destination==destination)
        {
            duplicate=true;
            break;
        }
    }
    if(duplicate) routingTable.erase(it);
    routingTable.push_back(rte);
}

void ofcontroller::generatePacketOut(int sw, int source, int destination, int routeNextHop)
{
    int nextHop=-1;
    for(std::vector<rtentry>::iterator it=routingTable.begin(); it!=routingTable.end(); it++)
    {
        if(it->destination==sw)
        {
            nextHop=it->port;
            break;
        }
    }

    if(nextHop!=-1){
        cChannel *txChannel = gate("port$o", nextHop)->getTransmissionChannel();
        simtime_t txFinishTime = txChannel->getTransmissionFinishTime();
        if (txFinishTime <= simTime())
        {
            tpacket *pktout=new tpacket();
            pktout->setName("Packet-Out");
            pktout->setType(PACKET_OUT);
            pktout->setSource(getId());
            pktout->setDestination(sw);
            pktout->setRouteSource(source);
            pktout->setRouteDestination(destination);
            pktout->setRouteNextHop(routeNextHop);
            pktout->setResponseTime(simTime());
            pktout->setByteLength(32);

            EV_INFO<<"Sending Packet-Out"<<endl;
            send(pktout, "port$o", nextHop);

        }
    }
}
