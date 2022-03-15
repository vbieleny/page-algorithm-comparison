import subprocess
import sys

subprocess.Popen(
    ["qemu-system-i386", "-s", "-S", "-drive", f"file={sys.argv[1]},format=raw,index=0,media=disk"]
)
