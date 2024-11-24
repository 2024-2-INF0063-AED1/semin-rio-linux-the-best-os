import socket
import os
import signal
import subprocess
import time
import sys
from clipboardManager.clipboard_manager_linux import handle_signal

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

def daemonize():
    """
    Configura o processo para rodar em segundo plano como daemon.
    """
    pid = os.fork()
    if pid > 0:
        # Processo pai termina, deixando o daemon rodando em segundo plano
        sys.exit(0)

    # Tornar o processo o líder de um novo grupo de sessão
    os.setsid()

    # Configurar permissões padrão de arquivos
    os.umask(0)

    # Redirecionar entrada, saída e erro padrão para /dev/null
    sys.stdin = open('/dev/null', 'r')
    sys.stdout = open('/dev/null', 'a+')
    sys.stderr = open('/dev/null', 'a+')

    # Ignorar sinais do terminal
    signal.signal(signal.SIGINT, handle_signal())
    signal.signal(signal.SIGTERM, handle_signal())

    print("Daemon iniciado. Escutando sinais...")

    # Loop infinito para manter o daemon ativo
    while True:
        time.sleep(1)
