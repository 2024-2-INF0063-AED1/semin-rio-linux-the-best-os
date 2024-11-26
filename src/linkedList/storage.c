#include "storage.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

Node* aux;
Node* prev;
Node* selected;

List* create() {
    List* lista = (List*)malloc(sizeof(List));
    if (lista != NULL) {
        lista->start = NULL;
        lista->end = NULL;
        while(lista->memSize > 100 || lista->memSize < 0) {
            printf("Quantos porcentos da memoria quer utilizar?\n");
            scanf("%d", &lista->memSize);
        }
    } else {
        fprintf(stderr, "Erro ao alocar memoria para a lista.\n");
        exit(EXIT_FAILURE);
    }
    return lista;
}

Node* createAreaTransferencia() {
    Node* ctrlV = (Node*)malloc(sizeof(Node));
    if (ctrlV == NULL) {
        fprintf(stderr, "Erro ao alocar memoria para a area de transferencia.\n");
        exit(EXIT_FAILURE);
    }
    ctrlV->data.str = NULL;
    ctrlV->next = NULL;
    printf("Area de transferencia disponivel.\n");
    return ctrlV;
}

void insertStart(List* x) {
    Node* node = (Node*)malloc(sizeof(Node));
    if (node == NULL || verificarLimiarMemoriaWindows(x) == -1) {
        fprintf(stderr, "Erro ao alocar memoria para o no.\n");
        while(verificarLimiarMemoriaWindows() == -1) {
            rotinaApagarInicio(x);
        }
        insertStart(x);
        return;
    }

    while (verificarLimiarMemoriaWindows() == -1) {
        rotinaApagarInicio(x);
    }

    node->data.str = NULL;
    node->isPinned = 0;
    printf("Insira uma string para se copiar:\n");

    if (!copiarString(node)) {
        free(node);
        return;
    }

    node->next = x->start;
    x->start = node;
    if (x->end == NULL) {
        x->end = node;
    }

    printf("\nUm novo no foi adicionado ao inicio da lista.\n");
}

void insertEnd(List* x) {
    Node* node = (Node*)malloc(sizeof(Node));
    if (node == NULL || verificarLimiarMemoriaWindows(x) == -1) {
        fprintf(stderr, "Erro ao alocar memoria para o no.\n");
        while(verificarLimiarMemoriaWindows() == -1) {
            rotinaApagarInicio(x);
        }
        insertEnd(x);
        return;
    }

    while(verificarLimiarMemoriaWindows() == -1) {
        rotinaApagarInicio(x);
    }

    node->data.str = NULL;
    node->isPinned = 0;
    printf("Insira uma string para se copiar:\n");

    if (!copiarString(node)) {
        free(node);
        return;
    }

    node->next = NULL;
    if (x->start == NULL) {
        x->start = node;
        x->end = node;
    } else {
        x->end->next = node;
        x->end = node;
    }

    printf("Um novo no foi adicionado ao fim da lista.\n");
}

int copiarString(Node* cabeca) {
    size_t capacity = 128;
    size_t size = 0;
    char* string = malloc(capacity * sizeof(char));
    if (string == NULL) {
        fprintf(stderr, "Erro na alocacao de memoria inicial\n");
        return 0;
    }

    int ch;
    while ((ch = getchar()) != EOF) {
        if (ch == '\n' && size > 0 && string[size - 1] == '\n' && string[size - 2] == '\n') {
            break;
        }
        string[size++] = ch;
        if (size >= capacity) {
            capacity = (int)(strlen(string) + 128);
            char* temp = realloc(string, capacity * sizeof(char));
            if (temp == NULL) {
                fprintf(stderr, "Ocorreu algum problema na realocação\n");
                free(string);
                return 0;
            }
            string = temp;
        }
    }

    string[size] = '\0';
    string[size - 1] = '\0';
    string[size - 2] = '\0';

    if (selected) {
        cabeca->data.str = (char*)malloc(strlen(string) * sizeof(char));
        if (cabeca->data.str == NULL) {
            fprintf(stderr, "Erro na alocacao de memoria para a string do no\n");
            free(string);
            return 0;
        }
    } else {
        free(string);
        return 0;
    }

    if (selected->data.str == NULL || strcmp(selected->data.str, string) != 0) {
        selected->data.str = (char*)realloc(selected->data.str, (int)strlen(string) * sizeof(char));
        if (selected->data.str == NULL) {
            fprintf(stderr, "Erro na realocacao de memoria para a area de transferencia\n");
            free(string);
            return 0;
        }
        strcpy(selected->data.str, string);
        strcpy(cabeca->data.str, string);
    } else {
        printf("Houve duplicacao. Favor digitar uma string diferente da anterior.\n");
        free(string);
        return 0;
    }

    free(string);
    return 1;
}

