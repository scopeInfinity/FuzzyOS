### Memory Layout

| From      | To        | Size      | Usage                                         |
|---------  |--------   |--------   |--------------------------------------------   |
| 0x00000   | fill      | 31KB      | RESERVED                                      |
| 0x7C00    | 0x7DFF    | 512B      | BOOTLOADER STAGE 1                            |
| 0x7E00    | 0x7EFF    | 256B      | SHARED STATIC CODE for real_mode library      |
| 0x7F00    | 0x7FFF    | 256B      | SHARED STATIC MEMORY for real_mode library    |
| 0x8000    | 0xB1FF    | 12.5KB    | BOOT LOADER STAGE 2                           |
| 0xC000    | 0x10200   | -         | KERNEL                                        |
| 0x20000   | fill      | -         | Application                                   |
| -         | 0xFFFFF   | -         | 20-bit memory limit                           |