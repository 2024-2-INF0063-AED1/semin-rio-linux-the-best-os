import win32clipboard
import os
import json
import time
from linked_list import LinkedList

class ClipboardWindows:
    def __init__(self, json_file):
        """
        Inicializa a classe ClipboardWindows.
        
        Args:
            json_file (str): Caminho para o arquivo JSON que armazena os dados do clipboard.
        """
        self.json_file = json_file
        self.clipboard_data = LinkedList()
        self.load_data()

    def load_data(self):
        """
        Carrega os dados do clipboard a partir de um arquivo JSON.
        """
        if os.path.exists(self.json_file):
            with open(self.json_file, 'r') as f:
                data = json.load(f)
                for item in data:
                    self.clipboard_data.append(item)

    def save_data(self):
        """
        Salva os dados do clipboard em um arquivo JSON.
        """
        with open(self.json_file, 'w') as f:
            json.dump(self.clipboard_data.to_list(), f, indent=4)

    def get_clipboard_content(self):
        """
        Obtém o conteúdo atual do clipboard usando a API do Windows.
        
        Returns:
            str: Conteúdo do clipboard.
        """
        try:
            win32clipboard.OpenClipboard()
            data = win32clipboard.GetClipboardData()
            win32clipboard.CloseClipboard()
            return data
        except TypeError:
            return ""

    def monitor_clipboard(self):
        """
        Monitora o clipboard para alterações e atualiza a lista ligada.
        """
        last_content = self.get_clipboard_content()
        while True:
            current_content = self.get_clipboard_content()
            if current_content and current_content != last_content:
                self.clipboard_data.append(current_content)
                self.save_data()
                last_content = current_content
                print(f"Novo conteúdo copiado: {current_content}")
            time.sleep(1)  # Espera 1 segundo antes de verificar novamente

    def search_clipboard(self, query):
        """
        Busca por itens no clipboard que contenham o termo de busca.
        
        Args:
            query (str): Termo de busca.
        
        Returns:
            list: Lista de itens que contêm o termo de busca.
        """
        results = []
        current = self.clipboard_data.head
        while current:
            if query.lower() in current.data.lower():
                results.append(current.data)
            current = current.next
        return results