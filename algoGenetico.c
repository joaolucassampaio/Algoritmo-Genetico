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

struct pop* gerarVetorPop(double Dist[][10], int tamanhoVetorPop) {
    struct pop *populacao = malloc(tamanhoVetorPop * sizeof(struct pop));  
    
    int i;
    for (i = 0; i < tamanhoVetorPop; i++) {
        gerarNumerosUnicos(populacao[i].indiv, 10);  
        populacao[i].valor = gerarSomaValor(populacao[i].indiv, 10, Dist);  
    }
    
    qsort(populacao, tamanhoVetorPop, sizeof(struct pop), compararPopulacao);
    
    imprimirPopulacao(populacao, tamanhoVetorPop);
    
    return populacao;
}

struct pop cruzamento(struct pop *populacao, int A, int B, struct pop *novaPopulacao, double Dist[][10]) {
	struct pop paiGuia, filhoAtual, filhosIntermediarios[50];
	int numFilhos = 0;
	
	//1° Verificar qual pai tem o menor custo e definir quem será o Pai Guia e o Filho Atual
	if (populacao[A].valor > populacao[B].valor) {
        paiGuia = populacao[B];
        filhoAtual = populacao[A];
    } else {
        paiGuia = populacao[A];
        filhoAtual = populacao[B];
    }
   
    //2° Realizar os Swaps para conceber os filhos intermediários
	for(int i=0; i<10; i++){
		if(filhoAtual.indiv[i] != paiGuia.indiv[i]){
			for(int j = i+1; j<10; j++){
				if(filhoAtual.indiv[j] == paiGuia.indiv[i]){
					//Realiza o swap
					int temp = filhoAtual.indiv[i];
					filhoAtual.indiv[i] = filhoAtual.indiv[j];
					filhoAtual.indiv[j] = temp;
					
					//Após o swap, calcula o novo custo
					filhoAtual.valor = gerarSomaValor(filhoAtual.indiv, 10, Dist);
					
					//Armazena o filho intermediário
					filhosIntermediarios[numFilhos++] = filhoAtual;
					break;
				}
			}
		}
	}
	
	//3° Encontrar o melhor filho entre os intermediários
	struct pop filhoProdigo = filhoAtual;
	for (int i = 0; i < numFilhos; i++) {
        if (filhosIntermediarios[i].valor < filhoProdigo.valor) {
            filhoProdigo = filhosIntermediarios[i];
        }
    }
    
    return filhoProdigo;
}	
	

int roleta(struct pop *populacao, double custoTotal){
	double randomico = ((double)rand() / RAND_MAX) * custoTotal;
	//printf("\nValor randomico: %.2f\n", randomico);
	
	double somaCusto = 0.00;
	int posicao;
	
	for(int p = 99; p >= 0; p--){
		if(somaCusto < randomico){
			somaCusto += populacao[p].valor;
			posicao = p;
		}
	}
	//printf("A posicao escolhida foi: %d\n", posicao);
	
	return 99 - posicao;
}

double custoTotal(struct pop *populacao, int tamanhoVetorPop){
	double custoTotal = 0.0;
	for(int i=0; i < tamanhoVetorPop; i++){
		custoTotal += populacao[i].valor;
	}
	return custoTotal;
}

void mutacao(struct pop *individuo, double taxaMutacao, double Dist[][10]) {
    // Mutação de um único gene com probabilidade
    for (int i = 0; i < 10; i++) {
        // Decide aleatoriamente se ocorrerá mutação para esse gene
        if (((double)rand() / RAND_MAX) < taxaMutacao) {
            // Realiza uma mutação aleatória no gene
            individuo->indiv[i] = rand() % 10;  // Pode ser ajustado para mutar dentro de um intervalo específico
        }
    }

    // Recalcula o valor do indivíduo após a mutação
    individuo->valor = gerarSomaValor(individuo->indiv, 10, Dist);
}

void novasGeracoes(struct pop *populacao, double Dist[][10], double taxaMutacao) {
	double totalCusto = 0.0;
	int A, B;
	
	for(int i=0; i<100; i++){
		struct pop novaPopulacao[40];
		totalCusto = custoTotal(populacao, 100);
		
		for(int j=0; j<40; j++){
			A = roleta(populacao, totalCusto);
			B = roleta(populacao, totalCusto);
			novaPopulacao[j] = cruzamento(populacao, A, B, novaPopulacao, Dist);
			
			// Aplicando mutação com a taxa de mutação fornecida
			mutacao(&novaPopulacao[j], taxaMutacao, Dist);
		}
		
		//Sobrepor populacao2   em   população  (40 últimos elementos)
		for(int k=0; k<40; k++) {
            populacao[60 + k] = novaPopulacao[k];
        }
        
        qsort(populacao, 100, sizeof(struct pop), compararPopulacao);
        
        //printf("\nGeracao %d:\n", i + 1);
        //imprimirPopulacao(populacao, 100);
	}
}

int main() {
    int tamanhoMax = 10;  
    int tamanhoVetorPop = 100;
    double Dist[tamanhoMax][tamanhoMax];
    double taxaMutacao = 0.05; // 5% de chance de mutação em cada gene
    
    struct ponto *pontos = gerarVetorPontos(tamanhoMax);  
    
    gerarMatrizDist(pontos, tamanhoMax, Dist);  
    
    struct pop* populacao = gerarVetorPop(Dist, tamanhoVetorPop);
    
    novasGeracoes(populacao, Dist, taxaMutacao);
    
    free(pontos);  
}
