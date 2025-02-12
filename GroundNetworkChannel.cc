/*
 * GroundNetworkChannel.cc
 *
 *  Created on: 19 sep. 2024
 *      Author: htiraboschi
 */

#include <omnetpp.h>
#include <archivoAdquisicion_m.h>
#include "tiempo.h"
#include "metricas.h"

using namespace omnetpp;


class GroundNetworkChannel : public cDatarateChannel
{
    private:
        simsignal_t usageTimeSignal;
        simtime_t startTime; //acá registro el horario de inicio de transmisión de un mensaje
        UsageTimeSignalListener *usageTimeSignalListener;
    public:
        GroundNetworkChannel();
        virtual ~GroundNetworkChannel() {}
    protected:
        virtual void initialize() override;
        virtual void processPacket(cPacket *pkt, const SendOptions& options, simtime_t t, Result& inoutResult) override;
};

Define_Channel(GroundNetworkChannel);

GroundNetworkChannel::GroundNetworkChannel() {
    usageTimeSignal = registerSignal("usageTime");
}


void GroundNetworkChannel::initialize()
{
    cDatarateChannel::initialize(); //sin esta línea en runtime da error de que el datarate no está definido
    setStartTime(par("sim_start_time_year").intValue(), par("sim_start_time_month").intValue(), par("sim_start_time_day").intValue(), 0, 0, 0);

    usageTimeSignalListener = new UsageTimeSignalListener();
    subscribe(usageTimeSignal, usageTimeSignalListener);
}

void GroundNetworkChannel::processPacket(cPacket *pkt, const SendOptions& options, simtime_t t, Result& inoutResult)
{
    double transmissionDelay = pkt->getBitLength() / this->getDatarate();
    emit(usageTimeSignal, transmissionDelay, pkt);
    return cDatarateChannel::processPacket(pkt, options, t, inoutResult);
}

