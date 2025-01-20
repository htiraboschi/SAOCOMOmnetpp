/*
 * USS.cc
 *
 *  Created on: 18 sep. 2024
 *      Author: htiraboschi
 */

#include <omnetpp.h>

using namespace omnetpp;

class USS : public cSimpleModule
{
    virtual void handleMessage(cMessage *msg) override;
};


Define_Module(USS);

void USS::handleMessage(cMessage *msg)
{

}
