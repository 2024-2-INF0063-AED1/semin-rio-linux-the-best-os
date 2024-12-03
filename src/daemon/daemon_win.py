import socket
import json
import os
import subprocess
import time
import sys
import threading
import win32clipboard  # pywin32 para acessar o clipboard

# Configurações do servidor
SERVER_HOST = "127.0.0.1"  # Endereço IP do servidor
SERVER_PORT = 65432  # Porta para comunicação com o servidor

def run_daemon():
    """
    Inicializa o daemon que:
    - Cria o socket e se conecta ao servidor.
    - Roda em segundo plano.
    - Monitora alterações no clipboard.
    - Envia o conteúdo copiado para o servidor.
    """
    print("Inicializando cliente...")
    daemonize()

def send_to_server(action, content):
    """
    Envia uma mensagem JSON ao servidor contendo uma ação e os dados associados.

    :param action: Ação que está sendo executada (e.g., "COPY").
    :param content: Conteúdo relacionado à ação.
    """
    message = {
        "action": action,
        "data": content,
        "id": os.urandom(8).hex(),
        "timestamp": time.strftime("%Y-%m-%dT%H:%M:%SZ")  # Formato ISO 8601
    }
    json_message = json.dumps(message)

    try:
        with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as client_socket:
            client_socket.connect((SERVER_HOST, SERVER_PORT))
            client_socket.sendall(json_message.encode())
            print(f"Mensagem enviada ao servidor: {json_message}")
    except Exception as e:
        print(f"Erro ao conectar ao servidor: {e}")

def get_clipboard_content():
    """
    Captura o conteúdo atual do clipboard usando a API do Windows.

    :return: Conteúdo do clipboard (str) ou None se houver erro.
    """
    try:
        win32clipboard.OpenClipboard()
        data = win32clipboard.GetClipboardData()
        win32clipboard.CloseClipboard()
        return data
    except Exception as e:
        print(f"Erro ao acessar o clipboard: {e}")
        return None

def monitor_clipboard():
    """
    Monitora alterações no clipboard e envia novas cópias ao servidor.
    """
    previous_content = None

    while True:
        clipboard_content = get_clipboard_content()
        if clipboard_content and clipboard_content != previous_content:
            previous_content = clipboard_content
            print(f"Conteúdo novo no clipboard: {clipboard_content}")
            send_to_server("COPY", clipboard_content)
        time.sleep(1)  # Evita uso excessivo de CPU

def daemonize():
    """
    Torna o processo um daemon para rodar em segundo plano e monitorar o clipboard.
    """
    print("Iniciando o monitoramento do clipboard em um thread separado...")
    thread = threading.Thread(target=monitor_clipboard, daemon=True)
    thread.start()
    try:
        while True:
            time.sleep(1)  # Mantém o daemon ativo
    except KeyboardInterrupt:
        print("Encerrando daemon...")

if __name__ == "__main__":
    if len(sys.argv) > 1 and sys.argv[1] == "get":
        print("Operação 'get' ainda não implementada para Windows.")
    else:
        run_daemon()
