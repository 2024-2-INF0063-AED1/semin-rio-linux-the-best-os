#include "server.h"

int main() {
    if (start_server() == 0) {
        init_server();
    } else {
        fprintf(stderr, "Falha ao iniciar o servidor.\n");
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}

