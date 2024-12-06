class TreeNode:
    """
    Representa um nó individual na árvore de busca binária.

    Attributes:
        text (str): O texto armazenado no nó.
        left (TreeNode): Referência ao nó filho à esquerda.
        right (TreeNode): Referência ao nó filho à direita.
    """
    def __init__(self, text):
        """
        Inicializa um nó da árvore com o texto fornecido.

        Args:
            text (str): Texto a ser armazenado no nó.
        """
        self.text = text
        self.left = None
        self.right = None

class ClipboardSearchTree:
    """
    Representa uma árvore de busca binária para gerenciar textos da área de transferência.

    Methods:
        insert(text): Insere um novo texto na árvore.
        search(query): Busca por um texto que contenha o termo fornecido.
        inorder_traversal(): Retorna uma lista ordenada dos textos armazenados na árvore.
    """
    def __init__(self):
        """
        Inicializa uma árvore de busca binária vazia.
        """
        self.root = None

    def insert(self, text):
        """
        Insere um novo texto na árvore de forma ordenada.

        Args:
            text (str): Texto a ser inserido.
        """
        if not self.root:
            self.root = TreeNode(text)
        else:
            self._insert_recursive(self.root, text)

    def _insert_recursive(self, node, text):
        """
        Método auxiliar para inserção recursiva de texto na árvore.

        Args:
            node (TreeNode): Nó atual onde a inserção é realizada.
            text (str): Texto a ser inserido.
        """
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
        """
        Busca um texto que contenha o termo fornecido.

        Args:
            query (str): Termo de busca (case insensitive).

        Returns:
            str or None: Texto encontrado ou None se nenhum texto corresponder.
        """
        return self._search_recursive(self.root, query)

    def _search_recursive(self, node, query):
        """
        Método auxiliar para busca recursiva na árvore.

        Args:
            node (TreeNode): Nó atual onde a busca é realizada.
            query (str): Termo de busca.

        Returns:
            str or None: Texto encontrado ou None se nenhum texto corresponder.
        """
        if not node:
            return None
        if query in node.text.lower():
            return node.text
        if query < node.text.lower():
            return self._search_recursive(node.left, query)
        return self._search_recursive(node.right, query)

    def inorder_traversal(self):
        """
        Realiza um percurso em-ordem na árvore e retorna uma lista de textos ordenados.

        Returns:
            list: Lista de textos armazenados em ordem alfabética.
        """
        texts = []
        self._inorder_recursive(self.root, texts)
        return texts

    def _inorder_recursive(self, node, texts):
        """
        Método auxiliar para percurso em-ordem recursivo.

        Args:
            node (TreeNode): Nó atual sendo visitado.
            texts (list): Lista acumulada de textos.
        """
        if node:
            self._inorder_recursive(node.left, texts)
            texts.append(node.text)
            self._inorder_recursive(node.right, texts)
