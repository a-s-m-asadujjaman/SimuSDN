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

#ifndef OFCONTROLLER_H_
#define OFCONTROLLER_H_

#include <omnetpp.h>
#include <fstream>
#include "tpacket_m.h"
#include <string>
#include <map>
#include "ofswitch.h"

using namespace omnetpp;
using namespace std;

class ofcontroller : public cSimpleModule{
private:
    struct rtentry
    {
        int source;
        int destination;
        int port;
    };
    std::vector<rtentry> routingTable;
    int numPktIn;
    bool hasC;
    int ia[1+1000000], ja[1+1000000];
    double ar[1+1000000];
    string cn[1+1000000];
    std::map<std::string, int> ci;
    std::map<std::string, bool> cc;
    std::map<std::string, double> cbw;
    std::map<std::string, int> cStartNode;
    std::map<std::string, int> cEndNode;
    std::map<std::string, int> cStartIndex;
    std::map<std::string, int> cEndIndex;
    std::map<std::string, string> cStartNodeName;
    std::map<std::string, string> cEndNodeName;
    ofstream ofs;

public:
    ofcontroller();
    virtual ~ofcontroller();
    virtual void initialize() override;
    virtual void finish() override;
    virtual void handleMessage(cMessage *msg) override;
    void handleCongestion(tpacket *msg);
    void handlePacketIn(tpacket *msg);
    void generatePacketOut(int sw, int src, int dst, int next);
    void addRoute(int source, int destination, int gateIndex);
    //void generatePacketOut(int sw);
};

#endif /* OFCONTROLLER_H_ */
