#include <stdio.h> //biblioteca que fornece funções para entrada e saída de dados (por ex: printf e scanf)
#include <stdlib.h> //biblioteca que fornece funções básicas em C (alocação de memória, conversões de strings)
#include <time.h> //biblioteca para inicializar a semente do gerador de números aleatórios  
#include <math.h> //biblioteca que fornece funções matemáticas (por ex: raíz quadrada "sqrt")
#include <stdbool.h> //biblioteca que define o tipo de dados bool

//1° A estrutura deste registro é composta pelos campos: Id (inteiro identificador do ponto), PX e PY (reais que representam a latitude e longitude dos pontos.
//DEFINIÇÃO DO REGISTRO PARA O VETOR PONTOS
struct ponto {
	int id; //identificador dos pontos
	double px; //longitude dos pontos
	double py; //latitude dos pontos
};

//5° A estrutura deste registro é composta pelos campos: Individuo (vetor de 10 elementos inteiros) e Valor (real para acumular custo da rota)
//DEFINIÇÃO DA ESTRUTURA PARA O VETOR POPULACAO
struct pop {
	double valor; //valor acumulado do custa da rota
	int indiv[10]; //definição do vetor indivíduo de 10 elementos inteiros
};

//função para gerar número aleatório double
double gerarDouble() {
    return ((double)rand() / RAND_MAX) * 360.0; //de 0 à 360
}

//2° Construir um vetor chamado Pontos com 10 elementos do tipo registro.
//FUNÇÃO PARA GERAR O VETOR PONTOS
struct ponto *gerarVetorPontos(int tamanhoMax) {
    srand(time(NULL)); //Inicializa o gerador de números aleatórios
    struct ponto *pontos = malloc(tamanhoMax * sizeof(struct ponto)); // aloca espaco na memoria para um vetor do tipo registro de 10 elementos
    
    //3° O valor de Id deverá coincidir com a posição no vetor e os valores de PX e PY deverão ser preenchidos randomicamente
    for (int i = 0; i < tamanhoMax; i++) { // popula o vetor
        pontos[i].id = i; // O valor de Id deverá coincidir com a posição no vetor 
        pontos[i].px = gerarDouble(); // Chama a função gerarDouble e gera o valor aleatório da coordenada no eixo x
        pontos[i].py = gerarDouble(); // Chama a função gerarDouble e gera o valor aleatório da coordenada no eixo y
    }
    
    return pontos; // retorna o vetor de pontos gerado
}

//função para calcular a distância euclideana
double distEuclideana(struct ponto p1, struct ponto p2) {
	double dx = p1.px - p2.px; // operação entre um grupo de pontos. (Pxa-Pxb)²
	double dy = p1.py - p2.py; // msm coisa aqui... (Pya-Pyb)²
	return sqrt(dx * dx + dy * dy); // (Pxa-Pxb)²+(Pya-Pyb)²
}

//4° Com o vetor Pontos preenchidos, deverá ser criada uma matriz chamada Dist, 10X10, contendo as distâncias euclideanas entre os pontos
void gerarMatrizDist(struct ponto *pontos, int tamanhoMax, double Dist[tamanhoMax][tamanhoMax]) {
    // Calculando distâncias euclidianas e preenchendo a matriz Dist
    for (int eixoY = 0; eixoY < tamanhoMax; eixoY++) {
        for (int eixoX = 0; eixoX < tamanhoMax; eixoX++) {
            Dist[eixoY][eixoX] = distEuclideana(pontos[eixoY], pontos[eixoX]);
            //Itera no eixo X 10 vezes.
            //Quando termina iteração, volta para o for de cima e itera ele ( no caso seria o eixo y).
        }
    }
	
    printf("Matriz de Distâncias Euclidianas:\n");
    for (int i = 0; i < tamanhoMax; i++) {
        for (int j = 0; j < tamanhoMax; j++) {
            printf("%.2f\t", Dist[i][j]);
        }
        printf("\n");
    } 
    printf("\n");
}

//7° Os vetores indivíduos deverão ser preenchidos com valores de 1 a 10 sem repetição
void gerarNumerosUnicos(int vetor[], int tamanho) {
    bool numeros_usados[10] = {false}; // Vetor de flags para controlar números usados
    int i, index;

    for (i = 0; i < tamanho; i++) {
        do {
            index = rand() % 10; // Gera um índice aleatório entre 0 e 9
        } while (numeros_usados[index]); // Verifica se o número já foi usado

        vetor[i] = index; // Atribui o número ao vetor
        numeros_usados[index] = true; // Marca o número como usado
    }
}

//8° O campo valor será o somatório das distâncias euclidianas entre estes pontos (já calculadas na matriz Dist)
double gerarSomaValor(int vetor[], int tamanho, double Dist[][10]){
	double soma = 0;
	int i;

    for (i = 0; i < tamanho - 1; i++) {
        int primeiroPonto = vetor[i];
        int segundoPonto = vetor[i + 1];
        soma += Dist[primeiroPonto][segundoPonto];
    }
    // Adiciona a distância do último para o primeiro ponto para fechar o ciclo
    soma += Dist[vetor[tamanho - 1]][vetor[0]];
    
    return soma;
}

void imprimirPopulacao(struct pop *populacao, int tamanho) {
    printf("Vetor População Ordenado:\n");
    for (int i = 0; i < tamanho; i++) {
        printf("%d: Indiv: ", i);
        for (int j = 0; j < 10; j++) {
            printf("%d ", populacao[i].indiv[j]);
        }
        printf("Valor acumulado: %.2f\n", populacao[i].valor);
    }
}

//9° Após ser preenchido e calculado, o vetor população deverá ser ordenado pelo campo valor.
// Função de comparação para qsort
int compararPopulacao(const void *a, const void *b) {
    const struct pop *pop1 = (const struct pop *)a;
    const struct pop *pop2 = (const struct pop *)b;
    
    if (pop1->valor < pop2->valor) return -1;
    else if (pop1->valor > pop2->valor) return 1;
    else return 0;
}

//6° Após calcular a matriz, construir um vetor chamado População com 100 elementos do tipo registro. 
// Função para gerar vetor de população com indivíduos únicos
void gerarVetorPop(double Dist[][10]) {
    int tamanhoVetorPop = 100;
    struct pop *populacao = malloc(tamanhoVetorPop * sizeof(struct pop));
    int i;
    
    for (i = 0; i < tamanhoVetorPop; i++) {
        gerarNumerosUnicos(populacao[i].indiv, 10);
        populacao[i].valor = gerarSomaValor(populacao[i].indiv, 10, Dist);
	}
    
    // Ordenar o vetor populacao pelo campo valor usando qsort
    qsort(populacao, tamanhoVetorPop, sizeof(struct pop), compararPopulacao);
    
    // Imprimir o vetor populacao ordenado
    imprimirPopulacao(populacao, tamanhoVetorPop);
    
    free(populacao);
}

int main(){
	int tamanhoMax = 10;
	double Dist[tamanhoMax][tamanhoMax]; // matriz de distâncias
	
    struct ponto *pontos = gerarVetorPontos(tamanhoMax);

    gerarMatrizDist(pontos, tamanhoMax, Dist);
    
    gerarVetorPop(Dist);
    
    free(pontos);
}
