#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "navio.h"

void inicializaFila(FilaNavios *fila) {
    fila->inicio = fila->fim = NULL;
}

void adicionaNavio(FilaNavios *fila, char *id) {
    if (buscaNavio(fila, id)) {
        printf("Navio com ID '%s' j� existe.\n", id);
        return;
    }

    Navio *novoNavio = (Navio *)malloc(sizeof(Navio));
    if (!novoNavio) {
        printf("Erro ao alocar mem�ria para o novo navio.\n");
        return;
    }

    strncpy(novoNavio->id, id, MAX_ID_NAVIO - 1);
    novoNavio->id[MAX_ID_NAVIO - 1] = '\0';
    int i;
    for (i = 0; i < MAX_PILHAS_POR_NAVIO; i++) {
        novoNavio->pilhas[i].topo = NULL;
        novoNavio->pilhas[i].quantidade = 0;
    }
    novoNavio->proximo = NULL;

    if (!fila->inicio) {
        fila->inicio = fila->fim = novoNavio;
    } else {
        fila->fim->proximo = novoNavio;
        fila->fim = novoNavio;
    }
}

Navio *buscaNavio(FilaNavios *fila, char *id) {
    Navio *atual = fila->inicio;
    while (atual) {
        if (strcmp(atual->id, id) == 0) {
            return atual;
        }
        atual = atual->proximo;
    }
    return NULL;
}

int adicionaContentor(Navio *navio, int peso, int pilha) {
    if (pilha < 0 || pilha >= MAX_PILHAS_POR_NAVIO) {
        printf("N�mero de pilha inv�lido.\n");
        return 0;
    }

    if (navio->pilhas[pilha].quantidade >= MAX_CONTENTORES_POR_PILHA) {
        printf("Pilha cheia.\n");
        return 0;
    }

    if (navio->pilhas[pilha].topo && navio->pilhas[pilha].topo->peso < peso) {
        printf("Contentor mais pesado n�o pode ser colocado em cima do mais leve.\n");
        return 0;
    }

    Contentor *novoContentor = (Contentor *)malloc(sizeof(Contentor));
    if (!novoContentor) {
        printf("Erro ao alocar mem�ria para o contentor.\n");
        return 0;
    }

    novoContentor->peso = peso;
    novoContentor->proximo = navio->pilhas[pilha].topo;
    navio->pilhas[pilha].topo = novoContentor;
    navio->pilhas[pilha].quantidade++;

    return 1;
}

void imprimeFila(FilaNavios *fila) {
    Navio *atual = fila->inicio;
    int numeroNavio = 1;

    while (atual != NULL) {
        printf("Navio %d (ID: %s):\n", numeroNavio, atual->id);
        int i;
        for (i = 0; i < MAX_PILHAS_POR_NAVIO; i++) {
            printf("  Pilha %d:", i + 1);
            Contentor *contentorAtual = atual->pilhas[i].topo;
            if (contentorAtual == NULL) {
                printf(" Vazia\n");
                continue;
            }

            while (contentorAtual != NULL) {
                printf(" [%d]", contentorAtual->peso);
                contentorAtual = contentorAtual->proximo;
            }
            printf("\n");
        }

        atual = atual->proximo;
        numeroNavio++;
    }
}

void removeNavio(FilaNavios *fila, char *id) {
    if (fila->inicio == NULL) {
        printf("Fila vazia.\n");
        return;
    }

    Navio *atual = fila->inicio;
    Navio *anterior = NULL;

    while (atual != NULL && strcmp(atual->id, id) != 0) {
        anterior = atual;
        atual = atual->proximo;
    }

    if (atual == NULL) {
        printf("Navio com ID '%s' n�o encontrado.\n", id);
        return;
    }

    if (anterior == NULL) {
        fila->inicio = atual->proximo;
    } else {
        anterior->proximo = atual->proximo;
    }

    if (atual == fila->fim) {
        fila->fim = anterior;
    }

    // Liberar a mem�ria dos contentores
    int i;
    for (i = 0; i < MAX_PILHAS_POR_NAVIO; i++) {
        Contentor *atualContentor = atual->pilhas[i].topo;
        while (atualContentor != NULL) {
            Contentor *temp = atualContentor;
            atualContentor = atualContentor->proximo;
            free(temp);
        }
    }

    // Liberar a mem�ria do navio
    free(atual);
    printf("Navio com ID '%s' removido.\n", id);
}

// Fun��o para desempilhar um contentor espec�fico de uma pilha em um navio.
// Retorna 1 se a remo��o foi bem-sucedida, e 0 caso contr�rio.
int desempilhar(FilaNavios *fila, char *idNavio, int numPilha, int pesoContentor) {
    // Verificar se a fila est� inicializada.
    if (fila == NULL || fila->inicio == NULL) {
        printf("Fila vazia ou n�o inicializada.\n");
        return 0;
    }

    // Procurar pelo navio com o ID especificado.
    Navio *navio = buscaNavio(fila, idNavio);
    if (navio == NULL) {
        printf("Navio com ID '%s' n�o encontrado.\n", idNavio);
        return 0;
    }

    // Verificar se o n�mero da pilha � v�lido.
    if (numPilha < 0 || numPilha >= MAX_PILHAS_POR_NAVIO) {
        printf("N�mero da pilha inv�lido.\n");
        return 0;
    }

    // Verificar se a pilha n�o est� vazia.
    PilhaContentores *pilha = &navio->pilhas[numPilha];
    if (pilha->topo == NULL) {
        printf("A pilha %d est� vazia.\n", numPilha + 1);
        return 0;
    }

    // Procurar pelo contentor com o peso especificado.
    Contentor *atual = pilha->topo;
    Contentor *anterior = NULL;
    while (atual != NULL && atual->peso != pesoContentor) {
        anterior = atual;
        atual = atual->proximo;
    }

    if (atual == NULL) {
        printf("Contentor com peso %d n�o encontrado na pilha %d.\n", pesoContentor, numPilha + 1);
        return 0;
    }

    // Remover o contentor encontrado.
    if (anterior == NULL) { // Contentor est� no topo.
        pilha->topo = atual->proximo;
    } else {
        anterior->proximo = atual->proximo;
    }
    pilha->quantidade--;
    free(atual);

    printf("Contentor com peso %d removido da pilha %d do navio %s.\n", pesoContentor, numPilha + 1, idNavio);
    return 1;
}





