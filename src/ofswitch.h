/*
 * ofswitch.h
 *
 *  Created on: Jan 10, 2019
 *      Author: root
 */

// Author: A S M Asadujjaman (asadujjaman@gmail.com)

#ifndef OFSWITCH_H_
#define OFSWITCH_H_

#include <omnetpp.h>
#include <vector>
#include "tpacket_m.h"

using namespace omnetpp;

class ofswitch : public cSimpleModule{
private:
    struct rtentry
    {
        int source;
        int destination;
        int port;
    };
    int maxrtsize;
    double x;
    double y;
    int uni;
public:
    std::vector<rtentry> routingTable;
    int dpid;
    bool isCCC;
    int controllerId;
    bool autoposition;

    ofswitch();
    virtual ~ofswitch();
    virtual void initialize() override;
    virtual void handleMessage(cMessage *msg) override;
    void generatePacketIn(tpacket *tpkt, int port=0);
    void addPacketInRoute(int gateIndex);
    void addRoute(int source, int destination, int gateIndex);
};

#endif /* OFSWITCH_H_ */
