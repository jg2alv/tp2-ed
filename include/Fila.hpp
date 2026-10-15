#ifndef FILA_HPP
#define FILA_HPP

#include <string>
#include <iostream>

#include "Paciente.hpp"

class Item {
public:
    Item();

private:
    Paciente* curr;
    Item* prox;

    friend class Fila;
};

class Fila
{
public:
    Fila();
    ~Fila();

    void Enfileira(Paciente*);
    Paciente* Desenfileira();
    bool Vazia();
    void Limpa();
    void Imprime();
    
private:
    Item* frente;
    Item* tras;
    int tamanho;
};


#endif