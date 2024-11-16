1. Estrutura Geral

- [] Criar o arquivo daemon.py.
- [] Identificar as bibliotecas necessárias para:

Monitorar eventos do sistema operacional (como copiar texto no clipboard).
Comunicação via socket.
Log para debugar problemas futuros.

2. Funcionalidades do Daemon

Monitorar o clipboard:

    Linux: Wayland (wl-clipboard) ou X11 (xclip/xsel).
    Windows: Clipboard API.

Capturar eventos:

    Copiar texto (Ctrl+C).
    Outros comandos (como simular deletar ou buscar).

    Enviar dados ao main.c:
        Configurar um cliente socket para envio das mensagens.

3. Configuração do Ambiente

Configurar o nvim para facilitar a edição:

    Colocar o editor em background.
    Retomar o editor do background.

Testar o daemon conforme avança:

    Simular eventos no clipboard.
    Validar logs e conexão via socket.
