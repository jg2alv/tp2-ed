#ifndef PACIENTE_HPP
#define PACIENTE_HPP

#include <string>
#include <iostream>
#include <ctime>

struct Paciente {
    int id;
    bool alta_apos_atendimento;
    std::time_t data_internado;
    std::time_t data_alta;
    int grau;
    int mh;
    int tl;
    int ei;
    int im;
    int estado;
    double tempo_sendo_atendido;
    int placeholder;
};

#endif