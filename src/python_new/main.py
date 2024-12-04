from interface import ClipboardInterface
from daemon import ClipboardDaemon
from search_tree import ClipboardSearchTree

class ClipboardManager:
    def __init__(self):
        self.tree = ClipboardSearchTree()

    def add_clipboard_text(self, text):
        self.tree.insert(text)

    def get_all_texts(self):
        texts = []
        self._inorder_traversal(self.tree.root, texts)
        return texts

    def _inorder_traversal(self, node, texts):
        if node:
            self._inorder_traversal(node.left, texts)
            texts.append(node.text)
            self._inorder_traversal(node.right, texts)

if __name__ == "__main__":
    clipboard_manager = ClipboardManager()
    clipboard_daemon = ClipboardDaemon(clipboard_manager)
    clipboard_interface = ClipboardInterface(clipboard_manager, clipboard_daemon)

    clipboard_daemon.start()
    clipboard_interface.run()
    clipboard_daemon.stop()
