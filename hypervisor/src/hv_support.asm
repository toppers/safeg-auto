$include (rh850asm.h)
$include (hv_asm.inc)
$include (hv_cfg_asm.inc)

;/*
 ;*  自コアのデータへのアクセスマクロ
 ;*/
MYCCB .macro reg1, reg2
    rpeid reg1
    Lea   _p_ccb_table, reg2
    shl   2, reg1
    add   reg1, reg2
    ld.w  0[reg2], reg1
.endm

MYHVTWDCB .macro reg1, reg2
    rpeid reg1
    Lea   _p_hvtwdcb_table, reg2
    shl   2, reg1
    add   reg1, reg2
    ld.w  0[reg2], reg1
.endm

;/*
 ;*  コンテキストの保存復帰マクロ
 ;*/
;/* 
 ;* スクラッチレジスタの保存 
 ;*/
SAVE_SCRATCH_REG .macro
    pushsp r1, r1
    stsr    fpsr, r1
    pushsp r1, r1
$ifndef __ASRH__ ;/* GHSの場合 */
    pushsp r6, r19    ;/* tp(r5)はパーマネントレジスタであるため保存しない */
$else ;/* CCRHの場合 */
    pushsp r5, r19
$endif ;/* __ASRH__ */
    pushsp r30, r31
.endm

;/* 
 ;* スクラッチレジスタの復帰
 ;*/
RESTORE_SCRATCH_REG .macro
    popsp r30, r31
$ifndef __ASRH__ ;/* GHSの場合 */
    popsp r6, r19    ;/* tp(r5)はパーマネントレジスタであるため保存しない */
$else ;/* CCRHの場合 */
    popsp r5, r19
$endif ;/* __ASRH__ */
    popsp r1, r1
    ldsr    r1, fpsr
    popsp r1, r1
.endm

;/*
 ;*  VMのコンテキストの保存
 ;*/
SAVE_VM_CONTEXT .macro
    ;/* ワークレジスタの退避 */
    ldsr   sp, eiwr
    ldsr   r6, fewr

    MYCCB  sp, r6
    ld.w   CCB_P_RUNVM[sp], sp  ;/* p_my_ccb->p_runvm -> sp */

    ;/* 汎用レジスタの保存 */
    addi   (VMCB_REG+(4*TNUM_REG)), sp, sp
    stsr   fewr, r6               ;/* r6_VM を FEWR から復帰 */
    pushsp r1, r31
    stsr   eiwr, r1               ;/* sp_VMを EIWR から復帰 */
    st.w   r1, VMCB_REG_SP[sp]  ;/* sp_VMを コンテキスト領域に保存 */

    ;/* 復帰先のアドレスを保存 */
    stsr   eipc, r6
    st.w   r6, VMCB_PC[sp]

    ;/* システムレジスタの保存 */
    addi   VMCB_S_SYSREG, sp , sp
    stm.gsr [sp]
$ifdef SUPPORT_SBUF
    addi  (VMCB_SBUFMPU - VMCB_S_SYSREG), sp , sp
    stm_mp_sbuf sp
$endif ;/* SUPPORT_SBUF */
.endm

;/*
 ;*  VMコンテキストの復帰
 ;*/
RESTORE_VM_CONTEXT .macro
    MYCCB  sp, r6
    ld.w   CCB_P_RUNVM[sp], r6  ;/* p_my_ccb->p_runvm -> r6 */

    ;/* システムレジスタの復帰 */
    addi   VMCB_S_SYSREG, r6 , r7
    ldm.gsr [r7]

    ;/* 汎用レジスタの復帰 */
    ld.w  VMCB_REG_SP[r6], r1  ;/* sp_VMを取得 */
    ldsr  r1, eiwr               ;/* sp_VMを EIWR に退避 */
    mov   r6, sp
    popsp r1, r31
    stsr  eiwr, sp               ;/* sp_VMを EIWR を復帰 */
.endm

;/*
 ;*  パーマネントレジスタの設定
 ;*/
SET_PERMANENT_REGISTER .macro reg1
$ifndef __ASRH__ ;/* GHSの場合 */
    Lea    __gp, gp
    Lea    __tp, tp
$else ;/* CCRHの場合 */
    mov   #__gp_data, gp
$endif ;/* __ASRH__ */
    mov HV_INIT_FPSR, reg1
    ldsr reg1, fpsr
.endm

