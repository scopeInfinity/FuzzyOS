SECTIONS
{
  .text ALIGN(8) : { *(.text) }
  .data ALIGN(8) : {
    *(.data)
    *(.bss)
    }
  _heap_start = .;
}