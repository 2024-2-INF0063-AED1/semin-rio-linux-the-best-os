#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Não há um número fixo de caracteres porque a área de transferência do Windows 10
// armazena dados em bytes, e o tamanho exato varia com base na codificação
// dos caracteres (por exemplo, UTF-8 ou UTF-16) e no tipo de dados (texto, imagem, etc.).

// Windows: A área de transferência pode, teoricamente, suportar até 2 GB de dados,
// mas isso depende da memória RAM e dos limites do sistema. Para textos simples em UTF-8,
// isso pode equivaler a centenas de milhões de caracteres.

// O projeto atual visa armazenar, teoricamente, até X% de dados da memória RAM, a se definir manualmente.

typedef struct string {
    char* str;
} String;

typedef struct node {
    String data;
    struct node* next;
} Node;

typedef struct list {
    Node* start;
    Node* end;
} List;

Node* aux;
Node* prev;
Node* selected;

List* create();
Node* createAreaTransferencia();
void insertStart(List* x);
void insertEnd(List* x);
void delete(List* x);
void clean(List* x);
void show(List* x);

int copiarString(Node* cabeca);
void selectToPaste(List* x);
void paste();
ssize_t my_getline(char **lineptr, size_t *n, FILE *stream);

int main(void)
{
    List* x = create();
    selected = createAreaTransferencia();
    insertStart(x);
    // paste();
    insertEnd(x);
    insertEnd(x);
    // paste();

    selectToPaste(x);
    paste();
    return 0;
}

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
    ctrlV->data.str = NULL;  // Inicializa como NULL para evitar acesso a ponteiro invalido
    ctrlV->next = NULL;
    printf("Area de transferencia disponivel.\n");
    return ctrlV;
}

void insertStart(List* x) {
    Node* node = (Node*)malloc(sizeof(Node));
    if (node == NULL) {
        fprintf(stderr, "Erro ao alocar memoria para o no.\n");
        return;
    }

    node->data.str = NULL;  // Inicializa o ponteiro como NULL
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
        return;
    }

    node->data.str = NULL;  // Inicializa o ponteiro como NULL
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
    size_t capacity = 128;  // Tamanho inicial da capacidade
    size_t size = 0;            // typedef unsigned long long size_t
    char *nome = (char*)malloc(capacity * sizeof(char));
    if (nome == NULL) {
        fprintf(stderr, "Erro na alocacao de memoria inicial\n");
        return 0;
    }

    int ch;
    while ((ch = getchar()) != EOF) {
        if (ch == '\n' && size > 0 && nome[size - 1] == '\n'&& nome[size - 2] == '\n') {
            break;
        }
        nome[size++] = ch;
        if (size >= capacity) {
            capacity = (int) (strlen(nome) + 128);
            char *temp = realloc(nome, capacity * sizeof(char));
            if (temp == NULL) {
                printf("Ocorreu algum problema na realocação\n");
                free(nome);
                return 0;
            }
            nome = temp;
        }
    }

    nome[size] = '\0';
    nome[size - 1] = '\0';
    nome[size - 2] = '\0';

    // Aloca memória para cabeca->data.str
    cabeca->data.str = (char*)malloc((size + 1) * sizeof(char));
    if (cabeca->data.str == NULL) {
        fprintf(stderr, "Erro na alocacao de memoria para a string do no\n");
        free(nome);
        return 0;
    }
    strcpy(cabeca->data.str, nome);

    if(selected) {
        if (selected->data.str == NULL || strcmp(selected->data.str, nome) != 0) {
            selected->data.str = (char*)realloc(selected->data.str, ((int) (strlen(nome) + 1)) * sizeof(char));
            if (selected->data.str == NULL) {
                fprintf(stderr, "Erro na realocacao de memoria para a area de transferencia\n");
                free(nome);
                return 0;
            }
            strcpy(selected->data.str, nome);
        }
    }

    free(nome);
    return 1;
}

void show(List* x) {
    if (x->start == NULL) {
        printf("Estado da lista: Lista vazia!\n");
        return;
    }
    aux = x->start;
    int i = 1;
    printf("Estado da lista:\n");
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

    if(x->start == NULL) {
        printf("Nao ha elementos na area de transferencia para serem colados.\n Favor copiar algum.");
        return;
    }

    show(x);
    int index;
    printf("Digite o indice que deseja copiar:\n");
    scanf("%d", &index);

    int i = 1;
    Node* aux = x->start;
    while(aux != NULL && i < index) {
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
    if(selected) {
        printf("Area de transferencia:\n%s\n\n", selected->data.str);
    } else {
        printf("\nFavor inicializar a area de transferencia.\n\n");
    }
}

