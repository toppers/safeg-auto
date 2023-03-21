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
    jr32 _Bootup

    .align 16
SYSERR_HVTW: ;/* 010H 010H */
    jr32 hv_fe_handler

    .align 16
HVTRAP_HVTW: ;/* 020H 020H */
    jr32 hvtrap_handler

    .align 16
FETRAP_HVTW: ;/* 030H 030H */
    jr32 hv_fe_handler

    .align 16
TRAP0_HVTW: ;/* 040H 040H */
    jr32 hv_ei_handler

    .align 16
TRAP1_HVTW: ;/* 050H 050H */
    jr32 hv_ei_handler

    .align 16
RIE_HVTW: ;/* 060H 060H */
    jr32 hv_fe_handler

    .align 16
FPPFPI_HVTW: ;/* 070H 070H */
    jr32 hv_ei_handler

    .align 16
UCPOP_HVTW: ;/* 080H 080H */
    jr32 hv_fe_handler

    .align 16
MIP_HVTW: ;/* 090H 090H */
    jr32 vm_hv_fe_handler

    .align 16
PIE_HVTW: ;/* 0A0H 0A0H */
    jr32 hv_fe_handler

    .align 16
DEBUG_HVTW: ;/* 0B0H 0B0H */
    jr32 hv_db_handler 

    .align 16
MAE_HVTW: ;/* 0C0H 0C0H */
    jr32 hv_fe_handler

    .align 16
RFU_HVTW: ;/* 0D0H 0D0H */
    jr32 RFU_HVTW

    .align 16
FENMI_HVTW: ;/* 0E0H 0E0H */
    jr32 hv_fe_handler

    .align 16
FEINT_HVTW: ;/* 0F0H 0F0H */
    jr32 hv_fe_handler

    .align 16
EIINTn0_HVTW: ;/* （優先度0） 100H */
    jr32 hvint_in_hvtm

    .align 16
EIINTn1_HVTW: ;/* （優先度1） 110H */
    jr32 hvint_in_hvtm

    .align 16
EIINTn2_HVTW: ;/* （優先度2） 120H */
    jr32 hvint_in_hvtm

    .align 16
EIINTn3_HVTW: ;/* （優先度3） 130H */
    jr32 hvint_in_hvtm

    .align 16
EIINTn4_HVTW: ;/* （優先度4） 140H */
    jr32 hvint_in_hvtm

    .align 16
EIINTn5_HVTW: ;/* （優先度5） 150H */
    jr32 hvint_in_hvtm

    .align 16
EIINTn6_HVTW: ;/* （優先度6） 160H */
    jr32 hvint_in_hvtm

    .align 16
EIINTn7_HVTW: ;/* （優先度7） 170H */
    jr32 hvint_in_hvtm

    .align 16
EIINTn8_HVTW: ;/* （優先度8） 180H */
    jr32 hvint_in_hvtm

    .align 16
EIINTn9_HVTW: ;/* （優先度9） 190H */
    jr32 hvint_in_hvtm

    .align 16
EIINTn10_HVTW: ;/* （優先度10） 1A0H */
    jr32 hvint_in_hvtm

    .align 16
EIINTn11_HVTW: ;/* （優先度11） 1B0H */
    jr32 hvint_in_hvtm

    .align 16
EIINTn12_HVTW: ;/* （優先度12） 1C0H */
    jr32 hvint_in_hvtm

    .align 16
EIINTn13_HVTW: ;/* （優先度13） 1D0H */
    jr32 hvint_in_hvtm

    .align 16
EIINTn14_HVTW: ;/* （優先度14） 1E0H */
    jr32 twdint_in_hvtw

    .align 16
EIINTn15_HVTW: ;/* （優先度15） 1F0H */
    jr32 syscint_in_hvtw


    ;/* 
     ;*  アイドル区間実行時のベクタテーブル
     ;*/
    .cseg text
    .align 512
    .extern _EBaseIDLE
_EBaseIDLE:
    jr32 _Bootup

    .align 16
SYSERR_IDLE: ;/* 010H 010H */
    jr32 hv_fe_handler

    .align 16
HVTRAP_IDLE: ;/* 020H 020H */
    jr32 hvtrap_handler

    .align 16
FETRAP_IDLE: ;/* 030H 030H */
    jr32 hv_fe_handler

    .align 16
TRAP0_IDLE: ;/* 040H 040H */
    jr32 trap0_in_idle

    .align 16
TRAP1_IDLE: ;/* 050H 050H */
    jr32 hv_ei_handler

    .align 16
RIE_IDLE: ;/* 060H 060H */
    jr32 hv_fe_handler

    .align 16    
FPPFPI_IDLE: ;/* 070H 070H */
    jr32 hv_ei_handler

    .align 16
UCPOP_IDLE: ;/* 080H 080H */
    jr32 hv_fe_handler

    .align 16    
MIP_IDLE: ;/* 090H 090H */
    jr32 hv_fe_handler

    .align 16    
PIE_IDLE: ;/* 0A0H 0A0H */
    jr32 hv_fe_handler

    .align 16    
DEBUG_IDLE: ;/* 0B0H 0B0H */
    jr32 hv_db_handler 

    .align 16    
MAE_IDLE: ;/* 0C0H 0C0H */
    jr32 hv_fe_handler

    .align 16    
RFU_IDLE: ;/* 0D0H 0D0H */
    jr32 RFU_IDLE

    .align 16
FENMI_IDLE: ;/* 0E0H 0E0H */
    jr32 hv_fe_handler

    .align 16   
FEINT_IDLE: ;/* 0F0H 0F0H */
    jr32 hv_fe_handler

    .align 16
EIINTn0_IDLE: ;/* （優先度0） 100H */
    jr32 hvint_in_idle

    .align 16
EIINTn1_IDLE: ;/* （優先度1） 110H */
    jr32 hvint_in_idle

    .align 16
EIINTn2_IDLE: ;/* （優先度2） 120H */
    jr32 hvint_in_idle

    .align 16
EIINTn3_IDLE: ;/* （優先度3） 130H */
    jr32 hvint_in_idle

    .align 16
EIINTn4_IDLE: ;/* （優先度4） 140H */
    jr32 hvint_in_idle

    .align 16    
EIINTn5_IDLE: ;/* （優先度5） 150H */
    jr32 hvint_in_idle

    .align 16    
EIINTn6_IDLE: ;/* （優先度6） 160H */
    jr32 hvint_in_idle

    .align 16   
EIINTn7_IDLE: ;/* （優先度7） 170H */
    jr32 hvint_in_idle

    .align 16    
EIINTn8_IDLE: ;/* （優先度8） 180H */
    jr32 hvint_in_hvtm

    .align 16   
EIINTn9_IDLE: ;/* （優先度9） 190H */
    jr32 hvint_in_idle

    .align 16    
EIINTn10_IDLE: ;/* （優先度10） 1A0H */
    jr32 hvint_in_idle

    .align 16    
EIINTn11_IDLE: ;/* （優先度11） 1B0H */
    jr32 hvint_in_idle

    .align 16   
EIINTn12_IDLE: ;/* （優先度12） 1C0H */
    jr32 hvint_in_idle

    .align 16   
EIINTn13_IDLE: ;/* （優先度13） 1D0H */
    jr32 hvint_in_idle

    .align 16    
EIINTn14_IDLE: ;/* （優先度14） 1E0H */
    jr32 twdint_in_idle

    .align 16   
EIINTn15_IDLE: ;/* （優先度15） 1F0H */
    jr32 syscint_in_idle

