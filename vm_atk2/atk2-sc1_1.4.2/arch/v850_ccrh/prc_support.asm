;/* 
 ;*  TOPPERS ATK2
 ;*      Toyohashi Open Platform for Embedded Real-Time Systems
 ;*      Automotive Kernel Version 2
 ;*
 ;*  Copyright (C) 2012-2016 by Center for Embedded Computing Systems
 ;*              Graduate School of Information Science, Nagoya Univ., JAPAN
 ;*  Copyright (C) 2012-2017 by FUJI SOFT INCORPORATED, JAPAN
 ;*  Copyright (C) 2012-2013 by Spansion LLC, USA
 ;*  Copyright (C) 2012-2013 by NEC Communication Systems, Ltd., JAPAN
 ;*  Copyright (C) 2012-2014 by Panasonic Advanced Technology Development Co., Ltd., JAPAN
 ;*  Copyright (C) 2012-2014 by Renesas Electronics Corporation, JAPAN
 ;*  Copyright (C) 2012-2014 by Sunny Giken Inc., JAPAN
 ;*  Copyright (C) 2012-2014 by TOSHIBA CORPORATION, JAPAN
 ;*  Copyright (C) 2012-2014 by Witz Corporation, JAPAN
 ;*
 ;*  上記著作権者は，以下の(1)～(4)の条件を満たす場合に限り，本ソフトウェ
 ;*  ア（本ソフトウェアを改変したものを含む．以下同じ）を使用・複製・改
 ;*  変・再配布（以下，利用と呼ぶ）することを無償で許諾する．
 ;*  (1) 本ソフトウェアをソースコードの形で利用する場合には，上記の著作
 ;*      権表示，この利用条件および下記の無保証規定が，そのままの形でソー
 ;*      スコード中に含まれていること．
 ;*  (2) 本ソフトウェアを，ライブラリ形式など，他のソフトウェア開発に使
 ;*      用できる形で再配布する場合には，再配布に伴うドキュメント（利用
 ;*      者マニュアルなど）に，上記の著作権表示，この利用条件および下記
 ;*      の無保証規定を掲載すること．
 ;*  (3) 本ソフトウェアを，機器に組み込むなど，他のソフトウェア開発に使
 ;*      用できない形で再配布する場合には，次のいずれかの条件を満たすこ
 ;*      と．
 ;*    (a) 再配布に伴うドキュメント（利用者マニュアルなど）に，上記の著
 ;*        作権表示，この利用条件および下記の無保証規定を掲載すること．
 ;*    (b) 再配布の形態を，別に定める方法によって，TOPPERSプロジェクトに
 ;*        報告すること．
 ;*  (4) 本ソフトウェアの利用により直接的または間接的に生じるいかなる損
 ;*      害からも，上記著作権者およびTOPPERSプロジェクトを免責すること．
 ;*      また，本ソフトウェアのユーザまたはエンドユーザからのいかなる理
 ;*      由に基づく請求からも，上記著作権者およびTOPPERSプロジェクトを
 ;*      免責すること．
 ;*
 ;*  本ソフトウェアは，AUTOSAR（AUTomotive Open System ARchitecture）仕
 ;*  様に基づいている．上記の許諾は，AUTOSARの知的財産権を許諾するもので
 ;*  はない．AUTOSARは，AUTOSAR仕様に基づいたソフトウェアを商用目的で利
 ;*  用する者に対して，AUTOSARパートナーになることを求めている．
 ;*
 ;*  本ソフトウェアは，無保証で提供されているものである．上記著作権者お
 ;*  よびTOPPERSプロジェクトは，本ソフトウェアに関して，特定の使用目的
 ;*  に対する適合性も含めて，いかなる保証も行わない．また，本ソフトウェ
 ;*  アの利用により直接的または間接的に生じたいかなる損害に関しても，そ
 ;*  の責任を負わない．
 ;*
 ;*  $Id: prc_support.S 834 2017-09-23 02:19:10Z ertl-honda $
 ;*/

;/*
 ;*		ターゲット依存情報の定義
 ;*/
$include (v850asm.inc)
$include (offset.h)

$ifdef USE_ASMCONFIG_INC
$include (asm_config.inc)
$endif ;/* USE_ASMCONFIG_INC */

$ifdef __v850e2v3__

;/*
 ;*  V850E2用の割込みコントローラ操作ルーチン
 ;*/
GET_ISPR .macro reg
	mov		ISPR_H, reg
	ld.h	0[reg],reg
.endm

CLEAR_ISPR .macro reg1, reg2
	mov		ISPC_H,reg1
	mov		0xffff,reg2
	st.h	reg2,0[reg1]
	mov		ISPR_H, reg1
	st.h	r0,0[reg1]
.endm

SET_PMR .macro reg_val, reg_tmp
	mov     PMR, reg_tmp
	st.h    reg_val, 0[reg_tmp]  ;/* set interrupt level */
	syncm
.endm

$endif ;/* __v850e2v3__ */

$ifdef __rh850__

GET_ISPR .macro reg
	stsr 10, reg, 2
.endm

SET_ISPR .macro val, reg 
	movea val,r0,reg
	ldsr reg, 10, 2
.endm

SET_INTCFG .macro val, reg 
	movea val,r0,reg
	ldsr reg, 13, 2
.endm

CLEAR_ISPR .macro reg1, reg2
	SET_INTCFG 1, reg1 ;/* ISPR を書き換え可能に */
	SET_ISPR   0, reg1 ;/* ISPR のクリア */
	SET_INTCFG 0, reg1 ;/* ISPR を書き換え禁止に(自動更新に) */
.endm

$endif ;/* __rh850__ */

$ifdef __v850e3v5__
SET_PMR .macro reg_val, reg_tmp
	stsr psw, reg_tmp
	di
	ldsr reg_val, 11, 2
	ldsr reg_tmp, psw
.endm
$endif ;/* __v850e3v5__ */

$ifdef __rh850v2__
SET_PLMR .macro reg_val, reg_tmp
	stsr psw, reg_tmp
	di
	ldsr reg_val, 14, 2
	ldsr reg_tmp, psw
.endm
$endif ;/* __rh850v2__ */

;/*
 ;*  OS割込み禁止マクロ
 ;*  ネストの一番外側で呼び出されることを想定している
 ;*/
OUTER_LOCK_OS_INT .macro   reg12, reg13
$ifndef __rh850v2__
    ;/* pmr_isr2_mask -> PMR */
    Lea     _kernel_pmr_isr2_mask, reg13
    ld.h    0[reg13], reg13
    SET_PMR reg13, reg12
$else ;/* __rh850v2__ */
    ;/* isr2max_iintpri -> PLMR */
    Lea      _kernel_isr2max_iintpri, reg13
    ld.b     0[reg13], reg13
    SET_PLMR reg13, reg12
$endif ;/* !__rh850v2__ */

    ;/* nested_lock_os_int_cnt = 1 */
    Lea     _kernel_nested_lock_os_int_cnt, reg12
    mov     1, reg13
    st.b    reg13, 0[reg12]

.endm

;/*
 ;*  OS割込み禁止解除マクロ
 ;*  ネストの一番外側で呼び出されることを想定している
 ;*/
OUTER_UNLOCK_OS_INT .macro  reg12, reg13

    ;/* nested_lock_os_int_cnt = 0 */
    Lea     _kernel_nested_lock_os_int_cnt, reg12
    mov     r0, reg13
    st.b    reg13, 0[reg12]

$ifndef __rh850v2__
    ;/* pmr_setting_tbl[current_iintpri] -> PMR */
    Lea     _kernel_current_iintpri, reg12
    ld.bu   0[reg12], reg12
    shl     1, reg12

    Lea     _kernel_pmr_setting_tbl, reg13
    add     reg12, reg13
    ld.h    0[reg13], reg13
    SET_PMR reg13, reg12  ;/* set interrupt level */
$else ;/* __rh850v2__ */
    Lea     _kernel_current_iintpri, reg12
    ld.bu   0[reg12], reg13
    SET_PLMR reg13, reg12
$endif ;/* !__rh850v2__ */

.endm

	;/* 
	 ;* 割込み/例外の先頭で実行する処理 
	;*/
PRE_INT_EXC .macro
$ifdef _RH850G3M_
	;/* 一部中断型例外ハンドラでのみ必要だが一律実行する */
	syncp
$endif ;/* _RH850G3M_ */
.endm

	;/*
	 ;* 割込み例外入り口でのレジスタの保存マクロ
	 ;*/
INT_EXC_SAVEREG .macro
$ifndef TOPPERS_USE_PUSHSP
$ifdef TOPPERS_USE_HFLOAT
	addi    -84 , sp , sp
