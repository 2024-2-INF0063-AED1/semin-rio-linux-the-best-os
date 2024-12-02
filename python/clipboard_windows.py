import python_new.linked_list as linked_list
import os
import ctypes
import json
from ctypes import wintypes

class ClipboardWindows:
    def __init__(self, json_file):
        self.json_file = json_file
        self.clipboard_data = linked_list.LinkedList()
        self.load_data()

    def load_data(self):
        if os.path.exists(self.json_file):
            with open(self.json_file, 'r') as f:
                data = json.load(f)
                for item in data:
                    self.clipboard_data.append(item)

    def save_data(self):
        with open(self.json_file, 'w') as f:
            json.dump(self.clipboard_data.to_list(), f, indent=4)

    def get_clipboard_content(self):
        CF_TEXT = 1
        kernel32 = ctypes.windll.kernel32
        user32 = ctypes.windll.user32
        user32.OpenClipboard(0)
        handle = user32.GetClipboardData(CF_TEXT)
        data = ctypes.c_char_p(handle).value.decode('utf-8')
        user32.CloseClipboard()
        return data

    def monitor_clipboard(self):
        last_content = ""
        while True:
            try:
                content = self.get_clipboard_content()
                if content != last_content:
                    print(f"Novo item copiado: {content}")
                    self.clipboard_data.append(content)
                    self.save_data()
                    last_content = content
            except Exception as e:
                pass

    def search_clipboard(self, query):
        return self.clipboard_data.search(query)
