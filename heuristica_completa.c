#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <time.h>

#define MAXIT 100

int** criarMatriz(int linhas, int colunas);
int* criarVetor(int tamanho);
void preencherVetor(int* vetor, int tamanho, int valor);
int lerMatrizBinaria(int** matriz, int linhas, int colunas, FILE* arquivo);
void escreverMatriz(int** matriz, int linhas, int colunas, FILE* arquivo);
void escreverVetor(int* vetor, int tamanho, FILE* arquivo);
void imprimirMatriz(int** matriz, int linhas, int colunas);
void imprimirVetor(int* vetor, int tamanho);
int somaLinha(int** matriz, int linha, int colunas);
void permutarVetor(int* vetor, int elemento1, int elemento2);
void permutarLinhas(int** matriz, int linha1, int linha2);
void destruirMatriz(int** matriz, int linhas);
void copiarMatriz(int** original, int** copia, int linhas, int colunas);
void copiarVetor(int* original, int* copia, int tamanho);
int buscaLinear(int* vetor, int tamanho, int elemento);
void heuristicaConstrutiva(int** matriz, int* sequencia, int linhas, int colunas);
int ktns(int** matriz, int linhas, int colunas, int capacidade);
int buscaLocal(int** matriz, int* sequencia, int linhas, int colunas, int capacidade, int valorLocal);
void perturbar(int** matriz, int* sequencia, int linhas);

int main(int argc, char** argv){
	FILE* arquivo = fopen(argv[1], "r");
	int i; /* índice para os laços for */
	int dim[2]; /* dimensões da matriz */
	int** matriz; /* matriz binária */
	int** matrizcopia; /* matriz binária para o ILS */
	int* sequencia; /* sequência permutada das linhas da matriz */
	int* sequenciacopia; /* sequência permutada das linhas da matriz para o ILS */
    int capacidade; /* capacidade do magazine */
    int trocas; /* número de trocas */
	int ktns1, ktns2; /* Valores retornados do KTNS, no código do ILS */

	/* Verifica se foram passadas quatro entradas */
	if (argc != 5) {
		perror("Erro: insira um arquivo, dois números "
				"inteiros indicando as dimensões da matriz e a capacidade.\n");
		return 5;
	}

	/* Verifica se o arquivo existe */
	if (arquivo == NULL) {
		perror("Erro ao abrir arquivo.\n");
		return 4;
	}

	/* Define as dimensões da matriz */
	dim[0] = atoi(argv[2]);
	dim[1] = atoi(argv[3]);
    capacidade = atoi(argv[4]);

	/* Cria matriz e vetores */
	matriz = criarMatriz(dim[0], dim[1]);
	if (lerMatrizBinaria(matriz, dim[0], dim[1], arquivo) != 0) {
		perror("Erro na leitura de matriz binária."
				" O arquivo não possui dados o suficiente.\n");
		return 3;
	}

    /* Fecha arquivo */
	fclose(arquivo);
	arquivo = NULL;

    /* Inicializa o vetor de sequência com a sequência original */
    sequencia = criarVetor(dim[0]);
	for (i = 0; i < dim[0]; i++) {
		sequencia[i] = i;
	}

    heuristicaConstrutiva(matriz, sequencia, dim[0], dim[1]);

    /* Colocar a função de busca local */
	matrizcopia = criarMatriz(dim[0], dim[1]);
	sequenciacopia = criarVetor(dim[0]);

	trocas = ktns(matriz, dim[0], dim[1], capacidade);
	ktns1 = trocas;
	printf("Quantidade de trocas (sem busca local): %d. \n", trocas);
	for (i = 0; i < MAXIT; i++) {
		copiarMatriz(matriz, matrizcopia, dim[0], dim[1]);
		copiarVetor(sequencia, sequenciacopia, dim[0]);
		perturbar(matrizcopia, sequenciacopia, dim[0]);
		ktns2 = buscaLocal(matrizcopia, sequenciacopia, dim[0], dim[1], capacidade, trocas);
		if (ktns2 < ktns1) {
			copiarMatriz(matrizcopia, matriz, dim[0], dim[1]);
			copiarVetor(sequenciacopia, sequencia, dim[0]);
			ktns1 = ktns2;
		}
	}
	trocas = ktns(matriz, dim[0], dim[1], capacidade);

	/* Escreve um arquivo com a matriz permutada */
	arquivo = fopen("saida", "w");
	if (arquivo == NULL) {
		perror("Erro ao abrir arquivo.\n");
		return 4;
	}
	escreverMatriz(matriz, dim[0], dim[1], arquivo);
	
	/* Fecha arquivo */
	fclose(arquivo);
	arquivo = NULL;

	/* Escreve um arquivo com a sequência final */
	arquivo = fopen("sequencia", "w");
	if (arquivo == NULL) {
		perror("Erro ao abrir o arquivo.\n");
		return 4;
	}
	escreverVetor(sequencia, dim[0], arquivo);

	/* Fecha arquivo */
	fclose(arquivo);
	arquivo = NULL;

	/* Libera memória dinamicamente alocada */
    free(sequencia);
	free(sequenciacopia);
	destruirMatriz(matriz, dim[0]);
	destruirMatriz(matrizcopia, dim[0]);
	sequencia = NULL;
	matriz = NULL;

	/* Imprime a quantidade de trocas */
	printf("Quantidade de trocas: %d. \n", trocas);

	/* Encerra o programa */
	return 0;
}

