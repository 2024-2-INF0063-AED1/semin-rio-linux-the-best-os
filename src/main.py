import tkinter as tk 
from tkinter import scrolledtext
import platform
import os
import subprocess
import time
import signal
from threading import Thread

from linux.daemon.daemon_linux import handle_signal, run_daemon

# Controle de subprocessos
server_process = None
daemon_thread = None

def init_os():
    global server_process
    if platform.system() == "Linux":
        from linux.daemon.daemon_linux import clipboard_history, run_daemon
        print("Start server")
        
        # Iniciar o servidor Linux e verificar sucesso
        server_started = start_server_linux()
        if not server_started:
            print("Falha ao iniciar o servidor. Encerrando processo.")
            return

        time.sleep(2)  # Aguardar para garantir que o servidor está ativo
        print("Running daemon...")
        start_daemon_in_thread()
        # subprocess.Popen(run_daemon())

    # elif platform.system() == "Windows":
    #     from daemon_win import run_daemon  # Import específico para Windows
    #     Código para Windows pode ser expandido aqui
    else:
        print("Sistema Operacional não suportado")

def start_server_linux():
    global server_process
    print("Iniciando servidor")
    # Compilando o código C
    compile_command = ["gcc", "./linux/server/server_linux.c", "./storage/storage_test.c", "-ljson-c", "-o", "teste"]
    try:
        subprocess.run(compile_command, check=True)
        print("Compilação concluída com sucesso.")
    except subprocess.CalledProcessError as e:
        print(f"Erro na compilação: {e}")
        return False

    # Executar o servidor compilado
    run_command = ["./teste"]
    try:
        server_process = subprocess.Popen(run_command)
        time.sleep(1) 
        print("Servidor iniciado com sucesso")
        return True
    except subprocess.CalledProcessError as e:
        print(f"Erro ao executar o servidor: {e}")
        return False

def stop_program():
    """
    Para o servidor e o daemon ao encerrar o programa.
    """
    global server_process
    print("Encerrando servidor e cliente...")
    # Enviar sinal para encerrar o servidor
    if server_process and server_process.poll() is None:
        server_process.terminate()
        server_process.wait()
        print("Servidor Encerrado")
    # Enviar sinais de término para o daemon e o servidor
    os.kill(os.getpid(), signal.SIGTERM)  # Exemplo: enviar SIGTERM ao processo principal
    print("Programa encerrado")
    
# def update_history():
#     """
#     Atualiza o histórico de cópias na interface gráfica.
#     """
#     history_text.delete(1.0, tk.END)  # Limpa o texto
#     for item in clipboard_history:
#         history_text.insert(tk.END, f"{item}\n")
#     root.after(1000, update_history)  # Atualiza a cada segundo

def start_daemon_in_thread():
    global daemon_thread
    daemon_thread = Thread(target=run_daemon(), daemon=True)
    daemon_thread.start()

def main():
    print("Iniciando processo")

    init_os()
    # Configurar janela principal
    print("Iniciando a interface")
    root = tk.Tk()
    root.title("Clipboard Monitor")

# Histórico de cópias
    history_label = tk.Label(root, text="Histórico de Cópias:")
    history_label.pack()

    history_text = scrolledtext.ScrolledText(root, width=50, height=15)
    history_text.pack()

# Botão para encerrar o programa
    stop_button = tk.Button(root, text="Encerrar Programa", command=stop_program, bg="red", fg="white")
    stop_button.pack()


# Atualizar histórico periodicamente
    # update_history()

# Iniciar a interface gráfica
    root.mainloop()

if __name__ == "__main__":
    try:
        main()
    finally:
        handle_signal(signal.SIGTERM, "") #encerrando o daemon linux
        stop_program()
