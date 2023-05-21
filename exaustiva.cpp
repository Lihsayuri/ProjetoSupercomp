#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm> 
#include <random>
#include <chrono>
#include <stdlib.h> 
#include <random>
#include <chrono>
#include <fstream>
#include <bitset>
#include <stack>
#include <utility>
#include <map>
#include <ctime>
#include <omp.h>
using std::vector;
using std::cin;
using std::cout;
using std::endl;
using std::bitset;
using std::map;
using std::stack;
using std::pair;
using std::make_pair;

struct Filme{
    int inicio;
    int fim;
    int categoria;
};

struct FilmeProcessado{
    int categoria;
    bitset<24> horario;
};

struct StructSchedule{
    vector<int> filmes;
    int qtd_filmes;
};

void ordena_final(vector<Filme> &vetor_filmes){
    std::sort(vetor_filmes.begin(), vetor_filmes.end(), [] (Filme &a, Filme &b){
		return a.fim < b.fim;
	});

}

void ordena_inicio(vector<Filme> &vetor_filmes){
    // Se e somente se o vetor tiver dois horários finais iguais, o que vem primeiro vai ser o vetor com o horśrio inicial menor.
    for (int i = 0; i < int(vetor_filmes.size()); i++){
        if (vetor_filmes[i].fim == vetor_filmes[i+1].fim){
            if (vetor_filmes[i].inicio > vetor_filmes[i+1].inicio){
                Filme aux = vetor_filmes[i];
                vetor_filmes[i] = vetor_filmes[i+1];
                vetor_filmes[i+1] = aux;
            }
        }
    }
}

void preenche_bitset(bitset<24> &horarios_disponiveis, int inicio, int fim){
    for (int i = 0; i < 24; i++){
        if (i >= inicio && i < fim){
            horarios_disponiveis.set(i);
        }
        else if (inicio > fim && (i >= inicio || i < fim)){// podemos assistir um filme das 23 as 1. Dessa forma, passamos assistindo às 23h e 24h inteiras. Mas não a 1h. 
            horarios_disponiveis.set(i);
        }
        else if(inicio == fim){
            horarios_disponiveis.set(inicio);
        }
    }
}


void busca_exaustiva(int n, vector<FilmeProcessado> &vetor_filmes, vector<int> filmes_por_categoria, vector<bitset<64>> &vetor_schedules){
    long int todas_combinacoes = pow(2, n) ;
    cout << todas_combinacoes << endl;
    long int i; 
    #pragma omp parallel for 
    for (i = 0; i < todas_combinacoes; i++){
        int num_films = 0;
        vector<int> filmes_por_categoria_aux = filmes_por_categoria;
        bitset<64> filmes(i);
        bitset<24> horarios_disponiveis(0x000000);
        for (int j = 0; j < n; j++){
            if (filmes[j] == 1){
                bitset<24> horario_analisado = horarios_disponiveis & vetor_filmes[j].horario;
                if ((horario_analisado != 0) || !(filmes_por_categoria_aux[vetor_filmes[j].categoria-1] > 0)){
                    break;
                }
                filmes_por_categoria_aux[vetor_filmes[j].categoria-1]--;
                horarios_disponiveis |= vetor_filmes[j].horario;
            }
            #pragma omp critical
            if (j == n-1){
                vetor_schedules.push_back(filmes);
            }
        }
    }
}


