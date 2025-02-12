/*
 * GroundStation.cc
 *
 *  Created on: 18 sep. 2024
 *      Author: htiraboschi
 */
#include <string.h>
#include <omnetpp.h>
#include <archivoAdquisicion_m.h>
#include "Evento_m.h"
#include "tiempo.h"
#include "metricas.h"
using namespace omnetpp;


class GroundStation : public cSimpleModule
{
private:
    cPacketQueue *colaAdquisiciones;
    simsignal_t contactSignal;
    void encolarPasada(Evento *evento);
    ContactSignalListener *contactListener;
protected:
    // The following redefined virtual function holds the algorithm.
    virtual void initialize() override;
    virtual void handleMessage(cMessage *msg) override;

};

Define_Module(GroundStation);

void GroundStation::initialize()
{
    contactSignal = registerSignal("contact");
    contactListener = new ContactSignalListener();
    subscribe(contactSignal, contactListener);
    std::vector<Evento*> contactos;
    contactos = procesarEventos("events.txt");
    for (Evento *contacto: contactos){
        contacto->setName("encolarPasada");
        scheduleAt(TimeT_to_SimulationTime(contacto->getInicio()), contacto);
    }
};

void GroundStation::handleMessage(cMessage *msg)
{
    if(strcmp(msg->getName(),"encolarPasada") == 0)
    {
        emit(contactSignal,0.0,msg);
        encolarPasada(dynamic_cast<Evento*>(msg));
    }
    else if(strcmp(msg->getName(),"nuevoArchivo") == 0) {
        if (this->gate("out")->getTransmissionChannel()->isBusy())
        {
            scheduleAt(this->gate("out")->getTransmissionChannel()->getTransmissionFinishTime(),new cMessage("nuevoArchivo"));
        }
        else
        {
            if (!colaAdquisiciones->isEmpty())
            {
                send(colaAdquisiciones->pop(), "out");
                simtime_t tiempoSiguienteAdq = this->gate("out")->getTransmissionChannel()->getTransmissionFinishTime();
                scheduleAt(tiempoSiguienteAdq,new cMessage("nuevoArchivo"));
            }
        }
    }
    else {
        std::cout << "Cadena no reconocida" << std::endl;
    }
}

void GroundStation::encolarPasada(Evento *evento)
{
    colaAdquisiciones = new cPacketQueue();
    ArchivoAdquisicion *archAdquisicion;

    for(int i = 1;i <=10;i++){
        archAdquisicion = new ArchivoAdquisicion();
        archAdquisicion->setName(("archivo " + std::to_string(i)).c_str());
        switch (i){
            case 3:
                archAdquisicion->setByteLength(100000000);
                break;
            default:
                archAdquisicion->setByteLength(15000000);
                break;
        }
        colaAdquisiciones->insert(archAdquisicion);
    }
    simtime_t t0 = TimeT_to_SimulationTime(evento->getFin());
    scheduleAt(t0, new cMessage("nuevoArchivo"));
}