$else ;/* !TOPPERS_USE_HFLOAT */
	addi    -80 , sp , sp
$endif ;/* TOPPERS_USE_HFLOAT */
	st.w    r30, 12[sp]  ;/* r30(ep) */
	mov     sp, ep 
	sst.w   r1, 76[ep]
	sst.w   r5, 72[ep]
	sst.w   r6, 68[ep]
	sst.w   r7, 64[ep]
	sst.w   r8, 60[ep]
	sst.w   r9, 56[ep]
	sst.w   r10, 52[ep]
	sst.w   r11, 48[ep]
	sst.w   r12, 44[ep]
	sst.w   r13, 40[ep]
	sst.w   r14, 36[ep]
	sst.w   r15, 32[ep]
	sst.w   r16, 28[ep]
	sst.w   r17, 24[ep]
	sst.w   r18, 20[ep]
	sst.w   r19, 16[ep]
	sst.w   r31, 8[ep]
$ifdef TOPPERS_USE_HFLOAT
	stsr    fpsr, r19     ;/* load FPSR */
	sst.w   r19, 80[ep]
$endif ;/* TOPPERS_USE_HFLOAT */
$else ;/* TOPPERS_USE_PUSHSP */
	pushsp  r1-r1
$ifdef TOPPERS_USE_HFLOAT
	stsr    fpsr, r1     ;/* load FPSR */
	pushsp  r1-r1
$endif ;/* TOPPERS_USE_HFLOAT */
	pushsp  r5-r19
	pushsp  r30-r31
	addi    -8, sp , sp
	mov     sp, ep 
$endif ;/* TOPPERS_USE_PUSHSP */
.endm

	;/*
	 ;* 割込み例外入り口でのレジスタの復帰マクロ
	 ;*/
INT_EXC_RESTOREREG .macro
$ifndef TOPPERS_USE_PUSHSP
$ifdef TOPPERS_USE_HFLOAT
	sld.w   80[ep], r19
	ldsr    r19, fpsr     ;/* store FPSR */
$endif ;/* TOPPERS_USE_HFLOAT */
	sld.w   76[ep], r1
	sld.w   72[ep], r5
	sld.w   68[ep], r6
	sld.w   64[ep], r7
	sld.w   60[ep], r8
	sld.w   56[ep], r9
	sld.w   52[ep], r10
	sld.w   48[ep], r11
	sld.w   44[ep], r12
	sld.w   40[ep], r13
	sld.w   36[ep], r14
	sld.w   32[ep], r15
	sld.w   28[ep], r16
	sld.w   24[ep], r17
	sld.w   20[ep], r18
	sld.w   16[ep], r19
	sld.w    8[ep], r31
$ifdef TOPPERS_USE_HFLOAT
	addi    84, ep, sp
$else ;/* !TOPPERS_USE_HFLOAT */
	addi    80, ep, sp
$endif ;/* TOPPERS_USE_HFLOAT */
	sld.w   12[ep], r30 ;/* r30(ep) */
$else ;/* TOPPERS_USE_PUSHSP */
	addi    8, sp , sp
	popsp   r30-r31
	popsp   r5-r19
$ifdef TOPPERS_USE_HFLOAT
	popsp   r1-r1
	ldsr    r1, fpsr     ;/* store FPSR */
$endif ;/* TOPPERS_USE_HFLOAT */
	popsp   r1-r1
$endif ;/* TOPPERS_USE_PUSHSP */
.endm

FE_SAVEREG .macro
	INT_EXC_SAVEREG
	stsr    fepc, r6
	sst.w   r6, 4[ep]
	stsr    fepsw, r6
	sst.w   r6, 0[ep]
.endm

FE_RESTOREREG .macro
	sld.w    4[ep], r6
	ldsr    r6, fepc
	sld.w    0[ep], r6
	ldsr    r6, fepsw
	INT_EXC_RESTOREREG
.endm

EI_SAVEREG .macro
	INT_EXC_SAVEREG
	stsr    eipc, r6
	sst.w   r6, 4[ep]
	stsr    eipsw, r6
	sst.w   r6, 0[ep]
.endm

EI_RESTOREREG .macro
	sld.w    4[ep], r6
	ldsr    r6, eipc
	sld.w    0[ep], r6
	ldsr    r6, eipsw
	INT_EXC_RESTOREREG
.endm

$ifdef __rh850__
	;/* 
	 ;* RH850用ベクタ
	 ;* ここにRBASEが設定されることを想定（ここが電源投入時の開始アドレス）
	 ;* (EIレベル割込みはテーブル参照方式を使用するため本ベクタは使用しない)
	 ;*/
	RESET .cseg text
	.extern __reset
__reset:
	jr __start
	.rept 6
	nop
	.endm
__syserr: ;/* 0x0010 */
	jr _kernel_fe_exception_entry
	.rept 6
	nop
	.endm
__hvtrap: ;/* 0x0020 */
	jr _kernel_fe_exception_entry
	.rept 6
	nop
	.endm
__fetrap: ;/* 0x0030 */
	jr _kernel_fe_exception_entry
	.rept 6
	nop
	.endm
__trap0:  ;/* 0x0040 */
	jr _kernel_ei_exception_entry
	.rept 6
	nop
	.endm
__trap1:  ;/* 0x0050 */
	jr _kernel_ei_exception_entry
	.rept 6
	nop
	.endm
__rie:    ;/* 0x0060 */
	jr _kernel_ei_exception_entry
	.rept 6
	nop
	.endm
__fppfpi: ;/* 0x0070 */
	jr _kernel_ei_exception_entry
	.rept 6
	nop
	.endm
__ucpop:  ;/* 0x080 */
	jr _kernel_fe_exception_entry
	.rept 6
	nop
	.endm
__mip:    ;/* 0x0090 */
	jr _kernel_fe_exception_entry
	.rept 6
	nop
	.endm
__pie:   ;/* 0x00a0 */
	jr _kernel_fe_exception_entry
	.rept 6
	nop
	.endm
__debug:  ;/* 0x00b0 */
	jr __debug
	.rept 6
	nop
	.endm
__mae:    ;/* 0x00c0 */
	jr _kernel_fe_exception_entry
	.rept 6
	nop
	.endm
__rfu:    ;/* 0x00d0 */
	jr __rfu
	.rept 6
	nop
	.endm
__fenmi:  ;/* 0x00e0 */
	jr _kernel_fe_exception_entry
	.rept 6
	nop
	.endm
__feint:  ;/* 0x00f0 */
	jr _kernel_fe_exception_entry
	.rept 6
	nop
	.endm
__eiintn0:  ;/*（優先度0） 0x0100 */
	jr _kernel_default_int_handler
	.rept 6
	nop
	.endm
__eiintn1:  ;/*（優先度1） 0x0110 */
	jr _kernel_default_int_handler
	.rept 6
	nop
	.endm
__eiintn2:  ;/*（優先度2） 0x0120 */
	jr _kernel_default_int_handler
	.rept 6
	nop
	.endm
__eiintn3:  ;/*（優先度3） 0x0130 */
	jr _kernel_default_int_handler
	.rept 6
	nop
	.endm
__eiintn4:  ;/*（優先度4） 0x0140 */
	jr _kernel_default_int_handler
	.rept 6
	nop
	.endm
__eiintn5:  ;/*（優先度5） 0x0150 */
	jr _kernel_default_int_handler
	.rept 6
	nop
	.endm
__eiintn6:  ;/*（優先度6） 0x0160 */
	jr _kernel_default_int_handler
	.rept 6
	nop
	.endm
__eiintn7:  ;/*（優先度7） 0x0170 */
	jr _kernel_default_int_handler
	.rept 6
	nop
	.endm
__eiintn8:  ;/*（優先度8） 0x0180 */
	jr _kernel_default_int_handler
	.rept 6
	nop
	.endm
__eiintn9:  ;/*（優先度9） 0x0190 */
	jr _kernel_default_int_handler
	.rept 6
	nop
	.endm
__eiintn10: ;/*（優先度10） 0x01a0 */
	jr _kernel_default_int_handler
	.rept 6
	nop
	.endm
__eiintn11: ;/*（優先度11） 0x01b0 */
	jr _kernel_default_int_handler
	.rept 6
	nop
	.endm
__eiintn12: ;/*（優先度12） 0x01c0 */
	jr _kernel_default_int_handler
	.rept 6
	nop
	.endm
__eiintn13: ;/*（優先度13） 0x01d0 */
	jr _kernel_default_int_handler
	.rept 6
	nop
	.endm
