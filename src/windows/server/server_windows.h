//
// Created by ryanr on 03/12/2024.
//

#ifndef SERVER_WINDOWS_H
#define SERVER_WINDOWS_H


#include <winsock2.h> // Biblioteca para sockets no Windows
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

// Constantes
#define SERVER_PORT 65432
#define BUFFER_SIZE 1024

// Variável global
extern SOCKET server_socket;

// Funções públicas

/**
 * Inicia o servidor configurando o socket e colocando-o em modo de escuta.
 * @return 0 em caso de sucesso, -1 em caso de erro.
 */
int start_server();

/**
 * Aguarda conexões de clientes e processa mensagens recebidas em um loop.
 */
void init_server();

/**
 * Processa os dados recebidos de um cliente.
 * @param client_socket O socket associado ao cliente.
 */
void handle_client(SOCKET client_socket);

/**
 * Processa mensagens JSON recebidas do cliente.
 * @param json_str A mensagem JSON em formato de string.
 */
void handle_message(const char *json_str);


#endif //SERVER_WINDOWS_H
