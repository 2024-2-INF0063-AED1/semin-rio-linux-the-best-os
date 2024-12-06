from search_tree import ClipboardSearchTree

class ClipboardManager:
    """
    Classe responsável por gerenciar o texto copiado na área de transferência
    utilizando uma estrutura de árvore de busca para armazenamento e recuperação.
    """

    def __init__(self):
        """
        Inicializa o gerenciador de área de transferência com uma árvore de busca.
        """
        self.clipboard_tree = ClipboardSearchTree()

    def add_clipboard_text(self, text):
        """
        Adiciona um novo texto à árvore de busca e exibe os textos armazenados.

        Args:
            text (str): Texto a ser adicionado à área de transferência.
        """
        print(f"Adicionando texto ao clipboard: {text}")
        self.clipboard_tree.insert(text)
        self.get_all_texts()

    def search_clipboard(self, query):
        """
        Pesquisa por um texto específico na árvore de busca.

        Args:
            query (str): Texto a ser pesquisado na área de transferência.

        Returns:
            bool: Verdadeiro se o texto for encontrado, falso caso contrário.
        """
        return self.clipboard_tree.search(query)

    def get_all_texts(self):
        """
        Recupera e exibe todos os textos armazenados na árvore de busca
        em ordem crescente.

        Returns:
            list: Lista de textos armazenados na árvore, ordenados.
        """
        print("Chamada get all")
        return self.clipboard_tree.inorder_traversal()
