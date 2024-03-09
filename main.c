#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include "navio.h"

int main() {
    FilaNavios fila;
    inicializaFila(&fila); // Inicializa a fila
    char escolha;
    char idNavio[50];
    int peso, pilha;
    
    setlocale(LC_ALL,"");

    do {
        printf("\n--- Menu de Gerenciamento do Porto ---\n");
        printf("1. Adicionar Navio\n");
        printf("2. Adicionar Contentor\n");
        printf("3. Desempilhar Contentor\n");
        printf("4. Remover Navio\n");
        printf("5. Imprimir Fila de Navios\n");
        printf("6. Sair\n");
        printf("Escolha uma opção: ");
        scanf(" %c", &escolha);

        switch (escolha) {
            case '1':
                printf("Digite o ID do navio: ");
                scanf("%s", idNavio);
                adicionaNavio(&fila, idNavio);
                break;
            case '2':
                printf("Digite o ID do navio: ");
                scanf("%s", idNavio);
                Navio *navio = buscaNavio(&fila, idNavio);
                if (navio == NULL) {
                    printf("Navio não encontrado.\n");
                    break;
                }
                printf("Digite o peso do contentor: ");
                scanf("%d", &peso);
                printf("Digite o número da pilha (1-3): ");
                scanf("%d", &pilha);
                if (!adicionaContentor(navio, peso, pilha - 1)) {
                    printf("Não foi possível adicionar o contentor.\n");
                }
                break;
            case '3':
                printf("Digite o ID do navio: ");
                scanf("%s", idNavio);
                printf("Digite o número da pilha (1-3): ");
                scanf("%d", &pilha);
                printf("Digite o peso do contentor a ser desempilhado: ");
                scanf("%d", &peso);
                if (!desempilhar(&fila, idNavio, pilha - 1, peso)) {
                    printf("Não foi possível desempilhar o contentor.\n");
                }
                break;
            case '4':
                printf("Digite o ID do navio para remover: ");
                scanf("%s", idNavio);
                removeNavio(&fila, idNavio);
                break;
            case '5':
                imprimeFila(&fila);
                break;
            case '6':
                printf("Saindo do programa...\n");
                break;
            default:
                printf("Opção inválida. Tente novamente.\n");
        }
    } while (escolha != '6');

    return 0;
}