void heuristicaConstrutiva(int** matriz, int* sequencia, int linhas, int colunas) {
    int* soma; /* vetor de somas totais das linhas da matriz -1 */
	int* grau; /* vetor de graus referentes à cada coluna da matriz */
    int i, j, k; /* índices para os laços for */
	int menor, maior; /* índices para coluna e linha escolhidas na matriz */

	soma = criarVetor(linhas);
	grau = criarVetor(colunas);

    /* Inicializa o vetor de graus com zeros */
	preencherVetor(grau, colunas, 0);

	/* Inicializa o vetor de somas */
	for (i = 0; i < linhas; i++) {
		soma[i] = somaLinha(matriz, i, colunas) - 1;
	}

	/* Inicializa o vetor de graus */
	for (j = 0; j < colunas; j++) {
		for (i = 0; i < linhas; i++) {
			if (matriz[i][j] == 1) {
				grau[j] = grau[j] + soma[i];
			}
		}
	}

	/* Laço principal */
	for (k = 0; k < linhas - 1; k++) {
		menor = maior = -1;

		/* Verifica se existe coluna com grau positivo.
		 * Se todos forem iguais a zero, acaba o laço */
		for (j = 0; j < colunas; j++) {
			if (grau[j] != 0) {
				menor = j;
				break;
			}
		}
		if (menor == -1) {
			break;
		}

		/* Consegue o índice da coluna com menor grau */
		for (j = menor + 1; j < colunas; j++) {
			if (grau[j] != 0 && grau[menor] > grau[j]) {
				menor = j;
			}
		}

		/* Fixada a coluna de menor grau, pega o índice da primeira
		 * linha em que o elemento da matriz nessa posição é 1.
		 * Sempre existe esse índice, porque o grau da coluna é positivo. */
		for (i = k; i < linhas; i++) {
			if (matriz[i][menor] == 1) {
				maior = i;
				break;
			}
		}

		/* Consegue o índice da linha de maior soma restrita à
		 * condição de que o elemento nessa linha fixada à coluna
		 * de menor grau é igual a 1. */
		for (i = maior + 1; i < linhas; i++) {
			if (matriz[i][menor] == 1 && soma[maior] < soma[i]) {
				maior = i;
			}
		}

		/* Atualiza o vetor de graus */
		for (j = 0; j < colunas; j++) {
			if (matriz[maior][j] == 1) {
				grau[j] = grau[j] - soma[maior];
			}
		}

		/* Permuta linhas das matriz e dos vetores de soma e sequência */
		permutarVetor(soma, maior, k);
		permutarVetor(sequencia, maior, k);
		permutarLinhas(matriz, maior, k);
	}
    free(soma);
	free(grau);
}

