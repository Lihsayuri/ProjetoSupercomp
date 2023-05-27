#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm> 
#include <random>
#include <chrono>
#include <stdlib.h> 
#include <iterator>
#include <random>
#include <chrono>
#include <fstream>
#include <bitset>
#include <stack>
#include <utility>
#include <map>
#include <ctime>
#include <omp.h>
#include <thrust/host_vector.h>
#include <thrust/device_vector.h>
#include <thrust/generate.h>
#include <thrust/functional.h>
#include <thrust/copy.h>
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

struct StructSchedule{
    vector<int> filmes;
    int qtd_filmes;
};

struct FilmeProcessado{
    int categoria;
    bitset<24> horario;
};

void preenche_bitset(int &horarios_disponiveis, int inicio, int fim){
    cout << inicio << " " << fim << endl; 
    //cout << horarios_disponiveis.size() << endl;
    for (int i = 0; i < 24; i++){
        if (i >= inicio && i < fim){
            horarios_disponiveis  |= (1 << i);
        }
        else if (inicio > fim && (i >= inicio || i < fim)){
            horarios_disponiveis |= (1 << i);
        }
        else if(inicio == fim){
            horarios_disponiveis |= (1 << i);
        }
    }
}

int and_vectors(const vector<bool>& v1, const vector<bool>& v2) {
    for (size_t i = 0; i < v1.size(); i++) {
        if (v1[i] && v2[i]) {
            return 1;
        }
    }
    return 0;
}

vector<bool> or_vectors(const vector<bool>& v1, const vector<bool>& v2) {
    vector<bool> result(v1.size());
    for (size_t i = 0; i < v1.size(); i++) {
        result[i] = v1[i] || v2[i];
    }
    return result;
}

struct busca_exaustiva_gpu 
{
    int config;   
    int qtd_filmes;
    vector<int> &filmes_por_categoria; 
    busca_exaustiva_gpu(int config_, int qtd_filmes_, vector<int> &filmes_por_categoria_) : config(config_), qtd_filmes(qtd_filmes_), filmes_por_categoria(filmes_por_categoria_) {}
    __host__ __device__
    int operator()(const vector<int> &categoria_filmes, const vector<vector<bool>> &horario_filmes) {
        vector<bool> horarios_disponiveis(24, false);
        vector<int> filmes_por_categoria_aux = filmes_por_categoria;
        int max_count = 0;
        for (int i = 0; i < qtd_filmes; i++){
            if (config & (1 << i)){
                if (filmes_por_categoria_aux[categoria_filmes[i]-1] > 0){
                    int horario_analisado = and_vectors(horarios_disponiveis, horario_filmes[i]);
                    // vector<bool> horario_analisado = horarios_disponiveis & vetor_filmes_processado[i].horario;
                    if ((horario_analisado != 0)) return -1;
                    filmes_por_categoria_aux[categoria_filmes[i]-1]--;
                    horarios_disponiveis = or_vectors(horarios_disponiveis, horario_filmes[i]);
                    max_count += 1;
                }
                else{
                    return -1;
                }
            }
        
        }

        return max_count;
    }
};



int main(){
    int qtd_filmes, qtd_categorias;
    cin >> qtd_filmes >> qtd_categorias;

    vector<int> filmes_por_categoria(qtd_categorias, 0);
    Filme filme_vazio = {0, 0, 0};
    vector<Filme> vetor_filmes (qtd_filmes, filme_vazio);

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

    thrust::host_vector<int> categoria_filmes(qtd_filmes);
    thrust::host_vector<int> horarios_filmes_cpu(qtd_filmes); 


    for (int i = 0; i < qtd_filmes; i++){
        horarios_filmes_cpu[i] = 0;
        preenche_bitset(horarios_filmes_cpu[i], vetor_filmes[i].inicio-1, vetor_filmes[i].fim-1);
        categoria_filmes[i] = vetor_filmes[i].categoria;
    }



    thrust::device_vector<int> config_vector_gpu(pow(2, qtd_filmes));

    thrust::sequence(config_vector_gpu.begin(), config_vector_gpu.end());

    thrust::device_vector<int> categoria_filmes_gpu(categoria_filmes);
    thrust::device_vector<int> horarios_filmes_gpu(horarios_filmes_cpu);


    for (int i = 0; i < qtd_filmes; i++){
        cout << horarios_filmes_cpu[i] << endl;
    }

    // for (int i = 0; i < pow(2, qtd_filmes); i++){
    //     thrust::transform(vetor_filmes_processado.begin(), vetor_filmes_processado.end(), filmes_por_categoria.begin(), configs_aceitas_gpu.begin(), busca_exaustiva_gpu(config_vector_gpu[i], qtd_filmes));
    // }

    // // thrust::transform(config_vector_gpu, config_vector_gpu, config_vector_gpu.begin(), busca_exaustiva_gpu);

    // thrust::host_vector<int> config_vector_cpu_final = config_vector_gpu;

    // int max_count = 0;
    // for (int i = 0; i < pow(2, qtd_filmes); i++){
    //     if (config_vector_cpu[i] > max_count){
    //         max_count = config_vector_cpu[i];
    //     }
    // }

    // cout << max_count << endl;
}




// g++ -Wl,-z,stack-size=4194304 exaustiva.cpp -o exaustiva
//  g++ -Wl,-z,stack-size=6000000000 -fopenmp exaustiva.cpp -o exaustiva
// user@monstrinho:~/ProjetoSupercomp$ ./exaustiva 
// nvcc -arch=sm_70 -rdc=true -o exaustiva_thrust exaustiva_thrust.cu