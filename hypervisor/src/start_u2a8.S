/*
 *  u2a8用スタートアップ用ファイル
 */
#include <rh850asm.h>
#include "hv_cfg_asm.h"

/*
 *  メモリの情報
 */
#define LRAM0_START  0xFDC00000
#define LRAM0_END    0xFDC0FFFF
#define LRAM1_START  0xFDA00000
#define LRAM1_END    0xFDA0FFFF
#define CRAM0_START  0xFE000000
#define CRAM0_END    0xFE07FFFF
#define CRAM2_START  0xFE400000
#define CRAM2_END    0xFE4FFFFF


    /* 
     *  メモリの初期化
     */
    .section .text
    .align 4
    .global _mem_init_hook
_mem_init_hook:
    mov    lp, r14

    /*
     *  各コアのLMEMを初期化
     */
    rpeid r7
    cmp   0, r7
    be    _pe0_mem_init
    cmp   1, r7
    be    _pe1_mem_init
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

_cram_init:
    rpeid r7
    mov   LEADER_COREID, r6
    cmp   r6, r7
    bne   _exit

    mov   CRAM0_START, r6
    mov   CRAM0_END,   r7
    jarl  _mem_clear,  lp

    mov   CRAM2_START, r6
    mov   CRAM2_END,   r7
    jarl  _mem_clear,  lp


_exit:
    mov   r14, lp
    jmp   [lp]


    /* 
     *  メモリクリア関数
     */
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
