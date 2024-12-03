//
// Created by ryanr on 03/12/2024.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <winsock2.h>
#include <cjson/cJSON.h>
#include "server_windows.h"

#pragma comment(lib, "ws2_32.lib")

#define SERVER_PORT 65432
#define BUFFER_SIZE 1024

SOCKET server_socket;

void handle_message(const char *json_str) {
    cJSON *json = cJSON_Parse(json_str);
    if (json == NULL) {
        printf("Erro ao interpretar JSON\n");
        return;
    }

    // Extrair campos action e data do JSON
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
    WSADATA wsa_data;
    if (WSAStartup(MAKEWORD(2, 2), &wsa_data) != 0) {
        printf("Erro na inicialização do Winsock.\n");
        return -1;
    }

    // Criar o scoket
    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket == INVALID_SOCKET) {
        printf("Erro na criação do socket.\n");
        return -1;
    }

    // Configurar o endereço do servidor
    struct sockaddr_in server_address;
    memset(&server_address, 0, sizeof(server_address));
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = INADDR_ANY;
    server_address.sin_port = htons(SERVER_PORT);

    // Vincular socket ao endereço
    if (bind(server_socket, (struct sockaddr *)&server_address, sizeof(server_address)) == SOCKET_ERROR) {
        perror("Erro ao vincular o server_socket");
        closesocket(server_socket);
        WSACleanup();
        return -1;
    }

    if (listen(server_socket, 5) == SOCKET_ERROR) {
        perror("Erro ao colocar o server_socket em listen");
        closesocket(server_socket);
        WSACleanup();
        return -1;
    }

    printf("Servidor iniciado e escutando na porta %d...\n", SERVER_PORT);
    return 0;
}

void init_server() {
    while (1) {
        printf("Aguardando conexão do cliente...\n");

        struct sockaddr_in client_address;
        int client_address_len = sizeof(client_address);

        // Aceitar conexões de clientes
        SOCKET client_socket = accept(server_socket, (struct sockaddr *)&client_address, &client_address_len);
        if (client_socket == INVALID_SOCKET) {
            perror("Erro ao aceitar a conexao");
            continue;
        }

        puts("Cliente conectado.\n");

        handle_client(client_socket);

        closesocket(client_socket);
        puts("Conexão encerrada.\n");
    }
}

void handle_client(SOCKET client_socket) {
    char buffer[BUFFER_SIZE];

    // Reber dados do cliente
    int bytes_received = recv(client_socket, buffer, sizeof(buffer) - 1, 0);
    if (bytes_received == SOCKET_ERROR) {
        printf("Erro ao receber dados do cliente.\n");
        return;
    }

    buffer[bytes_received] = '\0'; // Garantir que o buffer recebido seja uma string válida
    printf("Cliente: %s\n", buffer);

    handle_message(buffer);

    const char *response = "ACK Server";
    int bytes_sent = send(client_socket, response, strlen(response), 0);
    if (bytes_sent == SOCKET_ERROR) {
        perror("Erro ao enviar dados para o cliente.\n");
        return;
    }

    puts("Resposta enviada.\n");
}

int main() {
    if (start_server() != 0) {
        printf("Erro ao iniciar o servidor.\n");
        return -1;
    }

    init_server();

    closesocket(server_socket);
    WSACleanup();
    return 0;
}
