import socket
import json
import os
import signal
import subprocess
import time
import sys
from threading import Thread

# Caminho do socket UNIX utilizado para comunicação
SOCKET_PATH = "/tmp/arq_socket"

clipboard_history = []

def run_daemon():
    """
    Inicializa o daemon que:
    - Cria o socket e se conecta ao servidor.
    - Roda em segundo plano.
    - Monitora alterações no clipboard.
    - Envia o conteúdo copiado para o servidor.
    """
    print("Inicializando cliente...")
    init_socket()
    daemonize()

def init_socket():
    """
    Cria e conecta ao socket UNIX para estabelecer comunicação inicial com o servidor.
    """
    client_socket = socket.socket(socket.AF_UNIX, socket.SOCK_STREAM)

    try:
        print("Tentando se conectar ao servidor...")
        client_socket.connect(SOCKET_PATH)
        print("Conexão estabelecida com o servidor!")

        # Envia uma mensagem inicial para o servidor
        message = "Init Connection"
        send_to_server("COPY", message)

        # Tenta receber uma resposta
        client_socket.settimeout(2)
        try:
            # Recebe resposta do servidor
            response = client_socket.recv(1024)
            print("Resposta do servidor:", response.decode())
        except socket.timeout:
            print("Nenhuma resposta do servidor, continuando o programa")

    except FileNotFoundError:
        print(f"Erro: Arquivo {SOCKET_PATH} não encontrado. Verifique se o servidor está ativo.")

    except ConnectionRefusedError:
        print("Erro: Falha em se conectar com o servidor. Verifique se ele está escutando conexões.")

    except Exception as e:
        print(f"Erro inesperado: {e}")

    finally:
        client_socket.close()
        print("Conexão com o servidor encerrada.")

def send_to_server(action, content):
    """
    Envia uma mensagem JSON ao servidor contendo uma ação e os dados associados.

    :param action: Ação que está sendo executada (e.g., "COPY").
    :param content: Conteúdo relacionado à ação.
    """
    message = {
        "action": action,
        "data": content,
        "id": os.urandom(8).hex(),  # Gera um ID único
        "timestamp": time.strftime("%Y-%m-%dT%H:%M:%SZ")  # Formato ISO 8601
    }
    json_message = json.dumps(message)

    with socket.socket(socket.AF_UNIX, socket.SOCK_STREAM) as client_socket:
        try:
            client_socket.connect(SOCKET_PATH)
            client_socket.sendall(json_message.encode())
            print(f"Mensagem enviada ao servidor: {json_message}")

        except Exception as e:
            print(f"Erro ao conectar ao servidor: {e}")

        finally:
            client_socket.close()

def get_from_server():
    """
    Solicita ao servidor os dados armazenados e exibe a resposta.
    """
    message = {
        "action": "SELECT",
        "data": None,
        "id": os.urandom(8).hex(),
        "timestamp": time.strftime("%Y-%m-%dT%H:%M:%SZ")
    }
    json_message = json.dumps(message)

    with socket.socket(socket.AF_UNIX, socket.SOCK_STREAM) as client_socket:
        try:
            client_socket.connect(SOCKET_PATH)
            client_socket.sendall(json_message.encode())

            # Recebe a resposta do servidor
            response = client_socket.recv(4096)
            print("Dados recebidos do servidor:", response.decode())

        except Exception as e:
            print(f"Erro ao conectar ao servidor: {e}")

        finally:
            client_socket.close()

def daemonize():
    """
    Torna o processo um daemon para rodar em segundo plano e monitorar o clipboard.
    """
    pid = os.fork()
    if pid > 0:
        sys.exit()  # O processo pai termina

    os.setsid()  # Cria um novo grupo de sessão
    os.umask(0)  # Configura permissões dos arquivos

    signal.signal(signal.SIGINT, handle_signal)  # Lida com Ctrl+C
    signal.signal(signal.SIGTERM, handle_signal)  # Lida com término educado

    # Inicia o monitoramento do clipboard
    monitor_clipboard()

def monitor_clipboard():
    """
    Monitora alterações no clipboard e envia novas cópias ao servidor.
    """
    previous_content = None

    while True:
        clipboard_content = get_clipboard_content()
        if clipboard_content and clipboard_content != previous_content:
            previous_content = clipboard_content
            clipboard_history.append(clipboard_content)
            print(f"Conteúdo novo no clipboard: {clipboard_content}")
            send_to_server("COPY", clipboard_content)
        time.sleep(1)  # Evita uso excessivo de CPU

def get_clipboard_content():
    """
    Captura o conteúdo atual do clipboard usando o comando `xclip`.

    :return: Conteúdo do clipboard (str) ou None se houver erro.
    """
    try:
        result = subprocess.check_output(['/usr/bin/xclip', '-selection', 'clipboard'], stderr=subprocess.DEVNULL)
        return result.decode('utf-8').strip()
    except subprocess.CalledProcessError:
        print("Erro: Não foi possível acessar o clipboard. Verifique se o 'xclip' está instalado.")
        return None

def handle_signal(sig, frame):
    """
    Lida com sinais do sistema para encerramento seguro do daemon.

    :param sig: Sinal recebido.
    :param frame: Quadro atual da pilha.
    """
    if sig == signal.SIGINT:
        print("Recebido SIGINT. Encerrando daemon...")
    elif sig == signal.SIGTERM:
        print("Recebido SIGTERM. Encerrando daemon...")
    sys.exit(0)

if __name__ == "__main__":
    if len(sys.argv) > 1 and sys.argv[1] == "get":
        get_from_server()
    else:
        run_daemon()

def request_show_storage(server_address):
    try:
        sock = socket.socket(socket.AF_UNIX, socket.SOCK_STREAM)
        sock.connect(server_address)

        data = {"action": "SHOW"}
        sock.send(json.dumps(data).encode('utf-8'))

        response = sock.recv(4096).decode('utf-8')
        items = json.loads(response)

        print("Itens armazenados:")
        for item in items:
            print(f"ID: {item['id']}, Data: {item['data']}")
        
        sock.close()
    except Exception as e:
        print(f"Erro ao solicitar SHOW: {e}")
