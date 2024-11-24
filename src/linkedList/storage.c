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
    if (node == NULL) {
        fprintf(stderr, "Erro ao alocar memoria para o no.\n");
        while(verificarLimiarMemoriaWindows() == -1) {
            rotinaApagarInicio(x);
        }
        insertStart(x);
        return;
    }
    if (verificarLimiarMemoriaWindows() == 0) {
        rotinaApagarInicio(x);
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
    if (node == NULL) {
        fprintf(stderr, "Erro ao alocar memoria para o no.\n");
        while(verificarLimiarMemoriaWindows() == -1) {
            rotinaApagarInicio(x);
        }
        insertEnd(x);
        return;
    }
    if(verificarLimiarMemoriaWindows() == 0) {
        rotinaApagarInicio(x);
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

void pinAndSave(List* x) {
    if (x->start == NULL) {
        printf("A lista está vazia! Por favor insira algum elemento.\n");
        return;
    }
    pinnedItensRoutine(x);
}

void pinnedItensRoutine(List* x) {
    int pinIt;
    Node* aux = x->start;

    while (aux != NULL) {
        printf("Aperte 1 para fixar esse item ou qualquer outra tecla para continuar.");
        scanf("%d", &pinIt);
        if (pinIt == 1) {
            aux->isPinned = 1;
        }
        aux = aux->next;
    }

    printf("Fixado com sucesso.\n");
}

int verificarLimiarMemoriaWindows() {
    SYSTEM_INFO sysInfo;
    GetSystemInfo(&sysInfo);
    return sysInfo.dwNumberOfProcessors;
}

