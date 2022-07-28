MEMORY
{
	RAM_BANKA_OPB(raxi) : org = 0x00000000, len = 512
	RAM_BANKA(raxi) : org = 0x00000200, len = 4M - 512
	RAM_BANKB(raxi) : org = 0x00400000, len = 2M
	LRAM0(wax)      : org = 0xFDC00000, len = 64k
	LRAM1(wax)      : org = 0xFDA00000, len = 64k
	CRAM0(wax)      : org = 0xFE000000, len = 512k
	CRAM2(wax)      : org = 0xFE400000, len = 512k
	SCDS2           : org = 0xF8F00C20, len = 24
}

SECTIONS
{
    provide(_hardware_init_hook = 0);
    provide(_software_init_hook = 0);
    provide(_software_term_hook = 0);

	.text :  AT(0x00000200)
	{
		__text = . ;
		*(.reset.text)
		*(.text)
		_etext = . ;
	} > RAM_BANKA

	.call_table_data ALIGN (4) :
	{
	PROVIDE(__ctbp = .);
	*(.call_table_data)
	} > RAM_BANKA
	
	.call_table_text :
	{
	*(.call_table_text)
	} > RAM_BANKA

	. = ALIGN(4);
	.rosdata : { *(.rosdata) } > RAM_BANKA

	. = ALIGN(4);
	.rodata : { *(.rodata) } > RAM_BANKA

	_textend = .;

	. = ALIGN(4);
	__idata_start = . ;

	.data	:  AT(__idata_start) 
	{
		. = ALIGN(4);
		__data_start = . ;
		*(.data)
		. = ALIGN(8);
	} > CRAM2

	.sdabase  : AT(__idata_start + SIZEOF(.data))
	.sdata    : AT(__idata_start + SIZEOF(.data))
	{
		*(.sdata)
		_edata	=  . ;
	} > CRAM2

	__idata_end = __idata_start + SIZEOF(.data) + SIZEOF(.sdata);

	. = ALIGN(4);
	__sbss_start = .;
	.sbss                  :
	{
		. = ALIGN(4);
		*(.sbss)
	} > CRAM2
	. = ALIGN(4);
	__sbss_end = .;

	.bss	   :
	{
	  . = ALIGN(4);
	  __bss_start = .;
	  *(.bss)
	  *(COMMON)
	} > CRAM2

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
	} > CRAM2

	.cbss0	   :
	{
	 __cbss0_start = .;
	  . = ALIGN(4);
	  *(.cbss0)
	} > CRAM0
	. = ALIGN(4);
	__cbss0_end = .;

	.lbss0	   :
	{
	 __lbss0_start = .;
	  . = ALIGN(4);
	  *(.lbss0)
	} > LRAM0
	. = ALIGN(4);
	__lbss0_end = .;

	.lbss1	   :
	{
	 __lbss1_start = .;
	  . = ALIGN(4);
	  *(.lbss1)
	} > LRAM1
	. = ALIGN(4);
	__lbss1_end = .;

	.rodata_csaf :
	{ 
		*(.csaf.rodata)
	} > SCDS2

	.rodata_csa :
	{ 
		*(.csa.rodata)
	} > SCDS2

	.comment  : { *(.comment) } > RAM_BANKA
	.debug			 : { *(.debug) } > RAM_BANKA
	.line			 : { *(.line) } > RAM_BANKA
	.debug_srcinfo	 : { *(.debug_srcinfo) } > RAM_BANKA
	.debug_sfnames	 : { *(.debug_sfnames) } > RAM_BANKA
}
/* provide a pointer for the stack */
 