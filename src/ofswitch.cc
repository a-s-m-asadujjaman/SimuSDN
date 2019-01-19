/*
 * ofswitch.cc
 *
 *  Created on: Jan 10, 2019
 *      Author: root
 */

// Author: A S M Asadujjaman (asadujjaman@gmail.com)


#include "ofswitch.h"


Define_Module(ofswitch);

ofswitch::ofswitch() {
    // TODO Auto-generated constructor stub

}

ofswitch::~ofswitch() {
    // TODO Auto-generated destructor stub
}

void ofswitch::initialize(){
    EV_INFO<<"id="<<getId()<<endl;
    dpid = par("dpid");
    isCCC = false;
    controllerId=getModuleByPath("controller")->getId();
    autoposition=par("autoposition");
    uni=-1;

    if(autoposition)
    {
        int level=ceil(log2(getIndex()+2));
        int count=pow(2,level-1);
        int levelSl=getIndex()-count+2;

        x=levelSl*100;
        y=level*100;

        EV_INFO<<"index="<<getIndex()<<" level="<<level<<" count="<<count<<" levelSl="<<levelSl<<" x="<<x<<" y="<<y<<endl;

        char buf[32];
        snprintf(buf, sizeof(buf), "%lf", x);
        buf[sizeof(buf) - 1] = 0;
        getDisplayString().setTagArg("p", 0, buf);

        snprintf(buf, sizeof(buf), "%lf", y);
        buf[sizeof(buf) - 1] = 0;
        getDisplayString().setTagArg("p", 1, buf);
    }

}

void ofswitch::handleMessage(cMessage *msg)
{
    EV_INFO<<"Message arrived. Node:  "<<getName()<<" Message Type: "<<msg->getName()<<std::endl;

    if(msg->isSelfMessage()){

    }
    else{
        if(dynamic_cast<tpacket*>(msg)){
            tpacket * tpkt = (tpacket*) msg;
            EV_INFO<<"Received packet type "<<(tpkt->getType())<<endl;
            if(tpkt->getDestination()==-1)
            {
                uni=tpkt->getArrivalGate()->getIndex();
                delete msg;
                return;
            }
            if(tpkt->getDestination()==getId())
            {
                if(tpkt->getType()==DATA)
                {
                    if(uni>=0)
                    {
                        send(msg, "port$o", uni);
                        return;
                    }
                }
                else if(tpkt->getType()==PACKET_OUT)
                {
                    addRoute(tpkt->getRouteSource(), tpkt->getRouteDestination(), tpkt->getRouteNextHop());
                }

                delete msg;
                return;
            }
            int nextHop=-1, packetInNextHop=0;
            for(std::vector<rtentry>::iterator it=routingTable.begin(); it!=routingTable.end(); it++)
            {
                if(it->source==tpkt->getSource() && it->destination==tpkt->getDestination())
                {
                    nextHop=it->port;
                    break;
                }
                else if(it->source==getId() && it->destination==controllerId)
                {
                    packetInNextHop=it->port;
                }
            }
            if(nextHop==-1){
                EV_INFO<<"Generating packet in"<<std::endl;
                generatePacketIn(tpkt, packetInNextHop);
            }
            else
            {
                send(msg, "port$o", nextHop);
            }
        }
    }
}

void ofswitch::generatePacketIn(tpacket *tpkt, int port)
{
    cChannel *txChannel = gate("port$o", port)->getTransmissionChannel();
    simtime_t txFinishTime = txChannel->getTransmissionFinishTime();
    if (txFinishTime <= simTime())
    {
        tpacket *pktin=tpkt->dup();
        pktin->setName("Packet-In");
        pktin->setType(PACKET_IN);
        pktin->setSource(getId());
        pktin->setRequestTime(simTime());
        pktin->setResponseTime(0);
        pktin->setDestination(controllerId);
        pktin->setRouteSource(tpkt->getSource());
        pktin->setRouteDestination(tpkt->getDestination());
        pktin->setByteLength(32);
        pktin->setC(isCCC);
        if(port!=0) pktin->setC(false);
        EV_INFO<<"Sending Packet-In"<<endl;
        send(pktin, "port$o", port);
        isCCC=false;
    }
    else{
        EV_INFO<<"txFinishTime="<<txFinishTime<<endl;
        isCCC=true;
    }

    delete tpkt;
}

void ofswitch::addPacketInRoute(int gateIndex)//Next Hop for Packet-In message
{
    struct rtentry rte;
    rte.source=getId();
    rte.destination=controllerId;
    rte.port=gateIndex;
    bool duplicate=false;
    std::vector<rtentry>::iterator it;
    for(it=routingTable.begin(); it!=routingTable.end(); it++)
    {
        if(it->source==rte.source && it->destination==rte.destination)
        {
            duplicate=true;
            break;
        }
    }
    if(duplicate) routingTable.erase(it);
    routingTable.push_back(rte);
}

void ofswitch::addRoute(int source, int destination, int gateIndex)
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

