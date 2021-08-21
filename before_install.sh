#!/bin/bash
apt-get update
apt install -y make nasm gcc cpulimit qemu-system-x86 gocr expect gdb graphicsmagick-imagemagick-compat

python3 -m pip install -r requirements.txt
