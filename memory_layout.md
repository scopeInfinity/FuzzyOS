### Memory Layout

| From      | To        | Size      | Usage                                         |
|---------  |--------   |--------   |--------------------------------------------   |
| 0x00000   | 0x003FF   | -         | IVT (standard)                                |
| 0x00400   | 0x07BFF   | -         | BIOS data area + reserve                      |
| 0x07C00   | 0x07DFF   | 512B      | Bootloader Stage 1                            |
| 0x07E00   | 0x07FFF   | 512B      | STATIC CODE for real_mode library             |
| 0x08000   | 0x0BFFF   | -         | Bootloader Stage 2  + own stack               |
| 0x0F000   | 0x2EFFF   | 128KB     | Kernel  + (core, irq, syscall) stack          |
| 0x30000   | 0x4FFFF   | 128KB     | App pid 1                                     |
| 0x50000   | 0x6FFFF   | 128KB     | App pid 2                                     |
| 0x70000   | 0x7FFFF   |  64KB     | Extra space for real_mode library+client      |
| 0x80000   | 0x9FFFF   | 128KB     | Extended BIOS Data area                       |
| 0xA0000   | 0xBFFFF   | 128KB     | Video memory                                  |
| 0xC0000   | 0xFFFFF   | 256KB     | BIOS stuff                                    |
