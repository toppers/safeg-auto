;/*
 ;*  ベクタテーブルとスタートアップルーチン
 ;*/
$include (rh850asm.h)
$include (hv_asm.inc)
$include (hv_cfg_asm.inc)

    MAGIC_START .set 0x87654321 ;/* 同期用のマジックナンバー */
    RESET_PSW .set 0x00000020 ;/* PSWのリセット後の値(ID=1) */

    ;/* 
     ;* ブート処理 
     ;*/
    .align 4
    .extern _SetBasePointer
    .extern _Bootup
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
    ldsr    r0, eipc
    ldsr    r0, ctpc

    ;/* 
     ;*  HVサポートコアかの判断
     ;*/
    rpeid r7 
    mov   1, r8
    shl   r7, r8
    mov   TBIT_SUPPORT_CORE, r6
    and   r6, r8
    cmp   r0, r8  
    be    Terminate 

    ;/*
     ;*  bootsyncの初期化
     ;*/
    rpeid r7 
    mov   LEADER_COREID, r6
    cmp   r6, r7
    bne   _init_reg

    Lea   _bootsync, r10
    st.w  r0, 0x00[r10]

    ;/* 
     ;*  Initialize sp,ep,gp,ctbp 
     ;*/
_init_reg:
    rpeid r7
    shl   2, r7    ;/* 初期スタックポインタ値の設定 */
    Lea   _hv_stkpt_table, r3
    add   r3, r7
    ld.w  0[r7], sp
$ifndef __ASRH__  ;/* GHSの場合 */    
    Lea   __gp, gp
    Lea   __tp, tp
    Lea   __ctbp, r10
    ldsr  r10, ctbp
$else ;/* CCRHの場合 */
    mov   #__gp_data, gp
$endif ;/* __ASRH__ */

    ;/* 
     ;*  Initialize PSW to reset value
     ;*/
    mov  RESET_PSW, r12
    ldsr r12, psw       ;/* store psw */

    ;/* 
     ;*  Enable FPU Access  
     ;*/
    stsr psw, r12
    mov  0x00010000,r13 ;/* PSW.CU0(PSW[16]) = 1 */
    or   r13, r12
    ldsr r12, psw 
    mov  HV_INIT_FPSR, r11
    ldsr r11, fpsr
    ldsr r0,  fpepc

    ;/*
     ;*  _hardware_init_hook の呼出し
     ;*/
    jarl  _hardware_init_hook, lp

    ;/*
     ;*  _mem_init_hook の呼出し
     ;*/
    jarl  _mem_init_hook, lp

    ;/* 
     ;*  データセクションの初期化
     ;*/
    rpeid r7
    mov   LEADER_COREID, r6
    cmp   r6, r7
    bne   data_init_wait

$ifndef __ASRH__  ;/* GHSの場合 */
    ;/*
     ;*  dataセクションの初期化
     ;*
     ;*  __idata_start から __idata_end までを，__data_start 以降に
     ;*  コピーする．
     ;*/
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
$else ;/* CCRHの場合 */
    mov     #__s.data, r6
    mov     #__e.data, r7
    mov     #__s.data.R, r8
data_init_start:
    cmp     r6, r7
    bl      data_init_end
    ld.w    0[r6], r9
    st.w    r9, 0[r8]
    add     4, r6
    add     4, r8
    br      data_init_start
data_init_end:
$endif ;/* __ASRH__ */

   ;/*
    ;*  メモリ初期化処理の終了の通知
    ;*/
   Lea   _bootsync, r10
   mov   MAGIC_START, r6
   st.w  r6, 0x00[r10]

   br    rh850_main_call

    ;/* 
     ;*  BSS/DATAの初期化待ち 
     ;*/
data_init_wait:
   Lea   _bootsync, r10
   ld.w  0x00[r10], r6
   cmp   r0, r6
   be    data_init_wait

    ;/* 
     ;*  Call Main Function
     ;*/
rh850_main_call:
    Lea   _rh850_main, r10
    jarl [r10], lp

    ;/* 
     ;*  終了:無限ループ 
     ;*/
Terminate:
    halt
    br Terminate
