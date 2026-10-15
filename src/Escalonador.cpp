#include <iostream>
#include <exception>

#include "Escalonador.hpp"

Escalonador::Escalonador(int maxsize) {
    tamanho = 0;
    this->maxsize = maxsize;
    eventos = new Evento[maxsize];
}

Escalonador::~Escalonador() {
    delete[] eventos;
}

void Escalonador::Inserir(Evento evento) {
    if(tamanho >= maxsize) throw std::runtime_error("escalonador cheio");

    eventos[tamanho] = evento;
    int i = tamanho;
    tamanho++;
    
    while (i > 0 && eventos[i] < eventos[(i - 1) / 2]) {
        Evento temp = eventos[i];
        eventos[i] = eventos[(i - 1) / 2];
        eventos[(i - 1) / 2] = temp;
        i = (i - 1) / 2;
    }
}

Evento Escalonador::Remover() {
    Evento x = eventos[0];
    int i = 0;
    eventos[0] = eventos[tamanho - 1];
    tamanho--;

    while (2 * i + 1 < tamanho) {
        int filhoEsq = 2 * i + 1;
        int filhoDir = 2 * i + 2;
        int menorFilho = filhoEsq;
        
        if (filhoDir < tamanho && eventos[filhoDir] < eventos[filhoEsq]) {
            menorFilho = filhoDir;
        }

        if (eventos[i] <= eventos[menorFilho]) break;
        
        Evento temp = eventos[i];
        eventos[i] = eventos[menorFilho];
        eventos[menorFilho] = temp;
        
        i = menorFilho;
    }

    return x;
}

bool Escalonador::Vazio() { return tamanho == 0; }