MEMORY
{
    ram : ORIGIN = 0x30000000, LENGTH = 0x1000
}
SECTIONS
{
    .text   : { *(.text*)   } > ram
}
