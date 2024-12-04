//
// Created by ryanr on 02/12/2024.
//

#ifndef STORAGE_TEST_H
#define STORAGE_TEST_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Estruturas da lista ligada
typedef struct StorageNode {
    char id[17];          // ID único em formato hexadecimal (16 caracteres + '\0')
    char *data;           // Conteúdo armazenado
    struct StorageNode *next; // Ponteiro para o próximo nodo
} StorageNode;

typedef struct {
    StorageNode *head; // Cabeça da lista
} StorageList;

// Funções auxiliares para gerenciar a lista
StorageList *create_storage_list();
// void destroy_storage_list(StorageList *list);
void destroy_storage_list(StorageList *list_ptr);
int copy_to_storage(StorageList *list, const char *id, const char *data);
int delete_from_storage(StorageList *list, const char *id);
const char *select_from_storage(StorageList *list, const char *id);
void show_storage(const StorageList *list);
void show_storage_JSON(const StorageList *list, int client_fd, char* buffer, size_t buffer_size);

// Testes unitários
// void test_storage_copy(StorageList *list);
void test_storage_copy(StorageList *list, char* buffer, size_t buffer_size);
void test_storage_delete(StorageList *list);
void test_storage_select(StorageList *list);
void test_storage_show(StorageList *list);

#endif //STORAGE_TEST_H
