import socket
import json
import os
import signal
import subprocess
import time
import sys

define SOCKET_PATH = "/tmp/arq_socket"

def run_daemon():
    print("Inicializando cliente...")
    
    init_socket()
    daemonize()


def init_socket():
    # Criação do Socket
    client_socket = socket.socket(socket.AF_UNIX, socket.SOCK_STREAM)

    try:

        # Vinculação ao socket do servidor
        print("Tentando se conectar ao servidor...")
        client_socket.connect(SOCKET_PATH)
        print("Conexão estabelecida com o servidor!")

        # Mensagem inicial
        message = 'Init Connection'
        client_socket.send(message.encode())
        print("Mensagem enviada:", message)

        # Resposta do servidor
        response = client_socket.recv(1024)
        print("Resposta do servidor:", response.decode())  # Use decode() como função

    except FileNotFoundError:
        print(f"Erro: Arquivo {SOCKET_PATH} não encontrado. Verifique se o servidor está ativo.")

    except ConnectionRefusedError:
        print("Erro: Falha em se conectar com o servidor. Verifique se ele está escutando conexões.")

    except Exception as e:
        print(f"Erro inesperado: {e}")

    finally:
        # Fechando o socket para liberar recursos
        client_socket.close()
        print("Conexão com o servidor encerrada.")


def send_to_server(action, content):
    client_socket = socket.socket(socket.AF_UNIX, socket.SOCK_STREAM)
    # Logica do Json
    message = {
        "action":action,
        "data": content,
        "id": os.urandom(8).hex(),  # Gera um ID único
        "timestamp": time.strftime("%Y-%m-%dT%H:%M:%SZ")  # Formato ISO 8601
    }
    json_message = json.dumps(message)
    
    # Envia o JSON via socket UNIX
    with socket.socket(socket.AF_UNIX, socket.SOCK_STREAM) as client_socket:
        try:
            client_socket.connect(SOCKET_PATH)
            client_socket.sendall(json_message.encode())
        except Exception as e:
            print(f"Erro ao conectar ao servidor: {e}")

        finally:
            client_socket.close()

# Monitorar e manipular sinais do sistema
def daemonize():
    pid = os.fork()
    if pid > 0:
        sys.exit()  # O processo pai termina

    os.setsid()  # Criar um novo grupo de sessão
    os.umask(0)  # Configura permissões dos arquivos

    signal.signal(signal.SIGINT, handle_signal)  # Lidar com Ctrl+C
    signal.signal(signal.SIGTERM, handle_signal)  # Lidar com término educado

    # Loop infinito para monitorar clipboard
    monitor_clipboard()

def monitor_clipboard():
    previous_content = None  # Para verificar mudanças no clipboard

    while True:
        clipboard_content = get_clipboard_content()
        if clipboard_content and clipboard_content != previous_content:
            previous_content = clipboard_content
            print(f"Conteúdo novo no clipboard: {clipboard_content}")
            send_to_server("COPY",clipboard_content)
        time.sleep(1)  # Evitar uso excessivo de CPU

def get_clipboard_content():
    """
    Captura o conteúdo atual do clipboard usando xclip.
    """
    try:
        result = subprocess.check_output(['xclip', '-selection', 'clipboard', '-o'], stderr=subprocess.DEVNULL)
        return result.decode('utf-8').strip()
    except subprocess.CalledProcessError:
        print("Erro: Não foi possível acessar o clipboard. Verifique se o 'xclip' está instalado.")
        return None


def handle_signal(sig, frame):
    if sig == signal.SIGINT:
        print("Recebido SIGINT. Encerrando daemon...")
    elif sig == signal.SIGTERM:
        print("Recebido SIGTERM. Encerrando daemon...")
    sys.exit(0)  # Encerrar o programa com sucesso
