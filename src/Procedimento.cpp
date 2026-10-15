#include <iostream>
#include <string>
#include <exception>

#include "Procedimento.hpp"

Procedimento::Procedimento(double tempo_medio_execucao, int n) : tempo_medio_execucao(tempo_medio_execucao), unidades_totais(n), unidades_ocupadas(0) {}

void Procedimento::Ocupar() {
    if(unidades_ocupadas == unidades_totais) throw std::runtime_error("Ocupar chamado com unidades cheias");
    unidades_ocupadas++;
}

void Procedimento::Desocupar() {
    if(unidades_ocupadas == 0) throw std::runtime_error("Desocupar chamado com 0 unidades ocupadas");
    unidades_ocupadas--;
}

bool Procedimento::TemUnidadesLivres() { return unidades_ocupadas < unidades_totais; }

double Procedimento::GetTempo() { return tempo_medio_execucao; }