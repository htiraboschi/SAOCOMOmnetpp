/*
 * GroundStation.cc
 *
 *  Created on: 18 sep. 2024
 *      Author: htiraboschi
 */
#include <string.h>
#include <omnetpp.h>
#include <archivoAdquisicion_m.h>

using namespace omnetpp;


class GroundStation : public cSimpleModule
{
    private:
        cPacketQueue *colaAdquisiciones;
        void encolarPasada();
    protected:
        // The following redefined virtual function holds the algorithm.
        virtual void initialize() override;
        virtual void handleMessage(cMessage *msg) override;

};

Define_Module(GroundStation);

void GroundStation::initialize()
{
    scheduleAt(simTime(), new cMessage("encolarPasada"));
};

void GroundStation::handleMessage(cMessage *msg)
{
    if(strcmp(msg->getName(),"encolarPasada") == 0)
    {
        encolarPasada();
        scheduleAfter(SimTime::parse("12h"), new cMessage("encolarPasada"));
    }
    else
    {//mensaje por nuevo archivo a transmitir "nuevoArchivo"
        if (this->gate("out")->getTransmissionChannel()->isBusy())
        {
            scheduleAt(this->gate("out")->getTransmissionChannel()->getTransmissionFinishTime(),new cMessage(""));
        }
        else
        {
            send(colaAdquisiciones->pop(), "out");
            if (!colaAdquisiciones->isEmpty())
            {
                simtime_t tiempoSiguienteAdq = this->gate("out")->getTransmissionChannel()->getTransmissionFinishTime();
                scheduleAt(tiempoSiguienteAdq,new cMessage(""));
            }
        }
    }
}

void GroundStation::encolarPasada()
{
    colaAdquisiciones = new cPacketQueue();
    ArchivoAdquisicion *archAdquisicion;

    for(int i = 1;i <=10;i++){
        archAdquisicion = new ArchivoAdquisicion();
        archAdquisicion->setName(("archivo " + std::to_string(i)).c_str());
        switch (i){
            case 4:
                archAdquisicion->setByteLength(1000000);
                break;
            default:
                archAdquisicion->setByteLength(150000);
                break;
        }
        colaAdquisiciones->insert(archAdquisicion);
    }
    simtime_t t0 = simTime();
    scheduleAt(t0, new cMessage("nuevoArchivo"));
}
