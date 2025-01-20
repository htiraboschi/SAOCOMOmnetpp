/*
 * GroundNetworkChannel.cc
 *
 *  Created on: 19 sep. 2024
 *      Author: htiraboschi
 */

#include <omnetpp.h>
#include <archivoAdquisicion_m.h>

using namespace omnetpp;


class GroundNetworkChannel : public cDatarateChannel
{
    private:
        simsignal_t usageTimeSignal;

    protected:
        virtual void initialize() override;
        virtual void processPacket(cPacket *pkt, const SendOptions& options, simtime_t t, Result& inoutResult) override;
};

Define_Channel(GroundNetworkChannel);

void GroundNetworkChannel::initialize()
{
    cDatarateChannel::initialize(); //sin esta línea en runtime da error de que el datarate no está definido
    usageTimeSignal = registerSignal("usageTime");
}


void GroundNetworkChannel::processPacket(cPacket *pkt, const SendOptions& options, simtime_t t, Result& inoutResult)
{
    emit(usageTimeSignal, simTime().dbl());
    return cDatarateChannel::processPacket(pkt, options, t, inoutResult);
}
