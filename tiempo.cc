#include "tiempo.h"
#include <ctime>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <omnetpp.h>

using namespace omnetpp;

static time_t startTime_t;

void setStartTime(int year, int month, int day, int hour, int minutes, int seconds){
    // Define el tiempo de inicio
    std::tm startTime = {seconds, minutes, hour, day, month -1 , year - 1900};
    startTime_t = mktime(&startTime);
}

simtime_t TimeT_to_SimulationTime(time_t fechaHora){
    double diferencia = difftime(fechaHora, startTime_t);
    return simTime() + diferencia;
}

time_t convertirStringATimeT(const std::string& fecha, const std::string& hora) {
    struct tm tm = {0};
    std::string fechaHora = fecha + " " + hora;
    std::istringstream ss(fechaHora);

    // Parsear manualmente
    char discard;
    ss >> tm.tm_year >> discard >> tm.tm_mon >> discard >> tm.tm_mday
       >> tm.tm_hour >> discard >> tm.tm_min >> discard >> tm.tm_sec;

    if (ss.fail()) {
        throw std::runtime_error("Error al parsear la fecha y hora.");
    }

    // Ajustar los valores de tm
    tm.tm_year -= 1900;  // Años desde 1900
    tm.tm_mon -= 1;      // Meses desde enero (0-11)

    // Convertir a time_t
    time_t tiempo = mktime(&tm);
    if (tiempo == -1) {
        throw std::runtime_error("Error al convertir a time_t.");
    }

    return tiempo;
}

// Función para convertir el tiempo de simulación a una fecha y hora real
time_t simulationTimeToDateTime(double simulationTime) {

    // Calcula el tiempo actual sumando el tiempo de simulación
    time_t currentTime_t = startTime_t + static_cast<time_t>(simulationTime);

    return currentTime_t;
}


std::vector<Evento*> procesarEventos(const std::string& nombreArchivo) {
    std::vector<Evento*> eventos;

    std::ifstream archivo(nombreArchivo);
    if (!archivo.is_open()) {
        std::cerr << "Error al abrir el archivo." << std::endl;
        return eventos;
    }

    std::string linea;
    bool primeraLinea = true;

    while (std::getline(archivo, linea)) {
        if (primeraLinea) {
            // Ignorar la primera línea (encabezados)
            primeraLinea = false;
            continue;
        }

        std::istringstream ss(linea);
        std::string str3;
        std::string fecha1, hora1, fecha2, hora2;
        Evento *nuevoEvento = new Evento();

        // Leer el string de 3 caracteres
        ss >> str3;

        // Leer las dos fechas con hora
        ss >> fecha1 >> hora1 >> fecha2 >> hora2;

        nuevoEvento->setCodigo(str3.c_str());
        nuevoEvento->setInicio(convertirStringATimeT(fecha1, hora1));
        nuevoEvento->setFin(convertirStringATimeT(fecha2, hora2));

        eventos.push_back(nuevoEvento);
    }

    archivo.close();
    return eventos;
}