__eiintn14: ;/*（優先度14） 0x01e0 */
	jr _kernel_default_int_handler
	.rept 6
	nop
	.endm
__eiintn15: ;/*（優先度15） 0x01f0 */
	jr _kernel_default_int_handler
	.rept 6
	nop
	.endm

$endif ;/* __rh850__ */

	;/*
	 ;*  例外エントリ（feretでリターンする例外用）
	 ;*/
	.cseg text
	.align 2
	.extern _kernel_fe_exception_entry
_kernel_fe_exception_entry:
	;/* 割込み/例外の先頭で実行する処理 */
	PRE_INT_EXC

	;/* 
	 ;* レジスタの保存(FEPCとFEPSWも保存) 
	 ;*/
	FE_SAVEREG

	;/*
	 ;* プロテクションフックはOS処理レベルで実行するためOS割込み禁止状態にする
	 ;* OS割込み禁止状態で例外が発生する可能性があるため，ネスト管理を行う．
	 ;* 例外は次の条件で発生する
	 ;*  ・ISR1実行状態で発生した場合
	 ;*  ・ISR1実行状態以外で発生した場合
	 ;*  ・OS割込み解除状態で発生した場合
	 ;*  ・OS割込み禁止状態で発生した場合
	 ;*/

	;/*
	 ;* カテゴリ1の割込み以外で例外が発生したかの判断する
	 ;* カテゴリ1で例外が発生：fe_exception_1へジャンプ
	 ;*/
$ifndef TOPPERS_ENABLE_EIP
	GET_ISPR r6
	Lea     _kernel_pmr_isr1_mask, r7
	ld.h    0[r7], r7
	and     r7, r6
	cmp     r0, r6
	bne     fe_exception_1
$else ;/* TOPPERS_ENABLE_EIP */
	stsr    fepsw, r6
	Lea     _kernel_isr2max_iintpri, r7
	ld.bu   0[r7], r7
	shr     20, r6 ;/* r6 >> 20 */
	zxb     r6
	cmp     r7, r6
	bnl     fe_exception_1
$endif ;/* !TOPPERS_ENABLE_EIP */

	;/*
	 ;*  ISR1実行状態以外で発生した場合
	 ;*/
	Lea     _kernel_nested_lock_os_int_cnt, r12
	ld.b    0[r12], r13
	mov     1, r14
	add     r14, r13
	st.b    r13, 0[r12]
	cmp     1, r13
	bne     fe_exception_1

	;/*
	 ;* OS割込み解除状態で発生した場合
	 ;* OS割込み禁止状態とする
	 ;*/
$ifndef __rh850v2__
	Lea     _kernel_pmr_isr2_mask, r7
	ld.h    0[r7], r7
	SET_PMR r7, r6  ;/* pmr_isr2_mask -> PMR */
$else ;/* __rh850v2__ */
	Lea      _kernel_isr2max_iintpri, r7
	ld.bu    0[r7], r7
	SET_PLMR r7, r6  ;/* isr2max_iintpri -> PLMR */
$endif ;/* !__rh850v2__ */

fe_exception_1:
	;/*
	 ;*  割込み発生時に変更されるので割込み解除前に取得
	 ;*/
	stsr    feic, r19                   ;/* 例外要因 */
	stsr    fepc, r18                   ;/* 例外発生PC */

	;/* 
	 ;* C1ISRを受け付けるため，NPbit クリア 
	 ;*/
	stsr    psw, r12
	mov     !0x0080, r13
	and     r12, r13
	ldsr    r13, psw

fe_exception_2:
	;/*
	 ;*  カーネル起動していない場合に起きたCPU例外は，無限ループへ
	 ;*/
	Lea   _kernel_kerflg, r12          ;/* kerflgがFALSEなら無限ループ */
	ld.b  0[r12], r12
	cmp   r0, r12
	be    infinity_loop

	;/*
	 ;* 多重割込みか判定
	 ;*/
	Lea     _kernel_except_nest_cnt, r12
	ld.w    0[r12], r13
	mov     1, r14                      ;/*  割込み・例外のネスト回数のインクリメント */
	add     r14, r13
	st.w    r13, 0[r12]

	mov     sp, r9                      ;/* sp をr9に退避 */

	cmp     1, r13
	bne     fe_exception_3              ;/* _kernel_nested_lock_os_int_cnt > 1 -> fe_exception_3 */

	;/*
	 ;* C1ISR実行状態で発生した場合はスタックポインタの入れ替えしない
	 ;*/
$ifndef TOPPERS_ENABLE_EIP
	GET_ISPR r6
	Lea     _kernel_pmr_isr1_mask, r7
	ld.h    0[r7], r7
	and     r7, r6
	cmp     r0, r6
	bne     fe_exception_3
$else ;/* TOPPERS_ENABLE_EIP */
	stsr    fepsw, r6
	Lea     _kernel_isr2max_iintpri, r7
	ld.bu   0[r7], r7
	shr     20, r6 ;/* r6 >> 20 */
	zxb     r6
	cmp     r7, r6
	bnl     fe_exception_3
$endif ;/* !TOPPERS_ENABLE_EIP */

	;/* スタックポインタの保存と入れ替え */
	Lea     _kernel_ostkpt, r12
	ld.w    0[r12], r12
	add     -4, r12
	st.w    sp, 0[r12]                      ;/* スタックポインタの保存 */
	mov     r12, sp                         ;/* スタックポインタの切り替え */

fe_exception_3:
	;/* 
	 ;* FEPSW.ID == 1の場合は全割込み禁止状態で例外が発生したので解除しない 
	 ;*/ 
	stsr    fepsw, r12
	mov     0x20, r13
	tst     r12, r13
	bne     fe_exception_4 

	UNLOCK_ALL_INT

fe_exception_4:
$ifdef CFG_USE_PROTECTIONHOOK
	;/* v850_cpu_exp_*を退避するスペースを確保 */
	addi    -12, sp, sp

	;/* v850_cpu_exp_spの退避と設定 */
	Lea     _kernel_v850_cpu_exp_sp, r12
	st.w    r9, 0[r12]
	st.w    ep, 0[sp]
	mov     sp, ep

	;/* v850_cpu_exp_noの退避と設定 */
	Lea     _kernel_v850_cpu_exp_no, r12
	ld.w    0[r12], r13
	sst.w   r13, 8[ep]
	st.w    r19, 0[r12]                     ;/* r19 例外要因 */

	;/* v850_cpu_exp_pcの退避と設定 */
	Lea     _kernel_v850_cpu_exp_pc, r12
	ld.w    0[r12], r13
	sst.w   r13, 4[ep]
	mov     r18, r17                        ;/* r18:fepc */
	add     -4, r17
	st.w    r17, 0[r12]

$endif ;/* CFG_USE_PROTECTIONHOOK */

	;/*
	 ;* CPU例外が発生した場合，OSはE_OS_PROTECTION_EXCEPTIONをパラメータとして
	 ;* プロテクションフックを呼び出す
	 ;*/
	mov     E_OS_PROTECTION_EXCEPTION, r6
	jarl    _kernel_call_protectionhk_main, r31

$ifdef CFG_USE_PROTECTIONHOOK
	mov     sp, ep

	;/* v850_cpu_exp_noを復帰 */
	Lea     _kernel_v850_cpu_exp_no, r12
	sld.w   8[ep], r13
	st.w    r13, 0[r12]

	;/* v850_cpu_exp_pcを復帰 */
	Lea     _kernel_v850_cpu_exp_pc, r12
	sld.w   4[ep], r13
	st.w    r13, 0[r12]

	;/* v850_cpu_exp_sp退避 */
	Lea     _kernel_v850_cpu_exp_sp, r12
	sld.w   0[ep], r13
	st.w    r13, 0[r12]

	;/* v850_cpu_exp_*を退避したスペースを戻す */
	addi    12, sp, sp
$endif ;/* CFG_USE_PROTECTIONHOOK */

fe_ret_exc:
	LOCK_ALL_INT

	;/*
	 ;* プロテクションフックはOS割込み禁止状態で実行する
	 ;*/

	;/*
	 ;*  割込み・例外のネスト回数のデクリメント
	 ;*/
	Lea     _kernel_except_nest_cnt, r12
	ld.w    0[r12], r13
	mov     -1, r14
	add     r14, r13
	st.w    r13, 0[r12]

	;/*
	 ;*  OS割込み禁止状態の解除
	 ;*/
	;/*
	 ;* ISR1実行状態で発生した場合は
	 ;* (1)スタックポインタの入れ替えをしない
	 ;* (2)OS割込み禁止状態の解除をしない
	;*/
