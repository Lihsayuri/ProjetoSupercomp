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



void busca_exaustiva(int i, vector<Filme> &vetor_filmes, vector<int> filmes_por_categoria, bitset<24> &horarios_disponiveis, vector<Filme> &filmes, int potencial) {
    if (int(filmes.size()) > melhores_filmes.qtd_filmes){
        melhores_filmes.filmes = filmes;
        melhores_filmes.qtd_filmes = int(filmes.size());
    }

    // cout << "potencial: " << potencial << endl;
    // cout << "melhores_filmes.qtd_filmes: " << melhores_filmes.qtd_filmes << endl;
    // cout << "i: " << i << endl;
    // cout << "vetor_filmes.size(): " << filmes.size() << endl;
    
    if (horarios_disponiveis == 0xFFFFFF){
        return;
    }

    if (potencial < melhores_filmes.qtd_filmes){
        return;
    }

    vector<Filme> filmes2 = filmes;


    bitset<24> horario_analisado;
    preenche_bitset(horario_analisado, vetor_filmes[i].inicio-1, vetor_filmes[i].fim-1);
    
    if ((!(horarios_disponiveis & horario_analisado).any()) && (filmes_por_categoria[vetor_filmes[i].categoria-1] > 0)){   // Retorna true se algum dos bits do bitset for 1
        filmes.push_back(vetor_filmes[i]);
        filmes_por_categoria[vetor_filmes[i].categoria-1]--;
        preenche_bitset(horarios_disponiveis, vetor_filmes[i].inicio-1, vetor_filmes[i].fim-1);
        // conta o número de 0 no bitset de horários disponíveis
        int delta = calcula_delta(vetor_filmes[i].fim, vetor_filmes[i].inicio);
        potencial -= delta - 1;

    }

    busca_exaustiva(i+1, vetor_filmes, filmes_por_categoria, horarios_disponiveis, filmes, potencial);

    busca_exaustiva(i+1, vetor_filmes, filmes_por_categoria, horarios_disponiveis, filmes2, potencial);


    
}


int main(){
    int qtd_filmes, qtd_categorias;
    cin >> qtd_filmes >> qtd_categorias;

    vector<int> filmes_por_categoria(qtd_categorias, 0);
    Filme filme_vazio = {0, 0, 0};
    vector<Filme> vetor_filmes (qtd_filmes, filme_vazio);
    bitset<24> horarios_disponiveis(0x000000);
    bitset<24> mascara_horarios(0xFFFFFF);
    vector<Filme> vetor_filmes_vistos;
    int filmes_vistos = 0;

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

    map<int, vector<Filme>> myDict;

    for (int i = 0; i < qtd_filmes; i++){
        myDict[vetor_filmes[i].fim].push_back(vetor_filmes[i]);
    }

    
    busca_exaustiva(0, vetor_filmes, filmes_por_categoria, horarios_disponiveis, vetor_filmes_vistos, 24);

    cout << "Esses foram os filmes adicionados no schedule "  << melhores_filmes.qtd_filmes << endl;

    for (int i = 0; i < (melhores_filmes.qtd_filmes); i++){
        cout << melhores_filmes.filmes[i].inicio << " " << melhores_filmes.filmes[i].fim << " " << melhores_filmes.filmes[i].categoria << endl;
    }


    return melhores_filmes.qtd_filmes;
}
