#include <iostream>
#include <ctime>

#include "Evento.hpp"

Evento::Evento(std::time_t timestamp, Paciente* paciente) : timestamp(timestamp), paciente(paciente) {};
Evento::Evento() {}

bool Evento::operator<(Evento& other) const {
    if(timestamp == other.timestamp) return paciente->id < other.paciente->id;
    return timestamp < other.timestamp;
}

bool Evento::operator<=(Evento& other) const {
    if(timestamp == other.timestamp) return paciente->id <= other.paciente->id;
    return timestamp <= other.timestamp;
}