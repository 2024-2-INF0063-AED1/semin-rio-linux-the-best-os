
import pyperclip
import time
import logging

class ClipboardManager:
    def __init__(self, tree):
        self.tree = tree
        self.previous_content = None

    def monitor_clipboard(self):
        """Monitora o clipboard e adiciona novos conteúdos à árvore."""
        while True:
            try:
                clipboard_content = pyperclip.paste()
                if clipboard_content and clipboard_content != self.previous_content:
                    self.previous_content = clipboard_content
                    logging.info(f"Novo conteúdo copiado: {clipboard_content}")
                    self.tree.insert(clipboard_content)
            except Exception as e:
                logging.error(f"Erro ao acessar o clipboard: {e}")
            time.sleep(1)