;/*
 ;*  各種ハンドラ
 ;*/

    ;/*
     ;*  HVで発生するFE例外のハンドラ
      ;*/
    .cseg text
    .align 16
    .extern hv_fe_handler 
hv_fe_handler:
    ;/* 例外ハンドラで参照するため全てのコンテキストを保存 */
    ldsr    sp, fewr
    pushsp r1, r31
    stsr    fpsr, r1
    pushsp r1, r1
    stsr   fewr, r1       ;/* spを FEWR から復帰 */
    pushsp r1, r1

    ;/* HV FE例外ハンドラの呼び出し */
    mov   sp, r6
    jarl _call_hv_fe_handler, lp

    popsp r1, r1    ;/* sp */
    ldsr  r1, fewr
    popsp r1, r1    ;/* fpsr */
    ldsr  r1, fpsr
    popsp r1, r31
    stsr  fewr, sp

    feret


    ;/*
     ;*  HVで発生するEI例外のハンドラ
     ;*/
    .cseg text
    .align 16
    .extern hv_ei_handler 
hv_ei_handler:
    ;/* 例外ハンドラで参照するため全てのコンテキストを保存 */
    ldsr   sp, eiwr
    pushsp r1, r31
    stsr   fpsr, r1
    pushsp r1, r1
    stsr   eiwr, r1   ;/* spを EIWR から復帰 */
    pushsp r1, r1

    ;/* HV FE例外ハンドラの呼び出し */
    mov   sp, r6
    jarl _call_hv_ei_handler, lp

    popsp r1, r1    ;/* sp */
    ldsr  r1, eiwr 
    popsp r1, r1    ;/* fpsr */
    ldsr  r1, fpsr
    popsp r1, r31
    stsr  eiwr, sp

    eiret


    ;/*
     ;*  HVで発生するDB例外のハンドラ
     ;*/
    .cseg text
    .align 16
    .extern hv_db_handler 
hv_db_handler:
    br hv_db_handler


    ;/*
     ;*  VMとHVの両方で発生する可能性がある例外のハンドラ
     ;*  ・MIP/MDP
     ;*  ・SYSERR
     ;*/
    .cseg text
    .align 16
    .extern vm_hv_fe_handler 
vm_hv_fe_handler:
    ;/* 
     ;*  VM実行時に発生した例外か判断する
     ;*  FEPSWHのGM[31]ビットが立っていればVMから来た場合と判断
     ;*/
    ldsr   r10, fewr
    stsr   fepswh, r10
    shl    1, r10
    stsr   fewr, r10
    bnc    hv_fe_handler

    ;/*
     ;*  VM側から呼びされた場合
     ;*
     ;*  例外処理ではバンク化されていないシステムレジスタは操作しない．また，依存しない処理とする．
     ;*  例外処理内では割込みを許可しない．
     ;*/
    ;/* VMコンテキストの保存(スクラッチレジスタのみ) */
    ldsr    sp, eiwr   ;/* sp_VMを EIWR に待避 */
    ldsr    r1, fewr   ;/* r1_VMを FEWR に待避 */

    ;/* HVスタックに変更 */
    MYCCB r1, sp
    ld.w  CCB_SP[r1], sp  ;/* p_my_ccb->sp -> sp */

    ;/* 汎用レジスタの保存 */
    stsr   fewr, r1       ;/* r1_VMを FEWR から復帰 */
    pushsp r1, r31
    stsr   fpsr, r1
    pushsp r1, r1
    stsr   eiwr, r1       ;/* sp_VM を EIWR から復帰 */
    pushsp r1, r1

    ;/* ホストのコンテキストの一部復帰 */
    SET_PERMANENT_REGISTER r1 ;/* パーマネントレジスタを設定 */

    ;/* VM例外ハンドラ呼び出し */
    mov   sp, r6 ;/* 引数 */
    jarl _call_vm_exc_handler, lp

    ;/* VMコンテキストの復帰(スクラッチレジスタのみ) */
    popsp r1, r1          ;/* sp_VMの取得 */
    ldsr  r1, eiwr       ;/* sp_VMを EIWR に退避 */
    popsp r1, r1          ;/* fpsr */
    ldsr  r1, fpsr
    popsp r1, r31
    stsr  eiwr, sp       ;/* sp_VMを EIWR から復帰 */

    ;/* VMタイムウィンドウへのリターン */
    feret


    ;/*
     ;*  HVTRAPハンドラ
     ;*/
    .cseg text
    .align 16
    .extern hvtrap_handler
