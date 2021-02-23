SECTIONS
{
  .text ALIGN(8) : { *(.text) }
  .data ALIGN(8) : { *(.data) *(.bss) }
}