int ktns(int** matriz, int linhas, int colunas, int capacidade) {
	int i, j, k; /* índices para os laços for */
	int ultimo; /* auxiliar para criar a matriz de últimas posições */
	int aux; /* auxliiar para transformar a matriz lida */
	int quant; /* número de elementos no magazine, na inicialização */
	int soma; /* soma na contagem, usada na inicialização */
	int maior; /* índice do elemento no magazine com maior peso na matriz */
	int trocas; /* número de trocas */
	int** tabela; /* matriz binária */
	int* cont; /* vetor de contagem para a inicialização */
    int* magazine; /* vetor do magazine */
	FILE* arqmagazine; /* arquivo com informações sobre o magazine */

    tabela = criarMatriz(linhas, colunas);
	copiarMatriz(matriz, tabela, linhas, colunas);

    magazine = criarVetor(capacidade);
	arqmagazine = fopen("magazine", "w");
	if (arqmagazine == NULL) {
		perror("Erro ao abrir o arquivo.\n");
		return 4;
	}

	/* Cria uma matriz que indica a última posição em que a ferramenta de uma coluna é usada,
	   dada a posição inicial i */
	for (j = 0; j < colunas; j++) {
		ultimo = 0;
		for (i = 0; i < linhas; i++) {
			aux = tabela[i][j];
			tabela[i][j] = linhas;
			if (aux == 1) {
				for (k = ultimo; k <= i; k++) {
					tabela[k][j] = i;
				}
				ultimo = i + 1;
			}
		}
	}

	/* Inicializa o magazine */
	/* Um vetor de contagem de pesos da primeira linha da matriz é criado para que
	   os elementos de menor peso na matriz sejam inseridos no magazine */
	cont = criarVetor(linhas + 1);
	preencherVetor(cont, linhas + 1, 0);
	for (j = 0; j < colunas; j++) {
		cont[tabela[0][j]]++;
	}
	soma = 0;
	for (k = 0; k < linhas + 1 && soma <= capacidade; k++) {
		soma = soma + cont[k];
	}
	k--;
	/* Todos os elementos com peso menor que k cabem no magazine */
	quant = 0;
	for (j = 0; j < colunas; j++) {
		if (tabela[0][j] < k) {
			magazine[quant++] = j;
		}
	}
	/* Alguns elementos com peso igual a k podem caber no magazine */
	for (j = 0; quant < capacidade && j < colunas; j++) {
		if (tabela[0][j] == k) {
			magazine[quant++] = j;
		}
	}

	/* Escreve o estado inicial do magazine */
	escreverVetor(magazine, capacidade, arqmagazine);
	fprintf(arqmagazine, "\n");

	/* Percorre as demais linhas da matriz.
	   Se a ferramenta é necessária para a tarefa, uma busca linear é feita para
	   verificar se ela está no magazine. Se ela não está no magazine, então
	   o elemento no magazine com maior peso na matriz de pesos é escolhido para
	   sair e dar lugar à nova ferramenta. */
	trocas = 0;
	for (i = 1; i < linhas; i++) {
		for (j = 0; j < colunas; j++) {
			if (tabela[i][j] == i && buscaLinear(magazine, capacidade, j) == -1) {
				maior = 0;
				for (k = 1; k < capacidade; k++) {
					if (tabela[i][magazine[maior]] < tabela[i][magazine[k]]) {
						maior = k;
					}
				}
				magazine[maior] = j;
				trocas++;
			}
		}
		escreverVetor(magazine, capacidade, arqmagazine);
		fprintf(arqmagazine, "\n");
	}

	/* Fecha o arquivo */
	fclose(arqmagazine);
	arqmagazine = NULL;

	/* Libera memória dinamicamente alocada */
	free(cont);
    free(magazine);
	destruirMatriz(tabela, linhas);
	cont = magazine = NULL;
	tabela = NULL;

	/* Encerra o programa */
	return trocas;
}

int buscaLocal(int** matriz, int* sequencia, int linhas, int colunas, int capacidade, int valorLocal) {
    int i, k;
    int trocas;

	for (i = 0; i < linhas - 1; i++) {
		for (k = i + 1; k < linhas; k++) {
			permutarLinhas(matriz, i, k);
			if ((trocas = ktns(matriz, linhas, colunas, capacidade)) < valorLocal) {
				permutarVetor(sequencia, i, k);
				return buscaLocal(matriz, sequencia, linhas, colunas, capacidade, trocas);
			}
			else {
				permutarLinhas(matriz, i, k);
			}
		}
	}
	return trocas;
}

