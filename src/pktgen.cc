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

#include "pktgen.h"

Define_Module(pktgen);

pktgen::pktgen() {
    // TODO Auto-generated constructor stub

}

pktgen::~pktgen() {
    // TODO Auto-generated destructor stub
}


void pktgen::initialize()
{
    //cMessage *msg = new cMessage("S2C");
    sourceId=1000;
    packetsSent=0;
    packetsToSend=par("packetsToSend").longValue();
    packetDelay=par("packetDelay");
    packetSendStartTime=par("packetSendStartTime").doubleValue();
    destinationId=getModuleByPath(par("destination").stringValue())->getId();

    cMessage* msg=new cMessage("gen");

    scheduleAt(packetSendStartTime, msg);
}

void pktgen::handleMessage(cMessage *msg)
{
    // The handleMessage() method is called whenever a message arrives
    // at the module. Here, we just send it to the other module, through
    // gate `out'. Because both `tic' and `toc' does the same, the message
    // will bounce between the two.
    if(msg->isSelfMessage())
    {
        tpacket *tpkt=new tpacket("Data");
        tpkt->setSource(sourceId);
        sourceId++;
        tpkt->setType(DATA);
        tpkt->setDestination(destinationId);

        send(tpkt, "port$o");
        packetsSent++;
        if(packetsSent<packetsToSend)
        {
            double chaos=getRNG(0)->doubleRand();
            double nextTime=chaos*10*packetDelay;
            nextTime=packetDelay;
            EV_INFO<<"nextTime="<<nextTime<<endl;
            scheduleAt(simTime()+nextTime,msg->dup());
        }
    }
    delete msg;
}
