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
        simtime_t startTime; //acá registro el horario de inicio de transmisión de un mensaje
    public:
        GroundNetworkChannel();
        virtual ~GroundNetworkChannel() {}
        virtual void handleMessage(cMessage *msg);
    protected:
       // virtual void initialize() override;
        virtual void processPacket(cPacket *pkt, const SendOptions& options, simtime_t t, Result& inoutResult) override;
};

Define_Channel(GroundNetworkChannel);

GroundNetworkChannel::GroundNetworkChannel() {
    usageTimeSignal = registerSignal("usageTime");
}


/*void GroundNetworkChannel::initialize()
{
    cDatarateChannel::initialize(); //sin esta línea en runtime da error de que el datarate no está definido
}*/

void GroundNetworkChannel::processPacket(cPacket *pkt, const SendOptions& options, simtime_t t, Result& inoutResult)
{
    double transmissionDelay = pkt->getBitLength() / this->getDatarate();
    emit(usageTimeSignal, transmissionDelay);
    return cDatarateChannel::processPacket(pkt, options, t, inoutResult);
}

void GroundNetworkChannel::handleMessage(cMessage *msg) {
    if (msg->isSelfMessage()) {
        // Fin de la transferencia
        simtime_t endTime = simTime();
        emit(usageTimeSignal, endTime - startTime);
    }
    else {
        // Inicio de la transferencia
        startTime = simTime();
        // Procesa el mensaje (transfiere el archivo)
        }
}
