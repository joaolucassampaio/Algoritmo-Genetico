#include <stdio.h>
#include <stdlib.h>
#include <time.h> //biblioteca para gerar números aleatórios
#include <math.h>

//estrutura do registro
struct ponto {
	int id; //identificador dos pontos
	double px; //longitude dos pontos
	double py; //latitude dos pontos
};

//função para gerar número aleatório double
double gerarDouble() {
    return ((double)rand() / RAND_MAX) * 360.0; //de 0 à 360
}

struct ponto *gerarVetorPontos(int tamanhoMax) {
    srand(time(NULL)); // começa RNG
    struct ponto *pontos = malloc(tamanhoMax * sizeof(struct ponto)); // aloca espaco na memoria para um vetor do tipo registro de 10 itens
    
    for (int i = 0; i < tamanhoMax; i++) { // popula o vetor
        pontos[i].id = i; // id = i 
        pontos[i].px = gerarDouble(); // gera coordenada no eixo x
        pontos[i].py = gerarDouble(); // gera coordenada no eixo x
    }
    
    return pontos; // retorna o vetor de pontos gerado
}

//função para calcular a distância euclideana
double distEuclideana(struct ponto p1, struct ponto p2) {
	double dx = p1.px - p2.px; // operação entre um grupo de pontos. (Pxa-Pxb)²
	double dy = p1.py - p2.py; // msm coisa aqui... (Pya-Pyb)²
	return sqrt(dx * dx + dy * dy); // (Pxa-Pxb)²+(Pya-Pyb)²
}

void gerarMatrizDist(struct ponto *pontos, int tamanhoMax) {
    double Dist[tamanhoMax][tamanhoMax]; // matriz de distâncias

    // Calculando distâncias euclidianas e preenchendo a matriz Dist
    for (int eixoY = 0; eixoY < tamanhoMax; eixoY++) {
        for (int eixoX = 0; eixoX < tamanhoMax; eixoX++) {
            Dist[eixoY][eixoX] = distEuclideana(pontos[eixoY], pontos[eixoX]);
            //Itera no eixo X 10 vezes.
            //Quando termina iteração, volta para o for de cima e itera ele ( no caso seria o eixo y).
	
        }

    }
	
    // Exibindo a matriz Dist (opcional)
    printf("Matriz de Distâncias Euclidianas:\n");
    for (int i = 0; i < tamanhoMax; i++) {
        for (int j = 0; j < tamanhoMax; j++) {
            printf("%.2f\t", Dist[i][j]);
        }
        printf("\n");
    }
}

int main(){
	int tamanhoMax = 10;
    struct ponto *pontos=gerarVetorPontos(tamanhoMax);

    gerarMatrizDist(pontos, tamanhoMax);
    free(pontos);
}
