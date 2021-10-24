SECTIONS
{
  .text ALIGN(8) : { *(.text) }
  .data ALIGN(8) : {
    *(.data)
  }
  .bss ALIGN(8) : {
    *(.bss)
  }
  _heap_start = .;
}