#include <stdio.h> // Biblioteca que fornece funções para entrada e saída de dados (ex: printf e scanf)
#include <stdlib.h> // Biblioteca que fornece funções básicas em C (alocação de memória, conversões de strings)
#include <time.h> // Biblioteca para inicializar a semente do gerador de números aleatórios
#include <math.h> // Biblioteca que fornece funções matemáticas (ex: raiz quadrada "sqrt")
#include <stdbool.h> // Biblioteca que define o tipo de dado bool

// Estrutura para representar um ponto com um identificador (id), uma coordenada x (px) e uma coordenada y (py)
struct ponto {
    int id; // Identificador dos pontos
    double px; // Longitude dos pontos
    double py; // Latitude dos pontos
};

// Estrutura para representar um indivíduo na população, com um valor acumulado (valor) e um vetor de 10 elementos inteiros (indiv)
struct pop {
    double valor; // Valor acumulado do custo da rota
    int indiv[10]; // Definição do vetor indivíduo de 10 elementos inteiros
};

// Função para gerar um número aleatório do tipo double entre 0 e 360
double gerarDouble() {
    return ((double)rand() / RAND_MAX) * 360.0; // Retorna um número aleatório do tipo double entre 0 e 360
}

// Função para gerar um vetor de pontos com tamanho especificado
struct ponto *gerarVetorPontos(int tamanhoMax) {
    srand(time(NULL)); // Inicializa o gerador de números aleatórios com a semente baseada no tempo atual
    struct ponto *pontos = malloc(tamanhoMax * sizeof(struct ponto)); // Aloca espaço na memória para um vetor de pontos

    // Popula o vetor de pontos
    for (int i = 0; i < tamanhoMax; i++) {
        pontos[i].id = i; // Define o identificador do ponto como o índice i
        pontos[i].px = gerarDouble(); // Gera um valor aleatório para a coordenada x (longitude)
        pontos[i].py = gerarDouble(); // Gera um valor aleatório para a coordenada y (latitude)
    }

    return pontos; // Retorna o vetor de pontos gerado
}

// Função para calcular a distância euclidiana entre dois pontos
double distEuclideana(struct ponto p1, struct ponto p2) {
    double dx = p1.px - p2.px; // Calcula a diferença entre as coordenadas x dos pontos
    double dy = p1.py - p2.py; // Calcula a diferença entre as coordenadas y dos pontos
    return sqrt(dx * dx + dy * dy); // Retorna a raiz quadrada da soma dos quadrados das diferenças (distância euclidiana)
}

// Função para gerar a matriz de distâncias euclidianas entre os pontos
void gerarMatrizDist(struct ponto *pontos, int tamanhoMax, double Dist[tamanhoMax][tamanhoMax]) {
    // Calcula distâncias euclidianas e preenche a matriz Dist
    for (int eixoY = 0; eixoY < tamanhoMax; eixoY++) {
        for (int eixoX = 0; eixoX < tamanhoMax; eixoX++) {
            Dist[eixoY][eixoX] = distEuclideana(pontos[eixoY], pontos[eixoX]); // Calcula a distância entre os pontos eixoY e eixoX
        }
    }

    // Imprime a matriz de distâncias euclidianas
    printf("Matriz de Distâncias Euclidianas:\n");
    for (int i = 0; i < tamanhoMax; i++) {
        for (int j = 0; j < tamanhoMax; j++) {
            printf("%.2f\t", Dist[i][j]); // Imprime cada elemento da matriz
        }
        printf("\n"); // Quebra de linha após cada linha da matriz
    } 
    printf("\n"); // Quebra de linha adicional para separar a matriz da próxima saída
}

// Função para gerar números únicos em um vetor de tamanho especificado
void gerarNumerosUnicos(int vetor[], int tamanho) {
    bool numeros_usados[10] = {false}; // Vetor de flags para controlar números já usados
    int i, index; // Declaração das variáveis de controle

    for (i = 0; i < tamanho; i++) {
        do {
            index = rand() % 10; // Gera um índice aleatório entre 0 e 9
        } while (numeros_usados[index]); // Verifica se o número já foi usado, repetindo se necessário

        vetor[i] = index; // Atribui o número ao vetor
        numeros_usados[index] = true; // Marca o número como usado
    }
}

