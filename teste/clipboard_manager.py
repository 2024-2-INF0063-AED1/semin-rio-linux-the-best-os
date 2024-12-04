from search_tree import ClipboardSearchTree

class ClipboardManager:
    def __init__(self):
        self.clipboard_tree = ClipboardSearchTree()

    def add_clipboard_text(self, text):
        print(f"Adicionando texto ao clipboard: {text}")
        self.clipboard_tree.insert(text)

    def search_clipboard(self, query):
        return self.clipboard_tree.search(query)

    def get_all_texts(self):
        return self.clipboard_tree.inorder_traversal()