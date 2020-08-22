### Memory Layout

| From      | To        | Size      | Usage                                         |
|---------  |--------   |--------   |--------------------------------------------   |
|   0x0000  |     fill  |     31 KB | RESERVED                                      |
|   0x7C00  |   0x7DFF  |    512  B | BOOTLOADER STAGE 1                            |
|   0x7E00  |   0x7EFF  |    256  B | SHARED STATIC CODE for real_mode library      |
|   0x7F00  |   0x7FFF  |    256  B | SHARED STATIC MEMORY for real_mode library    |
|   0x8000  |   0xB1FF  |   12.5 KB | BOOT LOADER STAGE 2                           |
|   0xB200  |   0xFFFF  |     19 KB | Real Mode Stack                               |
|  0x10000  |  0x16200  |         - | Kernel Core                                   |
|        -  |  0x1B000  |         - | Kernel Core Stack                             |
|        -  |  0x1FFFF  |         - | Kernel Interrupts Stack (not ideal)           |
|  0x20000  |  0x1FFFF  |     64 KB | Application 0                                 |
|  0x30000  |  0x2FFFF  |     64 KB | Application 1                                 |
|        -  |        -  |         - | --- Real Mode Memory Limit ---                |
|  0xA0000  |  0xBFFFF  |         - | VGA mapped memory                             |
