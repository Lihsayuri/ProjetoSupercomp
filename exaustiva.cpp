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
#include <map>
#include <ctime>
#include <omp.h>
using std::vector;
using std::cin;
using std::cout;
using std::endl;
using std::bitset;
using std::map;

struct Filme{
    int inicio;
    int fim;
    int categoria;
};

struct melhorSchedule{
    vector<Filme> filmes;
    int qtd_filmes;
};

melhorSchedule melhores_filmes;


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


int calcula_delta(int horario_fim, int horario_inicio){
    if (horario_fim > horario_inicio){
        return horario_fim - horario_inicio;
    }
    else if (horario_fim < horario_inicio){
        return 24 - (horario_inicio - horario_fim);
    }
    else{
        return 1; // retorna 1 só pra filmes que começam e terminam no mesmo horário e aí não muda o potencial
    }
}



void busca_exaustiva(int i, int n, vector<Filme> &vetor_filmes, vector<int> filmes_por_categoria, bitset<24> horarios_disponiveis, vector<vector<int>> &vetor_schedules, vector<int> filmes) {
    if (horarios_disponiveis == 0xFFFFFF){
        vetor_schedules.push_back(filmes);
        return;
    }
    if (i >= n){
        vetor_schedules.push_back(filmes);
        return;
    }

    // vetor_schedules.push_back(filmes);

    busca_exaustiva(i+1, n, vetor_filmes, filmes_por_categoria, horarios_disponiveis, vetor_schedules, filmes);
    
    bitset<24> horario_analisado;
    preenche_bitset(horario_analisado, vetor_filmes[i].inicio-1, vetor_filmes[i].fim-1);
    
    if ((!(horarios_disponiveis & horario_analisado).any()) && (filmes_por_categoria[vetor_filmes[i].categoria-1] > 0)){   // Retorna true se algum dos bits do bitset for 1
        for(int j = vetor_filmes[i].inicio-1; j <= vetor_filmes[i].fim-1; j++){
            cout << "j: " << j << endl;
            cout << "i: " << i << endl;
            filmes[j] = i;
            horarios_disponiveis.set(j);
        }
        filmes_por_categoria[vetor_filmes[i].categoria-1]--;
        preenche_bitset(horarios_disponiveis, vetor_filmes[i].inicio-1, vetor_filmes[i].fim-1);
    }
    busca_exaustiva(i+1, n, vetor_filmes, filmes_por_categoria, horarios_disponiveis, vetor_schedules, filmes);
}


int main(){
    int qtd_filmes, qtd_categorias;
    cin >> qtd_filmes >> qtd_categorias;

    vector<int> filmes_por_categoria(qtd_categorias, 0);
    Filme filme_vazio = {0, 0, 0};
    vector<Filme> vetor_filmes (qtd_filmes, filme_vazio);
    bitset<24> horarios_disponiveis(0x000000);
    bitset<24> mascara_horarios(0xFFFFFF);
    //vector<Filme> vetor_filmes_vistos;
    vector<int> vetor_filmes_vistos(24, -1);
    vector<vector<int>> vetor_schedules;

    for (int i = 0; i < qtd_categorias; i++){
        cin >> filmes_por_categoria[i];
    }

    for (int i = 0; i < qtd_filmes; i++){
        Filme filme;
        cin >> filme.inicio >> filme.fim >> filme.categoria;
        if (filme.inicio == 0){
            filme.inicio = 24;
        }
        if (filme.fim == 0){
            filme.fim = 24;
        }
        if (filme.inicio < 0){
            continue;
        }
        if (filme.fim < 0){
            continue;
        }
        vetor_filmes[i] = filme;
    }

    ordena_final(vetor_filmes);
    ordena_inicio(vetor_filmes);

    busca_exaustiva(0, qtd_filmes, vetor_filmes, filmes_por_categoria, horarios_disponiveis, vetor_schedules, vetor_filmes_vistos);

    for(int i = 0; i < int(vetor_schedules.size()); i++){
        cout << "Schedule " << i+1 << ": ";
        for (int j = 0; j < int(vetor_schedules[i].size()); j++){
            cout << vetor_schedules[i][j] << " ";
        }
        cout << endl;
    }
    cout << "Esses foram os filmes adicionados no schedule "  << vetor_schedules.size() << endl;

    vector<int> melhor_schedule;
    for (int i = 0; i < int(vetor_schedules.size()); i++){
        vector <int> filmes;
        for (int j = 0; j < int(vetor_schedules[i].size()); j++){
            if ((vetor_schedules[i][j] != -1) && find(filmes.begin(), filmes.end(), vetor_schedules[i][j]) == filmes.end()){
                filmes.push_back(vetor_schedules[i][j]);
            }
        }

        if (int(filmes.size()) > int(melhor_schedule.size())){
            melhor_schedule = filmes;
        }  
    }


    melhores_filmes.qtd_filmes = melhor_schedule.size();
    

    cout << "Quantidade de filmes: " << melhores_filmes.qtd_filmes << endl;
    cout << "Melhor schedule: ";

    for (int i = 0; i < melhores_filmes.qtd_filmes; i++){
        cout << melhor_schedule[i] << " ";
        cout << "Filme: " << vetor_filmes[melhor_schedule[i]].inicio << " " << vetor_filmes[melhor_schedule[i]].fim << " " << vetor_filmes[melhor_schedule[i]].categoria << endl;
    }


    return melhores_filmes.qtd_filmes;
}