$ifndef TOPPERS_ENABLE_EIP
	GET_ISPR r6
	Lea     _kernel_pmr_isr1_mask, r7
	ld.h    0[r7], r7
	and     r7, r6
	cmp     r0, r6
	bne     ret_exc_2
$else ;/* TOPPERS_ENABLE_EIP */
	stsr    fepsw, r6
	Lea     _kernel_isr2max_iintpri, r7
	ld.bu   0[r7], r7
	shr     20, r6 ;/* r6 >> 20 */
	zxb     r6
	cmp     r7, r6
	bnl     ret_exc_2
$endif ;/* !TOPPERS_ENABLE_EIP */
	;/*
	 ;*  ネスト回数が1以上なら割込み元がISRなため例外発生元へ復帰
	 ;*  スタックポインタの入れ替えしない
	 ;*/
	cmp     r0, r13
	bne     ret_exc_1

	;/*
	 ;*  戻り先がタスクなためスタックを戻す
	 ;*/
	ld.w    0[sp], sp

ret_exc_1:
	;/*
	 ;*  ISR1実行状態以外で発生した場合，OS割込み禁止状態の解除
	 ;*/
	Lea     _kernel_nested_lock_os_int_cnt, r12
	ld.b    0[r12], r13
	mov     -1, r14
	add     r14, r13
	st.b    r13, 0[r12]
	cmp     r0, r13
	bne     ret_exc_2

	;/* OS割込みを解除 */
$ifndef __rh850v2__
	;/* pmr_setting_tbl[current_iintpri] -> PMR */
	Lea     _kernel_current_iintpri, r12
	ld.bu   0[r12], r12
	shl     1, r12

	Lea     _kernel_pmr_setting_tbl, r13
	add     r12, r13
	ld.h    0[r13], r13
	SET_PMR r13, r12     ;/* set interrupt level */
$else ;/* __rh850v2__ */
	Lea     _kernel_current_iintpri, r12
	ld.bu   0[r12], r12
	SET_PLMR r12, r6  ;/* current_iintpri -> PLMR */
$endif ;/* !__rh850v2__ */

	;/*
	 ;*  例外発生元へ復帰
	 ;*/
ret_exc_2:
	;/*
	 ;* レジスタを復帰(FEPCとFEPSWも復帰)
	 ;*/
	FE_RESTOREREG

	feret

	;/*
	 ;*  例外エントリ（eiretでリターンする例外用）
	 ;*/
	.extern _kernel_ei_exception_entry
_kernel_ei_exception_entry:
	;/* 割込み/例外の先頭で実行する処理 */
	PRE_INT_EXC

	;/* 
	 ;* レジスタの保存(EIPCとEIPSWも保存) 
	 ;*/
	EI_SAVEREG

	;/*
	 ;* プロテクションフックはOS処理レベルで実行するためOS割込み禁止状態にする
	 ;* OS割込み禁止状態で例外が発生する可能性があるため，ネスト管理を行う．
	 ;* 例外は次の条件で発生する
	 ;*  ・ISR1実行状態で発生した場合
	 ;*  ・ISR1実行状態以外で発生した場合
	 ;*  ・OS割込み解除状態で発生した場合
	 ;*  ・OS割込み禁止状態で発生した場合
	 ;*/

	;/*
	 ;* カテゴリ1の割り込み以外で例外が発生したかの判断する
	 ;* カテゴリ1で例外が発生：ei_exception_1へジャンプ
	 ;*/
$ifndef TOPPERS_ENABLE_EIP
	GET_ISPR r6
	Lea     _kernel_pmr_isr1_mask, r7
	ld.h    0[r7], r7
	and     r7, r6
	cmp     r0, r6
	bne     ei_exception_1
$else ;/* TOPPERS_ENABLE_EIP */
	stsr    eipsw, r6
	Lea     _kernel_isr2max_iintpri, r7
	ld.bu   0[r7], r7
	shr     20, r6 ;/* r6 >> 20 */
	zxb     r6
	cmp     r7, r6
	bnl     ei_exception_1
$endif ;/* !TOPPERS_ENABLE_EIP */
	;/*
	 ;*  ISR1実行状態以外で発生した場合
	 ;*/
	Lea     _kernel_nested_lock_os_int_cnt, r12
	ld.b    0[r12], r13
	mov     1, r14
	add     r14, r13
	st.b    r13, 0[r12]
	cmp     1, r13
	bne     ei_exception_1

	;/*
	 ;* OS割込み解除状態で発生した場合
	 ;* OS割込み禁止状態とする
	 ;*/
$ifndef __rh850v2__
	Lea     _kernel_pmr_isr2_mask, r7
	ld.h    0[r7], r7
	SET_PMR r7, r6  ;/* pmr_isr2_mask -> PMR */
$else ;/* __rh850v2__ */
	Lea      _kernel_isr2max_iintpri, r7
	ld.bu    0[r7], r7
	SET_PLMR r7, r6  ;/* isr2max_iintpri -> PLMR */
$endif ;/* !__rh850v2__ */

ei_exception_1:
	;/*
	 ;*  割込み発生時に変更されるので割込み解除前に取得
	 ;*/
	stsr    eiic, r19                   ;/* 例外要因 */
	stsr    eipc, r18                   ;/* 例外発生PC */


ei_exception_2:
	;/*
	 ;*  カーネル起動していない場合に起きたCPU例外は，無限ループへ
	 ;*/
	Lea   _kernel_kerflg, r12          ;/* kerflgがFALSEなら無限ループ */
	ld.b  0[r12], r12
	cmp   r0, r12
	be    infinity_loop

	;/*
	 ;* 多重割込みか判定
	 ;*/
	Lea     _kernel_except_nest_cnt, r12
	ld.w    0[r12], r13
	mov     1, r14                      ;/*  割込み・例外のネスト回数のインクリメント */
	add     r14, r13
	st.w    r13, 0[r12]

	mov     sp, r9                      ;/* sp をr9に退避 */

	cmp     1, r13
	bne     ei_exception_3             ;/* _kernel_nested_lock_os_int_cnt > 1 -> ei_exception_3 */

	;/*
	 ;* ISR1実行状態で発生した場合はスタックポインタの入れ替えしない
	 ;*/
$ifndef TOPPERS_ENABLE_EIP
	GET_ISPR r6
	Lea     _kernel_pmr_isr1_mask, r7
	ld.h    0[r7], r7
	and     r7, r6
	cmp     r0, r6
	bne     ei_exception_3
$else ;/* TOPPERS_ENABLE_EIP */
	stsr    eipsw, r6
	Lea     _kernel_isr2max_iintpri, r7
	ld.bu   0[r7], r7
	shr     20, r6 ;/* r6 >> 20 */
	zxb     r6
	cmp     r7, r6
	bnl     ei_exception_3
$endif ;/* !TOPPERS_ENABLE_EIP */

	;/* スタックポインタの保存と入れ替え */
	Lea     _kernel_ostkpt, r12
	ld.w    0[r12], r12
	add     -4, r12
	st.w    sp, 0[r12]                      ;/* スタックポインタの保存 */
	mov     r12, sp                         ;/* スタックポインタの切り替え */

ei_exception_3:
	;/* fe_ecxeption_3と異なりEIPSW.ID == 1の場合はここにこないため，条件判定は不要 */
	UNLOCK_ALL_INT

$ifdef CFG_USE_PROTECTIONHOOK
	;/* v850_cpu_exp_*を退避するスペースを確保 */
	addi    -12, sp, sp

	;/* v850_cpu_exp_spの退避と設定 */
	Lea     _kernel_v850_cpu_exp_sp, r12
	st.w    r9, 0[r12]
	st.w    ep, 0[sp]
	mov     sp, ep

	;/* v850_cpu_exp_noの退避と設定 */
	Lea     _kernel_v850_cpu_exp_no, r12
	ld.w    0[r12], r13
	sst.w   r13, 8[ep]
	st.w    r19, 0[r12]                     ;/* r19 例外要因 */

	;/* v850_cpu_exp_pcの退避と設定 */
	Lea     _kernel_v850_cpu_exp_pc, r12
	ld.w    0[r12], r13
	sst.w   r13, 4[ep]
	mov     r18, r17                        ;/* r18:fepc */
	add     -4, r17
	st.w    r17, 0[r12]

$endif ;/* CFG_USE_PROTECTIONHOOK */

	;/*
	 ;* CPU例外が発生した場合，OSはE_OS_PROTECTION_EXCEPTIONをパラメータとして
	 ;* プロテクションフックを呼び出す
	 ;*/
	mov     E_OS_PROTECTION_EXCEPTION, r6
	jarl    _kernel_call_protectionhk_main, r31

