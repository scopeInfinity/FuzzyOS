### Memory Layout

| From      | To        | Size      | Usage                                         |
|---------  |--------   |--------   |--------------------------------------------   |
| 0x00000   | 0x003FF   | -         | IVT (standard)                                |
| 0x00400   | 0x07BFF   | -         | BIOS data area + reserve                      |
| 0x07C00   | 0x07DFF   |  512B     | Bootloader Stage 1                            |
| 0x07E00   | 0x07FFF   |  512B     | STATIC CODE for real_mode library             |
| 0x08000   | 0x0BFFF   |  16KB     | Bootloader Stage 2  + own stack               |
| 0x0C000   | 0x6FFFF   | 400KB     | Kernel  + (core, isr, syscall) stack          |
| 0x70000   | 0x7FFFF   |  64KB     | Extra space for real_mode library+client      |
| 0x80000   | 0x9FFFF   | 128KB     | Extended BIOS Data area                       |
| 0xA0000   | 0xBFFFF   | 128KB     | Video memory                                  |
| 0xC0000   | 0xFFFFF   | 256KB     | BIOS stuff                                    |
| 0x100000  | 0x1FFFFF  | 1MB       | pid 0; Kernel future location (not used)      |
| 0x200000  | 0x2FFFFF  | 1MB       | App pid 1                                     |
| 0x300000  | 0x3FFFFF  | 1MB       | App pid 2                                     |
| ...       | ....      | ...       | ...                                           |
| ...       | ....      | 1MB       | App pid 126                                   |