void perturbar(int** matriz, int* sequencia, int linhas) {
	int r1, r2; /* Valores aleatórios */
	
	srand(time(NULL));
	r1 = rand() % linhas;
    r2 = rand() % linhas;
    while (r2 == r1) {
		r2 = rand() % linhas;
	}
	permutarLinhas(matriz, r1, r2);

	r1 = rand() % linhas;
    r2 = rand() % linhas;
    while (r2 == r1) {
		r2 = rand() % linhas;
	}
	permutarLinhas(matriz, r1, r2);

	r1 = rand() % linhas;
    r2 = rand() % linhas;
    while (r2 == r1) {
		r2 = rand() % linhas;
	}
	permutarLinhas(matriz, r1, r2);
}


int buscaLinear(int* vetor, int tamanho, int elemento) {
	int i;

	for (i = 0; i < tamanho; i++) {
		if (vetor[i] == elemento) {
			return i;
		}
	}
	return -1;
}

void copiarMatriz(int** original, int** copia, int linhas, int colunas) {
    int i, j;

    for (i = 0; i < linhas; i++) {
        for (j = 0; j < colunas; j++) {
            copia[i][j] = original[i][j];
        }
    }
}

int** criarMatriz(int linhas, int colunas) {
	int i;
	int** matriz;

	matriz = (int**) malloc(linhas * sizeof(int*));
	if (matriz == NULL) {
		perror("Erro ao criar matriz.\n");
	}
	for (i = 0; i < linhas; i++) {
		matriz[i] = (int*) malloc(colunas * sizeof(int));
		if (matriz[i] == NULL) {
			perror("Erro ao criar matriz.\n");
		}
	}
	return matriz;
}

int* criarVetor(int tamanho){
	int* vetor;

	vetor = (int*) malloc(tamanho * sizeof(int));
	if (vetor == NULL) {
		perror("Erro ao criar vetor.\n");
	}
	return vetor;
}

void preencherVetor(int* vetor, int tamanho, int valor){
	int i;

	for (i = 0; i < tamanho; i++) {
		vetor[i] = valor;
	}
}

void copiarVetor(int* original, int* copia, int tamanho){
	int i;

	for (i = 0; i < tamanho; i++) {
		copia[i] = original[i];
	}
}

int lerMatrizBinaria(int** matriz, int linhas, int colunas, FILE* arquivo){
	int i, j;
	int c;
	
	i = j = 0;
	while ((c = fgetc(arquivo)) != EOF) {
		if (c == '0' || c == '1') {
			if (c == '0') {
				matriz[i][j] = 0;
			}
			else {
				matriz[i][j] = 1;
			}
			j++;
			if (j == colunas) {
				i++;
				j = 0;
			}
			if (i == linhas) {
				break;
			}
		}
	}
	if (i != linhas) {
		return 1;
	}
	return 0;
}

void escreverMatriz(int** matriz, int linhas, int colunas,  FILE* arquivo){
	int i, j;

	for (i = 0; i < linhas; i++) {
		for (j = 0; j < colunas; j++) {
			fprintf(arquivo, "%d ", matriz[i][j]);
		}
		fprintf(arquivo, "\n");
	}
}

void escreverVetor(int* vetor, int tamanho, FILE* arquivo){
	int i;

	for (i = 0; i < tamanho; i++) {
		fprintf(arquivo, "%d ", vetor[i]);
	}
}

void imprimirMatriz(int** matriz, int linhas, int colunas){
	int i, j;

	for (i = 0; i < linhas; i++) {
		for (j = 0; j < colunas; j++) {
			printf("%d ", matriz[i][j]);
		}
		putchar('\n');
	}
}

void imprimirVetor(int* vetor, int tamanho){
	int i;

	for (i = 0; i < tamanho; i++) {
		printf("%d ", vetor[i]);
	}
	putchar('\n');
}

int somaLinha(int** matriz, int linha, int colunas){
	int j;
	int soma;

	soma = 0;
	for (j = 0; j < colunas; j++) {
		soma = soma + matriz[linha][j];
	}
	return soma;
}

void permutarVetor(int* vetor, int elemento1, int elemento2) {
	int aux;

	aux = vetor[elemento1];
	vetor[elemento1] = vetor[elemento2];
	vetor[elemento2] = aux;
}

void permutarLinhas(int** matriz, int linha1, int linha2){
	int* aux;

	aux = matriz[linha1];
	matriz[linha1] = matriz[linha2];
	matriz[linha2] = aux;
}

void destruirMatriz(int** matriz, int linhas) {
	int i;

	for (i = 0; i < linhas; i++) {
		free(matriz[i]);
	}
	free(matriz);
}
