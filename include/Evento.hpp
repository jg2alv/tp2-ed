#ifndef EVENTO_HPP
#define EVENTO_HPP

#include <string>
#include <iostream>
#include <ctime>

#include "Paciente.hpp"

struct Evento {
    Evento(std::time_t timestamp, Paciente* paciente);
    Evento();

    bool operator<(Evento& other) const;
    bool operator<=(Evento& other) const;

    std::time_t timestamp;
    Paciente* paciente;
};

#endif