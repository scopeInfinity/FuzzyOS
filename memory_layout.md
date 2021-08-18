### Memory Layout

| From      | To        | Size      | Usage                                         |
|---------  |--------   |--------   |--------------------------------------------   |
| 0x00000   | fill      | 31KB      | RESERVED                                      |
| 0x7C00    | 0x7DFF    | 512B      | BOOTLOADER STAGE 1                            |
| 0x7E00    | 0x7EFF    | 256B      | SHARED STATIC CODE for real_mode library      |
| 0x7F00    | 0x7FFF    | 256B      | SHARED STATIC MEMORY for real_mode library    |
| 0x8000    | 0xBFFF    | -         | BOOT LOADER STAGE 2  + own stack              |
| 0xC000    | 0x1FFFF   | -         | KERNEL                                        |
| 0x20000   | 0x2FFFF   | -         | Application 0                                 |
| 0x30000   | 0x3FFFF   | -         | Application 1                                 |
...
| -         | 0xFFFFF   | -         | 20-bit memory limit                           |