#include "server.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <cjson/cJSON.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>

#define SOCKET_PATH "/tmp/arq_socket"
#define BUFFER_SIZE 1024

int server_socket;

void handle_message(const char *json_str) {
    cJSON *json = cJSON_Parse(json_str);
    if (json == NULL) {
        printf("Erro ao interpretar JSON\n");
        return;
    }

    const cJSON *action = cJSON_GetObjectItemCaseSensitive(json, "action");
    const cJSON *data = cJSON_GetObjectItemCaseSensitive(json, "data");

    if (cJSON_IsString(action) && cJSON_IsString(data)) {
        if (strcmp(action->valuestring, "COPY") == 0) {
            printf("Texto copiado recebido: %s\n", data->valuestring);
            // Salvar ou processar o texto copiado aqui
        } else {
            printf("Ação não reconhecida: %s\n", action->valuestring);
        }
    }

    cJSON_Delete(json);
}

int start_server() {
    //Criar o socket
    server_socket = socket(AF_UNIX, SOCK_STREAM, 0); //Arg 1: Familia do Socket, tipo de socket tcp ou udp e 0 para protocolo padrao

    if (server_socket < 0) { //Verificação de erro 
        perror("Erro na criacao do server_socket.");
        return -1;
    }

    //Vinculação 
    struct sockaddr_un server_address; //declaração da estrutura de um endereço de socket
    memset(&server_address, 0, sizeof(server_address)); //Inicialização de todos os campos da estrutura como 0
    server_address.sun_family = AF_UNIX; //Familia do endereço
    const char* socket_path = "/tmp/arq_socket";
    strcpy(server_address.sun_path, socket_path); //Caminho do endereço
 
    if(bind(server_socket, (struct sockaddr *)&server_address, sizeof(server_address)) < 0) { //Vinculção e verificação se houve erro ao vincular
        perror("Erro ao vincular o server_socket");
        return -1;
        // Arg: Socket para vincular, endereço em forma de ponteiro para estrutura de socket de endereço e o tamanho do endereço
    }

    //Escuta
    if (listen(server_socket, 5) < 0) {
        perror("Erro ao colocar o server_socket em listen");
        return -1;
    }

    puts("Servidor iniciado e escutando...");
    return 0;

}

void init_server() {
    while (1) {
        puts("Aguardando conexao do cliente...");

        //Aceitar
        int client_socket = accept(server_socket, NULL, NULL);
        if (client_socket < 0) {
            perror("Erro ao aceitar a conexao");
            continue; //Tenta novamente
        }

        puts("Cliente conectado");

        //Cominucação inicial 
        handle_client(client_socket);

        //Fechar a conexao
        close(client_socket);
        puts("Conexao encerrada");
    }
}

void handle_client (int client_socket) {

    char buffer[256]; //Buffer para conexao

    //Recebe
    ssize_t bytes_received = recv(client_socket, buffer, sizeof(buffer) - 1, 0);
    //Socket, buffer onde vai armazenar os dados, tamanho do buffer, e flag 0
    if (bytes_received < 0) {
        perror("Erro ao receber dados do cliente");
        return;
    }

    buffer[bytes_received] = '\0'; //Garantir que o buffer seja uma string valida
    printf("Cliente: %s\n", buffer);

    //Resposta
    const char* response = "ACK Server";
    ssize_t bytes_sent = send(client_socket, response, strlen(response), 0);
    if (bytes_sent < 0) {
        perror("Erro ao enviar dados para o cliente");
        return;
    }

    puts("Resposta enviada");

}
