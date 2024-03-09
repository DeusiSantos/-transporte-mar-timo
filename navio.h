#ifndef NAVIO_H
#define NAVIO_H

#define MAX_PILHAS_POR_NAVIO 3
#define MAX_CONTENTORES_POR_PILHA 3
#define MAX_ID_NAVIO 50

// Estrutura para um contentor.
typedef struct contentor {
    int peso;
    struct contentor *proximo;
} Contentor;

// Estrutura para uma pilha de contentores.
typedef struct pilhaContentores {
    Contentor *topo;
    int quantidade;
} PilhaContentores;

// Estrutura para um navio.
typedef struct navio {
    char id[MAX_ID_NAVIO];
    PilhaContentores pilhas[MAX_PILHAS_POR_NAVIO];
    struct navio *proximo;
} Navio;

// Estrutura para uma fila de navios.
typedef struct filaNavios {
    Navio *inicio;
    Navio *fim;
} FilaNavios;

// Protótipos das funções.
void inicializaFila(FilaNavios *fila);
void adicionaNavio(FilaNavios *fila, char *id);
Navio *buscaNavio(FilaNavios *fila, char *id);
int adicionaContentor(Navio *navio, int peso, int pilha);
void removeNavio(FilaNavios *fila, char *id);
void imprimeFila(FilaNavios *fila);

#endif // NAVIO_H