$ifdef CFG_USE_PROTECTIONHOOK
	mov     sp, ep

	;/* v850_cpu_exp_noを復帰 */
	Lea     _kernel_v850_cpu_exp_no, r12
	sld.w   8[ep], r13
	st.w    r13, 0[r12]

	;/* v850_cpu_exp_pcを復帰 */
	Lea     _kernel_v850_cpu_exp_pc, r12
	sld.w   4[ep], r13
	st.w    r13, 0[r12]

	;/* v850_cpu_exp_sp退避 */
	Lea     _kernel_v850_cpu_exp_sp, r12
	sld.w   0[ep], r13
	st.w    r13, 0[r12]

	;/* v850_cpu_exp_*を退避したスペースを戻す */
	addi    12, sp, sp
$endif ;/* CFG_USE_PROTECTIONHOOK */

ei_ret_exc:
	LOCK_ALL_INT

	;/*
	 ;* プロテクションフックはOS割込み禁止状態で実行する
	 ;*/

	;/*
	 ;*  割込み・例外のネスト回数のデクリメント
	 ;*/
	Lea     _kernel_except_nest_cnt, r12
	ld.w    0[r12], r13
	mov     -1, r14
	add     r14, r13
	st.w    r13, 0[r12]

	;/*
	 ;*  OS割込み禁止状態の解除
	 ;*/
	;/*
	 ;* ISR1実行状態で発生した場合は
	 ;* (1)スタックポインタの入れ替えをしない
	 ;* (2)OS割込み禁止状態の解除をしない
	;*/
$ifndef TOPPERS_ENABLE_EIP
	GET_ISPR r6
	Lea     _kernel_pmr_isr1_mask, r7
	ld.h    0[r7], r7
	and     r7, r6
	cmp     r0, r6
	bne     ei_ret_exc_2
$else ;/* TOPPERS_ENABLE_EIP */
	stsr    eipsw, r6
	Lea     _kernel_isr2max_iintpri, r7
	ld.bu   0[r7], r7
	shr     20, r6 ;/* r6 >> 20 */
	zxb     r6
	cmp     r7, r6
	bnl     ei_ret_exc_2
$endif ;/* !TOPPERS_ENABLE_EIP */
	;/*
	 ;*  ネスト回数が1以上なら割込み元がISRなため例外発生元へ復帰
	 ;*  スタックポインタの入れ替えしない
	 ;*/
	cmp     r0, r13
	bne     ei_ret_exc_1

	;/*
	 ;*  戻り先がタスクなためスタックを戻す
	 ;*/
	ld.w    0[sp], sp

ei_ret_exc_1:
	;/*
	 ;*  ISR1実行状態以外で発生した場合，OS割込み禁止状態の解除
	 ;*/
	Lea     _kernel_nested_lock_os_int_cnt, r12
	ld.b    0[r12], r13
	mov     -1, r14
	add     r14, r13
	st.b    r13, 0[r12]
	cmp     r0, r13
	bne     ei_ret_exc_2

	;/* OS割込みを解除 */
$ifndef __rh850v2__
	;/* pmr_setting_tbl[current_iintpri] -> PMR */
	Lea     _kernel_current_iintpri, r12
	ld.bu   0[r12], r12
	shl     1, r12

	Lea     _kernel_pmr_setting_tbl, r13
	add     r12, r13
	ld.h    0[r13], r13
	SET_PMR r13, r12             ;/* set interrupt level */
$else ;/* __rh850v2__ */
	Lea     _kernel_current_iintpri, r12
	ld.bu   0[r12], r12
	SET_PLMR r12, r6  ;/* current_iintpri -> PLMR */
$endif ;/* !__rh850v2__ */

	;/*
	 ;*  例外発生元へ復帰
	 ;*/
ei_ret_exc_2:
	;/*
	 ;* レジスタを復帰(EIPCとEIPSWも復帰)
	 ;*/
	EI_RESTOREREG

	eiret

	;/*
	 ;*  カーネル起動していない場合に起きたCPU例外の無限ループ
	 ;*/
infinity_loop:
	br infinity_loop

	;/*
	 ;*  割り込みエントリ
	 ;*  V850E2
	 ;*   ベクターテーブルから Os_Lcfg.c に生成される各割込みの割込みエントリ
	 ;*   からジャンプし，r2に割込み番号が保存された状態で実行される
	 ;*  V850E3V5
	 ;*   ベクターテーブルから直接実行されるため，スタックの確保やレジスタの保存は
	 ;*   特に行われていない状態で実行される
	 ;*/
	.extern _kernel_interrupt
_kernel_interrupt:
	;/* 割込み/例外の先頭で実行する処理 */
	PRE_INT_EXC

	;/* 
	 ;* レジスタの保存(EIPCとEIPSWも保存) 
	 ;*/
	EI_SAVEREG

$ifdef __rh850__
	;/* 割込み要因の特定 */
	stsr	eiic,   r12
	;/* EIC割込みは0x1000 から */
	addi   -0x1000, r12, r12
$else ;/* __v850e2v3__ */
	;/* 
	 ;* V850E2V3では，Os_Lcfg_asm.xxにより割込み要因がr2に設定され，
	 ;* _kernel_interrupt が呼び出される．
	 ;* r2 は割込みの許可により上書きされるためr12を使用する． 
	 ;*/
	mov     r2, r12
$endif ;/* __rh850__ */

	;/* 割込み優先度を取得(r11) */
$ifndef TOPPERS_ENABLE_EIP
	GET_ISPR r13
	sch1r    r13, r11   
	add      -1,r11
$else ;/* TOPPERS_ENABLE_EIP */
	stsr    psw, r11
	shr     20,  r11
	zxb     r11
$endif ;/* !TOPPERS_ENABLE_EIP */

	;/* 
	 ;*  _kernel_current_iintpri の更新 
	 ;*  割り込み処理中はISPRが自動的に設定されているため，PMRは設定する必要がない．
	 ;*/
	Lea     _kernel_current_iintpri, r6
	ld.bu   0[r6], r19
	st.b    r11,0[r6]

	;/*
	 ;*  多重割込みか判定
	 ;*/
	Lea     _kernel_except_nest_cnt, r13
	ld.w    0[r13], r6

	;/*
	 ;*  割込み・例外のネスト回数のインクリメント
	 ;*/
	add     1,  r6
	st.w    r6, 0[r13]
	cmp     1,  r6
	bne     interrupt_1     ;/* ネストしている場合はスタック切替無し */

	;/*
	 ;*  タスクに割込んだ場合
	 ;*  スタックポインタの保存と入れ替え
	 ;*/
	mov     sp , r13
	Lea     _kernel_ostkpt, sp
	ld.w    0[r3], r3
	add     -4, sp
	st.w    r13, 0[sp]

interrupt_1:
    ;/*
     ;*  割込み発生時の割込み優先度マスクをスタックに保存
     ;*/
    add     -4, sp
    st.w    r19, 0[sp]

	UNLOCK_ALL_INT

$ifdef CFG_USE_STACKMONITORING
	;/*
	 ;*  タスクスタックのオーバフローチェック
	 ;*/

	;/*
	 ;* 多重割込みの場合はタスクスタックのスタックモニタリングを行わない
	 ;*/
	cmp     1,  r6
	bne     int_nested

	;/*
	 ;*  スタックポインタチェック方式
	 ;*/
	Lea     _kernel_p_runtsk, r6                   ;/* 管理ブロックの先頭アドレス取得 */
	ld.w    0[r6], r6
	ld.w    TCB_p_tinib[r6], r6             ;/* タスク初期化ブロック先頭アドレス取得 */
	ld.w    TINIB_stk[r6], r6               ;/* タスクスタック先頭アドレス取得 */
	cmp     r6, r13                         ;/* スタックポインタ <= スタックの先頭の場合NG  */
	bnh      stack_monitoring_error_isr

	;/*
	 ;* マジックナンバーチェック方式
	 ;*/
    ld.w    0[r6], r6
    mov     STACK_MAGIC_NUMBER, r7
	;/*
	 ;*  マジックナンバー未破壊なら，割込み処理に飛ぶ
	 ;*  タスクに割込んだ場合，割込みスタックをチェックしない
	 ;*/
	cmp     r7, r6
	be      stack_monitoring_exit
	br      stack_monitoring_error_isr

	;/* 
	 ;* 多重割込みの場合 
	 ;*/
