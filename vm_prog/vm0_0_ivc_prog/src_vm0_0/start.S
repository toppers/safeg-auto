#include <rh850asm.h>

    /* 
     *  ブート処理 
     */
    .align 2
    .global _Bootup
    .type   _Bootup, @function
_Bootup:
InitRegs:
    mov     r0, r1
    mov     r0, r2
    mov     r0, r3
    mov     r0, r4
    mov     r0, r5
    mov     r0, r6
    mov     r0, r7
    mov     r0, r8
    mov     r0, r9
    mov     r0, r10
    mov     r0, r11
    mov     r0, r12
    mov     r0, r13
    mov     r0, r14
    mov     r0, r15
    mov     r0, r16
    mov     r0, r17
    mov     r0, r18
    mov     r0, r19
    mov     r0, r20
    mov     r0, r21
    mov     r0, r22
    mov     r0, r23
    mov     r0, r24
    mov     r0, r25
    mov     r0, r26
    mov     r0, r27
    mov     r0, r28
    mov     r0, r29
    mov     r0, r30
    mov     r0, r31

    /* Initialize sp,ep,gp,ctbp */
    Lea   _stack, sp
    Lea   __ep, ep
    Lea   __gp, gp
    Lea   __tp, tp
    Lea   __ctbp, r10
    ldsr  r10, ctbp

    /* Enable FPU Access V850E3v5 only ? */
    stsr  psw, r6, 0
    movhi 1, r0, r7
    or    r7, r6
    ldsr  r6, psw, 0

    /* Initialize .sbss .bss */
    Lea   __sbss_start, r6
    Lea   __bss_end, r7
bss_clear_start:
    cmp   r7, r6
    be    bss_clear_end
    st.w  r0, 0[r6]
    add   4, r6
    br    bss_clear_start
bss_clear_end:

    /* Initialize .data */
    Lea     __idata_start, r6
    Lea     __idata_end,   r7
    Lea     __data_start,  r8
data_init_start:
    cmp     r7, r6
    bge     data_init_end
    ld.w    0[r6], r9
    st.w    r9, 0[r8]
    add     4, r6
    add     4, r8
    br      data_init_start
data_init_end:
    
    /* Go to rh850_main */
    Lea   _rh850_main, r10
    jarl [r10], lp

Terminate:
    /* 終了:無限ループ */
    halt
    br Terminate
    .size    _Bootup, .-_Bootup
