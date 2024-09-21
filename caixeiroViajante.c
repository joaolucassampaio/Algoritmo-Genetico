#include <stdio.h>    
#include <stdlib.h>   
#include <time.h>     
#include <math.h>     
#include <stdbool.h>  

struct ponto {
    int id;        
    double px;     
    double py;     
};

struct pop {
    double valor;    
    int indiv[10];   
};

double gerarDouble() {
    return ((double)rand() / RAND_MAX) * 360.0;  
}

struct ponto *gerarVetorPontos(int tamanhoMax) {
    srand(time(NULL));  
    struct ponto *pontos = malloc(tamanhoMax * sizeof(struct ponto));  
    
    int i;
    for (i = 0; i < tamanhoMax; i++) {
        pontos[i].id = i;                 
        pontos[i].px = gerarDouble();     
        pontos[i].py = gerarDouble();     
    }
    
    return pontos;  
}

double distEuclideana(struct ponto p1, struct ponto p2) {
    double dx = p1.px - p2.px;   
    double dy = p1.py - p2.py;   
    return sqrt(dx * dx + dy * dy);  
}

void gerarMatrizDist(struct ponto *pontos, int tamanhoMax, double Dist[tamanhoMax][tamanhoMax]) {
    int eixoY, eixoX, i, j;
    
    for (eixoY = 0; eixoY < tamanhoMax; eixoY++) {
        for (eixoX = 0; eixoX < tamanhoMax; eixoX++) {
            Dist[eixoY][eixoX] = distEuclideana(pontos[eixoY], pontos[eixoX]);  
        }
    }
    
    printf("Matriz de Distancias Euclidianas:\n");
    for (i = 0; i < tamanhoMax; i++) {
        for (j = 0; j < tamanhoMax; j++) {
            printf("%.2f\t", Dist[i][j]);  
        }
        printf("\n");
    }
    printf("\n");
}

void gerarNumerosUnicos(int vetor[], int tamanho) {
    bool numeros_usados[10] = { false };  
    int i, index;

    for (i = 0; i < tamanho; i++) {
        do {
            index = rand() % 10;  
        } while (numeros_usados[index]);  

        vetor[i] = index;  
        numeros_usados[index] = true;  
    }
}

double gerarSomaValor(int vetor[], int tamanho, double Dist[][10]) {
    double soma = 0;

    int i;
    for (i = 0; i < tamanho - 1; i++) {
        int primeiroPonto = vetor[i];
        int segundoPonto = vetor[i + 1];
        soma += Dist[primeiroPonto][segundoPonto];
    }
    soma += Dist[vetor[tamanho - 1]][vetor[0]];
    
    return soma;
}

void imprimirPopulacao(struct pop *populacao, int tamanho) {
    printf("Vetor Populacao Ordenado:\n");
    int i, j;
    for (i = 0; i < tamanho; i++) {
        printf("%d: Indiv: ", i);
        for (j = 0; j < 10; j++) {
            printf("%d ", populacao[i].indiv[j]);
        }
        printf("Valor acumulado: %.2f\n", populacao[i].valor);
    }
}

int compararPopulacao(const void *a, const void *b) {
    const struct pop *pop1 = (const struct pop *)a;
    const struct pop *pop2 = (const struct pop *)b;
    
    if (pop1->valor < pop2->valor) return -1;
    else if (pop1->valor > pop2->valor) return 1;
    else return 0;
}

void gerarVetorPop(double Dist[][10]) {
    int tamanhoVetorPop = 100;  
    struct pop *populacao = malloc(tamanhoVetorPop * sizeof(struct pop));  
    
    int i;
    for (i = 0; i < tamanhoVetorPop; i++) {
        gerarNumerosUnicos(populacao[i].indiv, 10);  
        populacao[i].valor = gerarSomaValor(populacao[i].indiv, 10, Dist);  
    }
    
    qsort(populacao, tamanhoVetorPop, sizeof(struct pop), compararPopulacao);
    
    imprimirPopulacao(populacao, tamanhoVetorPop);
    
    free(populacao);
}

int main() {
    int tamanhoMax = 10;  
    double Dist[tamanhoMax][tamanhoMax];  
    
    struct ponto *pontos = gerarVetorPontos(tamanhoMax);  
    
    gerarMatrizDist(pontos, tamanhoMax, Dist);  
    
    gerarVetorPop(Dist);  
    
    free(pontos);  
}
