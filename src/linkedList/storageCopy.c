#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef _WIN32
    #include <Windows.h>
#elif __linux__
    #include <sys/sysinfo.h>
#else
// Dá mensagem de erro informando que o sistemas não é suportado.
    #error "Sistema nao suportado."
#endif

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
    int isPinned;
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
void delete(List* x, int index);
void clean(List* x);
void show(List* x);

int copiarString(Node* cabeca);
void selectToPaste(List* x);
void paste();

void pinAndSave(List* x);
void pinnedItensRoutine(List* x);
int findNode(List* x, int index);

void obterMemoriaTotalWindows(void);
int verificarLimiarMemoriaWindows(void);
void rotinaApagarInicio(List* x);

int main(void)
{
    #ifdef _WIN32
        SetConsoleOutputCP(CP_UTF8);
    #endif

    List* x = create();
    selected = createAreaTransferencia();

    printf("Antes da inserção: \n");
    obterMemoriaTotalWindows();

    if(verificarLimiarMemoriaWindows()) {
        printf("\nPode alocar memoria.");
    }

    insertStart(x);
    insertEnd(x);
    insertEnd(x);
    printf("Depois da inserção: \n");
    obterMemoriaTotalWindows();

    clean(x);
    printf("Após clean: \n");
    obterMemoriaTotalWindows();

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
        
        while(verificarLimiarMemoriaWindows() == -1) {
            rotinaApagarInicio(x);
        }
        insertStart(x);
        return;
    }

    if(verificarLimiarMemoriaWindows() == 0) {
        rotinaApagarInicio(x);
    }

    while(verificarLimiarMemoriaWindows() == -1) {
        rotinaApagarInicio(x);
    }

    node->data.str = NULL;  // Inicializa o ponteiro como NULL
    node->isPinned = 0;     // Nao esta fixado o comentario, inicialmente
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

    node->data.str = NULL;  // Inicializa o ponteiro como NULL
    node->isPinned = 0; // Nao esta fixado o comentario, inicialmente
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
    char* string = malloc(capacity * sizeof(char));
    if (string == NULL) {
        fprintf(stderr, "Erro na alocacao de memoria inicial\n");
        return 0;
    }

    int ch;
    while ((ch = getchar()) != EOF) {
        if (ch == '\n' && size > 0 && string[size - 1] == '\n'&& string[size - 2] == '\n') {
            break;
        }
        string[size++] = ch;
        if (size >= capacity) {
            capacity = (int) (strlen(string) + 128);
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

    if(selected) {
        // Aloca memória útil para cabeca->data.str
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

    // Realoca memoria útil para "selected", contendo apenas caracteres != '\0'
    if (selected->data.str == NULL || strcmp(selected->data.str, string) != 0) {
        selected->data.str = (char*) realloc(selected->data.str, (int) strlen(string) * sizeof(char));
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
    if(aux->data.str != NULL) {
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

    if(x->start == NULL) {
        fprintf(stderr, "Nao ha elementos na area de transferencia para serem colados.\n Favor copiar algum.");
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
        fprintf(stderr, "\nFavor inicializar a area de transferencia.\n\n");
    }
}

// Fix: esta apagando o primeiro elemento por padrao,
// mesmo apos digitar um caractere não numérico
void delete(List* x, int index) {
    if(index <= 0) {
        fprintf(stderr, "Indice passado deve ser maior do que zero.");
        return;
    }

    if(x->start == NULL) {
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

    if(aux == NULL) {
        printf("Indice invalido.\n");
        return;
    }

    if(aux == x->start) {
        // inicio apontara para o segundo elemento da lista ou para NULL
        // caso o elemento removido seja o unico elemento da lista
        x->start = aux->next;
        // desalocar espaço da string.
        free(aux->data.str);
        aux->data.str = NULL;
        // desalocamos o espaco para onde aux apontava
        free(aux);
        // aux aponta para o inicio da lista
        aux = x->start;
    }
    // Se for o ultimo da lista
    else if (aux == x->end) {
        // o elemento anterior a fim, no atributo prox apontará para NULL
        prev->next = NULL;
        // fim aponta para o elemento apontado por anterior
        x->end = prev;
        // desalocar espaço da string.
        free(aux->data.str);
        aux->data.str = NULL;
        // Desalocamos o espaco para onde aux apontava
        free(aux);
        // Como era o ultmo elemento da lista, aux recebe NULL
        aux = NULL;
    }
    // Se nao for nem o primeiro nem o ultimo da lista
    else {
        // o elemento anterior ao elemento a ser removido, no atributo prox apontará para o elemento
        // para qual aux->next apontava
        prev->next = aux->next;
        // desalocar espaço da string.
        free(aux->data.str);
        aux->data.str = NULL;
        // Desalocamos o espaco para onde aux apontava
        free(aux);
        // aux aponta para o proximo elemento da lista, aquele que era o seguinte ao numero removido
        aux = prev->next;
    }

    printf("No deletado com sucesso.\n");
}

void clean(List* x) {
    while(x->start != NULL) {
        delete(x, 1);
    }

    printf("Area de transferencia limpa com sucesso.\n");
}

void pinAndSave(List* x) {
    FILE *arq;

    int index = 0;
    int op = 1;

    arq = fopen("fixedItens.txt", "a");

    while (op == 1){
        index = findNode(x, 0);

        if(aux != NULL && aux->isPinned != 1) {
            fprintf(arq, "%d\t-\t%s\n", index, aux->data.str);
            aux->isPinned = 1;
        } else {
            fprintf(stderr,"\nItem ja consta salvo.");
        }

        printf("\nContinuar? (0-nao / 1-sim): ");
        scanf("%d", &op);

        if(op == 1){
            fputs("\n", arq);
            index++;
        }
    }
    fclose(arq);
}

int findNode(List* x, int index) {
    show(x);
    while(index <= 0) {
        printf("Digite um indice para fixar.\nItems fixados nao serao deletados da lista ou perdidos no reinicio do sistema.\n");
        scanf("%d", &index);
    }

    aux = x->start;
    int i = 1;
    while (i != index && aux != NULL) {
        prev = aux;
        aux = aux->next;
        i++;
    }

    if(aux != NULL) {
        printf("Item encontrado.");
    } else {
        fprintf(stderr, "Item nao encontrado.\n");
    }

    return index;
}

void rotinaApagarInicio(List* x) {
    delete(x, 0);
}

void obterMemoriaTotalWindows(void) {

    // Configurar o console para UTF-8
    SetConsoleOutputCP(CP_UTF8);

    MEMORYSTATUSEX memInfo;
    memInfo.dwLength = sizeof(MEMORYSTATUSEX);

    // unsigned long long int = %llu
    if (GlobalMemoryStatusEx(&memInfo)) {
        printf("Memória física total: %llu MB\n", memInfo.ullTotalPhys / (1024 * 1024));
        printf("Memória física disponível: %llu MB\n", memInfo.ullAvailPhys / (1024 * 1024));
        printf("Memória virtual total: %llu MB\n", memInfo.ullTotalPageFile / (1024 * 1024));
        printf("Memória virtual disponível: %llu MB\n", memInfo.ullAvailPageFile / (1024 * 1024));
    } else {
        printf("Erro ao obter informações de memória.\n");
    }
}

int verificarLimiarMemoriaWindows(void) {
    // Devo realizar essa rotina e guardar esse valor inicial.
    MEMORYSTATUSEX memInfo;
    memInfo.dwLength = sizeof(MEMORYSTATUSEX);

    unsigned long long int memTotal = memInfo.ullTotalPageFile / (1024 * 1024);
    unsigned long long int memDisponivel = memInfo.ullAvailPageFile / (1024 * 1024);

    if(memDisponivel >= 0.2 * memTotal) {
        return 1;
    }

    if (memDisponivel <= 0.05 * memTotal) {
        return -1;
    }

    return 0;
}

#ifdef __linux__
void obterMemoriaTotalLinux(void) {
    struct sysinfo info;

    // Get system information
    if (sysinfo(&info) == 0) {
        // Print total memory (in bytes)
        printf("Total RAM: %lu MB\n", info.totalram / (1024 * 1024));
    } else {
        perror("sysinfo");
        return 1;
    }

    return 0;
}

int obterMemoriaRamDisponivelLinux(void) {
    struct sysinfo info;

    // Get system information
    if (sysinfo(&info) == 0) {
        // Print total memory (in bytes)
        int mem = (int) info.totalram / (1024 * 1024);

        if(mem < mem * 0.2) {
            printf("Memoria RAM disponivel: %d", mem);
            return 1;
        } else {
            printf("Memoria RAM critica, < 20%: %d" mem);
            return 0:
        }
    } else {
        perror("sysinfo");
        return 0;
    }
}
#endif