void show(List* x) {
    if (x->start == NULL) {
        printf("Estado da lista: Lista vazia!\n");
        return;
    }
    aux = x->start;
    int i = 1;
    if (aux->data.str != NULL) {
        printf("Estado da lista:\n");
    }

    while (aux != NULL) {
        printf("%d  -   %s\n", i, aux->data.str);
        aux = aux->next;
        i++;
    }
}

void selectToPaste(List* x) {
    if (selected == NULL) {
        fprintf(stderr, "E necessário inicializar a Area de Transferencia antes.\n");
        return;
    }

    if (x->start == NULL) {
        fprintf(stderr, "Nao ha elementos na area de transferencia para serem colados.\n Favor copiar algum.");
        return;
    }

    show(x);
    int index;
    printf("Digite o indice que deseja copiar:\n");
    scanf("%d", &index);

    int i = 1;
    Node* aux = x->start;
    while (aux != NULL && i < index) {
        aux = aux->next;
        i++;
    }

    if (aux != NULL) {
        if (selected->data.str != NULL) {
            free(selected->data.str);
        }
        selected->data.str = (char*)malloc((strlen(aux->data.str) + 1) * sizeof(char));
        if (selected->data.str == NULL) {
            fprintf(stderr, "Erro na alocacao de memoria para a area de transferencia\n");
            return;
        }
        strcpy(selected->data.str, aux->data.str);
        paste();
    } else {
        printf("Indice invalido.\n");
    }
}

void paste() {
    if (selected) {
        printf("Area de transferencia:\n%s\n\n", selected->data.str);
    } else {
        fprintf(stderr, "\nFavor inicializar a area de transferencia.\n\n");
    }
}

void delete(List* x, int index) {
    if (index <= 0) {
        fprintf(stderr, "Indice passado deve ser maior do que zero.");
        return;
    }

    if (x->start == NULL) {
        printf("Lista vazia!\n");
        return;
    }

    show(x);

    aux = x->start;
    int i = 1;
    printf("Estado da lista:\n");
    while (i != index && aux != NULL) {
        prev = aux;
        aux = aux->next;
        i++;
    }

    if (aux == NULL) {
        printf("Indice invalido.\n");
        return;
    }

    if (aux == x->start) {
        x->start = aux->next;
        free(aux->data.str);
        aux->data.str = NULL;
        free(aux);
        aux = x->start;
    } else {
        prev->next = aux->next;
        free(aux->data.str);
        aux->data.str = NULL;
        free(aux);
    }

    printf("\nNo excluido com sucesso.\n");
}

void clean(List* x) {
    Node* atual = x->start;
    Node* anterior = NULL;

    while (atual != NULL) {
        if (atual->isPinned) {
            // Nó fixado: Avança sem deletar
            printf("Nó fixado não pode ser deletado: %s\n", atual->data.str);
            anterior = atual;
            atual = atual->next;
        } else {
            // Nó não fixado: Remove da lista
            if (anterior == NULL) {
                // Caso o nó seja o primeiro da lista
                x->start = atual->next;
            } else {
                // Nó intermediário ou final
                anterior->next = atual->next;
            }

            // Libera o nó atual
            free(atual->data.str);
            free(atual);

            // Avança para o próximo nó
            if (anterior == NULL) {
                atual = x->start;
            } else {
                atual = anterior->next;
            }
        }
    }

    printf("Área de transferência limpa com sucesso.\n");
}

