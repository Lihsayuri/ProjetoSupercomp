# ProjetoSupercomp <img src="https://img.shields.io/static/v1?label=Projeto&message=Finalizado&color=success&style=flat-square&logo=ghost"/>

## Authors :raising_hand_woman: :raising_hand_man:

- Bernardo Cunha Capoferri.
- Lívia Sayuri Makuta.

## Project Statement :round_pushpin: :

You want to spend a weekend watching as many movies as possible, but there are restrictions regarding available times and the number of titles that can be watched in each category (comedy, drama, action, etc.).

Input: An integer N representing the number of available movies to watch and N triples of integers (H[i], F[i], C[i]), representing the start time, end time, and category of the i-th movie. Additionally, an integer M representing the number of categories and a list of M integers representing the maximum number of movies that can be watched in each category.

Output: An integer representing the maximum number of movies that can be watched according to the time restrictions and the maximum number allowed per category.

## Descrição dos documentos do repositório:

- `Outputs` : Folder containing the outputs of Part 1 of the project.
- `Parte1` : Folder containing the partial submission of Part 1 of the project.
  - `aleatorizacao.cpp` e `aleatorizacao` : Source code for the randomization strategy and its executable, respectively.
  - `gulosa.cpp` e `gulosa` : Source code for the greedy heuristic and its executable, respectively.
  - `projeto.cpp` e `projeto` : Source code for the combined greedy and random strategies and its executable, respectively.
  - `meu_programa.cpp` e `meu_programa` : Source code for the input generator and its executable, respectively.
  - `callgrind.out.6975` : Callgrind output for the greedy executable.
  - `callgrind.out.7748` : Callgrind output for the randomization executable.
  - `callgrind.out.8941` : Callgrind output for the combined project executable.
  - `RelatorioProjeto.ipynb` : Partial project report.
- `Parte2` : Folder containing the final submission of the project, with the implementation of exhaustive search and parallelization.
  - `Outputs` : Folder containing the outputs of Part 2 of the project.
  - `exaustiva.cpp` and `exaustiva`: Source code for the parallelized exhaustive search with OpenMP and its executable, respectively.
  - `exaustiva_thrust.cu` and `exaustiva_thrust`: Source code for the parallelized exhaustive search with Thrust and its executable, respectively.
  - `RelatorioProjeto.ipynb` : Final project report.

## Link do Drive para baixar os arquivos de entrada

- Input files for Part 1 of the project [greedy heuristic and randomization] : https://drive.google.com/drive/folders/1-AAfQe7ZwPQNmr-ekSbmH97YaI-AKiz5?usp=sharing
- Input files for Part 2 of the project [exhaustive search with OpenMP and Thrust]: https://drive.google.com/drive/folders/1UZmMwICMC7qHdwiqfNOR_GtMjvfDFO7K?usp=sharing
