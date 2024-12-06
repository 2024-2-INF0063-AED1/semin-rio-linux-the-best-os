import multiprocessing
from multiprocessing.managers import BaseManager
from clipboard_manager import ClipboardManager
from clipboard_daemon import ClipboardDaemon
from interface import ClipboardInterface

class ClipboardManagerManager(BaseManager):
    """
    Extende o BaseManager para gerenciar instâncias do ClipboardManager.

    A classe é necessária para compartilhar uma única instância do ClipboardManager
    entre diferentes processos usando o módulo multiprocessing.
    """
    pass

# Registra a classe ClipboardManager para ser gerenciada pelo ClipboardManagerManager
ClipboardManagerManager.register('ClipboardManager', ClipboardManager)

def start_daemon(clipboard_manager):
    """
    Função para iniciar o daemon da área de transferência.

    Args:
        clipboard_manager (ClipboardManager): Instância do ClipboardManager usada para gerenciar dados da área de transferência.
    """
    daemon = ClipboardDaemon(clipboard_manager)
    daemon.start()

if __name__ == "__main__":
    # Cria um gerenciador para compartilhar o ClipboardManager entre processos
    manager = ClipboardManagerManager()
    manager.start()

    # Obtém uma instância compartilhada do ClipboardManager
    clipboard_manager = manager.ClipboardManager()

    # Inicia o daemon em um processo separado
    daemon_process = multiprocessing.Process(target=start_daemon, args=(clipboard_manager,))
    daemon_process.start()

    # Inicia a interface gráfica do usuário (GUI)
    interface = ClipboardInterface(clipboard_manager, daemon_process.pid)
    interface.run()

    # Aguarda o término do processo do daemon antes de encerrar
    daemon_process.join()
