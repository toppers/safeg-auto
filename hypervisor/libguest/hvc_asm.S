/*
 *  サービスコール呼び出し用の関数
 */

/*
 *  GHSにおけるインラインアセンブラでは，破壊レジスタの指定が
 *  保証されないため，直接アセンブリで記述する
 */

#include <rh850asm.h>
 
/*
 *  サービスコールの出入口
 *    これらの関数呼び出し時のコンベンション
 *      r10 : 返り値
 *      r6 - r9 : 引数（最後のレジスタが関数コード）
 *      スクラッチレジスタ保存（関数呼出しによりここでは保存済み）
 *    HVのサービスコードのコンベンション
 *      r10 : 返り値
 *      r9 ：関数コード
 *      r6 - r8 : 引数(3個まで)
 *    以下のレジスタはここで保存
 *      gp(r4)  : fixedの場合があるため
 *      tp(r5)  : fixedの場合があるため
 *      ep(r30) : fixedの場合があるため
 *      lp(r31) : caller saved レジスタであるため．
 */
    .section .text

    /*
     *  引数0個の呼び出し
     */
    .align 4
    .global _cal_hvc_0
_cal_hvc_0:
    /* PSW(割込み状態)を保存 */
    stsr   psw, r12
    pushsp r12-r12

    /* 関数コードのコピー */
    mov r6, r9

    di
    pushsp r4-r5   /* gp-tp */
    pushsp r30-r31 /* ep-lp */
    hvtrap 1
    popsp  r30-r31 /* ep-lp */
    popsp  r4-r5   /* gp-tp */

    /* PSW(割込み状態)を復帰 */
    popsp r12-r12
    ldsr  r12, psw

    jmp [lp]
    nop

    /*
     *  引数1個の呼び出し
     */
    .align 4
    .global _cal_hvc_1
_cal_hvc_1:

    /* PSW(割込み状態)を保存 */
    stsr   psw, r12
    pushsp r12-r12

    /* 関数コードのコピー */
    mov r7, r9

    di
    pushsp r4-r5   /* gp-tp */
    pushsp r30-r31 /* ep-lp */
    hvtrap 1
    popsp  r30-r31 /* ep-lp */
    popsp  r4-r5   /* gp-tp */

    /* PSW(割込み状態)を復帰 */
    popsp r12-r12
    ldsr  r12, psw

    jmp [lp]
    nop

    /*
     *  引数2個の呼び出し
     */
    .align 4
    .global _cal_hvc_2
_cal_hvc_2:
    /* PSW(割込み状態)を保存 */
    stsr   psw, r12
    pushsp r12-r12

    /* 関数コードのコピー */
    mov r8, r9

    di
    pushsp r4-r5   /* gp-tp */
    pushsp r30-r31 /* ep-lp */
    hvtrap 1
    popsp  r30-r31 /* ep-lp */
    popsp  r4-r5   /* gp-tp */
 
    /* PSW(割込み状態)を復帰 */
    popsp r12-r12
    ldsr  r12, psw

    jmp [lp]
    nop

    /*
     *  引数3個の呼び出し
     */
    .align 4
    .global _cal_hvc_3
_cal_hvc_3:
    /* PSW(割込み状態)を保存 */
    stsr   psw, r12
    pushsp r12-r12

    di
    pushsp r4-r5   /* gp-tp */
    pushsp r30-r31 /* ep-lp */
    hvtrap 1
    popsp  r30-r31 /* ep-lp */
    popsp  r4-r5   /* gp-tp */

    /* PSW(割込み状態)を復帰 */
    popsp r12-r12
    ldsr  r12, psw

    jmp [lp]
    nop
