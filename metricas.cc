/*
 * metricas.cc
 * Recibe las señales de la simulación y genera los archivos en los formatos esperados
 *  Created on: 10 feb 2025
 *      Author: herna
 */
#include "metricas.h"
#include <omnetpp.h>
#include <fstream>
#include <archivoAdquisicion_m.h>
#include <iomanip>  // Para std::setprecision
#include "Evento_m.h"
#include "tiempo.h"

//UsageTimeSignalListener
UsageTimeSignalListener::UsageTimeSignalListener(){
    archivoMetrica.open(ARCHIVO_USO_RED);
    archivoMetrica << std::fixed << std::setprecision(15); //para que no se pierdan los decimales del tiempo al escribir en el CSV
}
UsageTimeSignalListener::~UsageTimeSignalListener(){
    if (archivoMetrica.is_open()) {
        archivoMetrica.close();
    }
}

void UsageTimeSignalListener::receiveSignal(cComponent *source, simsignal_t signalID, double value, cObject *obj) {
    //if (signalID == NNNSignal) {
        ArchivoAdquisicion *archivo = check_and_cast<ArchivoAdquisicion*>(obj);  // Convertir el objeto a cPacket
        //registro que la red está des-ocupada en el horario de inicio del archivo
        archivoMetrica << simTime().str() << ";0;" << archivo->getName()  << endl;
        //registro que la red está ocupada en el horario de inicio del archivo
        archivoMetrica << simTime().str() << ";1;" << archivo->getName()  << endl;
        //registro que la red está ocupada a la hora de finalizar la transmisión
        archivoMetrica << (simTime().dbl()+value) << ";1;" << archivo->getName()  << endl;
        //registro que la red está des-ocupada luego de transferir el archivo
        archivoMetrica << (simTime().dbl()+value) << ";" << "0;;" << endl;
    //}
}

//ContactSignalListener
ContactSignalListener::ContactSignalListener(){
    archivoMetrica.open(ARCHIVO_CONTACTO);
    archivoMetrica << std::fixed << std::setprecision(15); //para que no se pierdan los decimales del tiempo al escribir en el CSV
}
ContactSignalListener::~ContactSignalListener(){
    if (archivoMetrica.is_open()) {
        archivoMetrica.close();
    }
}


void ContactSignalListener::receiveSignal(cComponent *source, simsignal_t signalID, double value, cObject *obj) {
    //if (signalID == packetArrivalSignal) {
        Evento *contacto = check_and_cast<Evento*>(obj);  // Convertir el objeto a Evento
        //registro que el evento estaba desactivado y luego activado al inicio
        archivoMetrica << TimeT_to_SimulationTime(contacto->getInicio()) << ";0;" << contacto->getCodigo()  << endl;
        archivoMetrica << TimeT_to_SimulationTime(contacto->getInicio()) << ";1;" << contacto->getCodigo()  << endl;
        //registro que el evento estaba activado y luego des-activado al fin
        archivoMetrica << TimeT_to_SimulationTime(contacto->getFin()) << ";1;" << contacto->getCodigo()  << endl;
        archivoMetrica << TimeT_to_SimulationTime(contacto->getFin()) << ";0;" << contacto->getCodigo()  << endl;
    //}
}