hvtrap_handler:
    ;/* 
     ;*  VM実行時に発生したhvtrapか判断する
     ;*  EIPSWHのGM[31]ビットが立っていればVMから来た場合と判断
     ;*/ 
    ldsr   r10, eiwr         ;/* r10_VM を EIWR に待避 */
    stsr   eipswh, r10
    shl    1, r10
    bnc    _hvtrap_from_host

    ;/*
     ;*  VM側から呼びされた場合
     ;*  呼び出し元でレジスタは次のように設定されている
     ;*    r6 - r9 : 引数(R9には関数コードが格納)
     ;*    破壊可能レジスタ : r4(gp), r5(tp), r9 - r19,  r30(ep), r31(lp)
     ;*  戻り先は常に同じVMとする
     ;*    ハンドラ実行に必要なコンテキストのみ保存．
     ;*  HVTRAPではバンク化されていないシステムレジスタは操作しない．また，依存しない処理とする．
     ;*  HVTRAP内では割込みを許可しない．
     ;*/

    ;/*  
     ;* VMのコンテキストを保存する 
     ;*/
    MYCCB  r12, r13
    ld.w   CCB_P_RUNVM[r12], r13 ;/* p_my_ccb->p_runvm -> r13 */

    ;/* spの保存 */
    st.w   sp, (VMCB_REG+(4*0))[r13]    ;/* sp -> p_runvm->reg[0] */
    ;/* fpsrの保存 */
    stsr   fpsr, r10
    st.w   sp, (VMCB_S_SYSREG+(4*0))[r13]   ;/* fpsr -> p_runvm->s_sysreg[FPSR_NO] */

    ;/* HVC呼び出し中とする */
    mov     1, r13
    st.b   r13, CCB_RUNHVC[r12]  ;/* true -> p_my_ccb->runhvc */

    ;/*
     ;* ホストモードのレジスタを復帰
     ;*/
    SET_PERMANENT_REGISTER r1 ;/* パーマネントレジスタを設定 */
    MYCCB  r1,sp            ;/* HVスタックに変更 */
    ld.w   CCB_SP[r1], sp   ;/* p_my_ccb->sp -> sp */

hvc_entry:
    ;/*
     ;*  機能コードのチェック
     ;*/
    mov     TNUM_HVCID, r12
    cmp     r12, r9            ;/* 機能コードの上限チェック(r9 <= TMAX_HVCID) */
    bgt     hvc_entry_error    ;/* 上限をオーバしているならばエラー */

    Lea     _hvc_table, r12
    shl     2, r9              ;/* r9 *= 4 */
    add     r9, r12            ;/* r12 = &hvc_table[r9] */
    ld.w    0[r12], r12        ;/* テーブルを引く */
    Lea     hvc_entry_r, lp    ;/* 戻り番地を設定 */
    jmp     [r12]              ;/* サービスコール処理ルーチンを呼ぶ */

hvc_entry_r:
hvc_entry_error:

    ;/*
     ;*  戻り値が r10/r11 に格納されている．
     ;*/

    MYCCB  r9, r12
    st.b   r0, CCB_RUNHVC[r9]     ;/* false -> my_ccb->runhvc */

    ;/*
     ;*  VMのコンテキストを戻す
     ;*/
    ld.w   CCB_P_RUNVM[r9], r12       ;/* p_runvm -> r12 */
    ld.w   (VMCB_REG+(4*0))[r12], sp  ;/* p_runvm->reg[0] -> sp */
    ld.w   (VMCB_S_SYSREG+(4*0))[r12], r9   ;/* p_runvm->s_sysreg[FPSR_NO] -> fpsr */
    ldsr    r9, fpsr

    eiret

    ;/*
     ;*  ホスト側から呼びされた場合
     ;*  単にリターンする
     ;*/
_hvtrap_from_host:
    stsr   eiwr, r10   ;/* r10 の復帰 */
    eiret


;/*
 ;*  タイムウィンドウタイマ割込みハンドラ
 ;*
 ;*  実行中のタイムウィンドウに応じて使用するベクタを変更して，
 ;*  呼び出す割込みハンドラを切り替えている．
 ;*/

    ;/*
     ;*  VMタイウィンドウ実行時のタイムウィンドウタイマ割込み
     ;*/
    .cseg text
    .align 16
    .extern twdint_in_vmtw
