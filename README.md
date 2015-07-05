<h2>Paralelismo do Fractal de Mandelbrot em SIMD</h2></br>
<hr>
<h4>Objetivo</h4>

Comparamos o desempenho de diferentes codigos para construir o fractal do conjunto de Mandelbrot, baseados em um codigo fornecido pelo professor * https://github.com/menotti/arq2asm/tree/master/pbm
Apartir deste desenvolvemos duas versoes de codigo com paralelização, uma usando instruções SIMD com inline assembly e outra usando uma biblioteca chamada OpenMp


<hr>
<h4>SIMD</h4>

Instruções SIMD (Single Instruction Multiple Data) são funções primitivas para paralelizar o processamento de dados sobre o nível de registro de CPU.
Esta versão usa instrucoes AVX, uma das ultimas gerações da Intel, em que foi possivel usar reistradores de 256 bits, e fazendo paralelamente calculos com quatro Double's , variavel de 8 bytes cada.
Segue abaixo codigo otimizado do bloco original 
Zy=2*Zx*Zy + Cy;
Zx=Zx2-Zy2 +Cx;
Zx2=Zx*Zx;
Zy2=Zy*Zy;  

<img src = "http://github.com/iksmada/ParalelismoSIMD/simd_imagem.png"></img>
<img src="http://www.gama-ksa.com/wp-content/uploads/2014/11/22520129_l.jpg"></img>

<hr>
<h4>OpenMP</h4>

OpenMP (Open Multi-Processing) é uma API (Application Programming Interface) para programação de multi-processos de memoria compartilhada em múltiplas plataformas.
Sistemas de memória compartilhada são sistemas com multiprocessadores, mas cada um está compartilhando um único subsistema de memória.

Para o uso da OpenMP, utilizamos as seguintes instruções:
#include <omp.h>
#pragma omp parallel for

<hr>
<h4>Desempenho</h4>
Grafico comparativo do tempo de execucao

<hr>
<h4>Créditos</h4>

Trabalho apresentado ao Professor Ricardo Menotti do Departamento de Computação da UFSCar, que nos forneceu o algoritmo; a todos qus nos ajudaram a sanar dúvidas, aos colegas do semestre passado que usamos como base para cosntruir nossa Wiki e Make File, * https://github.com/menotti/arq2asm/


ParalelismoSIMD

Repositório destinado para o projeto final do curso Laboratório de Arquiteura e Organização de Computadores 2.

O objetivo do projeto e otimizar o algoritmo de mandelbrot.c com paraleliçao usando inline assembly e isntrucoes SIMD, depois comparar os resultados com autovetorizaçao e o codigo padrão para analisar o desenpenho.

ASS Desenvolvedores
