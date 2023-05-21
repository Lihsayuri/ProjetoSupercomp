#include <cmath>
#include <algorithm> 
#include <iostream>
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
 // imports do thrust
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
int main(){
    int N, M;
    cin >> N >> M;
    cout << N << endl;
    cout << M << endl;
    thrust::host_vector<int>filmes_por_categoria_cpu(M);
    thrust::host_vector<int> start_times_cpu(N);
    thrust::host_vector<int> end_times_cpu(N);
    thrust::host_vector<int> categories_cpu(N);

    for (int i = 0; i < M; i++){
        cin >> filmes_por_categoria_cpu[i];
    }

    for (int i = 0; i < N; i++){
        cin >> start_times_cpu[i];
        cin >> end_times_cpu[i];
        cin >> categories_cpu[i];
        if (start_times_cpu[i] == 0){
            start_times_cpu[i] = 24;
        }
        if (end_times_cpu[i] == 0){
            end_times_cpu[i] = 24;
        }
        if (start_times_cpu[i] < 0){
            continue;
        }
        if (end_times_cpu[i] < 0){
            continue;
        }
    }

    thrust::device_vector<int> start_times_gpu(start_times_cpu);
    thrust::device_vector<int> end_times_gpu(end_times_cpu);
    thrust::device_vector<int> categories_gpu(categories_cpu);
    thrust::device_vector<int> filmes_por_categoria_gpu(filmes_por_categoria_cpu);


    thrust::device_vector<int> dp(N * M, 0);

    // Inicializar a primeira linha da matriz com zeros
    thrust::fill(dp.begin(), dp.begin() + M, 0);

    // Preencher a matriz com as soluções para subproblemas menores
    // Preencher a matriz com as soluções para subproblemas menores
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < M; j++) {
            // Encontrar o número máximo de filmes que podem ser assistidos até o filme i e categoria j
            int max_count = 0;
            for (int k = 0; k < i; k++) {
              if (categories_gpu[k] == j && end_times_gpu[k] <= start_times_gpu[i] && dp[(k*(M)) + j] + 1 <= filmes_por_categoria_gpu[j]) {
                  max_count = max(max_count, dp[(k*(M)) + j] + 1);
              } else {
                  max_count = max(max_count, dp[(k*(M)) + j]);
              }
            }
            dp[(i*(M)) + j] = max_count;
        }
    }

    // Encontrar o número máximo de filmes que podem ser assistidos
    int max_count = 0;
    int max_j = 0;
    for (int j = 0; j < M; j++) {
        if (dp[(N-1 * (M)) + j] > max_count) {
            max_count = dp[(N-1 * (M)) + j];
            max_j = j;
        }
    }



    cout << max_count << endl;
    cout << max_j << endl;
    cout << dp[(N-1 * (M)) + max_j] << endl;



    return 0;
}