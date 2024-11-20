import platform
import os
import subprocess
import time

def init_os():
    if platform.system() == "Linux":
        from daemon_linux import run_daemon
        start_server_linux()
        time.sleep(2)
        run_daemon()

    elif platform.system() == "Windows":
        from daemon_win import run_daemon
    else:
        print("Operation System not suport")


def start_server_linux():
    print("Iniciando servidor")
    # Compilar o servidor
    subprocess.Popen(["../server/server"])
    time.sleep(1)

def main():
    print("Iniciando processo")

    init_os()

if __name__ == "__main__":
    main()
