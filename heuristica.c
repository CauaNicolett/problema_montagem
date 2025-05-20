#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>

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

int main(int argc, char** argv){
	FILE* arquivo = fopen(argv[1], "r");
	int i, j, k; /* índices para os laços for */
	int menor, maior; /* índices para coluna e linha escolhidas na matriz */
	int dim[2]; /* dimensões da matriz */
	int** matriz; /* matriz binária */
	int* soma; /* vetor de somas totais das linhas da matriz -1 */
	int* sequencia; /* sequência permutada das linhas da matriz */
	int* grau; /* vetor de graus referentes à cada coluna da matriz */

	/* Verifica se foram passadas três entradas */
	if (argc != 4) {
		perror("Erro: insira um arquivo e dois números "
				"inteiros indicando as dimensões da matriz.\n");
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

	/* Cria matriz e vetores */
	matriz = criarMatriz(dim[0], dim[1]);
	if (lerMatrizBinaria(matriz, dim[0], dim[1], arquivo) != 0) {
		perror("Erro na leitura de matriz binária."
				" O arquivo não possui dados o suficiente.\n");
		return 3;
	}
	soma = criarVetor(dim[0]);
	sequencia = criarVetor(dim[0]);
	grau = criarVetor(dim[1]);

	/* Fecha arquivo */
	fclose(arquivo);
	arquivo = NULL;

	/* Inicializa o vetor de graus com zeros */
	preencherVetor(grau, dim[1], 0);

	/* Inicializa o vetor de sequência com a sequência original */
	for (i = 0; i < dim[0]; i++) {
		sequencia[i] = i;
	}

	/* Inicializa o vetor de somas */
	for (i = 0; i < dim[0]; i++) {
		soma[i] = somaLinha(matriz, i, dim[1]) - 1;
	}

	/* Inicializa o vetor de graus */
	for (j = 0; j < dim[1]; j++) {
		for (i = 0; i < dim[0]; i++) {
			if (matriz[i][j] == 1) {
				grau[j] = grau[j] + soma[i];
			}
		}
	}

	/* Escreve a matriz lida em um arquivo */
	arquivo = fopen("entrada", "w");
	if (arquivo == NULL) {
		perror("Erro ao abrir o arquivo.\n");
		return 4;
	}
	escreverMatriz(matriz, dim[0], dim[1], arquivo);

	/* Fecha o arquivo */
	fclose(arquivo);
	arquivo = NULL;

	/* Laço principal */
	for (k = 0; k < dim[0] - 1; k++) {
		menor = maior = -1;

		/* Verifica se existe coluna com grau positivo.
		 * Se todos forem iguais a zero, acaba o laço */
		for (j = 0; j < dim[1]; j++) {
			if (grau[j] != 0) {
				menor = j;
				break;
			}
		}
		if (menor == -1) {
			break;
		}

		/* Consegue o índice da coluna com menor grau */
		for (j = menor + 1; j < dim[1]; j++) {
			if (grau[j] != 0 && grau[menor] > grau[j]) {
				menor = j;
			}
		}

		/* Fixada a coluna de menor grau, pega o índice da primeira
		 * linha em que o elemento da matriz nessa posição é 1.
		 * Sempre existe esse índice, porque o grau da coluna é positivo. */
		for (i = k; i < dim[0]; i++) {
			if (matriz[i][menor] == 1) {
				maior = i;
				break;
			}
		}

		/* Consegue o índice da linha de maior soma restrita à
		 * condição de que o elemento nessa linha fixada à coluna
		 * de menor grau é igual a 1. */
		for (i = maior + 1; i < dim[0]; i++) {
			if (matriz[i][menor] == 1 && soma[maior] < soma[i]) {
				maior = i;
			}
		}

		/* Atualiza o vetor de graus */
		for (j = 0; j < dim[1]; j++) {
			if (matriz[maior][j] == 1) {
				grau[j] = grau[j] - soma[maior];
			}
		}

		/* Permuta linhas das matriz e dos vetores de soma e sequência */
		permutarVetor(soma, maior, k);
		permutarVetor(sequencia, maior, k);
		permutarLinhas(matriz, maior, k);
	}

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
	free(soma);
	free(sequencia);
	free(grau);
	destruirMatriz(matriz, dim[0]);
	soma = sequencia = grau = NULL;
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
