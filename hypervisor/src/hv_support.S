#include <rh850asm.h>
#include "hv_asm.h"
#include "hv_cfg_asm.h"

/*
 *  自コアのデータへのアクセスマクロ
 */
.macro MYCCB reg1, reg2
    rpeid reg1
    Lea   _p_ccb_table, reg2
    shl   2, reg1
    add   reg1, reg2
    ld.w  0[reg2], reg1
.endm

.macro MYHVTWDCB reg1, reg2
    rpeid reg1
    Lea   _p_hvtwdcb_table, reg2
    shl   2, reg1
    add   reg1, reg2
    ld.w  0[reg2], reg1
.endm

/*
 *  コンテキストの保存復帰マクロ
 */
/* 
 * スクラッチレジスタの保存 
 */
.macro SAVE_SCRATCH_REG
    pushsp  r1-r1
    stsr    fpsr, r1
    pushsp  r1-r1
#ifndef __ASRH__ /* GHSの場合 */
    pushsp  r6-r19    /* tp(r5)はパーマネントレジスタであるため保存しない */
#else /* CCRHの場合 */
    pushsp  r5-r19
#endif /* __ASRH__ */
    pushsp  r30-r31
.endm

/* 
 * スクラッチレジスタの復帰
 */
.macro RESTORE_SCRATCH_REG
    popsp   r30-r31
#ifndef __ASRH__ /* GHSの場合 */
    popsp   r6-r19    /* tp(r5)はパーマネントレジスタであるため保存しない */
#else /* CCRHの場合 */
    popsp   r5-r19
#endif /* __ASRH__ */
    popsp   r1-r1
    ldsr    r1, fpsr
    popsp   r1-r1
.endm

/*
 *  VMのコンテキストの保存
 */
.macro SAVE_VM_CONTEXT
    /* ワークレジスタの退避 */
    ldsr   sp, eiwr
    ldsr   r6, fewr

    MYCCB  sp, r6
    ld.w   CCB_P_RUNVM[sp], sp  /* p_my_ccb->p_runvm -> sp */

    /* 汎用レジスタの保存 */
    addi   (VMCB_REG+(4*TNUM_REG)), sp, sp
    stsr   fewr, r6               /* r6_VM を FEWR から復帰 */
    pushsp r1-r31
    stsr   eiwr, r1               /* sp_VMを EIWR から復帰 */
    st.w   r1, VMCB_REG_SP[sp]  /* sp_VMを コンテキスト領域に保存 */

    /* 復帰先のアドレスを保存 */
    stsr   eipc, r6
    st.w   r6, VMCB_PC[sp]

    /* システムレジスタの保存 */
    addi   VMCB_S_SYSREG, sp , sp
    stm.gsr [sp]
#ifdef SUPPORT_SBUF
    addi  (VMCB_SBUFMPU - VMCB_S_SYSREG), sp , sp
    stm_mp_sbuf sp
#endif /* SUPPORT_SBUF */
.endm

/*
 *  VMコンテキストの復帰
 */
.macro RESTORE_VM_CONTEXT
    MYCCB  sp, r6
    ld.w   CCB_P_RUNVM[sp], r6  /* p_my_ccb->p_runvm -> r6 */

    /* システムレジスタの復帰 */
    addi   VMCB_S_SYSREG, r6 , r7
    ldm.gsr [r7]

    /* 汎用レジスタの復帰 */
    ld.w  VMCB_REG_SP[r6], r1  /* sp_VMを取得 */
    ldsr  r1, eiwr               /* sp_VMを EIWR に退避 */
    mov   r6, sp
    popsp r1-r31
    stsr  eiwr, sp               /* sp_VMを EIWR を復帰 */
.endm

/*
 *  パーマネントレジスタの設定
 */
.macro SET_PERMANENT_REGISTER
#ifndef __ASRH__ /* GHSの場合 */
    Lea    __gp, gp
    Lea    __tp, tp
#else /* CCRHの場合 */
    mov   #__gp_data, gp
#endif /* __ASRH__ */
.endm

/*
 *  各種ハンドラ
 */

    /*
      *  未定義の割込みのエントリ
      */
    .section .text
    .align 16
    .global int_handler 
int_handler:
    br int_handler

    /*
     *  MIPハンドラ
     */
    .section .text
    .align 16
    .global mip_handler 
mip_handler:
    br mip_handler

    /*
     *  HVTRAPハンドラ
     */
    .section .text
    .align 16
    .global hvtrap_handler
