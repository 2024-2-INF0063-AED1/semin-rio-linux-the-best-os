import multiprocessing
from multiprocessing.managers import BaseManager
from clipboard_manager import ClipboardManager
from clipboard_daemon import ClipboardDaemon
from interface import ClipboardInterface

class ClipboardManagerManager(BaseManager):
    pass

ClipboardManagerManager.register('ClipboardManager', ClipboardManager)

def start_daemon(clipboard_manager):
    daemon = ClipboardDaemon(clipboard_manager)
    daemon.start()

if __name__ == "__main__":
    manager = ClipboardManagerManager()
    manager.start()
    clipboard_manager = manager.ClipboardManager()

    # Inicia o daemon em um processo separado
    daemon_process = multiprocessing.Process(target=start_daemon, args=(clipboard_manager,))
    daemon_process.start()

    # Inicia a interface gráfica
    interface = ClipboardInterface(clipboard_manager, daemon_process.pid)
    interface.run()

    # Aguarda o término do processo do daemon
    daemon_process.join()