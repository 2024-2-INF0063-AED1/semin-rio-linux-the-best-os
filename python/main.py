import os
from clipboard_linux import ClipboardLinux

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
            break

if __name__ == "__main__":
    json_file = "clipboard.json"
    clipboard = ClipboardLinux(json_file)
    clipboard.monitor_clipboard()
    main_menu()