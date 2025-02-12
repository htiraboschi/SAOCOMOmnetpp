/*
 * metricas.h
 *
 *  Created on: 10 feb 2025
 *      Author: herna
 */

#ifndef METRICAS_H_
#define METRICAS_H_

#include <omnetpp.h>
#include <fstream>

using namespace omnetpp;

class UsageTimeSignalListener : public cListener {
private:
    std::ofstream archivoMetrica;
    static const char* ARCHIVO_USO_RED;
public:
    UsageTimeSignalListener();
    ~UsageTimeSignalListener();
    // Sobrescribir el método receiveSignal
    virtual void receiveSignal(cComponent *source, simsignal_t signalID, double value, cObject *obj) override;
};

const char* UsageTimeSignalListener::ARCHIVO_USO_RED = "metrica_uso_red.csv";

class ContactSignalListener : public cListener {
private:
    std::ofstream archivoMetrica;
    static const char* ARCHIVO_CONTACTO;
public:
    ContactSignalListener();
    ~ContactSignalListener();
    virtual void receiveSignal(cComponent *source, simsignal_t signalID, double value, cObject *obj) override;
};

const char* ContactSignalListener::ARCHIVO_CONTACTO = "metrica_contacto.csv";

#endif /* METRICAS_H_ */
