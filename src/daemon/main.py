import platform

if platform.system() == "Linux":
    from daemon_linux import run_daemon
elif platform.system() == "Windows":
    from daemon_win import run_daemon
else:
    print("Operation System not suport")

run_daemon()