int_nested:
	;/*
	 ;*  割込みスタックのオーバフローチェック
	 ;*/

	;/*
	 ;*  スタック残量チェック方式
	 ;*/
	Lea     _kernel_isr_p_isrcb_tbl, r7
	mov     r12, r11
	shl     2, r11                          ;/* 割込み番号を4倍してオフセットを生成  */
	add     r11, r7
	ld.w    0[r7],r7                        ;/* 該当するisrcb_tableのアドレスを取得 */
	ld.w    ISRCB_p_intinib[r7], r7
	ld.w    INTINIB_remain_stksz[r7], r7    ;/* 割込み番号に対応したスタックサイズの取得 */
	Lea     _kernel_ostk, r6                       ;/* 割込みスタックの先頭アドレス取得 */
	ld.w    0[r6], r6
	add     r6, r7                          ;/* 先頭アドレス＋ISRの使用するスタックサイズ */
	cmp     r7, sp                          ;/* スタックポインタ <= 先頭アドレス＋ISRの使用するスタックサイズの場合NG  */
	bnh     stack_monitoring_error_isr

	;/*
	 ;* マジックナンバーチェック方式
	 ;*/
	ld.w    0[r6], r6                       ;/* このアドレスの示す先にマジックナンバーが入っている */
	mov     STACK_MAGIC_NUMBER, r7
	cmp     r7, r6
	bne     stack_monitoring_error_isr

stack_monitoring_exit:
$endif ;/* CFG_USE_STACKMONITORING */

	;/*
	 ;*  _kernel_callevel_stat 保存
	 ;*/
	Lea     _kernel_callevel_stat, r6
	ld.h    0[r6], r7
	Push    r7

	;/*
	 ;*  _kernel_callevel_stat 設定
	 ;*/
	mov     TCL_ISR2, r8
	or      r8, r7
	st.h    r7, 0[r6]

	;/*
	 ;* p_runisr 設定
	 ;*/
	Lea     _kernel_p_runisr, r6
	ld.w    0[r6], r7
	Push    r7

	;/*
	 ;* p_runisr 設定
	 ;*/
	Lea     _kernel_isr_p_isrcb_tbl, r7
	shl     2, r12                     ;/* 割込み番号を4倍してオフセットを生成  */
	add     r12, r7
	Lea     _kernel_p_runisr, r8
	ld.w    0[r7], r7
	st.w    r7, 0[r8]

	;/*
	 ;*  割込みハンドラのアドレスを読み込む
	 ;*/
	Lea     _kernel_isr_tbl, r6
	add     r12, r6

	;/*
	 ;*  割込みハンドラ呼び出し
	 ;*/
	Lea     end_int_handler, r31
	ld.w    0[r6], r6
	jmp     [r6]

end_int_handler:

$ifdef CFG_USE_STACKMONITORING
	;/*
	 ;*  割込みスタックのオーバフローチェック
	 ;*  割込みから戻った時，スタックポインタも戻ったはずなので，
	 ;*  マジックナンバーチェック方式のみ実施
	 ;*/

	;/*
	 ;* マジックナンバーチェック方式
	 ;*/
	Lea     _kernel_ostk, r6                       ;/* 割込みスタックの先頭アドレス取得 */
	ld.w    0[r6], r6
	ld.w    0[r6], r6                       ;/* このアドレスの示す先にマジックナンバーが入っている */
	mov     STACK_MAGIC_NUMBER, r7
	cmp     r7, r6
	bne     stack_monitoring_error_isr
$endif ;/* CFG_USE_STACKMONITORING */

	;/*
	 ;* ISR2の不正終了チェック
	 ;*/
	jarl    _kernel_exit_isr2, r31

	;/*
	 ;* p_runisr の復帰
	 ;*/
	Lea     _kernel_p_runisr, r7
	Pop     r6
	st.w    r6, 0[r7]

	;/*
	 ;* _kernel_callevel_stat の復帰
	 ;*/
	Lea     _kernel_callevel_stat, r7
	Pop     r6
	st.h    r6, 0[r7]

;/*
 ;*  割込みハンドラ出口処理
 ;*/
ret_int:
	;/*
	 ;* _except_nest_cntの操作とスタック切り替えの間はOS割込み禁止にする
	 ;* 必要がある
	 ;* この先，割込み先へのリターンか，遅延ディスパッチに分岐する．
	 ;* 割込み先へのリターンには全割込みが必要であり，遅延ディスパッチ時には
	 ;* ネスト管理のOS割込み禁止にする必要があるため，ここでは一旦全割込み禁
	 ;* 止とする
	 ;*/

	LOCK_ALL_INT

	;/*
	 ;*  割込み発生時の割込み優先度マスクをスタックから復帰
	 ;*/
	ld.w    0[sp], r19
	add     4, sp
	Lea     _kernel_current_iintpri, r6
	st.b    r19, 0[r6]              ;/* current_iintpriを復帰 */

	;/*
	 ;*  割込み・例外のネスト回数のデクリメント
	 ;*/
	Lea     _kernel_except_nest_cnt, r6
	ld.w    0[r6], r7
	add     -1, r7
	st.w    r7, 0[r6]

	;/*
	 ;*  ネスト回数が1以上なら割込み元のISR2へ戻る
	 ;*/
	cmp     r0, r7
	bne     ret_int_1

	;/*
	 ;*  割込み元がタスクの場合
	 ;*/
ret_int_task:
	ld.w    0[sp], sp                       ;/* スタックポインタを戻す     */

	;/* 割込み優先度マスクを変えるAPIがないためATK2では割込み優先度マスクの復帰処理は不要 */

	;/*
	 ;*  割込みハンドラ実行時には，ISPRで優先度が自動的に上がるため，
	 ;*  PMRは変更していないが，割り込みハンドラ内で x_nested_unlock_os_int() を
	 ;*  呼び出すとPMRが書き換わる．ディスパッチが必要でない場合は，ret_int_1で
	 ;*  PMRを元の値に戻す．ディスパッチが必要な場合は，OS割り込み禁止とするため，
	 ;*  ここで，PMRを更新する必要はない．
	 ;*/

	;/*
	 ;*  戻り先がタスクの場合，ディスパッチの必要があるかチェック
	 ;*/
	Lea     _kernel_p_runtsk, r6
	ld.w    0[r6], r6
	Lea     _kernel_p_schedtsk, r7
	ld.w    0[r7], r7
	cmp     r6, r7
	be      ret_int_1      ;/* ディスパッチの必要がなければ割込み元へ戻る */

	;/*
	 ;*  ここでは，戻り先がタスクであり，スタックは，タスクスタックに
	 ;*  スクラッチレジスタのみが保存された状態になっている．また，
	 ;*  全割込みを禁止した状態となっている
	 ;*/

	;/* 
	 ;*  優先度マスクのクリア 
	 ;*  eiretを実行しないため，eiretによる割込みコントローラの優先度マスク
	 ;*  (インサービスプライオリティ)のクリアが行われないため，代わりにISPC
	 ;*  を用いてクリアする．
	 ;*  
	 ;*  G3M/G3KではINTCFG/INSPRの更新はdi状態で呼び出す必要がある．
	 ;*  ここは全割込み禁止状態で呼び出されるため問題ない．
	 ;*/
$ifndef TOPPERS_ENABLE_EIP
	CLEAR_ISPR r10, r12  
$else ;/* TOPPERS_ENABLE_EIP */
	stsr    psw, r10
	movhi   HIGHW(0xff00000),zero,r12
	or      r12, r10
	ldsr    r10, psw
$endif ;/* !TOPPERS_ENABLE_EIP */
	;/*
	 ;*  OS割込み禁止状態に移行する
	 ;*
	 ;*  この時点でOS割込み禁止状態とするのは，dispatcherを実行する際に
	 ;*  OS割込み禁止状態になっている必要があるためである
	 ;*
	 ;*  (モデル上の)割込み優先度マスクは0であるため，ISR1以外の割込みは全て禁止する
	 ;*/

	OUTER_LOCK_OS_INT r12, r13
	;/* ret_int で全割込み禁止状態としたため解除 */
	UNLOCK_ALL_INT

	;/*
	 ;* コンテキストの退避 
	 ;*/
$ifndef TOPPERS_USE_PUSHSP
	addi    -40, sp, sp
	mov     sp, ep
	sst.w   r20, 36[ep]
	sst.w   r21, 32[ep]
	sst.w   r22, 28[ep]
	sst.w   r23, 24[ep]
	sst.w   r24, 20[ep]
	sst.w   r25, 16[ep]
	sst.w   r26, 12[ep]
	sst.w   r27, 8[ep]
	sst.w   r28, 4[ep]
	sst.w   r29, 0[ep]
$else ;/* TOPPERS_USE_PUSHSP */
	pushsp  r20-r29
