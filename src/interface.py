import pyperclip
import tkinter as tk
import os
import signal

class ClipboardInterface:
    def __init__(self, clipboard_manager, daemon_pid):
        self.clipboard_manager = clipboard_manager
        self.daemon_pid = daemon_pid
        self.root = tk.Tk()
        self.root.title("Clipboard Manager")
        self.setup_ui()

    def setup_ui(self):
        self.listbox = tk.Listbox(self.root, width=50, height=20)
        self.listbox.pack(padx=10, pady=10)

        self.search_entry = tk.Entry(self.root)
        self.search_entry.pack(pady=5)

        self.search_button = tk.Button(self.root, text="Search", command=self.search_clipboard)
        self.search_button.pack(pady=5)

        self.refresh_button = tk.Button(self.root, text="Refresh", command=self.refresh_list)
        self.refresh_button.pack(pady=5)

        self.paste_button = tk.Button(self.root, text="Select", command=self.paste_selected)
        self.paste_button.pack(pady=5)

        self.stop_button = tk.Button(self.root, text="Stop", command=self.stop_daemon, bg="red", fg="white")
        self.stop_button.pack(pady=5)

    def refresh_list(self):
        self.listbox.delete(0, tk.END)
        texts = self.clipboard_manager.get_all_texts()
        print(f"Atualizando lista com textos: {texts}")
        for text in texts:
            self.listbox.insert(tk.END, text)

    def search_clipboard(self):
        query = self.search_entry.get().lower()
        result = self.clipboard_manager.search_clipboard(query)
        self.listbox.delete(0, tk.END)
        if result:
            self.listbox.insert(tk.END, result)
        else:
            self.listbox.insert(tk.END, "No results found")

    def paste_selected(self):
        selected = self.listbox.get(tk.ACTIVE)
        if selected:
            pyperclip.copy(selected)

    def stop_daemon(self):
        if self.daemon_pid:
            os.kill(self.daemon_pid, signal.SIGTERM)
        self.root.destroy()

    def run(self):
        self.refresh_list()
        self.root.mainloop()
