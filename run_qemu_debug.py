import subprocess
import sys

command = ["qemu-system-i386", "-s", "-S", "-icount", "0", "-serial", "stdio", "-drive", f"file={sys.argv[1]},format=raw,index=0,media=disk"]
qemu = subprocess.Popen(command, start_new_session=True)
