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

struct FilmeProcessado{
    int categoria;
    vector<bool> horario;
};

struct StructSchedule{
    vector<int> filmes;
    int qtd_filmes;
};


void preenche_bitset(vector<bool> &horarios_disponiveis, int inicio, int fim){
    for (int i = 0; i < 24; i++){
        if (i >= inicio && i < fim){
            horarios_disponiveis[i] = true;
        }
        else if (inicio > fim && (i >= inicio || i < fim)){
            horarios_disponiveis[i] = true;
        }
        else if(inicio == fim){
            horarios_disponiveis[i] = true;
        }
    }
}

struct saxpy
{
    int a;    
    saxpy(int a_) : a(a_) {};
    __host__ __device__
    double operator()(const int& x, const int& y) {
           return a * x + y;
    }
};

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
    busca_exaustiva_gpu(const vector<FilmeProcessado>& vetor_filmes_processado, int qtd_filmes, int qtd_categorias, const vector<int>& filmes_por_categoria) {}
    __host__ __device__
    int operator()(const int &config , const vector<FilmeProcessado>&vetor_filmes_processado, const int &qtd_filmes, const int &qtd_categorias, const vector<int> &filmes_por_categoria) {
        vector<bool> horarios_disponiveis(24, false);
        vector<int> filmes_por_categoria_aux = filmes_por_categoria;
        int max_count = 0;
        for (int i = 0; i < qtd_filmes; i++){
            if (config & (1 << i)){
                if (filmes_por_categoria_aux[vetor_filmes_processado[i].categoria-1] > 0){
                    int horario_analisado = and_vectors(horarios_disponiveis, vetor_filmes_processado[i].horario);
                    // vector<bool> horario_analisado = horarios_disponiveis & vetor_filmes_processado[i].horario;
                    if ((horario_analisado != 0)) return -1;
                    filmes_por_categoria_aux[vetor_filmes_processado[i].categoria-1]--;
                    // horarios_disponiveis |= vetor_filmes_processado[i].horario;
                    horarios_disponiveis = or_vectors(horarios_disponiveis, vetor_filmes_processado[i].horario);
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
    vector<bool> bool_vazio (24,false);
    FilmeProcessado filme_processado_vazio = {0, bool_vazio};
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
        vetor_filmes_processado[i].categoria = vetor_filmes[i].categoria;
        preenche_bitset(vetor_filmes_processado[i].horario, vetor_filmes[i].inicio-1, vetor_filmes[i].fim-1);
        // for (int j = 0; j < 24; j++){
        //   cout << vetor_filmes_processado[i].horario[j] << " " ;
        // }
        // cout << endl;
    }


    thrust::device_vector<int> config_vector_cpu(pow(2, qtd_filmes), 0);
    thrust::counting_iterator<int> config_begin(0);
    thrust::counting_iterator<int> config_end(pow(2, qtd_filmes));

    thrust::device_vector<int> config_vector_gpu(config_vector_cpu);
    thrust::transform(config_begin, config_end, config_vector_gpu.begin(), busca_exaustiva_gpu(vetor_filmes_processado, qtd_filmes, qtd_categorias, filmes_por_categoria));

    thrust::host_vector<int> config_vector_cpu_final = config_vector_gpu;

    int max_count = 0;
    for (int i = 0; i < pow(2, qtd_filmes); i++){
        if (config_vector_cpu[i] > max_count){
            max_count = config_vector_cpu[i];
        }
    }

    cout << max_count << endl;
}


// g++ -Wl,-z,stack-size=4194304 exaustiva.cpp -o exaustiva
//  g++ -Wl,-z,stack-size=6000000000 -fopenmp exaustiva.cpp -o exaustiva
// user@monstrinho:~/ProjetoSupercomp$ ./exaustiva 
// nvcc -arch=sm_70 -rdc=true -o exaustiva_thrust exaustiva_thrust.cu