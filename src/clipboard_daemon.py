import pyperclip
import time
import signal
import sys

class ClipboardDaemon:
    def __init__(self, clipboard_manager):
        self.clipboard_manager = clipboard_manager
        self.running = True

    def monitor_clipboard(self):
        previous_clipboard = ""
        while self.running:
            current_clipboard = pyperclip.paste()
            if current_clipboard != previous_clipboard:
                print(f"Novo conteúdo copiado: {current_clipboard}")
                self.clipboard_manager.add_clipboard_text(current_clipboard)
                previous_clipboard = current_clipboard
            time.sleep(1)

    def start(self):
        signal.signal(signal.SIGTERM, self.stop)
        self.monitor_clipboard()

    def stop(self, signum, frame):
        self.running = False
        sys.exit(0)