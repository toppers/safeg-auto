;/*
 ;*  u2a16用スタートアップ用ファイル
 ;*/
$include (rh850asm.h)
$include (hv_cfg_asm.inc)

;/*
 ;*  メモリの情報
 ;*/
    LRAM0_START .set 0xFDC00000
    LRAM0_END .set 0xFDC0FFFF
    LRAM1_START .set 0xFDA00000
    LRAM1_END .set 0xFDA0FFFF
    LRAM2_START .set 0xFD800000
    LRAM2_END .set 0xFD80FFFF
    LRAM3_START .set 0xFD600000
    LRAM3_END .set 0xFD60FFFF
    CRAM0_START .set 0xFE000000
    CRAM0_END .set 0xFE07FFFF
    CRAM1_START .set 0xFE100000
    CRAM1_END .set 0xFE17FFFF
    CRAM2_START .set 0xFE400000
    CRAM2_END .set 0xFE4FFFFF


    ;/* 
     ;*  メモリの初期化
     ;*/
    .cseg text
    .align 4
    .extern _mem_init_hook
_mem_init_hook:
    mov    lp, r14

    ;/*
     ;*  各コアのLMEMを初期化
     ;*/
    rpeid r7
    cmp   0, r7
    be    _pe0_mem_init
    cmp   1, r7
    be    _pe1_mem_init
    cmp   2, r7
    be    _pe2_mem_init
    cmp   3, r7
    be    _pe3_mem_init
    br    _exit

_pe0_mem_init:
    mov   LRAM0_START, r6
    mov   LRAM0_END,   r7
    jarl  _mem_clear,  lp
    br    _cram_init

_pe1_mem_init:
    mov   LRAM1_START, r6
    mov   LRAM1_END,   r7
    jarl  _mem_clear,  lp
    br    _cram_init

_pe2_mem_init:
    mov   LRAM2_START, r6
    mov   LRAM2_END,   r7
    jarl  _mem_clear,  lp
    br    _cram_init

_pe3_mem_init:
    mov   LRAM3_START, r6
    mov   LRAM3_END,   r7
    jarl  _mem_clear,  lp

_cram_init:
    rpeid r7
    mov   LEADER_COREID, r6
    cmp   r6, r7
    bne   _exit

    mov   CRAM0_START, r6
    mov   CRAM0_END,   r7
    jarl  _mem_clear,  lp

    mov   CRAM1_START, r6
    mov   CRAM1_END,   r7
    jarl  _mem_clear,  lp

    mov   CRAM2_START, r6
    mov   CRAM2_END,   r7
    jarl  _mem_clear,  lp


_exit:
    mov   r14, lp
    jmp   [lp]


    ;/* 
     ;*  メモリクリア関数
     ;*/
    .align 4
_mem_clear:
    br    _mem_clear2
_mem_clear1:
    st.w   r0, 0[r6]
    add    4, r6
_mem_clear2:
    cmp    r6, r7
    bh     _mem_clear1
    jmp    [lp]
