;/*
 ;*  例外ベクタテーブル
 ;*/

    ;/* 
     ;*  HVタイウィンドウ実行時のベクタテーブル
     ;*/
    .cseg text
    .align 512
    .extern _EBaseHVTM
_EBaseHVTM:
    jr _Bootup
    .rept 6
    nop
    .endm
SYSERR_HVTW: ;/* 010H 010H */
    .rept 8
    nop
    .endm
HVTRAP_HVTW: ;/* 020H 020H */
    jr hvtrap_handler
    .rept 6
    nop
    .endm
FETRAP_HVTW: ;/* 030H 030H */
    .rept 8
    nop
    .endm
TRAP0_HVTW: ;/* 040H 040H */
    .rept 8
    nop
    .endm
TRAP1_HVTW: ;/* 050H 050H */
    .rept 8
    nop
    .endm
RIE_HVTW: ;/* 060H 060H */
    .rept 8
    nop
    .endm
FPPFPI_HVTW: ;/* 070H 070H */
    .rept 8
    nop
    .endm
UCPOP_HVTW: ;/* 080H 080H */
    .rept 8
    nop
    .endm
MIP_HVTW: ;/* 090H 090H */
    jr mip_handler
    .rept 6
    nop
    .endm
PIE_HVTW: ;/* 0A0H 0A0H */
    .rept 8
    nop
    .endm
DEBUG_HVTW: ;/* 0B0H 0B0H */
    .rept 8
    nop
    .endm
MAE_HVTW: ;/* 0C0H 0C0H */
    jr MAE_HVTW
    .rept 6
    nop
    .endm
RFU_HVTW: ;/* 0D0H 0D0H */
    .rept 8
    nop
    .endm
FENMI_HVTW: ;/* 0E0H 0E0H */
    .rept 8
    nop
    .endm
FEINT_HVTW: ;/* 0F0H 0F0H */
    .rept 8
    nop
    .endm
EIINTn0_HVTW: ;/* （優先度0） 100H */
    jr hvint_in_hvtm
    .rept 6
    nop
    .endm
EIINTn1_HVTW: ;/* （優先度1） 110H */
    jr hvint_in_hvtm
    .rept 6
    nop
    .endm
EIINTn2_HVTW: ;/* （優先度2） 120H */
    jr hvint_in_hvtm
    .rept 6
    nop
    .endm
EIINTn3_HVTW: ;/* （優先度3） 130H */
    jr hvint_in_hvtm
    .rept 6
    nop
    .endm
EIINTn4_HVTW: ;/* （優先度4） 140H */
    jr hvint_in_hvtm
    .rept 6
    nop
    .endm
EIINTn5_HVTW: ;/* （優先度5） 150H */
    jr hvint_in_hvtm
    .rept 6
    nop
    .endm
EIINTn6_HVTW: ;/* （優先度6） 160H */
    jr hvint_in_hvtm
    .rept 6
    nop
    .endm
EIINTn7_HVTW: ;/* （優先度7） 170H */
    jr hvint_in_hvtm
    .rept 6
    nop
    .endm
EIINTn8_HVTW: ;/* （優先度8） 180H */
    jr int_handler
    .rept 6
    nop
    .endm
EIINTn9_HVTW: ;/* （優先度9） 190H */
    jr hvint_in_hvtm
    .rept 6
    nop
    .endm
EIINTn10_HVTW: ;/* （優先度10） 1A0H */
    jr hvint_in_hvtm
    .rept 6
    nop
    .endm
EIINTn11_HVTW: ;/* （優先度11） 1B0H */
    jr hvint_in_hvtm
    .rept 6
    nop
    .endm
EIINTn12_HVTW: ;/* （優先度12） 1C0H */
    jr hvint_in_hvtm
    .rept 6
    nop
    .endm
EIINTn13_HVTW: ;/* （優先度13） 1D0H */
    jr hvint_in_hvtm
    .rept 6
    nop
    .endm
EIINTn14_HVTW: ;/* （優先度14） 1E0H */
    jr twdint_in_hvtw
    .rept 6
    nop
    .endm