twdint_in_vmtw:
    ;/* VMコンテキストの保存 */
    SAVE_VM_CONTEXT

    ;/* ホストのコンテキストの一部復帰 */
    MYCCB r6,r7          ;/* スタックの復帰 */
    ld.w  CCB_SP[r6], sp ;/* p_my_ccb->sp -> sp */
    SET_PERMANENT_REGISTER r7 ;/* パーマネントレジスタを設定 */

    ;/* タイムウィンドウ切り替えの呼び出し */
    Lea   ret_idle,lp
    br    _twd_switch
    ;/* ここには戻って来ない */
twdint_in_vmtw_1:
    br twdint_in_vmtw_1


    ;/*
     ;*  HVタイウィンドウ実行時のタイムウィンドウタイマ割込み
     ;*/
    .cseg text
    .align 16
    .extern twdint_in_hvtw
twdint_in_hvtw:
    ;/* スクラッチレジスタの保存 */
    SAVE_SCRATCH_REG

    ;/* 残りのコンテキストを保存 */
    pushsp r20, r29
    MYHVTWDCB  ep, r6
    sst.w   sp, HVTWDCB_SP[ep]
    stsr    eipc, r1
    sst.w   r1, HVTWDCB_PC[ep]
    stsr    eipsw, r1
    sst.w   r1, HVTWDCB_PSW[ep]

    ;/* HVスタックに変更 */
    MYCCB r1,sp
    ld.w  CCB_SP[r1], sp ;/* p_my_ccb->sp -> sp */

    ;/* タイムウィンドウ切り替えの呼び出し */
    Lea   ret_idle, lp
    br    _twd_switch
    ;/* ここには戻って来ない */
twdint_in_hvtw_1:
    br twdint_in_hvtw_1


    ;/*
     ;*  IDLEタイムウィンドウ実行時のタイムウィンドウタイマ割込み
     ;*   エラー処理
     ;*/
    .cseg text
    .align 16
    .extern twdint_in_idle
twdint_in_idle:
    br twdint_in_idle


;/*
 ;*  システム周期タイマ割込みハンドラ
 ;*
 ;*  実行中のタイムウィンドウに応じて使用するベクタを変更して，
 ;*  呼び出す割込みハンドラを切り替えている．
 ;*/
    ;/*
     ;*  VMタイムウィンド実行時のシステム周期タイマ割込み
     ;*/
    .cseg text
    .align 16
    .extern syscint_in_vmtw
syscint_in_vmtw:
    ;/* 
     ;*  IDLEタイムウィンドウならIDLE-VMが動作
     ;*  IDLEタイムウィンドウでなければエラー
     ;*/
    ;/* 
     ;*  EIPSWHのGM[31]ビットが立っていればIDLE-VMから来た場合と判断
     ;*/ 
    ldsr   r10, eiwr
    ldsr   r11, fewr
    MYCCB  r10, r11
    ld.b   CCB_RUNIDLE[r10], r11
    cmp    r0, r11
    stsr   eiwr, r10
    stsr   fewr, r11
    be     syscint_in_vmtw_0

    ;/*
     ;*  IDLE-VMからの遷移
     ;*/
    ;/* VMコンテキストの保存 */
    SAVE_VM_CONTEXT

    ;/* ホストのコンテキストの一部復帰 */
    MYCCB r6,r7          ;/* スタックの復帰 */
    ld.w  CCB_SP[r6], sp ;/* p_my_ccb->sp -> sp */
    SET_PERMANENT_REGISTER r7 ;/* パーマネントレジスタを設定 */

    ;/* システム周期切り替えの呼び出し */
    br    _scyc_switch

    ;/* ここには戻って来ない */
syscint_in_vmtw_1:
    br syscint_in_vmtw_1

syscint_in_vmtw_0:
    br syscint_in_vmtw_0

    ;/*
     ;*  HVタイムウィンド実行時のシステム周期タイマ割込み
     ;*/
    .cseg text
    .align 16
    .extern syscint_in_hvtw
syscint_in_hvtw:
    br syscint_in_hvtw

    ;/*
     ;*  IDLEタイムウィンド実行時のシステム周期タイマ割込み
     ;*/
    .cseg text
    .align 16
    .extern syscint_in_idle
