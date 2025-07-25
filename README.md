# problema_montagem

Implementações dos programas usados nas heurísticas, em C:
Programa da heurística construtiva: heuristica.c
Programa da heurística construtiva com ILS: heuristica_completa.c
Programa do ktns: ktns.c



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
e40(8x15)c10 ||      11       ||         15        ||       10       ||    46    ||\
