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
using std::vector;
using std::cin;
using std::cout;
using std::endl;
using std::bitset;
using std::map;

//// DÚVIDAS: O QUE FAZER QUANDO O FILME COMEÇAR E TERMINAR NA MESMA HORA? E SE TIVER DOIS FILMES QUE COMEÇAM E TERMINAM NA MESMA HORA? 

struct Filme{
    int inicio;
    int fim;
    int categoria;
};


void ordena_final(vector<Filme> &matriz_filmes){
    std::sort(matriz_filmes.begin(), matriz_filmes.end(), [] (Filme &a, Filme &b){
		return a.fim < b.fim;
	});

}

void ordena_inicio(vector<Filme> &matriz_filmes){
    // Se e somente se o vetor tiver dois horários finais iguais, o que vem primeiro vai ser o vetor com o horśrio inicial menor.
    for (int i = 0; i < int(matriz_filmes.size()); i++){
        if (matriz_filmes[i].fim == matriz_filmes[i+1].fim){
            if (matriz_filmes[i].inicio > matriz_filmes[i+1].inicio){
                Filme aux = matriz_filmes[i];
                matriz_filmes[i] = matriz_filmes[i+1];
                matriz_filmes[i+1] = aux;
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

void heuristica_gulosa(vector<Filme> &matriz_filmes, vector<int> &filmes_por_categoria, bitset<24> &horarios_disponiveis, vector<Filme> &matriz_filmes_vistos, int &filmes_vistos, int qtd_filmes){

    for (int i = 0; i <int(matriz_filmes.size()); i++){
        bitset<24> horario_analisado;
        preenche_bitset(horario_analisado, matriz_filmes[i].inicio-1, matriz_filmes[i].fim-1);
        if ((!(horarios_disponiveis & horario_analisado).any()) && (filmes_por_categoria[matriz_filmes[i].categoria-1] > 0)){   // Retorna true se algum dos bits do bitset for 1
            filmes_vistos++;
            matriz_filmes_vistos.push_back(matriz_filmes[i]);
            filmes_por_categoria[matriz_filmes[i].categoria-1]--;
            preenche_bitset(horarios_disponiveis, matriz_filmes[i].inicio-1, matriz_filmes[i].fim-1);
            return;
        }
    }
}


void aleatorizacao(vector<Filme> &matriz_filmes, vector<int> &filmes_por_categoria, bitset<24> &horarios_disponiveis, vector<Filme> &matriz_filmes_vistos, int &filmes_vistos){
    vector<Filme> disponiveis_no_intervalo;
    if (int(matriz_filmes.size()) > 1) {
        for (int i = 1; i < int(matriz_filmes.size()); i++){
            bitset<24> horario_analisado;
            preenche_bitset(horario_analisado, matriz_filmes[i].inicio-1, matriz_filmes[i].fim-1);
            if ((!(horarios_disponiveis & horario_analisado).any()) && (filmes_por_categoria[matriz_filmes[i].categoria-1] > 0)){   // Retorna true se algum dos bits do bitset for 1
                disponiveis_no_intervalo.push_back(matriz_filmes[i]);
            }  
        }

        int numero_aleatorio = 0;

        if (int(disponiveis_no_intervalo.size()) == 0){
            return;
        }
        if (disponiveis_no_intervalo.size() == 1){
            numero_aleatorio = 0;
        } else{
            int size_myDictItem = int(disponiveis_no_intervalo.size()-1);
            srand(time(NULL));
            numero_aleatorio = rand() % size_myDictItem+1;
        }

        filmes_vistos++;
        matriz_filmes_vistos.push_back(disponiveis_no_intervalo[numero_aleatorio]);
        filmes_por_categoria[disponiveis_no_intervalo[numero_aleatorio].categoria-1]--;
        preenche_bitset(horarios_disponiveis, disponiveis_no_intervalo[numero_aleatorio].inicio-1, disponiveis_no_intervalo[numero_aleatorio].fim-1);

    } 
    
}




int main(){
    int qtd_filmes, qtd_categorias;
    cin >> qtd_filmes >> qtd_categorias;

    vector<int> filmes_por_categoria(qtd_categorias, 0);
    vector<Filme> matriz_filmes;
    bitset<24> horarios_disponiveis;
    bitset<24> mascara_horarios(0xFFFFFF);
    vector<Filme> matriz_filmes_vistos;
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
        if (filme.inicio == -1){
            continue;
        }
        if (filme.fim == -1){
            continue;
        }
        matriz_filmes.push_back(filme);
    }

    ordena_final(matriz_filmes);
    ordena_inicio(matriz_filmes);

    map<int, vector<Filme>> myDict;

    for (int i = 0; i < qtd_filmes; i++){
        myDict[matriz_filmes[i].fim].push_back(matriz_filmes[i]);
    }

    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
 	std::default_random_engine generator(seed); 	
	std::binomial_distribution<int> distribution(1, 0.75);
  
    for (int i = 1; i <= 24; i++){
        int sorteio = distribution(generator)*abs(rand()%2);
        if (horarios_disponiveis == mascara_horarios){
            break;
        }
        if (sorteio == 1){
            heuristica_gulosa(myDict[i], filmes_por_categoria, horarios_disponiveis, matriz_filmes_vistos, filmes_vistos, qtd_filmes);
        } else
        {   
            aleatorizacao(myDict[i], filmes_por_categoria, horarios_disponiveis, matriz_filmes_vistos, filmes_vistos);
        }
    }
   
    cout << "Foram vistos " << filmes_vistos << " filmes." << endl;

    for (int i = 0; i < int(matriz_filmes_vistos.size()); i++){
        cout << matriz_filmes_vistos[i].inicio << " " << matriz_filmes_vistos[i].fim << " " << matriz_filmes_vistos[i].categoria << endl;
    }


    return filmes_vistos;
}
