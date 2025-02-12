/*
 * tiempo.h
 *
 *  Created on: 6 feb 2025
 *      Author: Hernan Tiraboschi
 */

#ifndef TIEMPO_H_
#define TIEMPO_H_


#include <time.h>  // Para time_t
#include <string>
#include <vector>
#include <omnetpp.h>
#include "Evento_m.h"

using namespace omnetpp;

time_t convertirStringATimeT(const std::string& fecha, const std::string& hora);

// Declaración de la función simulationTimeToDateTime
time_t simulationTimeToDateTime(double simulationTime);

omnetpp::simtime_t TimeT_to_SimulationTime(time_t fechaHora);

// Declaración de la función procesarEventos
std::vector<Evento*> procesarEventos(const std::string& nombreArchivo);

//define la fecha hora real de inicio de la simulación
void setStartTime(int year, int month, int day, int hour, int minutes, int seconds);


#endif /* TIEMPO_H_ */
