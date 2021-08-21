QEMU_SHUT_FLAGS= -no-shutdown -no-reboot
QEMU_EXTRA_FLAGS=
QEMU_GDB_PORT=9000
QEMU_MONITOR_PORT=55555
GDB_EX='echo Use GDB_EX="<command>" to execute command on connect.\n'

qemu: $(image_vmdk)
	qemu-system-x86_64 -smp 1 -m 128M -hda $< $(QEMU_SHUT_FLAGS) $(QEMU_EXTRA_FLAGS)

qemu_vvv: $(image_vmdk)
	qemu-system-x86_64 -smp 1 -m 128M -hda $< $(QEMU_SHUT_FLAGS) $(QEMU_EXTRA_FLAGS) -d  cpu,exec,in_asm

qemu_monitor: $(image_vmdk)
	qemu-system-x86_64 -smp 1 -m 128M -hda $< $(QEMU_SHUT_FLAGS) $(QEMU_EXTRA_FLAGS) -monitor telnet:127.0.0.1:$(QEMU_MONITOR_PORT),server,nowait

qemu_debug: $(image_vmdk)
	qemu-system-x86_64 -S -gdb tcp::$(QEMU_GDB_PORT) -smp 1 -m 128M -hda $< $(QEMU_SHUT_FLAGS) -d  cpu,exec,in_asm

qemu_debug_connect:
	gdb -x $(ROOT_DIR)/.gdbinit -ex "target remote :$(QEMU_GDB_PORT)" -ex "$(GDB_EX)" -ex 'continue'


