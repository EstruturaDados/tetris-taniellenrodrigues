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

// ==== Funções auxiliares ====
Peca gerarPeca(int id) {
    Peca p;
    char tipos[] = {'I', 'O', 'T', 'L'};
    p.nome = tipos[rand() % 4];
    p.id = id;
    return p;
}

// ==== Fila ====
void iniciarFila(Fila* f) {
    f->frente = 0;
    f->tras = -1;
    f->qtd = 0;
}

int filaVazia(Fila* f) { return f->qtd == 0; }
int filaCheia(Fila* f) { return f->qtd == TAM_FILA; }

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

// ==== Pilha ====
void iniciarPilha(Pilha* p) { p->topo = -1; }
int pilhaVazia(Pilha* p) { return p->topo == -1; }
int pilhaCheia(Pilha* p) { return p->topo == TAM_PILHA - 1; }

void push(Pilha* p, Peca item) {
    if (pilhaCheia(p)) {
        printf("Pilha cheia! Nao e possivel reservar mais pecas.\n");
        return;
    }
    p->pecas[++p->topo] = item;
}

Peca pop(Pilha* p) {
    Peca peca = {'-', -1};
    if (pilhaVazia(p)) {
        printf("Pilha vazia! Nao ha pecas reservadas.\n");
        return peca;
    }
    return p->pecas[p->topo--];
}

// ==== Exibir estado ====
void exibirEstado(Fila* f, Pilha* p) {
    printf("\n================ ESTADO ATUAL ================\n");

    printf("Fila de pecas:\t");
    if (filaVazia(f))
        printf("(vazia)");
    else {
        int i = f->frente;
        for (int c = 0; c < f->qtd; c++) {
            printf("[%c %d] ", f->pecas[i].nome, f->pecas[i].id);
            i = (i + 1) % TAM_FILA;
        }
    }

    printf("\nPilha de reserva (Topo -> Base):\t");
    if (pilhaVazia(p))
        printf("(vazia)");
    else
        for (int i = p->topo; i >= 0; i--)
            printf("[%c %d] ", p->pecas[i].nome, p->pecas[i].id);

    printf("\n==============================================\n");
}

// ==== Trocas ====
void trocarPeçaAtual(Fila* f, Pilha* p) {
    if (filaVazia(f) || pilhaVazia(p)) {
        printf("Nao e possivel trocar: fila ou pilha vazia.\n");
        return;
    }

    int idxFila = f->frente;
    Peca temp = f->pecas[idxFila];
    f->pecas[idxFila] = p->pecas[p->topo];
    p->pecas[p->topo] = temp;

    printf("Troca entre a peca da frente e o topo realizada!\n");
}

void trocaMultipla(Fila* f, Pilha* p) {
    if (f->qtd < 3 || p->topo < 2) {
        printf("Nao e possivel trocar: faltam pecas suficientes.\n");
        return;
    }

    int idxFila = f->frente;
    for (int i = 0; i < 3; i++) {
        int idx = (idxFila + i) % TAM_FILA;
        Peca temp = f->pecas[idx];
        f->pecas[idx] = p->pecas[p->topo - i];
        p->pecas[p->topo - i] = temp;
    }

    printf("Troca multipla realizada entre fila e pilha!\n");
}

// ==== Main ====
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
        printf("\n1. Jogar peca\n2. Reservar peca\n3. Usar peca reservada\n4. Trocar peca atual\n5. Troca multipla\n0. Sair\nEscolha: ");
        scanf("%d", &opcao);

        switch (opcao) {
            case 1: {
                if (!filaVazia(&fila)) {
                    Peca jogada = dequeue(&fila);
                    printf("Peca [%c %d] jogada!\n", jogada.nome, jogada.id);
                    enqueue(&fila, gerarPeca(id++));
                } else {
                    printf("Fila vazia!\n");
                }
                break;
            }

            case 2: {
                if (!filaVazia(&fila) && !pilhaCheia(&pilha)) {
                    Peca reservada = dequeue(&fila);
                    push(&pilha, reservada);
                    printf("Peca [%c %d] movida para a reserva.\n", reservada.nome, reservada.id);
                    enqueue(&fila, gerarPeca(id++));
                } else {
                    printf("Nao foi possivel reservar peca.\n");
                }
                break;
            }

            case 3: {
                Peca usada = pop(&pilha);
                if (usada.id != -1)
                    printf("Peca [%c %d] usada da reserva.\n", usada.nome, usada.id);
                break;
            }

            case 4:
                trocarPeçaAtual(&fila, &pilha);
                break;

            case 5:
                trocaMultipla(&fila, &pilha);
                break;

            case 0:
                printf("\nEncerrando o jogo...\n");
                break;

            default:
                printf("Opcao invalida!\n");
        }

    } while (opcao != 0);

    return 0;
}