void pinAndSave(List* x) {
    if (x->start == NULL) {
        printf("A lista está vazia! Por favor insira algum elemento.\n");
        return;
    }

    FILE *arq;

    int index = 0;
    int op = 1;

    while (op == 1){
        arq = fopen("pinnedItens.txt", "a");

        findNode(x, 0);
        index = findLastItemSaved();

        if(aux != NULL) {
            if(aux->isPinned != 1) {
                fprintf(arq, "%d|\t-\t%s\n", index, aux->data.str);
                aux->isPinned = 1;
            } else {
                fprintf(stderr,"\nItem ja consta salvo.Voce acaba de desafixar o item.\n");
                aux->isPinned = 0;
                removePinnedItem(aux->data.str);
                // TODO: retirar item salvo do arquivo pinnedItens.txt
                // Continua como uma incógnita.
            }
        }

        printf("\nContinuar? (0-nao / 1-sim): ");
        scanf("%d", &op);

        if(op == 1){
            fputs("\n", arq);
        }

        fclose(arq);
    }
}

void pinnedItensRoutine(List* x) {
    //TODO: Inserir  cada item fixado dentro da lista, novamente, ao inicializa-la
}

int findLastItemSaved() {
    FILE *arquivo = fopen("pinnedItens.txt", "r");
    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo.\n");
        return -1;
    }

    int indiceAtual = 0, ultimoIndice = 0;
    char linha[1024]; // Buffer para armazenar cada linha do arquivo

    while (fgets(linha, sizeof(linha), arquivo) != NULL) {
        // Verifica se a linha segue o formato correto "%d|"
        char* pipePos = strchr(linha, '|');
        if (pipePos != NULL && pipePos > linha) {
            // Garante que tudo antes do '|' é um número válido
            *pipePos = '\0'; // Temporariamente substitui '|' por '\0' para validar o número
            if (sscanf(linha, "%d", &indiceAtual) == 1) {
                if (indiceAtual > ultimoIndice) {
                    ultimoIndice = indiceAtual;
                }
            }
            *pipePos = '|'; // Restaura o '|' após validação
        }
    }

    fclose(arquivo);
    return ultimoIndice + 1; // Retorna o próximo índice
}

void rotinaApagarInicio(List* x) {
    if (x->start == NULL) {
        printf("A lista está vazia.\n");
        return;
    }

    Node* atual = x->start;
    Node* anterior = NULL;

    // Percorre a lista para encontrar um nó que possa ser removido
    while (atual != NULL) {
        if (!atual->isPinned) {
            // Nó não fixado encontrado: Remove-o
            if (anterior == NULL) {
                // Caso o nó a ser removido seja o primeiro
                x->start = atual->next;
            } else {
                // Nó intermediário ou final
                anterior->next = atual->next;
            }

            // Atualiza o ponteiro `end` se necessário
            if (atual == x->end) {
                x->end = anterior;
            }

            // Libera memória do nó removido
            free(atual->data.str);
            free(atual);

            printf("Um nó foi removido com sucesso.\n");
            return;
        }

        // Avança para o próximo nó
        anterior = atual;
        atual = atual->next;
    }

    // Se não encontrou nenhum nó removível
    printf("Nenhum nó pode ser removido, todos estão fixados.\n");
}

int verificarLimiarMemoriaWindows(List* x) {
    MEMORYSTATUSEX memInfo;
    memInfo.dwLength = sizeof(MEMORYSTATUSEX);

    if (!GlobalMemoryStatusEx(&memInfo)) {
        printf("Erro ao acessar informações de memória.\n");
        return 0;
    }

    unsigned long long int memTotal = memInfo.ullTotalPageFile / (1024 * 1024);
    unsigned long long int memDisponivel = memInfo.ullAvailPageFile / (1024 * 1024);

    if(memDisponivel >= x->memSize * memTotal) {
        return 1;
    }

    if (memDisponivel <= x->memSize * memTotal) {
        return -1;
    }

    return 0;
}