int main(){
    int qtd_filmes, qtd_categorias;
    cin >> qtd_filmes >> qtd_categorias;

    vector<int> filmes_por_categoria(qtd_categorias, 0);
    Filme filme_vazio = {0, 0, 0};
    vector<Filme> vetor_filmes (qtd_filmes, filme_vazio);
    FilmeProcessado filme_processado_vazio = {0, 0x000000};
    vector<FilmeProcessado> vetor_filmes_processado (qtd_filmes, filme_processado_vazio);

    vector<bitset<64>> vetor_schedules;

    for (int i = 0; i < qtd_categorias; i++){
        cin >> filmes_por_categoria[i];
    }

    for (int i = 0; i < qtd_filmes; i++){
        Filme filme;
        cin >> filme.inicio >> filme.fim >> filme.categoria;
        if (filme.inicio == 0) filme.inicio = 24;
        if (filme.fim == 0) filme.fim = 24;
        if (filme.inicio < 0 || filme.fim < 0) continue;

        vetor_filmes[i] = filme;
    }

    for (int i = 0; i < qtd_filmes; i++){
        FilmeProcessado filme_processado;
        filme_processado.categoria = vetor_filmes[i].categoria;
        preenche_bitset(filme_processado.horario, vetor_filmes[i].inicio-1, vetor_filmes[i].fim-1);
        vetor_filmes_processado[i] = filme_processado;
    }

    //busca_exaustiva(qtd_filmes, vetor_filmes_processado, filmes_por_categoria, vetor_schedules);
    long int todas_combinacoes = pow(2, qtd_filmes) ;
    cout << todas_combinacoes << endl;
    long int i;
    #pragma omp parallel 
    { 
        vector<bitset<64>> vetor_schedules_privado;
        #pragma omp parallel for private(vetor_schedules) 
        for (i = 0; i < todas_combinacoes; i++){
            vector<int> filmes_por_categoria_aux = filmes_por_categoria;
            bitset<64> filmes(i);
            bitset<24> horarios_disponiveis;
            for (int j = 0; j < qtd_filmes; j++){
                if (filmes[j] == 1){
                    bitset<24> horario_analisado = horarios_disponiveis & vetor_filmes_processado[j].horario;
                    if ((horario_analisado != 0)) break;
                    if ((filmes_por_categoria_aux[vetor_filmes_processado[j].categoria-1] == 0)) break;
                    filmes_por_categoria_aux[vetor_filmes_processado[j].categoria-1]--;
                    horarios_disponiveis |= vetor_filmes_processado[j].horario;
                }
                if (j == qtd_filmes-1){
                    vetor_schedules_privado.push_back(filmes);
                }
            }
        }
        #pragma omp critical
        vetor_schedules.insert(vetor_schedules.end(), vetor_schedules_privado.begin(), vetor_schedules_privado.end());
    }


    int max = 0;
    int indice_melhor_schedule = 0;
    int qtd_schedules = vetor_schedules.size();
    vector<StructSchedule> vetor_struct_schedules(qtd_schedules); 
    //#pragma omp parallel for
    for (int i = 0; i < qtd_schedules; i++){
        StructSchedule schedule;
        schedule.qtd_filmes = 0;
        for (int j = 0; j < 64; j++){
            if (vetor_schedules[i][j] == 1) {
                schedule.filmes.push_back(j);
                schedule.qtd_filmes++;
            } 
        }
        vetor_struct_schedules[i] = schedule;
    }

    for (int i = 0; i < qtd_schedules; i++){
        if (vetor_struct_schedules[i].qtd_filmes > max){
            max = vetor_struct_schedules[i].qtd_filmes;
            indice_melhor_schedule = i;
        }
    }

    StructSchedule melhores_filmes = vetor_struct_schedules[indice_melhor_schedule];
    vector<int> melhor_schedule = melhores_filmes.filmes;

    cout << "Quantidade de filmes: " << melhores_filmes.qtd_filmes << endl;
    cout << "Melhor schedule: " << endl;

    for (int i = 0; i < melhores_filmes.qtd_filmes; i++){
        cout << "Filme: " << vetor_filmes[melhor_schedule[i]].inicio << " " << vetor_filmes[melhor_schedule[i]].fim << " " << vetor_filmes[melhor_schedule[i]].categoria << endl;
    }


    return melhores_filmes.qtd_filmes;
}


// g++ -Wl,-z,stack-size=4194304 exaustiva.cpp -o exaustiva
//  g++ -Wl,-z,stack-size=6000000000 -fopenmp exaustiva.cpp -o exaustiva
// user@monstrinho:~/ProjetoSupercomp$ ./exaustiva 