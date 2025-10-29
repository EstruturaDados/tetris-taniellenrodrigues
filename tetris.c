#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define TAM 5 // tamanho fixo da fila

typedef struct {
    char nome; // tipo da peça (I, O, T, L)
    int id;    // identificador único
} Peca;

typedef struct {
    Peca fila[TAM];
    int frente;
    int tras;
    int qtd;
} Fila;

// gera uma peça nova aleatória
Peca gerarPeca(int id) {
    Peca p;
    char tipos[] = {'I', 'O', 'T', 'L'};
    p.nome = tipos[rand() % 4];
    p.id = id;
    return p;
}

// inicializa a fila
void iniciarFila(Fila* f) {
    f->frente = 0;
    f->tras = -1;
    f->qtd = 0;
}

// verifica se está cheia
int filaCheia(Fila* f) {
    return f->qtd == TAM;
}

// verifica se está vazia
int filaVazia(Fila* f) {
    return f->qtd == 0;
}

// insere nova peça no fim da fila
void enqueue(Fila* f, Peca p) {
    if (filaCheia(f)) {
        printf("Fila cheia! Nao e possivel adicionar nova peca.\n");
        return;
    }
    f->tras = (f->tras + 1) % TAM;
    f->fila[f->tras] = p;
    f->qtd++;
}

// remove a peça da frente
void dequeue(Fila* f) {
    if (filaVazia(f)) {
        printf("Fila vazia! Nao ha peca para jogar.\n");
        return;
    }
    Peca jogada = f->fila[f->frente];
    printf("Peca [%c %d] jogada!\n", jogada.nome, jogada.id);
    f->frente = (f->frente + 1) % TAM;
    f->qtd--;
}

// exibe o estado atual da fila
void exibirFila(Fila* f) {
    if (filaVazia(f)) {
        printf("\nFila vazia!\n");
        return;
    }
    printf("\nFila de pecas:\n");
    int i = f->frente;
    for (int c = 0; c < f->qtd; c++) {
        printf("[%c %d] ", f->fila[i].nome, f->fila[i].id);
        i = (i + 1) % TAM;
    }
    printf("\n");
}

int main() {
    srand(time(NULL));

    Fila fila;
    iniciarFila(&fila);

    int id = 0;

    // inicializa com 5 peças
    for (int i = 0; i < TAM; i++) {
        enqueue(&fila, gerarPeca(id++));
    }

    int opcao;
    do {
        exibirFila(&fila);
        printf("\n1. Jogar peca\n2. Inserir nova peca\n0. Sair\nEscolha: ");
        scanf("%d", &opcao);

        if (opcao == 1) {
            dequeue(&fila);
        } else if (opcao == 2) {
            enqueue(&fila, gerarPeca(id++));
        } else if (opcao != 0) {
            printf("Opcao invalida!\n");
        }

    } while (opcao != 0);

    printf("\nJogo encerrado. Até a proxima!\n");
    return 0;
}