syscint_in_idle:
    ;/* コンテキストの保存 */
    SAVE_SCRATCH_REG

    addi    -8, sp , sp
    mov     sp, ep 
    stsr    eipc, r6    ;/* EIPC の保存 */
    sst.w   r6, 4[ep]
    stsr    eipsw, r6   ;/* EIPSW の保存 */
    sst.w   r6, 0[ep]
    pushsp r20, r29

    MYCCB r6,r7
    st.w  sp, CCB_SP[r6] ;/* sp -> p_my_ccb->sp */

    ;/* システム周期切り替えの呼び出し */
    Lea   ret_idle, lp
    br    _scyc_switch
    ;/* ここには戻って来ない */
syscint_in_idle_1:
    br syscint_in_idle_1

;/*
 ;*  HV割込みハンドラ
 ;*
 ;*  実行中のタイムウィンドウに応じて使用するベクタを変更して，
 ;*  呼び出す割込みハンドラを切り替えている． 
 ;*/
    ;/*
     ;*  VMタイムウィンドウ実行時のHV割込み
     ;*/
    .cseg text
    .align 16
    .extern hvint_in_vmtw
hvint_in_vmtw:
    ;/* VMコンテキストの保存(スクラッチレジスタのみ) */
    ldsr    sp, eiwr   ;/* sp_VMを EIWR に待避 */
    ldsr    r1, fewr   ;/* r1_VMを FEWR に待避 */

    ;/* HVスタックに変更 */
    MYCCB r1, sp
    ld.w  CCB_SP[r1], sp  ;/* p_my_ccb->sp -> sp */

    ;/* 汎用レジスタの保存 */
    stsr   fewr, r1       ;/* r1_VMを FEWR から復帰 */
    pushsp r1, r1
    pushsp r4, r19         ;/* r4(gp)/r5(tp)も保存 */
    pushsp r30, r31
    stsr   fpsr, r1
    pushsp r1, r1
    stsr   eiwr, r1       ;/* sp_VM を EIWR から復帰 */
    pushsp r1, r1

    ;/* ホストのコンテキストの一部復帰 */
    SET_PERMANENT_REGISTER r1 ;/* パーマネントレジスタを設定 */

    ;/* HVINT呼び出し中とする */
    MYCCB r12, r13
    mov     1, r13
    st.b   r13, CCB_RUNHVINT[r12]  ;/* true -> p_my_ccb->runhvint */

    ;/* HV割込みハンドラの呼び出し */
    mov   sp, r6       ;/* コンテキストを保存したスタック */
    jarl _call_hvint_in_vmtw, lp

    MYCCB r12, r13
    st.b   r0, CCB_RUNHVINT[r12]  ;/* false -> p_my_ccb->runhvint */

    ;/* VMコンテキストの復帰(スクラッチレジスタのみ) */
    popsp r1, r1          ;/* sp_VMの取得 */
    ldsr  r1, eiwr       ;/* sp_VMを EIWR に退避 */
    popsp r1, r1          ;/* fpsr */
    ldsr  r1, fpsr
    popsp r30, r31
    popsp r4, r19
    popsp r1, r1
    stsr  eiwr, sp       ;/* sp_VMを EIWR から復帰 */

    ;/* VMタイムウィンドウへのリターン */
    eiret

    ;/*
     ;*  HVタイムウィンドウ実行時のHV割込み
     ;*/
    .cseg text
    .align 16
    .extern hvint_in_hvtm
hvint_in_hvtm:
    ;/* スクラッチレジスタの保存 */
    SAVE_SCRATCH_REG

    ;/* スタックポインタの保存 */
    MYHVTWDCB  ep, r6
    sst.w sp, HVTWDCB_SP[ep]
    ;/* HVスタックに変更 */
    MYCCB r1, sp
    ld.w  CCB_SP[r1], sp  ;/* p_my_ccb->hv_current_sp -> sp */

    ;/* HVINT呼び出し中とする */
    MYCCB r12, r13
    mov     1, r13
    st.b   r13, CCB_RUNHVINT[r12]  ;/* true -> p_my_ccb->runhvint */

    ;/* HV割込みハンドラの呼び出し */
    jarl _call_hvint_in_hvtw, lp

    MYCCB r12, r13
    st.b   r0, CCB_RUNHVINT[r12]  ;/* false -> p_my_ccb->runhvint */

    ;/* スタックポインタの復帰 */
    MYHVTWDCB  ep, r6
    sld.w HVTWDCB_SP[ep], sp

    ;/* スクラッチレジスタの復帰 */
    RESTORE_SCRATCH_REG

    eiret

    ;/*
     ;*  IDLEタイムウィンドウ実行時のHV割込み
     ;*/
    .cseg text
    .align 16
    .extern hvint_in_idle
