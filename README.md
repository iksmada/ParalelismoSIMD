﻿<h2>Paralelismo do Fractal de Mandelbrot em SIMD</h2></br>
<hr>
<h4>Objetivo</h4>

Comparamos o desempenho de diferentes codigos para construir o fractal do conjunto de Mandelbrot, baseados em um codigo fornecido pelo professor https://github.com/menotti/arq2asm/tree/master/pbm
Apartir deste desenvolvemos duas versoes de codigo com paralelização, uma usando instruções SIMD com inline assembly e outra usando uma biblioteca chamada OpenMp


<hr>
<h4>SIMD</h4>

Instruções SIMD (Single Instruction Multiple Data) são funções primitivas para paralelizar o processamento de dados sobre o nível de registro de CPU.
Esta versão usa instrucoes AVX, uma das ultimas gerações da Intel, em que foi possivel usar registradores de 256 bits, e fazendo paralelamente calculos com quatro Double's , variavel de 8 bytes cada e de dupla precisao para cálculo de ponto flutuante.

![My simd_imagem] (https://github.com/iksmada/ParalelismoSIMD/blob//master/simd_imagem.png)


<hr>
<h4>OpenMP</h4>

OpenMP (Open Multi-Processing) é uma API (Application Programming Interface) para programação de multi-processos de memoria compartilhada em múltiplas plataformas.
Sistemas de memória compartilhada são sistemas com multiprocessadores, mas cada um está compartilhando um único subsistema de memória.

![My imagem_OpenMP] (https://github.com/iksmada/ParalelismoSIMD/blob//master/imagem_OpenMP.png)

Para o uso da OpenMP, utilizamos as seguintes instruções: "#include <omp.h>" e "#pragma omp parallel for"

<hr>
<h4>Desempenho</h4>
Grafico comparativo do tempo de execução

![My grafico] (https://github.com/iksmada/ParalelismoSIMD/blob//master/grafico.jpg)

Apartir do gráfico podemos notar que quando executamos em alto desempenho temos um resultado melhor no uso do SIMD seguido do  OpenMP e por ultimo o Original.
Nesse caso era o resultado esperado, apesar que o algoritmo com openMP não melhoro muito o tempo. Isso se deve ao fato de não conseguir uma autonomia entre cada interação no "for" para melhor a quebra entre as threads.
Já quando executamos em Economia de Energia, continuamos com o melhor desempenho com o uso do SIMD mas o uso da OpenMP tem desempenho pior que o do arquivo Original, isso se deu ao que com a economia de energia temos um uso menor dos processadores, prejudicando o desempenho da OpenMP em que cada thread pode usar um processador diferente.
O uso de instruções SIMD também não ocorreu de maneira totalmente desejada, uma vez que a otimização de algumas partes não teve vantagem sobre o código sequencial padrão, nos obrigando a retroceder o desenvolvimento.


<hr>
<h4>Créditos</h4>

Trabalho apresentado ao Professor Ricardo Menotti do Departamento de Computação da UFSCar, que nos forneceu o algoritmo original; Agradecemos a todos qus nos ajudaram a sanar dúvidas, aos colegas do semestre passado que usamos como base para cosntruir nossa Wiki e Make File: https://github.com/menotti/arq2asm/

Agradecemos as imagens e referencia literaria que foram retiradas do site: http://www.codeproject.com/Articles/19065/Begin-Parallel-Programming-With-OpenMP
