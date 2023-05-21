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
#include <thrust/host_vector.h>0000000
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

    for (int i = 0; i < N*M; i++) {
        cout << dp[i] << " ";
    }



    return 0;
}



// O primeiro loop `for` no código está preenchendo a matriz `dp` com as soluções para subproblemas menores. Ele itera sobre os valores de `i` de 0 a N-1 e `j` de 0 a M-1, representando as posições da matriz `dp`.

// Dentro desse loop, ele calcula o número máximo de filmes que podem ser assistidos até o filme i e categoria j. Isso é feito comparando as informações dos filmes anteriores (de 0 a i-1) e verificando se as condições são atendidas para adicionar um novo filme.

// A condição para adicionar um novo filme é a seguinte:
// - A categoria do filme k (com k de 0 a i-1) é igual a j.
// - O horário de término do filme k é menor ou igual ao horário de início do filme i.
// - O número total de filmes na categoria j até o momento, representado por `dp[(k*(M)) + j]`, mais 1, é menor ou igual ao número máximo de filmes permitidos na categoria j, representado por `filmes_por_categoria_gpu[j]`.

// Se todas essas condições forem atendidas, o número máximo de filmes é atualizado para `dp[(k*(M)) + j] + 1`. Caso contrário, o número máximo de filmes permanece o mesmo, `dp[(k*(M)) + j]`.

// No final do primeiro loop `for`, a matriz `dp` estará preenchida com as soluções ótimas para cada subproblema, ou seja, o número máximo de filmes que podem ser assistidos até cada posição da matriz.

// Após o primeiro loop `for`, o código encontra o número máximo de filmes que podem ser assistidos verificando o valor mais alto na última linha da matriz `dp`, correspondente aos filmes N-1 e todas as categorias.

// Por fim, o código imprime o número máximo de filmes e também imprime a matriz `dp` para fins de depuração.

// Desculpe pela confusão anterior. Houve um erro de interpretação no código. Vamos esclarecer o significado dos elementos da matriz e como encontrar o número máximo de filmes que podem ser assistidos em um dia.

// No código fornecido, a matriz `dp` não representa o número máximo de filmes que podem ser assistidos para cada categoria. Na verdade, a matriz `dp` é usada para calcular a quantidade máxima de filmes que podem ser assistidos considerando todas as combinações possíveis de filmes e categorias até um determinado ponto.

// Cada elemento `dp[i*(M) + j]` da matriz `dp` representa o número máximo de filmes que podem ser assistidos considerando os primeiros `i` filmes e as primeiras `j` categorias. Portanto, o valor em `dp[N*(M) + j]` representa o número máximo de filmes que podem ser assistidos considerando todos os filmes e a categoria `j`.

// Para encontrar o número máximo de filmes que podem ser assistidos em um dia, considerando todas as categorias, é necessário encontrar o maior valor entre `dp[N*(M) + j]` para `j` variando de 0 a M-1.

// Em relação aos horários, o código já realiza a verificação das condições `end_times[k] <= start_times[i]` para garantir que os horários não se sobreponham. Portanto, os filmes só serão considerados se o horário de término do filme anterior for menor ou igual ao horário de início do filme atual.

// No entanto, o código atual não considera a restrição de não assistir mais de um filme ao mesmo tempo. Se você deseja garantir que os horários não estejam se sobrepondo, será necessário modificar o código para levar em conta essa restrição adicional.

// Uma abordagem possível seria usar programação dinâmica com uma matriz tridimensional, onde o terceiro índice representaria o horário de início. Cada elemento `dp[i][j][k]` representaria o número máximo de filmes que podem ser assistidos considerando os primeiros `i` filmes, as primeiras `j` categorias e o horário de início `k`. Você teria que ajustar o código para essa nova estrutura e considerar as restrições de horário ao calcular as soluções.

// Espero que isso esclareça a sua dúvida. Se precisar de mais informações, fique à vontade para perguntar!