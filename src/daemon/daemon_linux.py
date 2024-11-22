import socket
import os
import signal
import subprocess
import time
import sys
import clipboardManager.clipboard_manager_linux

def run_daemon():
    print("Inicializando cliente...")
    
    init_socket()
    daemonize()


def init_socket():
    # Criação do Socket
    client_socket = socket.socket(socket.AF_UNIX, socket.SOCK_STREAM)

    try:
        # Path do arquivo socket
        client_socket_path = "/tmp/arq_socket"

        # Vinculação ao socket do servidor
        print("Tentando se conectar ao servidor...")
        client_socket.connect(client_socket_path)
        print("Conexão estabelecida com o servidor!")

        # Mensagem inicial
        message = 'Init Connection'
        client_socket.send(message.encode())
        print("Mensagem enviada:", message)

        # Resposta do servidor
        response = client_socket.recv(1024)
        print("Resposta do servidor:", response.decode())  # Use decode() como função

    except FileNotFoundError:
        print(f"Erro: Arquivo {client_socket_path} não encontrado. Verifique se o servidor está ativo.")

    except ConnectionRefusedError:
        print("Erro: Falha em se conectar com o servidor. Verifique se ele está escutando conexões.")

    except Exception as e:
        print(f"Erro inesperado: {e}")

    finally:
        # Fechando o socket para liberar recursos
        client_socket.close()
        print("Conexão com o servidor encerrada.")


def send_to_server(content):
    client_socket = socket.socket(socket.AF_UNIX, socket.SOCK_STREAM)
    try:
        client_socket.connect('/tmp/arq_socket')
        command = f"COPY {content}"
        client_socket.send(command.encode())
        print(f"Comando enviado: {command}")

        # Esperar resposta do servidor
        response = client_socket.recv(1024)
        print(f"Resposta do servidor: {response.decode()}")

    except Exception as e:
        print(f"Erro ao conectar ao servidor: {e}")
    finally:
        client_socket.close()

# Configurar o daemon para rodar em segundo plano
def daemonize():
    pid = os.fork()
    if pid > 0:
        sys.exit()  # O processo pai termina, deixando o daemon rodando

    os.setsid()  # Criar um novo grupo de sessão
    os.umask(0)  # Configura permissões dos arquivos do daemon

    # Ignorar sinais de controle do terminal (como SIGINT)
    signal.signal(signal.SIGINT, clipboardManager.clipboard_manager_linux.handle_signal())
    signal.signal(signal.SIGTERM, clipboardManager.clipboard_manager_linux.handle_signal())

    # Ficar em um loop escutando o sinal SIGINT
    while True:
        time.sleep(1)  # Esperar pela interrupção do SIGINT

