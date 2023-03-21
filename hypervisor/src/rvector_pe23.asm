    ;/* 
     ;*  PE2/PE3例外ベクタ
     ;*  PE2/PE3におけるVMTW実行時のHVのベクタ
     ;*  ここにRBASEが設定されることを想定（ここが電源投入時の開始アドレス）
     ;*/
    reset_bankc .cseg text
    .align 512
    .extern _ResetVector_bankc
_ResetVector_bankc:
    jr32 _Bootup

    .align 16
_SYSERR: ;/* 010H 010H */
    jr32 vm_hv_fe_handler

    .align 16
_HVTRAP: ;/* 020H 020H */
    jr32 hvtrap_handler

    .align 16
_FETRAP: ;/* 030H 030H */
    jr32 hv_fe_handler

    .align 16
_TRAP0: ;/* 040H 040H */
    jr32 hv_ei_handler

    .align 16
_TRAP1: ;/* 050H 050H */
    jr32 hv_ei_handler

    .align 16
_RIE: ;/* 060H 060H */
    jr32 hv_fe_handler

    .align 16
_FPPFPI: ;/* 070H 070H */
    jr32 hv_ei_handler

    .align 16
_UCPOP: ;/* 080H 080H */
    jr32 hv_fe_handler

    .align 16
_MIP: ;/* 090H 090H */
    jr32 vm_hv_fe_handler

    .align 16
_PIE: ;/* 0A0H 0A0H */
    jr32 hv_fe_handler

    .align 16
_DEBUG: ;/* 0B0H 0B0H */
    jr32 hv_db_handler 

    .align 16
_MAE: ;/* 0C0H 0C0H */
    jr32 hv_fe_handler

    .align 16
_RFU: ;/* 0D0H 0D0H */
    jr32 _RFU

    .align 16
_FENMI: ;/* 0E0H 0E0H */
    jr32 hv_fe_handler

    .align 16
_FEINT: ;/* 0F0H 0F0H */
    jr32 hv_fe_handler

    .align 16
_EIINTn0: ;/* （優先度0） 100H */
    jr32 hvint_in_vmtw

    .align 16
_EIINTn1: ;/*（優先度1） 110H */
    jr32 hvint_in_vmtw

    .align 16
_EIINTn2: ;/*（優先度2） 120H */
    jr32 hvint_in_vmtw

    .align 16
_EIINTn3: ;/*（優先度3） 130H */
    jr32 hvint_in_vmtw

    .align 16
_EIINTn4: ;/*（優先度4） 140H */
    jr32 hvint_in_vmtw

    .align 16
_EIINTn5: ;/*（優先度5） 150H */
    jr32 hvint_in_vmtw

    .align 16
_EIINTn6: ;/*（優先度6） 160H */
    jr32 hvint_in_vmtw

    .align 16
_EIINTn7: ;/*（優先度7） 170H */
    jr32 hvint_in_vmtw

    .align 16
_EIINTn8: ;/*（優先度8） 180H */
    jr32 hvint_in_vmtw

    .align 16
_EIINTn9: ;/*（優先度9） 190H */
    jr32 hvint_in_vmtw

    .align 16
_EIINTn10: ;/*（優先度10） 1A0H */
    jr32 hvint_in_vmtw

    .align 16
_EIINTn11: ;/*（優先度11） 1B0H */
    jr32 hvint_in_vmtw

    .align 16
_EIINTn12: ;/*（優先度12） 1C0H */
    jr32 hvint_in_vmtw

    .align 16
_EIINTn13: ;/*（優先度13） 1D0H */
    jr32 hvint_in_vmtw

    .align 16
_EIINTn14: ;/*（優先度14） 1E0H */
    jr32 twdint_in_vmtw

    .align 16
_EIINTn15: ;/*（優先度15） 1F0H */
    jr32 syscint_in_vmtw
