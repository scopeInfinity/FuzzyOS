### Memory Layout

| From      | To        | Size      | Usage                                         |
|---------  |--------   |--------   |--------------------------------------------   |
| 0x00000   | fill      | 31KB      | RESERVED                                      |
| 0x7C00    | 0x7DFF    | 512B      | BOOTLOADER STAGE 1                            |
| 0x7E00    | 0x7FFF    | 512B      | SHARED STATIC CODE for real_mode library      |
| 0x8000    | 0xBFFF    | -         | BOOT LOADER STAGE 2  + own stack              |
| 0x10000   | 0x2FFFF   | -         | KERNEL                                        |
| 0x30000   | 0x4FFFF   | -         | Application 0                                 |
| 0x50000   | 0x6FFFF   | -         | Application 1                                 |
...
| -         | 0xFFFFF   | -         | 20-bit memory limit                           |