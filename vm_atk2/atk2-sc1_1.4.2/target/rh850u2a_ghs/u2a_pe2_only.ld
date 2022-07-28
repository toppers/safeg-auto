/*
 *	RH850U2Aリンカスクリプト
 */

MEMORY
{
	PE1ROM(raxi) : org = 0x00000000, len = 4M
	INTROM(raxi) : org = 0x00400000, len = 4M
	LRAM(wax)    : org = 0xFDA00000, len = 64k
	CRAM(wax)    : org = 0xFE040000, len = 256k
}

SECTIONS
{
    provide(hardware_init_hook = 0);
    provide(software_init_hook = 0);
    provide(software_term_hook = 0);

	.reset :
	{
		*(.reset.text)
	} > PE1ROM

	.text :
	{
		__text = . ;
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
	} > CRAM

	.sdabase  : AT(__idata_start + SIZEOF(.data))
	.sdata    : AT(__idata_start + SIZEOF(.data))
	{
		*(.sdata)
		_edata	=  . ;
	} > CRAM

	__idata_end = __idata_start + SIZEOF(.data) + SIZEOF(.sdata);

	.sbss                  :
	{
	  . = ALIGN(4);
	  __bss_start = .;
		*(.sbss)
	} > CRAM

	.bss	   :
	{
	  . = ALIGN(4);
	  *(.bss)
	  *(COMMON)
	  . = ALIGN(4);
	  __bss_end = .;
	} > CRAM

	.tdata ALIGN (4) :
	{
		PROVIDE (__ep = .);
		*(.tbyte)
		*(.tcommon_byte)
		*(.tdata)
		*(.tbss)
		*(.tcommon)
	} > CRAM

	.lbss	   :
	{
	 __lbss_start = .;
	  . = ALIGN(4);
	  *(.lbss)
	} > LRAM
	. = ALIGN(4);
	__lbss_end = .;

	.comment 0 : { *(.comment) }	
	.debug			0 : { *(.debug) }
	.line			0 : { *(.line) }
	.debug_srcinfo	0 : { *(.debug_srcinfo) }
	.debug_sfnames	0 : { *(.debug_sfnames) }
}
/* provide a pointer for the stack */
 