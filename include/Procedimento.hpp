#ifndef PROCEDIMENTO_HPP
#define PROCEDIMENTO_HPP

#include <string>

class Procedimento {
public:
    Procedimento(double, int);
    void Ocupar();
    void Desocupar();
    bool TemUnidadesLivres();
    double GetTempo();

private:
    double tempo_medio_execucao;
    int unidades_totais;
    int unidades_ocupadas;
};

#endif