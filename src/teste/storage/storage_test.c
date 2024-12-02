    //
    // Created by ryanr on 02/12/2024.
    //

    #include "storage_test.h"

    // Cria uma nova lista de armazenamento
    StorageList *create_storage_list() {
        StorageList *list = (StorageList *)malloc(sizeof(StorageList));
        if (!list) {
            fprintf(stderr, "Erro ao alocar memória para a lista de armazenamento.\n");
            return NULL;
        }
        list->head = NULL;
        return list;
    }

    // Destroi a lista de armazenamento
    // void destroy_storage_list(StorageList *list) {
    //     if (!list) return;
    //
    //     StorageNode *current = list->head;
    //     while (current) {
    //         StorageNode *next = current->next;
    //         free(current->data);
    //         free(current);
    //         current = next;
    //     }
    //     free(list);
    //     list = NULL;
    // }

    void destroy_storage_list(StorageList **list_ptr) {
        if (!list_ptr || !(*list_ptr)) return;

        StorageNode *current = (*list_ptr)->head;
        while (current) {
            StorageNode *next = current->next;
            free(current->data);
            free(current);
            current = next;
        }
        free(*list_ptr);

        *list_ptr = NULL; // Redefine o ponteiro original para NULL
    }


    // Adiciona um item à lista
    int copy_to_storage(StorageList *list, const char *id, const char *data) {
        if (!list || !id || !data) return 0;

        StorageNode *new_node = (StorageNode *)malloc(sizeof(StorageNode));
        if (!new_node) {
            fprintf(stderr, "Erro ao alocar memória para um novo nodo.\n");
            return 0;
        }

        strncpy(new_node->id, id, sizeof(new_node->id));
        new_node->id[16] = '\0';
        new_node->data = strdup(data);
        new_node->next = list->head;
        list->head = new_node;
        return 1;
    }

    // Remove um item da lista
    int delete_from_storage(StorageList *list, const char *id) {
        if (!list || !id) return 0;

        StorageNode *current = list->head;
        StorageNode *prev = NULL;

        while (current) {
            if (strcmp(current->id, id) == 0) {
                if (prev) {
                    prev->next = current->next;
                } else {
                    list->head = current->next;
                }
                free(current->data);
                free(current);
                return 1;
            }
            prev = current;
            current = current->next;
        }
        return 0;
    }

    // Seleciona um item da lista
    const char *select_from_storage(StorageList *list, const char *id) {
        if (!list || !id) return NULL;

        StorageNode *current = list->head;
        while (current) {
            if (strcmp(current->id, id) == 0) {
                return current->data;
            }
            current = current->next;
        }
        return NULL;
    }

    void show_storage(const StorageList *list) {
        if (!list) {
            printf("A lista passada nao esta alocada dinamicamente");
        }

        StorageNode *current = list->head;
        while (current) {
            printf("ID: %s, Data: %s\n", current->id, current->data);
            current = current->next;
        }
    }

    // Mostra todos os itens da lista
    void show_storage_JSON(const StorageList *list, int client_fd, char *buffer, size_t buffer_size) {

        memset(buffer, 0, buffer_size);

        strcat(buffer, "[");

        StorageNode* current = list->head;

        while (current != NULL) {
            char item[256];
            snprintf(item, sizeof(item), "{\"id\":%s,\"data\":\"%s\"}", current->id, current->data);
            strcat(buffer, item);
            if (current->next != NULL) {
                strcat(buffer, ",");
            }
            current = current->next;
        }

        strcat(buffer, "]");

    }

    // // Testa a função de cópia
    void test_storage_copy(StorageList *list, char* buffer, size_t buffer_size) {
        printf("Testando COPY...\n");
        copy_to_storage(list, "an", "Primeiro Item")
            ? printf("Item adicionado com sucesso\n")
            : printf("Houve algum erro.\n");

        if(copy_to_storage(list, "abcdef1234567890", "Segundo Item")) {
            printf("Item adicionado com suceesso.\n");
        } else {
            printf("Houve algum erro.");
        }
        show_storage_JSON(list, 1, buffer, buffer_size);
    }
    //
    // Testa a função de exclusão
    void test_storage_delete(StorageList *list) {
        printf("Testando DELETE...\n");
        if(delete_from_storage(list, "an")) {
            printf("Item deletado com sucesso\n");
        } else {
            printf("Id passado do item nao existe.\n");
        }
        show_storage(list);
    }
    //
    // Testa a função de seleção
    void test_storage_select(StorageList *list) {
        printf("Testando SELECT...\n");
        const char *data = select_from_storage(list, "abcdef1234567890");
        if (data) {
            printf("Item selecionado: %s\n", data);
        } else {
            printf("Item não encontrado.\n");
        }
    }

    // Testa a função de exibição
    void test_storage_show(StorageList *list) {
        printf("Testando SHOW...\n");
        show_storage(list);
    }