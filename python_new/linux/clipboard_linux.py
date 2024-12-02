import sys
import os
import json
import time
import subprocess
import signal
import daemon
from daemon import pidfile

# Adiciona o diretório pai ao sys.path
sys.path.append(os.path.abspath(os.path.join(os.path.dirname(__file__), '..')))

from linked_list import LinkedList

SOCKET_PATH = "/tmp/clipboard_daemon.pid"

class ClipboardLinux:
    def __init__(self, json_file):
        """
        Inicializa a classe ClipboardLinux.
        
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
        Captura o conteúdo atual do clipboard usando o comando `xclip`.

        :return: Conteúdo do clipboard (str) ou None se houver erro.
        """
        try:
            result = subprocess.check_output(
                ['xclip', '-selection', 'clipboard', '-o'],  # Incluindo '-o'
                stderr=subprocess.DEVNULL
            )
            return result.decode('utf-8').strip()  # Adicionando parênteses no 'strip'
        except subprocess.CalledProcessError:
            print("Erro: Não foi possível acessar o clipboard. Verifique se o 'xclip' está instalado e funcionando.")
            return None

     
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
            time.sleep(0.1)  # Espera 0.1 segundo antes de verificar novamente

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

def run_daemon():
    """
    Inicializa o daemon que monitora o clipboard.
    """
    json_file = "clipboard.json"
    clipboard = ClipboardLinux(json_file)
    clipboard.monitor_clipboard()

if __name__ == "__main__":
    with daemon.DaemonContext(
        pidfile=pidfile.TimeoutPIDLockFile(SOCKET_PATH),
        signal_map={
            signal.SIGTERM: 'terminate',
            signal.SIGHUP: 'terminate'
        }
    ):
        run_daemon()