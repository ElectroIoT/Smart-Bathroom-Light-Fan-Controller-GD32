/* 64 KB flash (last page reserved) / 20 KB SRAM: figures from the project brief, NOT yet checked
 * against the datasheet. Using the smaller figures is the safe side. */
MEMORY
{
  FLASH (rx)  : ORIGIN = 0x08000000, LENGTH = 63K   /* last 1 KB page is reserved for saved settings */
  RAM   (rwx) : ORIGIN = 0x20000000, LENGTH = 20K
}
_estack = ORIGIN(RAM) + LENGTH(RAM);
ENTRY(Reset_Handler)
SECTIONS
{
  .isr_vector : { KEEP(*(.isr_vector)) } > FLASH
  .text : { *(.text*) *(.rodata*) . = ALIGN(4); } > FLASH
  _sidata = LOADADDR(.data);
  .data : { _sdata = .; *(.data*) . = ALIGN(4); _edata = .; } > RAM AT > FLASH
  .bss  : { _sbss = .; *(.bss*) *(COMMON) . = ALIGN(4); _ebss = .; } > RAM
  /DISCARD/ : { *(.ARM.exidx*) *(.comment) }
}
