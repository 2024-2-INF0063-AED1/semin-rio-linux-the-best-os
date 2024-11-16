import socket
import os

def run_daemon():
    print("Run in linux")

# Criação do Socket
s = socket.socket(socket.AF_UNIX, socket.SOCK_STREAM)

# Path do arquivo socket
s_path = "/tmp/arq_socket"

# Verificação se arq_socket ja existe
if os.path.exists(s_path):
    os.remove(s_path)

# Vinculação
s.bind((s_path))

# Numero de conexoes
s.listen(1)

# Aceita a conexão
conn, adrr = s.accept()
