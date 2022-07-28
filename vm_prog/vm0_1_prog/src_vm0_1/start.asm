$include (rh850asm.h)

;-----------------------------------------------------------------------------
; Stack
;-----------------------------------------------------------------------------
    .section ".stack.bss", bss
    .align 4
    .ds  (0x8000)
    .align 4
_stacktop:

;-----------------------------------------------------------------------------
; section initialize table
;-----------------------------------------------------------------------------
    .section ".INIT_DSEC.const", const
    .align 4
    .dw #__s.data,    #__e.data,    #__s.data.R

    .section ".INIT_BSEC.const", const
    .align 4
    .dw #__s.bss,    #__e.bss

;-----------------------------------------------------------------------------
; Entry
;-----------------------------------------------------------------------------
    .section ".text", text
    .align 2
    .public _Bootup
_Bootup:
    di
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
    ldsr    r0, 0, 0    ;EIPC
    ldsr    r0, 16, 0   ;CTPC

;-----------------------------------------------------------------------------
; 各種ポインタの初期化(GP/EP/SP)
;-----------------------------------------------------------------------------
    mov    #_stacktop, sp    
    mov    #__gp_data, gp
    mov    #__ep_data, ep

;-----------------------------------------------------------------------------
; dataセクションの初期値コピーおよびbssセクションのゼロクリア
;-----------------------------------------------------------------------------
    mov    #__s.INIT_DSEC.const, r6
    mov    #__e.INIT_DSEC.const, r7
    mov    #__s.INIT_BSEC.const, r8
    mov    #__e.INIT_BSEC.const, r9
    jarl32 __INITSCT_RH, lp    ;  initialize RAM area

;-----------------------------------------------------------------------------
;  FPUを有効に
;-----------------------------------------------------------------------------
    stsr psw, r12       ;/* load psw */
    mov  0x00010000,r13 ;/* PSW.CU0(PSW[16]) = 1 */
    or   r13, r12
    ldsr r12, psw       ;/* store psw */
    movhi 0x0002, r0, r11
    ldsr  r11, 6, 0	    ; FPSR
    ldsr  r0, 7, 0      ; FPEPC

;-----------------------------------------------------------------------------
; main関数へ分岐
;-----------------------------------------------------------------------------
    jarl    _rh850_main, lp

;-----------------------------------------------------------------------------
; mainから戻ってきた後の処理
;-----------------------------------------------------------------------------
__exit:
    halt    
__startend:

;-----------------------------------------------------------------------------
;    dummy section to avoid linker warning
;-----------------------------------------------------------------------------
    .section    ".data", data
.L.dummy.data:
    .section    ".bss", bss
.L.dummy.bss:
    .section    ".const", const
.L.dummy.const:
    .section    ".stack.bss", bss
.L.dummy.stack.bss:
    .section    "EIINTTBL", const
    .align 512
.L.dummy.EIINTTBL:
