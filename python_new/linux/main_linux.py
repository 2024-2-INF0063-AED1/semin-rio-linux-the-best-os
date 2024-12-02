import os
import signal
from clipboard_linux import ClipboardLinux, SOCKET_PATH

def main_menu(clipboard):
    while True:
        print("\n=== Clipboard Avançado ===")
        print("1. Ver itens armazenados")
        print("2. Buscar item")
        print("3. Sair")
        choice = input("Escolha uma opção: ")

        if choice == "1":
            items = clipboard.clipboard_data.to_list()
            print("\nItens armazenados:")
            for i, item in enumerate(items, start=1):
                print(f"{i}. {item}")

        elif choice == "2":
            query = input("Digite o termo para busca: ")
            results = clipboard.search_clipboard(query)
            print("\nResultados da busca:")
            for i, item in enumerate(results, start=1):
                print(f"{i}. {item}")

        elif choice == "3":
            stop_daemon()
            break

def start_daemon():
    """
    Inicia o daemon em segundo plano.
    """
    if not os.path.exists(SOCKET_PATH):
        daemon_script = os.path.abspath(os.path.join(os.path.dirname(__file__), 'clipboard_linux.py'))
        os.system(f"python3 {daemon_script} &")

def stop_daemon():
    """
    Envia um sinal para parar o daemon.
    """
    if os.path.exists(SOCKET_PATH):
        with open(SOCKET_PATH, 'r') as f:
            pid = int(f.read().strip())
            os.kill(pid, signal.SIGTERM)
        os.remove(SOCKET_PATH)

if __name__ == "__main__":
    json_file = "clipboard.json"
    clipboard = ClipboardLinux(json_file)
    
    # Inicia o daemon
    start_daemon()
    
    # Executa o menu principal
    main_menu(clipboard)