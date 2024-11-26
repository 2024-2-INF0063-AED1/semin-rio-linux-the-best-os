#ifndef STORAGE_H 
#define STORAGE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef _WIN32
    #include <Windows.h>
#elif __linux__
    #include <sys/sysinfo.h>
#else
    #error "Sistema não suportado."
#endif

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
    int memSize;
} List;

// Funções relacionadas à lista
List* create();
Node* createAreaTransferencia();
void insertStart(List* x);
void insertEnd(List* x);
void delete(List* x, int index);
void clean(List* x);
void show(List* x);

// Funções relacionadas à manipulação de strings
int copiarString(Node* cabeca);
void selectToPaste(List* x);
void paste();

// Funções relacionadas à memória e gerenciamento
void pinAndSave(List* x);
void pinnedItensRoutine(List* x);
void removePinnedItem(const char* itemStr);
void findNode(List* x, int index);
int findLastItemSaved();
void obterMemoriaTotalWindows();
int verificarLimiarMemoriaWindows();
void rotinaApagarInicio(List* x);

#endif //STORAGE_H 