hvtrap_handler:
    /* 
     *  VM実行時に発生したhvtrapか判断する
     *  EIPSWHのGM[31]ビットが立っていればVMから来た場合と判断
     */ 
    ldsr   r10, eiwr         /* r10_VM を EIWR に待避 */
    stsr   eipswh, r10
    shl    1, r10
    bnc    _hvtrap_from_host

    /*
     *  VM側から呼びされた場合
     *  呼び出し元でレジスタは次のように設定されている
     *    r6 - r9 : 引数(R9には関数コードが格納)
     *    破壊可能レジスタ : r4(gp), r5(tp), r9 - r19,  r30(ep), r31(lp)
     *  戻り先は常に同じVMとする
     *    ハンドラ実行に必要なコンテキストのみ保存．
     *  HVTRAPではバンク化されていないシステムレジスタは操作しない．また，依存しない処理とする．
     *  HVTRAP内では割込みを許可しない．
     */

    /*  
     * VMのコンテキストを保存する 
     */
    MYCCB  r12, r13
    ld.w   CCB_P_RUNVM[r12], r13 /* p_my_ccb->p_runvm -> r13 */

    /* spの保存 */
    st.w   sp, (VMCB_REG+(4*0))[r13]    /* sp -> p_runvm->reg[0] */

    /* HVC呼び出し中とする */
    mov     1, r13
    st.b   r13, CCB_RUNHVC[r12]  /* true -> p_my_ccb->runhvc */

    /*
     * ホストモードのレジスタを復帰
     */
    SET_PERMANENT_REGISTER /* パーマネントレジスタを設定 */
    MYCCB  r1,sp            /* HVスタックに変更 */
    ld.w   CCB_SP[r1], sp   /* p_my_ccb->sp -> sp */

hvc_entry:
    /*
     *  機能コードのチェック
     */
    mov     TNUM_HVCID, r12
    cmp     r12, r9            /* 機能コードの上限チェック(r9 <= TMAX_HVCID) */
    bgt     hvc_entry_error    /* 上限をオーバしているならばエラー */

    Lea     _hvc_table, r12
    shl     2, r9              /* r9 *= 4 */
    add     r9, r12            /* r12 = &hvc_table[r9] */
    ld.w    0[r12], r12        /* テーブルを引く */
    Lea     hvc_entry_r, lp    /* 戻り番地を設定 */
    jmp     [r12]              /* サービスコール処理ルーチンを呼ぶ */

hvc_entry_r:
hvc_entry_error:

    /*
     *  戻り値が r10/r11 に格納されている．
     */

    MYCCB  r9, r12
    st.b   r0, CCB_RUNHVC[r9]     /* false -> my_ccb->runhvc */

    /*
     *  VMのコンテキストを戻す
     */
    ld.w   CCB_P_RUNVM[r9], r12       /* p_runvm -> r12 */
    ld.w   (VMCB_REG+(4*0))[r12], sp  /* p_runvm->reg[0] -> sp */

    eiret

    /*
     *  ホスト側から呼びされた場合
     *  単にリターンする
     */
_hvtrap_from_host:
    stsr   eiwr, r10   /* r10 の復帰 */
    eiret


/*
 *  タイムウィンドウタイマ割込みハンドラ
 *
 *  実行中のタイムウィンドウに応じて使用するベクタを変更して，
 *  呼び出す割込みハンドラを切り替えている．
 */

    /*
     *  VMタイウィンドウ実行時のタイムウィンドウタイマ割込み
     */
    .section .text
    .align 16
    .global twdint_in_vmtw
twdint_in_vmtw:
    /* VMコンテキストの保存 */
    SAVE_VM_CONTEXT

    /* ホストのコンテキストの一部復帰 */
    MYCCB r6,r7          /* スタックの復帰 */
    ld.w  CCB_SP[r6], sp /* p_my_ccb->sp -> sp */
    SET_PERMANENT_REGISTER /* パーマネントレジスタを設定 */

    /* タイムウィンドウ切り替えの呼び出し */
    Lea   ret_idle,lp
    br    _twd_switch
    /* ここには戻って来ない */
twdint_in_vmtw_1:
    br twdint_in_vmtw_1


    /*
     *  HVタイウィンドウ実行時のタイムウィンドウタイマ割込み
     */
    .section .text
    .align 16
    .global twdint_in_hvtw
