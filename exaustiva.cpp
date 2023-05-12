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

struct melhorSchedule{
    vector<int> filmes;
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


void busca_exaustiva(int n, vector<Filme> &vetor_filmes, vector<int> filmes_por_categoria){
    unsigned long int todas_combinacoes = pow(2, n-1)-1 ;
    // cout << todas_combinacoes << endl;
    // #pragma omp parallel for
    for (int i = 0; i < todas_combinacoes; i++){
        int num_films = 0;
        // cout << i << endl;
        vector<int> vetor_id_filmes_vistos;
        vector<int> filmes_por_categoria_aux = filmes_por_categoria;
        bitset<64> filmes(i);
        bitset<24> horarios_disponiveis(0x000000);
        for (int j = 0; j < n; j++){
            if (filmes[j] == 1){
                bitset<24> horario_analisado;
                preenche_bitset(horario_analisado, vetor_filmes[j].inicio-1, vetor_filmes[j].fim-1);
                if ((!(horarios_disponiveis & horario_analisado).any()) && (filmes_por_categoria_aux[vetor_filmes[j].categoria-1] > 0)){   // Retorna true se algum dos bits do bitset for 1
                    filmes_por_categoria_aux[vetor_filmes[j].categoria-1]--;
                    preenche_bitset(horarios_disponiveis, vetor_filmes[j].inicio-1, vetor_filmes[j].fim-1);
                    num_films++;
                    vetor_id_filmes_vistos.push_back(j);
                }
            }

        }
        // #pragma omp critical
        if (num_films > melhores_filmes.qtd_filmes ){
            melhores_filmes.qtd_filmes = num_films;
            melhores_filmes.filmes = vetor_id_filmes_vistos;
        }
    }
}


int main(){
    int qtd_filmes, qtd_categorias;
    cin >> qtd_filmes >> qtd_categorias;
    melhores_filmes.filmes = vector<int>();
    melhores_filmes.qtd_filmes = 0;

    vector<int> filmes_por_categoria(qtd_categorias, 0);
    Filme filme_vazio = {0, 0, 0};
    vector<Filme> vetor_filmes (qtd_filmes, filme_vazio);
    bitset<24> horarios_disponiveis(0x000000);
    bitset<24> mascara_horarios(0xFFFFFF);
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

    busca_exaustiva(qtd_filmes, vetor_filmes, filmes_por_categoria);

    vector<int> melhor_schedule = melhores_filmes.filmes;
    for (int i = 0; i < int(melhor_schedule.size()); i++){
        cout << melhor_schedule[i] << " ";
    }

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