hvint_in_idle:
    ;/* スクラッチレジスタの保存 */
    SAVE_SCRATCH_REG

    ;/* HV割込みハンドラの呼び出し */
    jarl _call_hvint_in_idle, lp

    ;/* スクラッチレジスタの復帰 */
    RESTORE_SCRATCH_REG

    eiret


;/*
 ;*  各処理単位への遷移関数
 ;*/

    ;/*
      ;*  VMタイムウィンドウへの遷移
      ;*   引数 : 
     ;*     第1（r6）: vm_control_blockの先頭番地
     ;*     第2（r7）: システムレジスタの保存領域
      ;*/
    .align 16
    .extern _vmtwd_start_asm
_vmtwd_start_asm:
    ;/*
     ;*  ゲストモードのシステムレジスタの復帰
     ;*/
    ldm.gsr [r7]

    ;/*
     ;*  ゲストモードの汎用レジスタの復帰
     ;*/
    ld.w  VMCB_REG_SP[r6], r1  ;/* spの値を取得 */
    ldsr  r1, eiwr             ;/* EIWR への一時保存 */
    mov   r6, sp
    popsp r1, r31
    stsr  eiwr, sp             ;/* EIWR からの復帰 */

    ;/* VMの呼び出し */

    feret

    ;/*
     ;*  HVタイムウィンドウへの遷移
     ;*/
    .cseg text
    .align 16
    .extern _hvtwd_start_asm
_hvtwd_start_asm:
    ;/* EBASEの設定 */
    Lea  _EBaseHVTM, r6
    ldsr r6, 3, 1

    ;/* EIPSWH を0クリア */
    ldsr    r0, eipswh

    ;/* 
     ;*  my_hvtwdcbからコンテキストの復帰 
     ;*/
    MYHVTWDCB  ep, r6             ;/* p_my_hvtwdcb -> r6 */
    sld.w HVTWDCB_SP[ep], sp      ;/* スタックの復帰 */
    sld.w HVTWDCB_PC[ep], r1      ;/* EIPC の設定 */
    ldsr  r1, eipc
    sld.w HVTWDCB_PSW[ep], r1     ;/* EIPSW の設定 */
    ldsr  r1, eipsw
    popsp r20, r29               ;/* レジスタの復帰 */
    RESTORE_SCRATCH_REG
    eiret

    ;/*
      ;*  IDLEタイムウィンドウへの遷移
      ;*/
ret_idle:
    ;/* EBASEの設定 */
    Lea  _EBaseIDLE, r6
    ldsr r6, 3, 1

   ;/* EIPSWH を0クリア */
    ldsr    r0, eipswh

    ;/* 
     ;*  HVスタックからコンテキストの復帰 
     ;*/
    popsp r20, r29
    mov     sp, ep
    sld.w    4[ep], r6
    ldsr    r6, eipc
    sld.w    0[ep], r6
    ldsr    r6, eipsw
    addi    8, sp, sp
    RESTORE_SCRATCH_REG
    eiret

    ;/*
     ;*  引数1個のtrap0呼び出し
     ;*/
    .align 4
    .extern _cal_trap0_1
_cal_trap0_1:
    trap 1
    jmp [lp]
    nop

    ;/*
     ;*  IDLEタイムウィンドウ実行時のtrap0呼び出し
     ;*  現状でIDLE-VM呼び出しのみである
     ;*/
    .cseg text
    .align 16
    .extern trap0_in_idle
trap0_in_idle:
    ;/* 引数の退避 */
    ldsr   r6, eiwr

    ;/* IDLE処理のコンテキストの保存 */
    SAVE_SCRATCH_REG

    addi    -8, sp , sp
    mov     sp, ep 
    stsr    eipc, r6    ;/* EIPC の保存 */
    sst.w   r6, 4[ep]
    stsr    eipsw, r6   ;/* EIPSW の保存 */
    sst.w   r6, 0[ep]
    pushsp r20, r29

    MYCCB r6,r7
    st.w  sp, CCB_SP[r6] ;/* sp -> p_my_ccb->sp */

    ;/* IDLE-VMへの切り替え */
    stsr  eiwr,r6
    Lea   ret_idle, lp
    br    _switch_to_idle_vm
    ;/* ここには戻って来ない */

trap0_in_idle_1:
    br trap0_in_idle_1