EIINTn15_HVTW: ;/* （優先度15） 1F0H */
    jr syscint_in_hvtw
    .rept 6
    nop
    .endm

    ;/* 
     ;*  アイドル区間実行時のベクタテーブル
     ;*/
    .cseg text
    .align 512
    .extern _EBaseIDLE
_EBaseIDLE:
    jr _Bootup
    .rept 6
    nop
    .endm
SYSERR_IDLE: ;/* 010H 010H */
    .rept 8
    nop
    .endm
HVTRAP_IDLE: ;/* 020H 020H */
    jr hvtrap_handler
    .rept 6
    nop
    .endm
FETRAP_IDLE: ;/* 030H 030H */
    .rept 8
    nop
    .endm
TRAP0_IDLE: ;/* 040H 040H */
    .rept 8
    nop
    .endm
TRAP1_IDLE: ;/* 050H 050H */
    .rept 8
    nop
    .endm
RIE_IDLE: ;/* 060H 060H */
    .rept 8
    nop
    .endm
FPPFPI_IDLE: ;/* 070H 070H */
    .rept 8
    nop
    .endm
UCPOP_IDLE: ;/* 080H 080H */
    .rept 8
    nop
    .endm
MIP_IDLE: ;/* 090H 090H */
    jr mip_handler
    .rept 6
    nop
    .endm
PIE_IDLE: ;/* 0A0H 0A0H */
    .rept 8
    nop
    .endm
DEBUG_IDLE: ;/* 0B0H 0B0H */
    .rept 8
    nop
    .endm
MAE_IDLE: ;/* 0C0H 0C0H */
    jr MAE_IDLE
    .rept 6
    nop
    .endm
RFU_IDLE: ;/* 0D0H 0D0H */
    .rept 8
    nop
    .endm
FENMI_IDLE: ;/* 0E0H 0E0H */
    .rept 8
    nop
    .endm
FEINT_IDLE: ;/* 0F0H 0F0H */
    .rept 8
    nop
    .endm
EIINTn0_IDLE: ;/* （優先度0） 100H */
    jr hvint_in_idle
    .rept 6
    nop
    .endm
EIINTn1_IDLE: ;/* （優先度1） 110H */
    jr hvint_in_idle
    .rept 6
    nop
    .endm
EIINTn2_IDLE: ;/* （優先度2） 120H */
    jr hvint_in_idle
    .rept 6
    nop
    .endm
EIINTn3_IDLE: ;/* （優先度3） 130H */
    jr hvint_in_idle
    .rept 6
    nop
    .endm
EIINTn4_IDLE: ;/* （優先度4） 140H */
    jr hvint_in_idle
    .rept 6
    nop
    .endm
EIINTn5_IDLE: ;/* （優先度5） 150H */
    jr hvint_in_idle
    .rept 6
    nop
    .endm
EIINTn6_IDLE: ;/* （優先度6） 160H */
    jr hvint_in_idle
    .rept 6
    nop
    .endm
EIINTn7_IDLE: ;/* （優先度7） 170H */
    jr hvint_in_idle
    .rept 6
    nop
    .endm
EIINTn8_IDLE: ;/* （優先度8） 180H */
    jr int_handler
    .rept 6
    nop
    .endm
EIINTn9_IDLE: ;/* （優先度9） 190H */
    jr hvint_in_idle
    .rept 6
    nop
    .endm
EIINTn10_IDLE: ;/* （優先度10） 1A0H */
    jr hvint_in_idle
    .rept 6
    nop
    .endm
EIINTn11_IDLE: ;/* （優先度11） 1B0H */
    jr hvint_in_idle
    .rept 6
    nop
    .endm
EIINTn12_IDLE: ;/* （優先度12） 1C0H */
    jr hvint_in_idle
    .rept 6
    nop
    .endm
EIINTn13_IDLE: ;/* （優先度13） 1D0H */
    jr hvint_in_idle
    .rept 6
    nop
    .endm
EIINTn14_IDLE: ;/* （優先度14） 1E0H */
    jr twdint_in_idle
    .rept 6
    nop
    .endm
EIINTn15_IDLE: ;/* （優先度15） 1F0H */
    jr syscint_in_idle
    .rept 6
    nop
    .endm
