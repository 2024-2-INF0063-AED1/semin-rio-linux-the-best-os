import pyperclip
import time
import signal
import sys

class ClipboardDaemon:
    """
    Um daemon que monitora o conteúdo da área de transferência e executa uma ação
    sempre que um novo texto é copiado.

    A classe utiliza um gerenciador de área de transferência (clipboard_manager) para
    lidar com o texto copiado.
    """

    def __init__(self, clipboard_manager):
        """
        Inicializa o daemon com um gerenciador de área de transferência.

        Args:
            clipboard_manager: Objeto responsável por gerenciar o texto copiado.
        """
        self.clipboard_manager = clipboard_manager
        self.running = True

    def monitor_clipboard(self):
        """
        Monitora continuamente o conteúdo da área de transferência.
        Sempre que um novo texto é detectado, ele é enviado ao gerenciador.
        """
        previous_clipboard = ""

        while self.running:
            current_clipboard = pyperclip.paste()
            if current_clipboard != previous_clipboard:
                print(f"Novo conteúdo copiado: {current_clipboard}")
                self.clipboard_manager.add_clipboard_text(current_clipboard)
                previous_clipboard = current_clipboard
            time.sleep(1)

    def start(self):
        """
        Inicia o monitoramento da área de transferência e configura o sinal
        para interrupção segura do processo.
        """
        signal.signal(signal.SIGTERM, self.stop)
        self.monitor_clipboard()

    def stop(self, signum, frame):
        """
        Encerra o monitoramento da área de transferência de forma segura.

        Args:
            signum: Número do sinal recebido.
            frame: Frame atual da execução do programa.
        """
        self.running = False
        sys.exit(0)
