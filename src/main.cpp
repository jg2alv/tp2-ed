#include <iostream>
#include <fstream>
#include <ctime>
#include <string>
#include <iomanip>

#include "Paciente.hpp"
#include "Fila.hpp"
#include "Procedimento.hpp"
#include "Escalonador.hpp"

// Funcao principal do programa, que lida com
// a leitura do arquivo de entrada e a saida a
// ser impressa
int main(int argc, char** argv) {
    // Abertura do arquivo
    // Estrategia de robustez:
    // lidar com arquivo corrompido
    std::ifstream arquivo;
    arquivo.open(argv[1]);
    if(!arquivo.good()) {
        std::cout << "erro na abertura do arquivo csv" << std::endl;
        arquivo.close();
        return 1;
    }


    // Declaracao das variaveis de prontuario do paciente
    // e leitura dos dados do arquivo
    double tempotriagem, tempoatendimento, tempomh,
            tempotl, tempoei, tempoim;

    int numerotriagem, numeroatendimento, numeromh,
        numerotl, numeroei, numeroim, n;

    arquivo >> tempotriagem >> numerotriagem >> tempoatendimento
            >> numeroatendimento >> tempomh >> numeromh >> tempotl
            >> numerotl >> tempoei >> numeroei >> tempoim >> numeroim
            >> n;

    // Declaracao das variaveis de controle do sistema de simulacao:
    // vetor de pacientes, escalonador, relogio, procedimentos e filas
    Paciente* pacientes = new Paciente[n];
    Escalonador escalonador(n);
    std::time_t relogio;

    Procedimento proc_triagem(tempotriagem, numerotriagem),
                 proc_atendimento(tempoatendimento, numeroatendimento),
                 proc_mh(tempomh, numeromh),
                 proc_tl(tempotl, numerotl),
                 proc_ei(tempoei, numeroei),
                 proc_im(tempoim, numeroim);

    Fila fila_triagem,
         fila_atendimento[3],
         fila_mh[3],
         fila_tl[3],
         fila_ei[3],
         fila_im[3];

    // Declaracao das lambda functions utilizadas para
    // simular o avanco das filas.
    // `proximo` desenfileira um paciente da fila e escalona o
    // evento da completude do exame
    auto proximo = [](Procedimento* proc, Fila* fila, std::time_t relogio, int Paciente::*exames, Escalonador* escalonador) {
        // "Escalonar o evento de execução do serviço"
        Paciente* paciente = fila->Desenfileira();
        Evento evento(relogio + paciente->*exames * proc->GetTempo() * 3600, paciente);

        if(paciente->estado % 2 == 1) throw std::runtime_error("paciente com estado impar na fila");
            
        escalonador->Inserir(evento);
        proc->Ocupar();            
        paciente->estado++;
    };

    // `avancar_fila` lida com o avanco das 3 filas de urgencia
    // de um procedimento
    auto avancar_fila = [proximo](Fila* fila, Procedimento* proc, std::time_t relogio, int Paciente::*exames, Escalonador* escalonador) {
        // "Se há unidade disponível para execução"
        for(int i = 2; i >= 0; i--) {
            while(!fila[i].Vazia() && proc->TemUnidadesLivres()) {
                proximo(proc, &fila[i], relogio, exames, escalonador);
            }
        }
    };

    // Nucleo do codigo
    // Estrategia de robustez: try/catch envelopando
    // toda a logica para lidar de forma correta
    // com possiveis erros
    try {
        for(int i = 0; i < n; i++) {
            // Para cada paciente:
            //  1. declara as variaveis de prontuario
            //      para armazenamento dos dados
            //  2. le as informacoes do paciente do
            //      arquivo de entrada
            //  3. guarda as informacoes no struct paciente
            //      do vetor de pacientes
            //  4. esclona a chegada do paciente ao HZ
            //  5. pre-computa o tempo em atendimento do paciente
            Paciente* p = &pacientes[i];
            bool alta_apos_atendimento;
            int id, ano, mes, dia, hora,
                grau, mh, tl, ei, im;
        
            arquivo >> id >> alta_apos_atendimento >> ano
                    >> mes >> dia >> hora >> grau >> mh
                    >> tl >> ei >> im;

            std::tm tm = { 0, 0, hora, dia, mes-1, ano-1900 };

            p->data_internado = std::mktime(&tm);
            p->id = id;
            p->alta_apos_atendimento = alta_apos_atendimento;
            p->grau = grau;
            p->mh = mh;
            p->tl = tl;
            p->ei = ei;
            p->im = im;
            p->estado = 1;
            p->tempo_sendo_atendido = 0;
            p->placeholder = 1;
            
            Evento evento(p->data_internado, p);
            escalonador.Inserir(evento);

            p->tempo_sendo_atendido = tempotriagem + tempoatendimento;
            if(!p->alta_apos_atendimento)
                p->tempo_sendo_atendido += p->mh * tempomh + p->tl * tempotl + p->ei * tempoei + p->im * tempoim;
        }

        // Loop de simulacao do HZ
        // "Enquanto houver eventos"
        while(!escalonador.Vazio()) {
            // "Remove o próximo evento do escalonador"
            Evento evento = escalonador.Remover();
            relogio = evento.timestamp;

            // "Avança o relógio para o instante do próximo evento"
            Paciente* paciente = evento.paciente;
            int& estado = paciente->estado;
            int grau = paciente->grau;

            if(estado % 2 == 0) throw std::runtime_error("paciente com estado par no escalonador");

            // "Verifica o próximo estado do paciente"
            // "Se houver mais serviços para paciente: enfileira o paciente na fila adequada"
            if(estado == 1) fila_triagem.Enfileira(paciente);
            else if(estado == 3) {
                fila_atendimento[grau].Enfileira(paciente);
                proc_triagem.Desocupar();
            }
            else if(estado == 5) {
                if(paciente->alta_apos_atendimento) paciente->estado = 14;
                else {
                    fila_mh[grau].Enfileira(paciente);
                    // if(paciente->mh != 0) fila_mh[grau].Enfileira(paciente);
                    // else paciente->estado = 7;
                    // Ponto extra: comente 158 e descomente 159,160
                }
                proc_atendimento.Desocupar();
            }
            if(estado == 7) {
                fila_tl[grau].Enfileira(paciente);
                proc_mh.Desocupar();
                // if(paciente->tl != 0) fila_tl[grau].Enfileira(paciente);
                // else paciente->estado = 9;
                // if(paciente->mh != 0) proc_mh.Desocupar();
                // Ponto extra: comente 166,167 e descomente 168-170
            }
            if(estado == 9) {
                fila_ei[grau].Enfileira(paciente);
                proc_tl.Desocupar();
                // if(paciente->ei != 0) fila_ei[grau].Enfileira(paciente);
                // else paciente->estado = 11;
                // if(paciente->tl != 0) proc_tl.Desocupar();
                // Ponto extra: comente 174,175 e descomente 176-178
            }
            if(estado == 11) {
                fila_im[grau].Enfileira(paciente);
                proc_ei.Desocupar();
                // if(paciente->im != 0) fila_im[grau].Enfileira(paciente);
                // else paciente->estado = 13;
                // if(paciente->ei != 0) proc_ei.Desocupar();
                // Ponto extra: comente 182,183 e descomente 184-186
            }
            if(estado == 13) proc_im.Desocupar();
            // if(estado == 13 && paciente->im != 0) proc_im.Desocupar();
            // Ponto extra: comente 189 e descomente 190

            if(estado % 2 == 1) paciente->estado++;
            
            // "Senão: finaliza o atendimento (Alta Hospitalar)"
            // "Atualizar as estatísticas"
            if(estado == 14) paciente->data_alta = relogio;

            // "Para cada fila de espera que tenha pacientes"
            // (Note que como a fila da triagem nao segue ordem de prioridade
            // como as demais, nao foi utilizado a lambda `avancar_fila`.
            // Dessa forma, chama-se diretamente a funcao `proximo`).
            while(!fila_triagem.Vazia() && proc_triagem.TemUnidadesLivres()) {
                proximo(&proc_triagem, &fila_triagem, relogio, &Paciente::placeholder, &escalonador);
            }

            avancar_fila(fila_atendimento, &proc_atendimento, relogio, &Paciente::placeholder, &escalonador);
            avancar_fila(fila_mh, &proc_mh, relogio, &Paciente::mh, &escalonador);
            avancar_fila(fila_tl, &proc_tl, relogio, &Paciente::tl, &escalonador);
            avancar_fila(fila_ei, &proc_ei, relogio, &Paciente::ei, &escalonador);
            avancar_fila(fila_im, &proc_im, relogio, &Paciente::im, &escalonador);
        }

        // "Gerar relatórios de estatísticas"
        for(int i = 0; i < n; i++) {
            Paciente p = pacientes[i];
            if(p.estado != 14) throw std::runtime_error("paciente com estado != 14");

            double totalhoras = (p.data_alta - p.data_internado) / 3600.0;
            double filahoras = std::abs(totalhoras - p.tempo_sendo_atendido);

            std::string ctime_entrada = std::ctime(&p.data_internado),
                        ctime_saida = std::ctime(&p.data_alta);

            ctime_entrada = ctime_entrada.erase(ctime_entrada.length() - 1);
            ctime_saida = ctime_saida.erase(ctime_saida.length() - 1);

            std::cout << p.id << " "
                      << ctime_entrada << " "
                      << ctime_saida << " "
                      << std::fixed << std::setprecision(2) << std::setw(5) << totalhoras << " "
                      << std::fixed << std::setprecision(2) << std::setw(5) << p.tempo_sendo_atendido << " "
                      << std::fixed << std::setprecision(2) << std::setw(5) << filahoras << std::endl;
        }

        delete[] pacientes;
        arquivo.close();
    
    // Estrategia de robustez:
    // lidar com erros nao previstos
    } catch(std::exception& exc) {
        std::cout << exc.what() << std::endl;
        delete[] pacientes;
        arquivo.close();
        return 1;
    }
}