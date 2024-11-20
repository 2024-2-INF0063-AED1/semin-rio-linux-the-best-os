import socket
import os

def run_daemon():
    print("Inicializando cliente...")
    
    init_socket();
    # Aqui você pode adicionar a lógica para iniciar o daemon específico para Linux, se necessário.
    # Exemplo: subprocess.call(["systemctl", "start", "meu_daemon.service"])

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



