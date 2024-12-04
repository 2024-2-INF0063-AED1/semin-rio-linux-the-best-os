import pyperclip
import threading
import time

class ClipboardDaemon:
    def __init__(self, clipboard_manager):
        self.clipboard_manager = clipboard_manager
        self.running = False

    def monitor_clipboard(self):
        previous_clipboard = ""
        while self.running:
            current_clipboard = pyperclip.paste()
            if current_clipboard != previous_clipboard:
                self.clipboard_manager.add_clipboard_text(current_clipboard)
                previous_clipboard = current_clipboard
            time.sleep(1)

    def start(self):
        self.running = True
        self.thread = threading.Thread(target=self.monitor_clipboard)
        self.thread.start()

    def stop(self):
        self.running = False
        if self.thread.is_alive():
            self.thread.join()
