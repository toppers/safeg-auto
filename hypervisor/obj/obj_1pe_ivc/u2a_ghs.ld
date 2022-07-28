MEMORY
{
	ROM_BANKA(raxi) : org = 0x00000000, len = 4M
	ROM_BANKB(raxi) : org = 0x00400000, len = 4M
	ROM_BANKC(raxi) : org = 0x00800000, len = 4M
	ROM_BANKD(raxi) : org = 0x00C00000, len = 4M
	LRAM0(wax)      : org = 0xFDC00000, len = 64k
	LRAM1(wax)      : org = 0xFDA00000, len = 64k
	LRAM2(wax)      : org = 0xFD800000, len = 64k
	LRAM3(wax)      : org = 0xFD600000, len = 64k
	CRAM0(wax)      : org = 0xFE000000, len = 512k - 64k
	CRAM00(wax)     : org = 0xFE070000, len = 8k
	CRAM01(wax)     : org = 0xFE072000, len = 8k
	CRAM1(wax)      : org = 0xFE100000, len = 512k
	CRAM2(wax)      : org = 0xFE400000, len = 2048k
}

SECTIONS
{
    provide(hardware_init_hook = 0);
    provide(software_init_hook = 0);
    provide(software_term_hook = 0);

	.text :  AT(0)
	{
		__text = . ;
		*(.reset.text)
		*(.text)
		_etext = . ;
	} > ROM_BANKA

	.call_table_data ALIGN (4) :
	{
	PROVIDE(__ctbp = .);
	*(.call_table_data)
	} > ROM_BANKA
	
	.call_table_text :
	{
	*(.call_table_text)
	} > ROM_BANKA

	. = ALIGN(4);
	.rosdata : { *(.rosdata) } > ROM_BANKA

	. = ALIGN(4);
	.rodata : { *(.rodata) } > ROM_BANKA

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

	.cbss00	   :
	{
	 __cbss_00_start = .;
	  . = ALIGN(4);
	  *(.cbss00)
	} > CRAM00
	. = ALIGN(4);
	__cbss_00_end = .;

	.cbss01	   :
	{
	 __cbss_01_start = .;
	  . = ALIGN(4);
	  *(.cbss01)
	} > CRAM01
	. = ALIGN(4);
	__cbss_01_end = .;

	.cbss1	   :
	{
	 __cbss1_start = .;
	  . = ALIGN(4);
	  *(.cbss1)
	} > CRAM1
	. = ALIGN(4);
	__cbss1_end = .;

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

	.lbss2	   :
	{
	 __lbss2_start = .;
	  . = ALIGN(4);
	  *(.lbss2)
	} > LRAM2
	. = ALIGN(4);
	__lbss2_end = .;

	.lbss3	   :
	{
	 __lbss3_start = .;
	  . = ALIGN(4);
	  *(.lbss3)
	} > LRAM3
	. = ALIGN(4);
	__lbss3_end = .;

	.text_bankc :
	{ 
		*(.reset_bankc.text)
	} > ROM_BANKC

	.comment  : { *(.comment) } > ROM_BANKA
	.debug			 : { *(.debug) } > ROM_BANKA
	.line			 : { *(.line) } > ROM_BANKA
	.debug_srcinfo	 : { *(.debug_srcinfo) } > ROM_BANKA
	.debug_sfnames	 : { *(.debug_sfnames) } > ROM_BANKA
}
/* provide a pointer for the stack */
 