// Função para calcular o valor acumulado das distâncias euclidianas de uma rota especificada por um vetor
double gerarSomaValor(int vetor[], int tamanho, double Dist[][10]) {
    double soma = 0; // Inicializa a soma das distâncias com zero
    int i; // Declaração da variável de controle

    for (i = 0; i < tamanho - 1; i++) {
        int primeiroPonto = vetor[i]; // Primeiro ponto da rota
        int segundoPonto = vetor[i + 1]; // Segundo ponto da rota
        soma += Dist[primeiroPonto][segundoPonto]; // Adiciona a distância entre os pontos consecutivos
    }
    // Adiciona a distância do último ponto para o primeiro para fechar o ciclo
    soma += Dist[vetor[tamanho - 1]][vetor[0]];

    return soma; // Retorna o valor acumulado das distâncias
}

// Função para imprimir a população ordenada
void imprimirPopulacao(struct pop *populacao, int tamanho) {
    printf("Vetor População Ordenado:\n"); // Imprime o cabeçalho
    for (int i = 0; i < tamanho; i++) {
        printf("%d: Indiv: ", i); // Imprime o índice do indivíduo
        for (int j = 0; j < 10; j++) {
            printf("%d ", populacao[i].indiv[j]); // Imprime cada valor do vetor indivíduo
        }
        printf("Valor acumulado: %.2f\n", populacao[i].valor); // Imprime o valor acumulado da rota
    }
}

// Função de comparação para qsort, usada para ordenar a população pelo campo valor
int compararPopulacao(const void *a, const void *b) {
    const struct pop *pop1 = (const struct pop *)a; // Casting para a estrutura pop
    const struct pop *pop2 = (const struct pop *)b; // Casting para a estrutura pop

    // Compara os valores acumulados das rotas
    if (pop1->valor < pop2->valor) return -1; // Retorna -1 se o valor de pop1 é menor que pop2
    else if (pop1->valor > pop2->valor) return 1; // Retorna 1 se o valor de pop1 é maior que pop2
    else return 0; // Retorna 0 se os valores são iguais
}

// Função para gerar o vetor de população com indivíduos únicos e calcular seus valores
void gerarVetorPop(double Dist[][10]) {
    int tamanhoVetorPop = 100; // Tamanho do vetor população
    struct pop *populacao = malloc(tamanhoVetorPop * sizeof(struct pop)); // Aloca espaço na memória para o vetor população
    int i; // Declaração da variável de controle

    for (i = 0; i < tamanhoVetorPop; i++) {
        gerarNumerosUnicos(populacao[i].indiv, 10); // Gera indivíduos com valores únicos
        populacao[i].valor = gerarSomaValor(populacao[i].indiv, 10, Dist); // Calcula o valor acumulado da rota para cada indivíduo
    }

    // Ordena o vetor população pelo campo valor usando qsort
    qsort(populacao, tamanhoVetorPop, sizeof(struct pop), compararPopulacao);

    // Imprime o vetor população ordenado
    imprimirPopulacao(populacao, tamanhoVetorPop);

    free(populacao); // Libera a memória alocada para a população
}

// Função principal que executa o programa
int main() {
    int tamanhoMax = 10; // Define o tamanho máximo do vetor de pontos
    double Dist[tamanhoMax][tamanhoMax]; // Declara a matriz de distâncias

    struct ponto *pontos = gerarVetorPontos(tamanhoMax); // Gera o vetor de pontos

    gerarMatrizDist(pontos, tamanhoMax, Dist); // Gera a matriz de distâncias euclidianas

    gerarVetorPop(Dist); // Gera o vetor de população com base na matriz de distâncias

    free(pontos); // Libera a memória alocada para o vetor de pontos
}
