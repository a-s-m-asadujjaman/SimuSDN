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

// Created: 2019-0-9 for project SDNCongestion

package sdncongestion.simulations;

import ned.DelayChannel;
import sdncongestion.ofswitch;
import sdncongestion.ofcontroller;
import sdncongestion.pktgen;
import sdncongestion.host;

module Node
{
    parameters:
        @display("i=misc/node_vs");
    gates:
        inout parent;
        inout left;
        inout right;
        inout ofchannel[];
    connections allowunconnected:
}

//
// A generated network with binary tree topology.
//
network Demo
{
    @display("bgb=443,279");

    types:
        channel C extends ned.DatarateChannel
        {
            datarate = 1Mbps;
        }
        channel D extends ned.DatarateChannel
        {
            datarate = 100Mbps;
        }
    submodules:

        pktgen1: pktgen {
            @display("p=30,93");
        }

        controller: ofcontroller {

            @display("p=210,140");
        }

        host1: host {
            @display("p=397,178");
        }


        s2: ofswitch {

            @display("p=109,223");
        }
        s3: ofswitch {
            @display("p=329,215");
        }
        s1: ofswitch {


            @display("p=102,61");
        }

        s4: ofswitch {


            @display("p=304,61");
        }

    connections allowunconnected:

        s2.port++ <--> {  datarate = 1Mbps; } <--> controller.port++;
        s3.port++ <--> {  datarate = 10Mbps; } <--> controller.port++;
        s1.port++ <--> C <--> controller.port++;//Temp
        s4.port++ <--> C <--> controller.port++;

        s3.port++ <--> D <--> s2.port++;
        s1.port++ <--> D <--> s2.port++;

        s3.port++ <--> D <--> s4.port++;
        s4.port++ <--> D <--> s1.port++;

        pktgen1.port <--> D <--> s1.port++;
        host1.port <--> D <--> s3.port++;

}
