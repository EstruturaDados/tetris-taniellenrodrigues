#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define TAM_FILA 5
#define TAM_PILHA 3

typedef struct {
    char nome;
    int id;
} Peca;

typedef struct {
    Peca pecas[TAM_FILA];
    int frente;
    int tras;
    int qtd;
} Fila;

typedef struct {
    Peca pecas[TAM_PILHA];
    int topo;
} Pilha;

// ======== Funções básicas ========

Peca gerarPeca(int id) {
    Peca p;
    char tipos[] = {'I', 'O', 'T', 'L'};
    p.nome = tipos[rand() % 4];
    p.id = id;
    return p;
}

void iniciarFila(Fila* f) {
    f->frente = 0;
    f->tras = -1;
    f->qtd = 0;
}

void iniciarPilha(Pilha* p) {
    p->topo = -1;
}

int filaCheia(Fila* f) { return f->qtd == TAM_FILA; }
int filaVazia(Fila* f) { return f->qtd == 0; }
int pilhaCheia(Pilha* p) { return p->topo == TAM_PILHA - 1; }
int pilhaVazia(Pilha* p) { return p->topo == -1; }

void enqueue(Fila* f, Peca p) {
    if (filaCheia(f)) return;
    f->tras = (f->tras + 1) % TAM_FILA;
    f->pecas[f->tras] = p;
    f->qtd++;
}

Peca dequeue(Fila* f) {
    Peca p = {'-', -1};
    if (filaVazia(f)) return p;
    p = f->pecas[f->frente];
    f->frente = (f->frente + 1) % TAM_FILA;
    f->qtd--;
    return p;
}

void push(Pilha* p, Peca item) {
    if (pilhaCheia(p)) {
        printf("Pilha cheia! Nao e possivel reservar mais pecas.\n");
        return;
    }
    p->topo++;
    p->pecas[p->topo] = item;
}

Peca pop(Pilha* p) {
    Peca peca = {'-', -1};
    if (pilhaVazia(p)) {
        printf("Pilha vazia! Nao ha pecas reservadas.\n");
        return peca;
    }
    peca = p->pecas[p->topo];
    p->topo--;
    return peca;
}

void exibirEstado(Fila* f, Pilha* p) {
    printf("\n================ ESTADO ATUAL ================\n");
    printf("Fila de pecas:\t");
    int i = f->frente;
    for (int c = 0; c < f->qtd; c++) {
        printf("[%c %d] ", f->pecas[i].nome, f->pecas[i].id);
        i = (i + 1) % TAM_FILA;
    }
    printf("\n");

    printf("Pilha de reserva (Topo -> Base):\t");
    if (pilhaVazia(p))
        printf("(vazia)");
    else
        for (int i = p->topo; i >= 0; i--)
            printf("[%c %d] ", p->pecas[i].nome, p->pecas[i].id);
    printf("\n==============================================\n");
}

// ======== Função principal ========

int main() {
    srand(time(NULL));

    Fila fila;
    Pilha pilha;
    iniciarFila(&fila);
    iniciarPilha(&pilha);

    int id = 0;
    for (int i = 0; i < TAM_FILA; i++) enqueue(&fila, gerarPeca(id++));

    int opcao;
    do {
        exibirEstado(&fila, &pilha);
        printf("\n1. Jogar peca\n2. Reservar peca\n3. Usar peca reservada\n0. Sair\nEscolha: ");
        scanf("%d", &opcao);

        if (opcao == 1) {
            if (!filaVazia(&fila)) {
                Peca jogada = dequeue(&fila);
                printf("Peca [%c %d] jogada!\n", jogada.nome, jogada.id);
                enqueue(&fila, gerarPeca(id++)); // mantém fila cheia
            } else {
                printf("Fila vazia!\n");
            }
        }

        else if (opcao == 2) {
            if (!filaVazia(&fila)) {
                if (!pilhaCheia(&pilha)) {
                    Peca reservada = dequeue(&fila);
                    printf("Peca [%c %d] movida para a reserva.\n", reservada.nome, reservada.id);
                    push(&pilha, reservada);
                    enqueue(&fila, gerarPeca(id++));
                } else {
                    printf("Nao e possivel reservar: pilha cheia!\n");
                }
            }
        }

        else if (opcao == 3) {
            Peca usada = pop(&pilha);
            if (usada.id != -1) {
                printf("Peca [%c %d] usada da reserva.\n", usada.nome, usada.id);
                // não volta pra fila, apenas “usada”
            }
        }

    } while (opcao != 0);

    printf("\nEncerrando o jogo...\n");
    return 0;
}