$endif ;/* TOPPERS_USE_PUSHSP */

	;/* スタックと次の起動番地を保存 */
	Lea     _kernel_p_runtsk, ep
	sld.w   0[ep], ep
	sst.w   sp, TCB_sp[ep]
	Lea     ret_int_r, r6
	sst.w   r6, TCB_pc[ep]
	br      dispatcher

	;/*
	 ;*  割込み・例外でコンテキスト保存した場合の復帰ルーチン
	 ;*/
ret_int_r:
	;/*
	 ;* コンテキストの復帰
	 ;*/
$ifndef TOPPERS_USE_PUSHSP
	mov     sp, ep
	sld.w   36[ep], r20
	sld.w   32[ep], r21
	sld.w   28[ep], r22
	sld.w   24[ep], r23
	sld.w   20[ep], r24
	sld.w   16[ep], r25
	sld.w   12[ep], r26
	sld.w   8[ep], r27
	sld.w   4[ep], r28
	sld.w   0[ep], r29
	addi    40, ep, sp
$else ;/* TOPPERS_USE_PUSHSP */
	popsp   r20-r29
$endif ;/* TOPPERS_USE_PUSHSP */

	LOCK_ALL_INT

	;/*
	 ;*  割込みからの復帰ルーチン
	 ;*/
ret_int_1:
	;/*
	 ;* レジスタを復帰
	 ;*/
	mov     sp, ep

	;/* 割込み優先度マスクを変えるAPIがないためATK2では割込み優先度マスクの復帰不要 */

	;/*
	 ;*  PMRを元に戻す．
	 ;*  割込みハンドラ実行時には，ISPRの優先度が自動的に上がるため，
	 ;*  PMRは変更していないが，割り込みハンドラ内で x_nested_unlock_os_int() を
	 ;*  呼び出すとPMRが書き換わるためここで元の値に戻す．
	 ;*  OUTER_UNLOCK_OS_INT は nested_lock_os_int_cnt を0にするが，
	 ;*  ここでは既に0であるため問題ない．
	 ;*/
	OUTER_UNLOCK_OS_INT  r7, r6

	;/*
	 ;* レジスタを復帰(EIPCとEIPSWも復帰)
	 ;*/
	EI_RESTOREREG

	eiret


;/*
 ;* タスクディスパッチ
 ;*/
	.extern _kernel_dispatch
_kernel_dispatch:
	;/*
	 ;*  このルーチンは，タスクコンテキスト・OS割込み禁止状態・ディスパッチ
	 ;*  許可状態・（モデル上の）割込み優先度マスク全解除状態で呼び出さ
	 ;*  れる
	 ;*/
$ifndef TOPPERS_USE_PUSHSP
	addi    -48, sp, sp
	st.w    r30,  4[sp] ;/* r30(ep) */
	mov     sp,  ep
	sst.w   r20, 44[ep]
	sst.w   r21, 40[ep]
	sst.w   r22, 36[ep]
	sst.w   r23, 32[ep]
	sst.w   r24, 28[ep]
	sst.w   r25, 24[ep]
	sst.w   r26, 20[ep]
	sst.w   r27, 16[ep]
	sst.w   r28, 12[ep]
	sst.w   r29, 8[ep]
	sst.w   r31, 0[ep]
$else ;/* TOPPERS_USE_PUSHSP */
	pushsp  r20-r31
$endif ;/* TOPPERS_USE_PUSHSP */

	;/* スタックと次の起動番地を保存 */
	Lea     _kernel_p_runtsk, ep
	sld.w   0[ep], ep
	sst.w   sp, TCB_sp[ep]
	Lea     dispatch_r, r10
	sst.w   r10, TCB_pc[ep]
$ifdef CFG_USE_STACKMONITORING
	;/*
	 ;*  実行中タスクのタスクスタックのオーバフローチェック
	 ;*/
	;/*
	 ;*  スタックポインタチェック方式
	 ;*/
	Lea     _kernel_p_runtsk, r6                   ;/* 管理ブロックの先頭アドレス取得 */
	ld.w    0[r6], r6
	ld.w    TCB_p_tinib[r6], r6             ;/* タスク初期化ブロック先頭アドレス取得 */
	ld.w    TINIB_stk[r6], r6               ;/* タスクスタック先頭アドレス取得 */
	cmp     r6, sp                          ;/* スタックポインタ <= スタックの先頭の場合NG  */
	bnh      stack_monitoring_error
	;/*
	 ;* マジックナンバーチェック方式
	 ;*/
	ld.w    0[r6], r6
	mov     STACK_MAGIC_NUMBER, r7
	cmp     r7, r6
	bne     stack_monitoring_error
$endif ;/* CFG_USE_STACKMONITORING */
	br      dispatcher

	;/* 
	 ;* ディスパッチャの復帰ルーチン 
	 ;*/
dispatch_r:
$ifndef TOPPERS_USE_PUSHSP
	mov     sp, ep
	sld.w   44[ep], r20
	sld.w   40[ep], r21
	sld.w   36[ep], r22
	sld.w   32[ep], r23
	sld.w   28[ep], r24
	sld.w   24[ep], r25
	sld.w   20[ep], r26
	sld.w   16[ep], r27
	sld.w   12[ep], r28
	sld.w    8[ep], r29
	sld.w    0[ep], r31
	addi    48, ep, sp
	ld.w     4[ep], r30  ;/* r30(ep) */
$else ;/* TOPPERS_USE_PUSHSP */
	popsp  r20-r31
$endif ;/* TOPPERS_USE_PUSHSP */
	jmp     [lp]

;/*
 ;*  ディスパッチャの動作開始
 ;*/
	.extern _kernel_start_dispatch
_kernel_start_dispatch:
	;/*
	 ;*  このルーチンは，カーネル起動時に，すべての割込みを禁止した状態
	 ;* （全割込み禁止状態と同等）で呼び出される．また，割込みモード（非
	 ;*  タスクコンテキストと同等）で呼び出されることを想定している
	 ;*
	 ;*  dispatcherは，OS割込み禁止状態で呼び出す．
	 ;*/
	;/* OS割込み禁止状態 */
	OUTER_LOCK_OS_INT r12, r13
	;/* 全割込み禁止解除状態へ */
	UNLOCK_ALL_INT
	br    dispatcher_0

	;/*
	 ;*  現在のコンテキストを捨ててディスパッチ
	 ;*/
	.extern _kernel_exit_and_dispatch
_kernel_exit_and_dispatch:
$ifdef CFG_USE_STACKMONITORING
	;/*
	 ;*  実行中のタスクスタックのオーバフローチェック
	 ;*/
	;/*
	 ;*  スタックポインタチェック方式
	 ;*/
	Lea     _kernel_p_runtsk, r6                   ;/* 管理ブロックの先頭アドレス取得 */
	ld.w    0[r6], r6
	ld.w    TCB_p_tinib[r6], r6             ;/* タスク初期化ブロック先頭アドレス取得 */
	ld.w    TINIB_stk[r6], r6               ;/* タスクスタック先頭アドレス取得 */
	cmp     r6, sp                          ;/* スタックポインタ <= スタックの先頭の場合NG  */
	bnh      stack_monitoring_error
	;/*
	 ;* マジックナンバーチェック方式
	 ;*/
	ld.w    0[r6], r6
	mov     STACK_MAGIC_NUMBER, r7
	cmp     r7, r6
	bne     stack_monitoring_error
$endif ;/* CFG_USE_STACKMONITORING */
	;/* ディスパッチャ本体（dispatcher）へ */

dispatcher:
	;/*
	 ;*  PostTaskHookの呼び出し
	 ;*/
$ifdef CFG_USE_POSTTASKHOOK
	Lea     _PostTaskHook, r20
	cmp     r20, r0
	be      dispatcher_0
posttaskhook_call:
	Lea     _kernel_call_posttaskhook, r6
	jarl    _kernel_stack_change_and_call_func_1, r31
$endif ;/* CFG_USE_POSTTASKHOOK */

dispatcher_0:
	;/*
	 ;*  このルーチンは，タスクコンテキスト・OS割込み禁止状態・ディスパッチ
	 ;*  許可状態・（モデル上の）割込み優先度マスク全解除状態で呼び出さ
	 ;*  れる．実行再開番地へもこの状態のまま分岐する
	 ;*/
$ifdef LOG_DSP_ENTER
	Lea     _kernel_p_runtsk, r21
	ld.w    0[r21], r21
	cmp     r0, r21
	be      dispatcher_0_1
	mov     r21, r6                            ;/* 引数に_p_runtskを指定 */
	jarl    _log_dsp_enter, r31
