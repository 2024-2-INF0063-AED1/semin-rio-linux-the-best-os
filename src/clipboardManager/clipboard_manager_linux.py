import subprocess
import daemon.daemon_linux

# Mover a função para ClipboarManager
def handle_signal(sig, frame):
    clipboard_content = get_clipboard_content()  # Captura o conteúdo copiado
    if clipboard_content:
        daemon.daemon_linux.send_to_server(clipboard_content)
    else:
        print("Nenhum conteúdo copiado encontrado!")

def get_clipboard_content():
    try:
        result = subprocess.check_output(['xclip', '-selection', 'clipboard', '-o'])
        return result.decode('utf-8').strip()  # Remove os espaços extras
    except subprocess.CalledProcessError:
        return None  # Caso não consiga pegar o conteúdo do clipboard

