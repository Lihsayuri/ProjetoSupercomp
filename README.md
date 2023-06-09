# ProjetoSupercomp <img src="https://img.shields.io/static/v1?label=Projeto&message=Finalizado&color=success&style=flat-square&logo=ghost"/>

## Feito por :raising_hand_woman: :raising_hand_man:

- Bernardo Cunha Capoferri.
- Lívia Sayuri Makuta.

## Enunciado do projeto :round_pushpin: :

Você quer passar um final de semana assistindo ao máximo de filmes possível, mas há restrições quanto aos horários disponíveis e ao número de títulos que podem ser vistos em cada categoria (comédia, drama, ação, etc).

Entrada: Um inteiro N representando o número de filmes disponíveis para assistir e N trios de inteiros (H[i], F[i], C[i]), representando a hora de início, a hora de fim e a categoria do i-ésimo filme. Além disso, um inteiro M representando o número de categorias e uma lista de M inteiros representando o número máximo de filmes que podem ser assistidos em cada categoria.

Saída: Um inteiro representando o número máximo de filmes que podem ser assistidos de acordo com as restrições de horários e número máximo por categoria.

## Descrição dos documentos do repositório:

- `Outputs` : pasta que contêm os outputs da parte 1 do projeto.
- `Parte1` : pasta que contém a entrega parcial da parte 1 do projeto.
  - `aleatorizacao.cpp` e `aleatorizacao` : código fonte da estratégia de aleatorização e seu executável, respectivamente.
  - `gulosa.cpp` e `gulosa` : código fonte da heurística gulosa e seu executável, respectivamente.
  - `projeto.cpp` e `projeto` : código fonte da estratégia gulosa e aleatória juntas e seu executável, respectivamente.
  - `meu_programa.cpp` e `meu_programa` : código fonte do gerador de entradas e seu executável, respectivamente.
  - `callgrind.out.6975` : callgrind do executável gulosa.
  - `callgrind.out.7748` : callgrind do executável aleatorizacao.
  - `callgrind.out.8941` : callgrind do executável projeto.
  - `RelatorioProjeto.ipynb` : relatório parcial do projeto.
- `Parte2` : pasta que contém a entrega final do projeto, com a implementação da busca exaustiva e paralelização. 
  - `Outputs` : pasta que contêm os outputs da parte 2 do projeto.
  - `exaustiva.cpp` e `exaustiva`: código fonte da busca exaustiva paralelizada com OpenMP e seu executável, respectivamente.
  - `exaustiva_thrust.cu` e `exaustiva_thrust`: código fonte da busca exaustiva paralelizada com Thrust e seu executável, respectivamente.
  - `RelatorioProjeto.ipynb` : relatório final do projeto.

## Link do Drive para baixar os arquivos de entrada

- Arquivos de entrada da parte 1 do projeto [heurística gulosa e aleatorização] : https://drive.google.com/drive/folders/1-AAfQe7ZwPQNmr-ekSbmH97YaI-AKiz5?usp=sharing
- Arquivos de entrada da parte 2 do projeto [busca exaustiva com OpenMP e Thrust] : https://drive.google.com/drive/folders/1UZmMwICMC7qHdwiqfNOR_GtMjvfDFO7K?usp=sharing
