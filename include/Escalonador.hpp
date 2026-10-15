#ifndef ESCALONADOR_HPP
#define ESCALONADOR_HPP

#include <string>
#include <iostream>

#include "Evento.hpp"

class Escalonador {
public:
    Escalonador(int);
    void Inserir(Evento);
    Evento Remover();
    bool Vazio();
    ~Escalonador();

private:
    int tamanho;
    int maxsize;
    Evento* eventos;
};

#endif