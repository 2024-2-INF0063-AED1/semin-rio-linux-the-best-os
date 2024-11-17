#ifdef SERVER_H
#define SERVER_H

#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int start_server(); //Inicia o servidor

void handle_client(int client_socket); //Processa os dados recebidos do cliente 

#endif // DEBUG
