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
 // imports do thrust
#include <thrust/host_vector.h>
#include <thrust/device_vector.h>
#
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

    thrust::device_vector<int> start_times(N);
    thrust::device_vector<int> end_times(N);
    thrust::device_vector<int> categories(N);
    thrust::device_vector<int> filmes_por_categoria(M, 0);

    for (int i = 0; i < M; i++){
        cin >> filmes_por_categoria[i];
    }

    for (int i = 0; i < N; i++){
        cin >> start_times[i];
        cin >> end_times[i];
        cin >> categories[i];
        if (start_times[i] == 0){
            start_times[i] = 24;
        }
        if (end_times[i] == 0){
            end_times[i] = 24;
        }
        if (start_times[i] < 0){
            continue;
        }
        if (end_times[i] < 0){
            continue;
        }
    }

    thrust::device_vector<int> dp((N+1) * (M+1), 0);

    // Inicializar a primeira linha da matriz com zeros
    thrust::fill(dp.begin(), dp.begin() + M + 1, 0);

    // Preencher a matriz com as soluções para subproblemas menores
    // Preencher a matriz com as soluções para subproblemas menores
    for (int i = 1; i <= N; i++) {
        for (int j = 1; j <= M; j++) {
            // Encontrar o número máximo de filmes que podem ser assistidos até o filme i e categoria j
            int max_count = 0;
            for (int k = 0; k < i; k++) {
            if (categories[k] == j && end_times[k] <= start_times[i] && dp[(k*(M+1)) + j-1] + 1 <= L[j-1]) {
                max_count = max(max_count, dp[(k*(M+1)) + j-1] + 1);
            } else {
                max_count = max(max_count, dp[(k*(M+1)) + j]);
            }
            }
            dp[(i*(M+1)) + j] = max_count;
        }
    }

    // Encontrar o número máximo de filmes que podem ser assistidos
    int max_count = 0;
    int max_j = 0;
    for (int j = 1; j <= M; j++) {
        if (dp[(N * (M + 1)) + j] > max_count) {
            max_count = dp[(N * (M + 1)) + j];
            max_j = j;
        }
    }

    while (i > 0 && j > 0) {
        if (categories[i - 1] == j && end_times[i - 1] <= start_times[i] && dp[(i - 1) * (M + 1) + j - 1] + 1 == dp[i * (M + 1) + j]) {
            filmes_selecionados.push_back(i);
            i--;
            j--;
        } else {
            i--;
        }
    }

    // Os filmes selecionados estão armazenados em ordem inversa, então é necessário inverter a ordem
    std::reverse(filmes_selecionados.begin(), filmes_selecionados.end());

    // Imprimir os filmes selecionados
    std::cout << "Filmes selecionados: ";
    for (int filme : filmes_selecionados) {
        std::cout << filme << " ";
    }
    std::cout << std::endl;

    return 0;

}


// g++ -Wl,-z,stack-size=4194304 exaustiva.cpp -o exaustiva
//  g++ -Wl,-z,stack-size=6000000000 -fopenmp exaustiva.cpp -o exaustiva
// user@monstrinho:~/ProjetoSupercomp$ ./exaustiva 