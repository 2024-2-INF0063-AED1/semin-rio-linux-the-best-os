#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>
#include <signal.h>
#include <json-c/json.h>
#include "../../../src/storage/storage_test.h" //Header com as funções da lista ligada

#define SOCKET_PATH "/tmp/arq_socket"
#define BUFFER_SIZE 1024


volatile sig_atomic_t stop = 0;  // Variável de controle para sair do loop

// Função que vai ser chamada quando o sinal SIGINT for recebido
void handle_sigint(int sig) {
    stop = 1;  // Define a variável stop como 1 quando o sinal SIGINT for recebido
}

void handle_copy(const char *data, const char *time_copy);
void handle_delete(const char *id);
void handle_select(const char *id, int client_socket);
// void handle_show(int client_socket);

StorageList* list;

int main() {
    int server_socket, client_socket;
    struct sockaddr_un server_addr;
    char buffer[BUFFER_SIZE];

    // Criando o socket UNIX
    if ((server_socket = socket(AF_UNIX, SOCK_STREAM, 0)) == -1) {
        char *erro_create="Erro ao criar o socket\n";
        write(client_socket, erro_create, strlen(erro_create));
        perror("Erro ao criar o socket");
        exit(EXIT_FAILURE);
    }

    // Configurando o endereço do socket
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sun_family = AF_UNIX;
    strncpy(server_addr.sun_path, SOCKET_PATH, sizeof(server_addr.sun_path) - 1);

    // Remover arquivo socket antigo, se existir
    unlink(SOCKET_PATH);

    // Bind do socket
    if (bind(server_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1) {
        char *erro_bind="Erro ao fazer bind\n";
        write(client_socket, erro_bind, strlen(erro_bind));
        perror("Erro ao fazer bind");
        close(server_socket);
        exit(EXIT_FAILURE);
    }

    // Escutando no socket
    if (listen(server_socket, 5) == -1) {
         
        char *erro_listen="Erro ao escutar no socket\n";
        write(client_socket, erro_listen, strlen(erro_listen));
        perror("Erro ao escutar no socket");
        close(server_socket);
        exit(EXIT_FAILURE);
    }

    printf("Servidor ouvindo no socket: %s\n", SOCKET_PATH);

    char *response="Servidor ouvindo no socket"; // TODO: Melhorar e colocar o socket tb
    write(client_socket, response, strlen(response));

    list = create_storage_list();

   
    while (!stop) {
        // Aceitando conexão
        if ((client_socket = accept(server_socket, NULL, NULL)) == -1) {
            char *erro_socket="Erro ao aceitar conexão\n";
            write(client_socket, erro_socket, strlen(erro_socket));
            perror("Erro ao aceitar conexão");
            continue;
        }

        printf("Cliente conectado.\n");

        // Lendo dados do cliente
        int bytes_read = read(client_socket, buffer, sizeof(buffer) - 1);
        if (bytes_read > 0) {
            buffer[bytes_read] = '\0';

            // Parse do JSON recebido
            struct json_object *parsed_json = json_tokener_parse(buffer);
            if (parsed_json == NULL) {
                char *erro_json_response="Erro ao interpretar JSON\n";
                write(client_socket, erro_json_response, strlen(erro_json_response));
                fprintf(stderr, "Erro ao interpretar JSON.\n");
                close(client_socket);
                continue;
            }

            // Extraindo campos do JSON
            struct json_object *action_obj, *data_obj, *id_obj, *time_obj;

            json_object_object_get_ex(parsed_json, "action", &action_obj);
            const char *action = json_object_get_string(action_obj);

            json_object_object_get_ex(parsed_json, "data", &data_obj);
            const char *data = data_obj ? json_object_get_string(data_obj) : NULL;

            json_object_object_get_ex(parsed_json, "id", &id_obj);
            const char *id = id_obj ? json_object_get_string(id_obj) : NULL;

            json_object_object_get_ex(parsed_json, "time", &time_obj);
            const char *time = time_obj ? json_object_get_string(time_obj) : NULL;

            // Processando ações
            if (strcmp(action, "COPY") == 0 && data) {
                handle_copy(data, time);
            } else if (strcmp(action, "DELETE") == 0 && id) {
                handle_delete(id);
            } else if (strcmp(action, "SELECT") == 0 && id) {
                handle_select(id, client_socket);
            // } else if (strcmp(action, "SHOW") == 0) {
            //     handle_show(client_socket);
            } else if (strcmp(action, "TEST") == 0 && id) {
                printf("Servidor: Mensagem teste chegou no servidor");
                const char *response = "TEST OK";
                    write(client_socket, response, strlen(response)); // Envia a resposta ao cliente
            } else {
                fprintf(stderr, "Ação inválida ou dados ausentes.\n");
            }

            // Liberando JSON
            json_object_put(parsed_json);
        } else {
            perror("Erro ao ler dados do cliente");
        }

        close(client_socket);
    }

    // Registra o sinal SIGINT para chamar a função handle_sigint
    signal(SIGINT, handle_sigint);

    close(server_socket);
    unlink(SOCKET_PATH);
    return 0;
}

void handle_copy(const char *data, const char *time_copy) {
    srand(time(NULL));

    char id[20];
    int random_number = rand();
    snprintf(id, sizeof(id), "%d", random_number);
    
    printf("Executando COPY com data: %s\n Time: %s", data, time_copy);
    copy_to_storage(list ,id, data); // Função da lista ligada para armazenar o dado
}

void handle_delete(const char *id) {
    printf("Executando DELETE com id: %s\n", id);
    if (delete_from_storage(list, id) == 0) { // Função para deletar pelo id
        printf("Item com id %s removido com sucesso.\n", id);
    } else {
        fprintf(stderr, "Erro ao remover item com id %s.\n", id);
    }
}

void handle_select(const char *id, int client_socket) {
    printf("Executando SELECT com id: %s\n", id);
    const char *data = select_from_storage(list, id); // Função para obter o dado pelo id
    if (data) {
        write(client_socket, data, strlen(data)); // Retorna o dado para o cliente
    } else {
        const char *error_msg = "Item não encontrado.";
        write(client_socket, error_msg, strlen(error_msg));
    }
}

// void handle_show(int client_socket) {
//     printf("Executando SHOW\n");
//     char *all_data = show_storage(list); // Função para obter todos os itens armazenados
//     if (all_data) {
//         write(client_socket, all_data, strlen(all_data));
//         free(all_data);
//     } else {
//         const char *error_msg = "Nenhum item armazenado.";
//         write(client_socket, error_msg, strlen(error_msg));
//     }
// }


