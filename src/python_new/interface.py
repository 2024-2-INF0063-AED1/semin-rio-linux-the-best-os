import pyperclip
import tkinter as tk

class ClipboardInterface:
    def __init__(self, clipboard_manager, daemon):
        self.clipboard_manager = clipboard_manager
        self.daemon = daemon
        self.root = tk.Tk()
        self.root.title("Clipboard Manager")
        self.setup_ui()

    def setup_ui(self):
        self.listbox = tk.Listbox(self.root, width=50, height=20)
        self.listbox.pack(padx=10, pady=10)

        self.refresh_button = tk.Button(self.root, text="Refresh", command=self.refresh_list)
        self.refresh_button.pack(pady=5)

        self.paste_button = tk.Button(self.root, text="Paste", command=self.paste_selected)
        self.paste_button.pack(pady=5)

        self.stop_button = tk.Button(self.root, text="Stop", command=self.stop_daemon, bg="red", fg="white")
        self.stop_button.pack(pady=5)

        # self.search_button = tk.Button(self.root, text="Search", command=search)

    def refresh_list(self):
        self.listbox.delete(0, tk.END)
        for text in self.clipboard_manager.get_all_texts():
            self.listbox.insert(tk.END, text)

    def paste_selected(self):
        selected = self.listbox.get(tk.ACTIVE)
        if selected:
            pyperclip.copy(selected)

    # def search(self):
    #     term = tk.Entry.get(self)
    #     label_result.config(text=f"You search for : {term}")

    def stop_daemon(self):
        self.daemon.stop()
        self.root.destroy()

    def run(self):
        self.refresh_list()
        self.root.mainloop()