twdint_in_hvtw:
    /* スクラッチレジスタの保存 */
    SAVE_SCRATCH_REG

    /* 残りのコンテキストを保存 */
    pushsp  r20-r29
    MYHVTWDCB  ep, r6
    sst.w   sp, HVTWDCB_SP[ep]
    stsr    eipc, r1
    sst.w   r1, HVTWDCB_PC[ep]
    stsr    eipsw, r1
    sst.w   r1, HVTWDCB_PSW[ep]

    /* HVスタックに変更 */
    MYCCB r1,sp
    ld.w  CCB_SP[r1], sp /* p_my_ccb->sp -> sp */

    /* タイムウィンドウ切り替えの呼び出し */
    Lea   ret_idle, lp
    br    _twd_switch
    /* ここには戻って来ない */
twdint_in_hvtw_1:
    br twdint_in_hvtw_1


    /*
     *  IDLEタイムウィンドウ実行時のタイムウィンドウタイマ割込み
     *   エラー処理
     */
    .section .text
    .align 16
    .global twdint_in_idle
twdint_in_idle:
    br twdint_in_idle


/*
 *  システム周期タイマ割込みハンドラ
 *
 *  実行中のタイムウィンドウに応じて使用するベクタを変更して，
 *  呼び出す割込みハンドラを切り替えている．
 */
    /*
     *  VMタイムウィンド実行時のシステム周期タイマ割込み
     *   エラー処理
     */
    .section .text
    .align 16
    .global syscint_in_vmtw
syscint_in_vmtw:
    br syscint_in_vmtw

    /*
     *  HVタイムウィンド実行時のシステム周期タイマ割込み
     *   エラー処理
     */
    .section .text
    .align 16
    .global syscint_in_hvtw
syscint_in_hvtw:
    br syscint_in_hvtw

    /*
     *  IDLEタイムウィンド実行時のシステム周期タイマ割込み
     */
    .section .text
    .align 16
    .global syscint_in_idle
syscint_in_idle:
    /* コンテキストの保存 */
    SAVE_SCRATCH_REG

    addi    -8, sp , sp
    mov     sp, ep 
    stsr    eipc, r6    /* EIPC の保存 */
    sst.w   r6, 4[ep]
    stsr    eipsw, r6   /* EIPSW の保存 */
    sst.w   r6, 0[ep]
    pushsp  r20-r29

    MYCCB r6,r7
    st.w  sp, CCB_SP[r6] /* sp -> p_my_ccb->sp */

    /* システム周期切り替えの呼び出し */
    Lea   ret_idle, lp
    br    _scyc_switch
    /* ここには戻って来ない */
syscint_in_idle_1:
    br syscint_in_idle_1

/*
 *  HV割込みハンドラ
 *
 *  実行中のタイムウィンドウに応じて使用するベクタを変更して，
 *  呼び出す割込みハンドラを切り替えている． 
 */
    /*
     *  VMタイムウィンドウ実行時のHV割込み
     */
    .section .text
    .align 16
    .global hvint_in_vmtw
hvint_in_vmtw:
    /* VMコンテキストの保存(スクラッチレジスタのみ) */
    ldsr    sp, eiwr   /* sp_VMを EIWR に待避 */
    ldsr    r1, fewr   /* r1_VMを FEWR に待避 */

    /* HVスタックに変更 */
    MYCCB r1, sp
    ld.w  CCB_SP[r1], sp  /* p_my_ccb->sp -> sp */

    /* 汎用レジスタの保存 */
    stsr   fewr, r1       /* r1_VMを FEWR から復帰 */
    pushsp r1-r1
    pushsp r4-r19         /* r4(gp)/r5(tp)も保存 */
    pushsp r30-r31

    stsr   eiwr, r1       /* sp_VM を EIWR から復帰 */
    pushsp  r1-r1

    /* ホストのコンテキストの一部復帰 */
    SET_PERMANENT_REGISTER /* パーマネントレジスタを設定 */

    /* HVINT呼び出し中とする */
    MYCCB r12, r13
    mov     1, r13
    st.b   r13, CCB_RUNHVINT[r12]  /* true -> p_my_ccb->runhvint */

    /* HV割込みハンドラの呼び出し */
    mov   1, r6          /* タイムウィドウタイマを停止する */
    jarl _call_hvint, lp

    MYCCB r12, r13
    st.b   r0, CCB_RUNHVINT[r12]  /* false -> p_my_ccb->runhvint */

    /* VMコンテキストの復帰(スクラッチレジスタのみ) */
    popsp r1-r1          /* sp_VMの取得 */
    ldsr  r1, eiwr       /* sp_VMを EIWR に退避 */
    popsp r30-r31
    popsp r4-r19
    popsp r1-r1
    stsr  eiwr, sp       /* sp_VMを EIWR から復帰 */

    /* VMタイムウィンドウへのリターン */
    eiret

    /*
     *  HVタイムウィンドウ実行時のHV割込み
     */
    .section .text
    .align 16
    .global hvint_in_hvtm
