#include <iostream>
#include "Fila.hpp"

Item::Item() {
    prox = nullptr;
}

Fila::Fila() {
    frente = new Item;
    tras = frente;
    tamanho = 0;
}

Fila::~Fila() {
    Limpa();
    delete frente;
}

void Fila::Enfileira(Paciente* paciente) {
    Item *nova = new Item();
    nova->curr = paciente;
    tras->prox = nova;
    tras = nova;
    tamanho++;
}

Paciente* Fila::Desenfileira() {
    if (tamanho == 0) throw "Fila está vazia!";

    Paciente* aux = frente->prox->curr;
    Item* p = frente;
    frente = frente->prox;

    delete p;
    tamanho--;

    return aux;
}

bool Fila::Vazia() { return tamanho == 0; }

void Fila::Limpa(){
    Item* p = frente->prox;
    while(!Vazia()){
        frente->prox = p->prox;
        delete p;
        p = frente->prox;
        tamanho--;
    }
    tras = frente;
}

void Fila::Imprime() {
    Item* p = frente->prox;
    while(p != nullptr){
        std::cout << p->curr->id << std::endl;
        p = p->prox;
    }
}