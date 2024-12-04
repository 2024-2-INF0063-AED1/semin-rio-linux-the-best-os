//
// Created by ryanr on 02/12/2024.
//

#include <stdio.h>
#include "storage_test.h"

int main(void) {
    StorageList* list = create_storage_list();

    size_t buffer_size = 1024;
    char *buffer = (char*) malloc(buffer_size);
    if (!buffer) {
        fprintf(stderr, "Erro ao alocar memória para o buffer.\n");
        destroy_storage_list(list);
        return EXIT_FAILURE;
    }

    // Inicializa o buffer
    memset(buffer, 0, buffer_size);

    // Testa a função de cópia
    test_storage_copy(list, buffer, buffer_size);

    // Exibe o conteúdo do buffer passado para a função copy
    printf("Buffer: %s\n", buffer);

    // Testa a função delete:
    test_storage_delete(list);

    // Libera a memória alocada
    // free(buffer);

    test_storage_show(list);

    destroy_storage_list(&list);

    if(!list) {
        printf("A lista esta destruida.");
    }

    return EXIT_SUCCESS;
}