hvint_in_hvtm:
    /* スクラッチレジスタの保存 */
    SAVE_SCRATCH_REG

    /* スタックポインタの保存 */
    MYHVTWDCB  ep, r6
    sst.w sp, HVTWDCB_SP[ep]
    /* HVスタックに変更 */
    MYCCB r1, sp
    ld.w  CCB_SP[r1], sp  /* p_my_ccb->hv_current_sp -> sp */

    /* HVINT呼び出し中とする */
    MYCCB r12, r13
    mov     1, r13
    st.b   r13, CCB_RUNHVINT[r12]  /* true -> p_my_ccb->runhvint */

    /* HV割込みハンドラの呼び出し */
    mov   1, r6         /* TWタイマを停止する */
    jarl _call_hvint, lp

    MYCCB r12, r13
    st.b   r0, CCB_RUNHVINT[r12]  /* false -> p_my_ccb->runhvint */

    /* スタックポインタの復帰 */
    MYHVTWDCB  ep, r6
    sld.w HVTWDCB_SP[ep], sp

    /* スクラッチレジスタの復帰 */
    RESTORE_SCRATCH_REG

    eiret

    /*
     *  IDLEタイムウィンドウ実行時のHV割込み
     */
    .section .text
    .align 16
    .global hvint_in_idle
hvint_in_idle:
    /* スクラッチレジスタの保存 */
    SAVE_SCRATCH_REG

    /* HV割込みハンドラの呼び出し */
    mov   r0, r6          /* TWタイマを停止しない */
    jarl _call_hvint, lp

    /* スクラッチレジスタの復帰 */
    RESTORE_SCRATCH_REG

    eiret


/*
 *  各処理単位への遷移関数
 */

    /*
      *  VMタイムウィンドウへの遷移
      *   引数 : 
     *     第1（r6）: vm_control_blockの先頭番地
     *     第2（r7）: システムレジスタの保存領域
      */
    .align 16
    .global _vmtwd_start_asm
_vmtwd_start_asm:
    /*
     *  ゲストモードのシステムレジスタの復帰
     */
    ldm.gsr [r7]

    /*
     *  ゲストモードの汎用レジスタの復帰
     */
    ld.w  VMCB_REG_SP[r6], r1  /* spの値を取得 */
    ldsr  r1, eiwr             /* EIWR への一時保存 */
    mov   r6, sp
    popsp r1-r31
    stsr  eiwr, sp             /* EIWR からの復帰 */

    /* VMの呼び出し */

    feret

    /*
     *  HVタイムウィンドウへの遷移
     */
    .section .text
    .align 16
    .global _hvtwd_start_asm
_hvtwd_start_asm:
    /* EBASEの設定 */
    Lea  _EBaseHVTM, r6
    ldsr r6, 3, 1

    /* EIPSWH を0クリア */
    ldsr    r0, eipswh

    /* 
     *  my_hvtwdcbからコンテキストの復帰 
     */
    MYHVTWDCB  ep, r6             /* p_my_hvtwdcb -> r6 */
    sld.w HVTWDCB_SP[ep], sp      /* スタックの復帰 */
    sld.w HVTWDCB_PC[ep], r1      /* EIPC の設定 */
    ldsr  r1, eipc
    sld.w HVTWDCB_PSW[ep], r1     /* EIPSW の設定 */
    ldsr  r1, eipsw
    popsp   r20-r29               /* レジスタの復帰 */
    RESTORE_SCRATCH_REG
    eiret

    /*
      *  IDLEタイムウィンドウへの遷移
      */
ret_idle:
    /* EBASEの設定 */
    Lea  _EBaseIDLE, r6
    ldsr r6, 3, 1

   /* EIPSWH を0クリア */
    ldsr    r0, eipswh

    /* 
     *  HVスタックからコンテキストの復帰 
     */
    popsp  r20-r29
    mov     sp, ep
    sld.w    4[ep], r6
    ldsr    r6, eipc
    sld.w    0[ep], r6
    ldsr    r6, eipsw
    addi    8, sp, sp
    RESTORE_SCRATCH_REG
    eiret
