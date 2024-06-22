#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

// Estrutura do registro 'ponto'
struct ponto {
    int id;
    double px;
    double py;
};

#define tamanhoVetorPop 10

// Estrutura do registro 'pop'
struct pop {
    double valor;
    int indv[tamanhoVetorPop];
};

// Função para gerar número aleatório double
double gerarDouble() {
    return ((double)rand() / RAND_MAX) * 360.0;
}

// Função para gerar números aleatórios únicos no intervalo de 1 a max
void gerarAleatoriosUnicos(int vetor[], int max) {
    int numeros[max];
    int i;
    for ( i = 0; i < max; ++i) {
        numeros[i] = i + 1; // Preenche com números de 1 a max
    }

    // Embaralha os números no vetor
    for (i = 0; i < max; ++i) {
        int j = rand() % (max - i) + i;
        int temp = numeros[i];
        numeros[i] = numeros[j];
        numeros[j] = temp;
    }

    // Copia os números embaralhados para o vetor de saída

    for (i = 0; i < tamanhoVetorPop; ++i) {
        vetor[i] = numeros[i];
    }
}

// Função para calcular a distância euclideana entre dois pontos
double distEuclideana(struct ponto p1, struct ponto p2) {
    double dx = p1.px - p2.px;
    double dy = p1.py - p2.py;
    return sqrt(dx * dx + dy * dy);
}

// Função para gerar a população
struct pop *gerarPop(int tamanhoPop, struct ponto *pontos, int tamanhoMax) {
    struct pop *populacao = malloc(tamanhoPop * sizeof(struct pop));
    if (populacao == NULL) {
        fprintf(stderr, "Erro ao alocar memória para a população.\n");
        exit(1);
    }

    srand(time(NULL));
    int i;

    for (i = 0; i < tamanhoPop; i++) {
        gerarAleatoriosUnicos(populacao[i].indv, tamanhoVetorPop);

        // Calcular o campo 'valor' como o somatório das distâncias euclidianas
        double somaDistancias = 0.0;
        
        int j;
        for (j = 0; j < tamanhoVetorPop - 1; j++) {
            int indice1 = populacao[i].indv[j] - 1; // Ajuste para índice do vetor 'pontos'
            int indice2 = populacao[i].indv[j + 1] - 1; // Ajuste para índice do vetor 'pontos'
            somaDistancias += distEuclideana(pontos[indice1], pontos[indice2]);
        }
        // Adicionar a distância do último ponto de volta ao primeiro
        int indice1 = populacao[i].indv[tamanhoVetorPop - 1] - 1;
        int indice2 = populacao[i].indv[0] - 1;
        somaDistancias += distEuclideana(pontos[indice1], pontos[indice2]);

        populacao[i].valor = somaDistancias;
    }

    return populacao;
}

// Função de comparação para o qsort
int compararPop(const void *a, const void *b) {
    const struct pop *pop1 = (const struct pop *)a;
    const struct pop *pop2 = (const struct pop *)b;
    return (pop1->valor > pop2->valor) - (pop1->valor < pop2->valor);
}

int main() {
    int tamanhoMax = 10; // Número de pontos
    int tamanhoPop = 100; // Tamanho da população

    // Gerar pontos aleatórios
    struct ponto *pontos = malloc(tamanhoMax * sizeof(struct ponto));
    if (pontos == NULL) {
        fprintf(stderr, "Erro ao alocar memória para os pontos.\n");
        return 1;
    }
    
    int i;

    for (i = 0; i < tamanhoMax; i++) {
        pontos[i].id = i;
        pontos[i].px = gerarDouble();
        pontos[i].py = gerarDouble();
    }

    // Gerar população
    struct pop *populacao = gerarPop(tamanhoPop, pontos, tamanhoMax);

    // Ordenar a população pelo campo 'valor'
    qsort(populacao, tamanhoPop, sizeof(struct pop), compararPop); // função de quicksort que vem com o C.
    
    printf("População ordenada pelo campo 'valor':\n");
    //Mostra todos os valores... percorre o vetor e printa todos os elementos.
    for (i = 0; i < tamanhoPop; ++i) {
        printf("Indv: ");
        int j;
        for (j = 0; j < tamanhoVetorPop; ++j) {
            printf("%d ", populacao[i].indv[j]);
        }
        printf("\tValor: %.2f\n", populacao[i].valor);
    }

    // Exibir o individuo da populacao com menor valor. A função de quicksort colocou-o na primeira posição
    // então só é necessário imprimir a primeira posição; 
    printf("Vetor com o menor valor:\n");
    printf("Indv: ");
    int j;
    for (j = 0; j < tamanhoVetorPop; ++j) {  // imprime o vetor individuo do primeiro struct pop.
        printf("%d ", populacao[0].indv[j]);
    } 
    printf("\tValor: %.2f\n", populacao[0].valor);

    // Liberar memória alocada
    free(pontos);
    free(populacao);

    return 0;
}
