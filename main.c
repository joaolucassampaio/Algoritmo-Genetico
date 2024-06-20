#include <stdio.h>
#include <stdlib.h>
#include <time.h> //biblioteca para gerar números aleatórios

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


void gerarVetorPontos() {
	srand(time(NULL)); //função para inicializar gerador de número aleatório
	
	struct ponto *pontos; //ponteiro auxiliar para estrutura do registro
	
	int tamanhoMax = 10; //quantidade total dos elementos do vetor
	
	pontos = malloc(tamanhoMax * sizeof(struct ponto)); //criando um vetor de 10 com 10 elementos do tipo registro através de uma alocação dinâmica
	
	for(int i = 0; i < tamanhoMax; i++){ //gerando cada campo do vetor (id, px e py)
		pontos[i].id = i; //identificação de cada ponto
		pontos[i].px = gerarDouble(); //posicação no eixo x dada por um número aleatório do tipo double
		pontos[i].py = gerarDouble(); //posicação no eixo y dada por um número aleatório do tipo double
		
		/*
		printf("%i\n", pontos[i].id);
		printf("%f\n", pontos[i].px);
		printf("%f\n", pontos[i].py);
		printf("\n\n");
		*/
	};
}

//função para calcular a distância euclideana
double distEuclideana(struct ponto p1, struct ponto p2) {
	double dx = p1.px - p2.px;
	double dy = p1.py - p2.py;
	return sqrt(dx * dx + dy * dy);
}



void gerarMatriz() {
	
}

int main(){
	gerarVetorPontos();
}