dispatcher_0_1:
$endif	;/* LOG_DSP_ENTER */

	Lea     _kernel_callevel_stat, r21
	ld.h    0[r21], r22
	mov     TSYS_DISALLINT, r20
	and     r22, r20
	be      dispatcher_1

	mov     !TSYS_DISALLINT, r20
	and     r22, r20
	st.h    r20, 0[r21]
	UNLOCK_ALL_INT                             ;/* 全割込み禁止解除状態へ */

	;/*
	 ;*  例外（割込み/CPU例外）のネスト回数はstart_dispatchや，dispatcher_3:
	 ;*  から来た場合は'1'となっているため，タスクスタックに切り換え後，
	 ;*  '0'に初期化する．
	 ;*/
dispatcher_1:
	Lea     _kernel_p_runtsk, r21
	Lea     _kernel_p_schedtsk, r20
	ld.w    0[r20], r22
	st.w    r22, 0[r21]
	cmp     r22, r0
	be      dispatcher_3

	mov     r22, ep 
	sld.w   TCB_sp[ep], sp
	Lea     _kernel_except_nest_cnt, r20
	st.w    r0, 0[r20]

$ifdef LOG_DSP_LEAVE
	ld.w    0[r21], r6                          ;/* 引数に_p_runtskを指定 */
	jarl    _log_dsp_leave, r31
$endif	;/* LOG_DSP_LEAVE */

$ifdef CFG_USE_PRETASKHOOK
	Lea     _PreTaskHook, r20
	cmp     r20, r0
	be      dispatcher_2

pretaskhook_call:
	Lea     _kernel_call_pretaskhook, r6
	jarl    _kernel_stack_change_and_call_func_1, r31
$endif ;/* CFG_USE_PRETASKHOOK */

dispatcher_2:
	Lea     _kernel_p_runtsk, r20
	ld.w    0[r20], r22
	mov     r22, ep
	sld.w   TCB_pc[ep], r31
	jmp     [r31]

dispatcher_3:
	;/*
	 ;* OS割込み禁止状態の解除と，非タスクコンテキスト実行状態への
	 ;* 準備をする
	 ;*
	 ;*  ここで非タスクコンテキストに切り替える（sp＝非タスクコンテキス
	 ;*  ト用スタックポインタ，except_nest_cnt＝1)のは，OS割込み禁止解
	 ;*  除後に発生する割込み処理にどのスタックを使うかという問題の解決
	 ;*  と，割込みハンドラ内でのタスクディスパッチの防止という2つの意
	 ;*  味がある
	 ;*/
	Lea     _kernel_ostkpt, r20		;/* 非タスクコンテキスト用のスタックの読み込みスタックポインタの入れ替え */
	ld.w    0[r20], r3

	Lea     _kernel_except_nest_cnt, r20
	mov     1, r21
	st.w    r21, 0[r20]

	;/*
	 ;*  OS割込み禁止解除状態へ
	 ;*  割込みを許可し，非タスクコンテキスト実行状態とし割込みを待つ
	 ;*
	 ;*  プロセッサを割込み待ちに移行させる処理と，割込み許可とは，不可
	 ;*  分に行なう必要がある
	 ;*  これを不可分に行なわない場合，割込みを許可した直後に割込
	 ;*  みが入り，その中でタスクが実行可能状態になると，実行すべきタス
	 ;*  クがあるにもかかわらずプロセッサが割込み待ちになってしまう．
	 ;*  不可分に行うため，diによりコアでの割り込みを禁止した状態で，
	 ;*  INTCの割り込み優先度マスクを下げてからhaltを呼び出す．
	 ;*  割り込みが発生した場合は，haltから復帰して次のeiでベクタに
	 ;*  ジャンプする．
	 ;*
	 ;*  割込み待ちの間は，p_runtskをNULL（＝0）に設定しなければならない
	 ;*  このように設定しないと，割込みハンドラからGetTaskIDを呼び出
	 ;*  した際の動作が仕様に合致しなくなる
	 ;*/

	di
	OUTER_UNLOCK_OS_INT r12, r13
	halt
	ei

	;/*
	 ;*  OS割込み禁止状態へ
	 ;*  (モデル上の)割込み優先度マスクは0であるため，ISR1以外の割込みは全て禁止する
	 ;*  スタックは非タスクコンテキストのスタックを使用しているため，_except_nest_cntは，
	 ;*  '1'のままとする
	 ;*/
	OUTER_LOCK_OS_INT r12, r13
	br      dispatcher_1

	;/*
	 ;*  スタックオーバフロー時の処理
	 ;*/
$ifdef CFG_USE_STACKMONITORING
	;/*
	 ;*  スタックオーバフロー時プロテクションフックを呼び出し
	 ;*  スタックを更に壊さないため，割込みスタックの初期値を使用する
	 ;*/
stack_monitoring_error_isr:
	OUTER_LOCK_OS_INT r12, r13                  ;/* ISRから来た場合はOS割込み禁止になっていない */

stack_monitoring_error:
	Lea     _kernel_except_nest_cnt, r12               ;/* ostkptを使う場合はexcept_nest_cntをインクリメントする */
	ld.w    0[r12], r13
	add     1, r13
	st.w    r13, 0[r12]

	Lea     _kernel_ostkpt, r12                        ;/* スタックを更に壊さないため，割込みスタックの初期値を使用する */
	ld.w    0[r12], r12
	mov     r12, sp

	mov     E_OS_STACKFAULT, r6                 ;/* プロテクションフックの引数を設定 */
	jarl    _kernel_call_protectionhk_main, r31        ;/* プロテクションフックを呼び出し */
	;/*
	 ;* プロテクションフックからは戻ってこない
	 ;*/
	halt                                        ;/* ここには来ないはず */
$endif ;/* CFG_USE_STACKMONITORING */

;/*
 ;*  タスク開始時処理
 ;*
 ;*  dispatcherから呼び出される
 ;*/
	.extern _kernel_start_r
_kernel_start_r:
	;/*
	 ;*  OS割込み禁止解除前に現在優先度(TCB.curpri)を実行優先度(TINIB.exepri)に
	 ;*  変更(内部リソースを獲得)
	 ;*/
	Lea     _kernel_p_runtsk, ep
	ld.w    0[ep], ep
	ld.w    TCB_p_tinib[ep], r22
	ld.b    TINIB_exepri[r22], r21			;/* TINIB.exepri -> TCB.curpri */
	st.b    r21, TCB_curpri[ep]

	;/*
	 ;*  OS割込み禁止解除状態へ
	 ;*  (モデル上の)割込み優先度マスクは0であるため，有効な割込みは全て許可する
	 ;*/

	OUTER_UNLOCK_OS_INT r12, r13

	;/*
	 ;*  タスクが不正終了した場合は保護処理を行うよう
	 ;*  戻りアドレスにexit_taskを設定
	 ;*/
	Lea     _kernel_exit_task, r31

	ld.w    TCB_p_tinib[ep], r22
	ld.w    TINIB_task[r22], r10
	jmp     [r10]

	.extern _kernel_stack_change_and_call_func_1 
	.extern _kernel_stack_change_and_call_func_2
_kernel_stack_change_and_call_func_1:
_kernel_stack_change_and_call_func_2:

	push    lp
	Lea     _kernel_except_nest_cnt, r12
	ld.w    0[r12], r13
	add     1, r13
	st.w    r13, 0[r12]
	cmp     1, r13
	bne     _stack_change_and_call_func_1_1

	;/* スタックポインタの保存と入れ替え */
	Lea     _kernel_ostkpt, r12
	ld.w    0[r12], r12
	add     -4, r12
	st.w    sp, 0[r12]                      ;/* スタックポインタの保存 */
	mov     r12, sp                         ;/* スタックポインタの切り替え */
    
_stack_change_and_call_func_1_1:
	Lea     stack_change_and_call_func_r, lp  ;/* 戻りアドレス */
	mov     r6, r12
	mov     r7, r6
	mov     r8, r7
	jmp     [r12]

stack_change_and_call_func_r:
	Lea     _kernel_except_nest_cnt, r12
	ld.w    0[r12], r13
	add     -1, r13
	st.w    r13, 0[r12]
	cmp     r0, r13
	bne     stack_change_and_call_func_r_1

	ld.w    0[sp], sp                    ;/* スタックポインタの復帰     */
stack_change_and_call_func_r_1:
	pop     lp
	jmp     [lp]


$ifdef ENABLE_RETURN_MAIN
;/*
 ;*  AKTSP用のmain()へのリターンコード
 ;*/
	.extern _kernel_return_main
_kernel_return_main:
	Lea   _kernel_ostkpt, sp
	ld.w  0[sp],sp
	jarl  _main, r31
$endif ;/* ENABLE_RETURN_MAIN */
