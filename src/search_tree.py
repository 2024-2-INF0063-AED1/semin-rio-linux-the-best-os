class TreeNode:
    def __init__(self, text):
        self.text = text
        self.left = None
        self.right = None

class ClipboardSearchTree:
    def __init__(self):
        self.root = None

    def insert(self, text):
        if not self.root:
            self.root = TreeNode(text)
        else:
            self._insert_recursive(self.root, text)

    def _insert_recursive(self, node, text):
        if text < node.text:
            if node.left is None:
                node.left = TreeNode(text)
            else:
                self._insert_recursive(node.left, text)
        elif text > node.text:
            if node.right is None:
                node.right = TreeNode(text)
            else:
                self._insert_recursive(node.right, text)

    def search(self, query):
        return self._search_recursive(self.root, query)

    def _search_recursive(self, node, query):
        if not node:
            return None
        if query in node.text.lower():
            return node.text
        if query < node.text.lower():
            return self._search_recursive(node.left, query)
        return self._search_recursive(node.right, query)

    def inorder_traversal(self):
        texts = []
        self._inorder_recursive(self.root, texts)
        return texts

    def _inorder_recursive(self, node, texts):
        if node:
            self._inorder_recursive(node.left, texts)
            texts.append(node.text)
            self._inorder_recursive(node.right, texts)
