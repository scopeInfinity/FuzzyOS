QEMU_SHUT_FLAGS= -no-shutdown -no-reboot
QEMU_EXTRA_FLAGS=
QEMU_GDB_PORT=9000

qemu: $(image_vmdk)
	qemu-system-x86_64 -smp 1 -m 128M -hda $< $(QEMU_SHUT_FLAGS) $(QEMU_EXTRA_FLAGS)

qemu_vvv: $(image_vmdk)
	qemu-system-x86_64 -smp 1 -m 128M -hda $< $(QEMU_SHUT_FLAGS) $(QEMU_EXTRA_FLAGS) -d  cpu,exec,in_asm

qemu_debug: $(image_vmdk)
	qemu-system-x86_64 -S -gdb tcp::$(QEMU_GDB_PORT) -smp 1 -m 128M -hda $< $(QEMU_SHUT_FLAGS) -d  cpu,exec,in_asm

qemu_debug_connect:
	gdb -x $(ROOT_DIR)/.gdbinit -ex "target remote :$(QEMU_GDB_PORT)"


