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

#include "host.h"

Define_Module(host);

host::host() {
    // TODO Auto-generated constructor stub

}

host::~host() {
    // TODO Auto-generated destructor stub
}

void host::initialize()
{
    tpacket *tpkt=new tpacket("Data");
    tpkt->setSource(getId());
    tpkt->setType(DATA);
    tpkt->setDestination(-1);

    send(tpkt, "port$o");
}

void host::handleMessage(cMessage *msg)
{
    delete msg;
}
