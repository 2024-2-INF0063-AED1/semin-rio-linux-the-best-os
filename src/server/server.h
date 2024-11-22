#ifndef SERVER_H
#define SERVER_H

#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

extern int server_socket;

int start_server(); //Inicia o servidor

void init_server();

void handle_client(int client_socket); //Processa os dados recebidos do cliente 

#endif // DEBUG
