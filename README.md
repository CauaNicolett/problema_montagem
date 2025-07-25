# problema_montagem

Implementações dos programas usados nas heurísticas, em C:
Programa da heurística construtiva: heuristica.c
Programa da heurística construtiva com ILS: heuristica_completa.c
Programa do ktns: ktns.c

Compile o arquivo heuristica_completa.c
gcc heuristica_completa.c -o heur
(gera um arquivo executável chamado "heur")

Considere matrizes binárias em que cada linha representa uma tarefa e em que cada coluna representa uma ferramenta.
O elemento da i-ésima linha e j-ésima coluna é 1 se a j-ésima ferramenta é usada na i-ésima tarefa, e 0 caso contrário.

O executável recebe quatro entradas.
primeira entrada: diretório da matriz binária, no estilo das matrizes binárias na pasta "casos_teste";
segunda entrada: número de linhas da matriz;
terceira entrada: número de colunas da matriz;
quarta entrada: capacidade do magazine.

Exemplo: suponha que o executável esteja na mesma pasta que o programa em C.
A matriz do caso teste e1(8x15)c5 tem 8 linhas, 15 colunas e a capacidade do magazine é 5.
Então, rode
./heur 'casos_teste/e1(8x15)c5' 8 15 5

O programa gera arquivos diferentes para o resultado final da matriz binária com linhas permutadas, para a sequência final das tarefas e para uma matriz os estados do magazine, em que a primeira linha indica as ferramentas na estação para a primeira tarefa, em que a segunda linha indica as ferramentas para a segunda tarefa e etc.
No terminal também são mostradas as quantidades de movimento feitas com a solução encontrada.

----

Implementações dos modelos, em Python:
Modelo de Tang e Denardo: arquivo modeloTD.py
Modelo de fluxo multicommodity: arquivo fluxo.py

Ambos os códigos têm a mesma função para gerar as matrizes dos casos teste.
Os casos teste foram gerados da seguinte forma:

   Arquivo   ||   # tarefas   ||   # ferramentas   ||   capacidade   ||   seed   ||\
  e1(8x15)c5 ||       8       ||         15        ||       5        ||    42    ||\
  e2(8x15)c5 ||       8       ||         15        ||       5        ||    43    ||\
  e3(8x15)c5 ||       8       ||         15        ||       5        ||    44    ||\
  e4(8x15)c5 ||       8       ||         15        ||       5        ||    45    ||\
  e5(8x15)c5 ||       8       ||         15        ||       5        ||    46    ||\
 e6(8x15)c10 ||       8       ||         15        ||       10       ||    42    ||\
 e7(8x15)c10 ||       8       ||         15        ||       10       ||    43    ||\
 e8(8x15)c10 ||       8       ||         15        ||       10       ||    44    ||\
 e9(8x15)c10 ||       8       ||         15        ||       10       ||    45    ||\
e10(8x15)c10 ||       8       ||         15        ||       10       ||    46    ||\
 e11(8x15)c5 ||       9       ||         15        ||       5        ||    42    ||\
 e12(8x15)c5 ||       9       ||         15        ||       5        ||    43    ||\
 e13(8x15)c5 ||       9       ||         15        ||       5        ||    44    ||\
 e14(8x15)c5 ||       9       ||         15        ||       5        ||    45    ||\
 e15(8x15)c5 ||       9       ||         15        ||       5        ||    46    ||\
e16(8x15)c10 ||       9       ||         15        ||       10       ||    42    ||\
e17(8x15)c10 ||       9       ||         15        ||       10       ||    43    ||\
e18(8x15)c10 ||       9       ||         15        ||       10       ||    44    ||\
e19(8x15)c10 ||       9       ||         15        ||       10       ||    45    ||\
e20(8x15)c10 ||       9       ||         15        ||       10       ||    46    ||\
 e21(8x15)c5 ||      10       ||         15        ||       5        ||    42    ||\
 e22(8x15)c5 ||      10       ||         15        ||       5        ||    43    ||\
 e23(8x15)c5 ||      10       ||         15        ||       5        ||    44    ||\
 e24(8x15)c5 ||      10       ||         15        ||       5        ||    45    ||\
 e25(8x15)c5 ||      10       ||         15        ||       5        ||    46    ||\
e26(8x15)c10 ||      10       ||         15        ||       10       ||    42    ||\
e27(8x15)c10 ||      10       ||         15        ||       10       ||    43    ||\
e28(8x15)c10 ||      10       ||         15        ||       10       ||    44    ||\
e29(8x15)c10 ||      10       ||         15        ||       10       ||    45    ||\
e30(8x15)c10 ||      10       ||         15        ||       10       ||    46    ||\
 e31(8x15)c5 ||      11       ||         15        ||       5        ||    42    ||\
 e32(8x15)c5 ||      11       ||         15        ||       5        ||    43    ||\
 e33(8x15)c5 ||      11       ||         15        ||       5        ||    44    ||\
 e34(8x15)c5 ||      11       ||         15        ||       5        ||    45    ||\
 e35(8x15)c5 ||      11       ||         15        ||       5        ||    46    ||\
e36(8x15)c10 ||      11       ||         15        ||       10       ||    42    ||\
e37(8x15)c10 ||      11       ||         15        ||       10       ||    43    ||\
e38(8x15)c10 ||      11       ||         15        ||       10       ||    44    ||\
e39(8x15)c10 ||      11       ||         15        ||       10       ||    45    ||\
e40(8x15)c10 ||      11       ||         15        ||       10       ||    46    ||
