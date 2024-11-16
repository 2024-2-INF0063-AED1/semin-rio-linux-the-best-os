import socket
import os

def run_daemon():
    print("Run in linux")

def init_socket():
# Criação do Socket
    client_socket = socket.socket(socket.AF_UNIX, socket.SOCK_STREAM)

    try:
    # Path do arquivo socket
        client_socket_path = "/tmp/arq_socket"

    # Vinculação
        client_socket.connect(client_socket_path)
    
    # Mensagem inicial
        menssage = 'Init Connection'
        client_socket.send(menssage.encode())
        print("Enviada: ", menssage)

    # Resposta Servidor
        response = client_socket.recv(1024)
        print("Resposta: ", response.decode)

    except FileNotFoundError:
        print(f"Arquivo {client_socket_path} não encontrado")

    except ConnectionRefusedError:
        print("Falha em se conectar com o servidor")

    except Exception as e:
        print("Erro: ", e)

    finally:
        client_socket.close()
        print("Conexão fechada com o servidor")



