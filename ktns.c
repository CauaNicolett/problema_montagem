#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>

int** criarMatriz(int linhas, int colunas);
int* criarVetor(int tamanho);
int buscaLinear(int* vetor, int tamanho, int elemento);
void preencherVetor(int* vetor, int tamanho, int valor);
int lerMatrizBinaria(int** matriz, int linhas, int colunas, FILE* arquivo);
void escreverMatriz(int** matriz, int linhas, int colunas,  FILE* arquivo);
void escreverVetor(int* vetor, int tamanho, FILE* arquivo);
void imprimirMatriz(int** matriz, int linhas, int colunas);
void imprimirVetor(int* vetor, int tamanho);
int somaLinha(int** matriz, int linha, int colunas);
void permutarVetor(int* vetor, int elemento1, int elemento2);
void permutarLinhas(int** matriz, int linha1, int linha2);
void destruirMatriz(int** matriz, int linhas);

int main(int argc, char** argv){
	FILE* arquivo = fopen(argv[1], "r");
	int i, j, k; /* índices para os laços for */
	int ultimo; /* auxiliar para criar a matriz de últimas posições */
	int aux; /* auxliiar para transformar a matriz lida */
	int dim[2]; /* dimensões da matriz */
	int capacidade; /* tamanho do vetor magazina / capacidade do magazine */
	int quant; /* número de elementos no magazine, na inicialização */
	int soma; /* soma na contagem, usada na inicialização */
	int maior; /* índice do elemento no magazine com maior peso na matriz */
	int trocas; /* número de trocas */
	int** matriz; /* matriz binária */
	int* cont; /* vetor de contagem para a inicialização */
	int* magazine; /* vetor do magazine */

	/* Verifica se foram passadas três entradas */
	if (argc != 5) {
		perror("Erro: insira um arquivo, dois números "
				"inteiros indicando as dimensões da matriz "
				"e a capacidade.\n");
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

	/* Define a capacidade */
	capacidade = atoi(argv[4]);

	/* Cria matriz e vetores */
	matriz = criarMatriz(dim[0], dim[1]);
	if (lerMatrizBinaria(matriz, dim[0], dim[1], arquivo) != 0) {
		perror("Erro na leitura de matriz binária."
				" O arquivo não possui dados o suficiente.\n");
		return 3;
	}

	magazine = criarVetor(capacidade);

	/* Fecha arquivo */
	fclose(arquivo);
	arquivo = NULL;

	/* Cria uma matriz que indica a última posição em que a ferramenta de uma coluna é usada,
	   dada a posição inicial i */
	for (j = 0; j < dim[1]; j++) {
		ultimo = 0;
		for (i = 0; i < dim[0]; i++) {
			aux = matriz[i][j];
			matriz[i][j] = dim[0];
			if (aux == 1) {
				for (k = ultimo; k <= i; k++) {
					matriz[k][j] = i;
				}
				ultimo = i + 1;
			}
		}
	}

	/* Inicializa o magazine */
	/* Um vetor de contagem de pesos da primeira linha da matriz é criado para que
	   os elementos de menor peso na matriz sejam inseridos no magazine */
	cont = criarVetor(dim[0] + 1);
	preencherVetor(cont, dim[0] + 1, 0);
	for (j = 0; j < dim[1]; j++) {
		cont[matriz[0][j]]++;
	}
	soma = 0;
	for (k = 0; k < dim[0] + 1 && soma <= capacidade; k++) {
		soma = soma + cont[k];
	}
	k--;
	/* Todos os elementos com peso menor que k cabem no magazine */
	quant = 0;
	for (j = 0; j < dim[1]; j++) {
		if (matriz[0][j] < k) {
			magazine[quant++] = j;
		}
	}
	/* Alguns elementos com peso igual a k podem caber no magazine */
	for (j = 0; quant < capacidade && j < dim[1]; j++) {
		if (matriz[0][j] == k) {
			magazine[quant++] = j;
		}
	}
	
	/* Cria um arquivo para guardar as formas em que o magazine se assume */
	arquivo = fopen("magazine", "w");
	if (arquivo == NULL) {
		perror("Erro ao abrir arquivo.\n");
		return 4;
	}

	/* Escreve o estado inicial do magazine no arquivo */
	escreverVetor(magazine, capacidade, arquivo);
	fprintf(arquivo, "\n");

	/* Percorre as demais linhas da matriz.
	   Se a ferramenta é necessária para a tarefa, uma busca linear é feita para
	   verificar se ela está no magazine. Se ela não está no magazine, então
	   o elemento no magazine com maior peso na matriz de pesos é escolhido para
	   sair e dar lugar à nova ferramenta. */
	trocas = 0;
	for (i = 1; i < dim[0]; i++) {
		for (j = 0; j < dim[1]; j++) {
			if (matriz[i][j] == i && buscaLinear(magazine, capacidade, j) == -1) {
				maior = 0;
				for (k = 1; k < capacidade; k++) {
					if (matriz[i][magazine[maior]] < matriz[i][magazine[k]]) {
						maior = k;
					}
				}
				magazine[maior] = j;
				trocas++;
			}
		}
		/* Escreve estasdos do magazine no arquivo */
		escreverVetor(magazine, capacidade, arquivo);
		fprintf(arquivo, "\n");
	}

	/* Imprime a quantidade de trocas no terminal */
	printf("Trocas: %d", trocas);

	/* Fecha o arquivo */
	fclose(arquivo);
	arquivo = NULL;

	/* Libera memória dinamicamente alocada */
	free(cont);
	free(magazine);
	destruirMatriz(matriz, dim[0]);
	magazine = cont = NULL;
	matriz = NULL;

	/* Encerra o programa */
	return 0;
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

int buscaLinear(int* vetor, int tamanho, int elemento){
	int i;

	for (i = 0; i < tamanho; i++) {
		if (vetor[i] == elemento) {
			return i;
		}
	}
	return -1;
}

void preencherVetor(int* vetor, int tamanho, int valor){
	int i;

	for (i = 0; i < tamanho; i++) {
		vetor[i] = valor;
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

void escreverMatriz(int** matriz, int linhas, int colunas, FILE* arquivo){
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
