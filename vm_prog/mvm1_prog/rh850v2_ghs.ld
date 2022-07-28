MEMORY
{
	INTROM(raxi) : org = 0x00200000, len = 1M
	INTRAM(wax)  : org = 0xfe040000, len = 128k
}

SECTIONS
{
    provide(hardware_init_hook = 0);
    provide(software_init_hook = 0);
    provide(software_term_hook = 0);

	.text :
	{
		__text = . ;
		*(.reset.text)
		*(.text)
		_etext = . ;
	} > INTROM

	.call_table_data ALIGN (4) :
	{
	PROVIDE(__ctbp = .);
	*(.call_table_data)
	} > INTROM
	
	.call_table_text :
	{
	*(.call_table_text)
	} > INTROM

	. = ALIGN(4);
	.rosdata : { *(.rosdata) } > INTROM

	. = ALIGN(4);
	.rodata : { *(.rodata) } > INTROM

	_textend = .;

	. = ALIGN(4);
	__idata_start = . ;

	.data	:  AT(__idata_start) 
	{
		. = ALIGN(4);
		__data_start = . ;
		*(.data)
		. = ALIGN(8);
	} > INTRAM

	.sdabase  : AT(__idata_start + SIZEOF(.data))
	.sdata    : AT(__idata_start + SIZEOF(.data))
	{
		*(.sdata)
		_edata	=  . ;
	} > INTRAM

	__idata_end = __idata_start + SIZEOF(.data) + SIZEOF(.sdata);

	. = ALIGN(4);
	 __sbss_start = .;
	.sbss                  :
	{
		. = ALIGN(4);
		*(.sbss)
	} > INTRAM

	.bss	   :
	{
	  . = ALIGN(4);
	  *(.bss)
	  *(COMMON)
	} > INTRAM

	. = ALIGN(4);
	__bss_end = .;

	.tdata ALIGN (4) :
	{
		PROVIDE (__ep = .);
		*(.tbyte)
		*(.tcommon_byte)
		*(.tdata)
		*(.tbss)
		*(.tcommon)
	} > INTRAM

	.stack_core0 :
	{
	. += 0x8000;
	_stack_core0 = .;
	*(.stack_core0)
	} > INTRAM

	.stack_core1 :
	{
	. += 0x8000;
	_stack_core1 = .;
	*(.stack_core1)
	} > INTRAM

	.comment  : { *(.comment) } > INTROM
	.debug			 : { *(.debug) } > INTROM
	.line			 : { *(.line) } > INTROM
	.debug_srcinfo	 : { *(.debug_srcinfo) } > INTROM
	.debug_sfnames	 : { *(.debug_sfnames) } > INTROM
}
/* provide a pointer for the stack */
 