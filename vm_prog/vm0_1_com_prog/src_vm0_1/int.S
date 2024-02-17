#include <rh850asm.h>

#define OSTM0_INTNO    199
#define OSTM1_INTNO    200
#define OSTM2_INTNO    201
#define OSTM3_INTNO    202
#define OSTM4_INTNO    203

    /* 
     *  ベクターテーブル
     *  ここにRBASEが設定されることを想定（ここが電源投入時の開始アドレス）
     */
    .section .reset.text
    .align 2
    .global _ResetVector
_ResetVector:
    jr _Bootup
    .rept 6
    nop
    .endr
_SYSERR: /* 010H 010 */
    .rept 8
    nop
    .endr
_HVTRAP: /* 020H 020 */
    .rept 8
    nop
    .endr
_FETRAP: /* 030H 030 */
    .rept 8
    nop
    .endr
_TRAP0: /* 040H 040 */
    .rept 8
    nop
    .endr
_TRAP1: /* 050H 050 */
    .rept 8
    nop
    .endr
_RIE: /* 060H 060 */
    .rept 8
    nop
    .endr
_FPPFPI: /* 070H 070 */
    .rept 8
    nop
    .endr
_UCPOP: /* 080H 080 */
    .rept 8
    nop
    .endr
_MIP: /* 090H 090 */
    .rept 8
    nop
    .endr
_PIE: /* 0A0H 0A0 */
    .rept 8
    nop
    .endr
_DEBUG: /* 0B0H 0B0 */
    .rept 8
    nop
    .endr
_MAE: /* 0C0H 0C0 */
    .rept 8
    nop
    .endr
_RFU: /* 0D0H 0D0 */
    .rept 8
    nop
    .endr
_FENMI: /* 0E0H 0E0 */
    .rept 8
    nop
    .endr
_FEINT: /* 0F0H 0F0 */
    .rept 8
    nop
    .endr
_EIINTn0: /* （優先度0） 100H */
    .rept 8
    nop
    .endr
_EIINTn1: /* （優先度1） 110 */
    .rept 8
    nop
    .endr
_EIINTn2: /* （優先度2） 120H */
    .rept 8
    nop
    .endr
_EIINTn3: /* （優先度3） 130 */
    .rept 8
    nop
    .endr
_EIINTn4: /* （優先度4） 140H */
    .rept 8
    nop
    .endr
_EIINTn5: /* （優先度5） 150 */
    .rept 8
    nop
    .endr
_EIINTn6: /* （優先度6） 160H */
    .rept 8
    nop
    .endr
_EIINTn7: /* （優先度7） 170 */
    .rept 8
    nop
    .endr
_EIINTn8: /* （優先度8） 180H */
    jr int_handler
    .rept 6
    nop
    .endr
_EIINTn9: /* （優先度9） 190 */
    .rept 8
    nop
    .endr
_EIINTn10: /* （優先度10） 1A0 */
    jr int_handler
    .rept 6
    nop
    .endr
_EIINTn11: /* （優先度11） 1B0 */
    .rept 8
    nop
    .endr
_EIINTn12: /* （優先度12） 1C0 */
    .rept 8
    nop
    .endr
_EIINTn13: /* （優先度13） 1D0 */
    .rept 8
    nop
    .endr
_EIINTn14: /* （優先度14） 1E0 */
    .rept 8
    nop
    .endr
_EIINTn15: /* （優先度15） 1F0 */
    jr int_handler
    .rept 6
    nop
    .endr

    TCB_p_tinib	.set	8

    /*
     *  割り込みハンドラ
     */
    .section .text
    .align 2
    .global int_handler 
int_handler:
    /* スクラッチレジスタを保存 */
    addi    -76 , sp , sp
    st.w    r1 ,  72[sp]
    st.w    r6 ,  68[sp]
    st.w    r7 ,  64[sp]
    st.w    r8 ,  60[sp]
    st.w    r9 ,  56[sp]
    st.w    r10 , 52[sp]
    st.w    r11 , 48[sp]
    st.w    r12 , 44[sp]
    st.w    r13 , 40[sp]
    st.w    r14 , 36[sp]
    st.w    r15 , 32[sp]
    st.w    r16 , 28[sp]
    st.w    r17 , 24[sp]
    st.w    r18 , 20[sp]
    st.w    r19 , 16[sp]
    st.w    ep,   12[sp]
    st.w    r31 ,  8[sp]
    /* 多重割込みのため，現在のEIPCとEIPSWを保存 */
    stsr    eipc,   r6
    st.w    r6 ,   4[sp]
    stsr    eipsw,  r6
    st.w    r6 ,   0[sp]

    /* 割込み要因の特定 */
    stsr    eiic,   r6
    /* EIC割込みは0x1000 から(V850E3v5) */
    addi   -0x1000, r6, r6
    nop

    /* OSTM0の場合 */
    movea OSTM0_INTNO,r0,r11
    cmp   r11,r6
    bne   call_ostm1

    /* 優先度8をマスク */
    ei
    jarl  _ostm0_handler,lp
    di
    br    int_handler_exit

    /* OSTM1の場合 */
call_ostm1:
    /* OSTM1の場合 */
    movea OSTM1_INTNO,r0,r11
    cmp   r11,r6
    bne   call_ostm2
    ei
    jarl  _ostm1_handler,lp
    di
    br    int_handler_exit

    /* OSTM2の場合 */
call_ostm2:
    movea OSTM2_INTNO,r0,r11
    cmp   r11,r6
    bne   call_ostm3
    ei
    jarl  _ostm2_handler,lp
    di
    br    int_handler_exit

    /* OSTM3の場合 */
call_ostm3:
    movea OSTM3_INTNO,r0,r11
    cmp   r11,r6
    bne   call_ostm4
    ei
    jarl  _ostm3_handler,lp
    di
    br    int_handler_exit

    /* OSTM4の場合 */
call_ostm4:
    movea OSTM4_INTNO,r0,r11
    cmp   r11,r6
    bne   twtgint
    ei
    jarl  _ostm4_handler,lp
    di

    /* タイムウィンドウトリガ割込みの場合 */
twtgint:
    movea 545,r0,r11
    cmp   r11,r6
    bne   int_handler_exit
    ei
    jarl  _twtgint_handler,lp
    di

int_handler_exit:
    ld.w     4[sp] , r10
    ldsr    r10 , eipc
    ld.w    0[sp] , r10
    ldsr    r10 , eipsw
    ld.w    72[sp] , r1
    ld.w    68[sp] , r6
    ld.w    64[sp] , r7
    ld.w    60[sp] , r8
    ld.w    56[sp] , r9
    ld.w    52[sp] , r10
    ld.w    48[sp] , r11
    ld.w    44[sp] , r12
    ld.w    40[sp] , r13
    ld.w    36[sp] , r14
    ld.w    32[sp] , r15
    ld.w    28[sp] , r16
    ld.w    24[sp] , r17
    ld.w    20[sp] , r18
    ld.w    16[sp] , r19
    ld.w     8[sp] , r31
    ld.w    12[sp] , ep
    addi    76, sp, sp
    eiret
