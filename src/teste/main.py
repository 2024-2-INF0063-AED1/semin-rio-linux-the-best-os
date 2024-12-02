import platform
import os
import subprocess
import time

def init_os():
    if platform.system() == "Linux":
        from daemon_linux import run_daemon
        print("start server")
        start_server_linux()
        time.sleep(2)
        print("run_daemon()")
        run_daemon()

    elif platform.system() == "Windows":
        from daemon_win import run_daemon
    else:
        print("Operation System not suport")


def start_server_linux():
    print("Iniciando servidor")
    # Compilando o código C
    compile_command = ["gcc", "server_test.c", "storage/storage_test.c", "-ljson-c", "-o", "teste"]
    try:
        subprocess.run(compile_command, check=True)
        print("Compilação concluída com sucesso.")
    except subprocess.CalledProcessError as e:
        print(f"Erro na compilação: {e}")
        return

    # Executar o servidor compilado
    run_command = ["./teste"]
    subprocess.run(run_command, check=True)
    time.sleep(1) 

def main():
    print("Iniciando processo")

    init_os()

if __name__ == "__main__":
    main()
