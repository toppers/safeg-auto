/*
 *  TOPPERS ATK2
 *      Toyohashi Open Platform for Embedded Real-Time Systems
 *      Automotive Kernel Version 2
 *
 *  Copyright (C) 2012-2017 by Center for Embedded Computing Systems
 *              Graduate School of Information Science, Nagoya Univ., JAPAN
 *  Copyright (C) 2012-2014 by FUJI SOFT INCORPORATED, JAPAN
 *  Copyright (C) 2012-2013 by Spansion LLC, USA
 *  Copyright (C) 2012-2013 by NEC Communication Systems, Ltd., JAPAN
 *  Copyright (C) 2012-2014 by Panasonic Advanced Technology Development Co., Ltd., JAPAN
 *  Copyright (C) 2012-2014 by Renesas Electronics Corporation, JAPAN
 *  Copyright (C) 2012-2014 by Sunny Giken Inc., JAPAN
 *  Copyright (C) 2012-2014 by TOSHIBA CORPORATION, JAPAN
 *  Copyright (C) 2012-2014 by Witz Corporation, JAPAN
 *
 *  上記著作権者は，以下の(1)～(4)の条件を満たす場合に限り，本ソフトウェ
 *  ア（本ソフトウェアを改変したものを含む．以下同じ）を使用・複製・改
 *  変・再配布（以下，利用と呼ぶ）することを無償で許諾する．
 *  (1) 本ソフトウェアをソースコードの形で利用する場合には，上記の著作
 *      権表示，この利用条件および下記の無保証規定が，そのままの形でソー
 *      スコード中に含まれていること．
 *  (2) 本ソフトウェアを，ライブラリ形式など，他のソフトウェア開発に使
 *      用できる形で再配布する場合には，再配布に伴うドキュメント（利用
 *      者マニュアルなど）に，上記の著作権表示，この利用条件および下記
 *      の無保証規定を掲載すること．
 *  (3) 本ソフトウェアを，機器に組み込むなど，他のソフトウェア開発に使
 *      用できない形で再配布する場合には，次のいずれかの条件を満たすこ
 *      と．
 *    (a) 再配布に伴うドキュメント（利用者マニュアルなど）に，上記の著
 *        作権表示，この利用条件および下記の無保証規定を掲載すること．
 *    (b) 再配布の形態を，別に定める方法によって，TOPPERSプロジェクトに
 *        報告すること．
 *  (4) 本ソフトウェアの利用により直接的または間接的に生じるいかなる損
 *      害からも，上記著作権者およびTOPPERSプロジェクトを免責すること．
 *      また，本ソフトウェアのユーザまたはエンドユーザからのいかなる理
 *      由に基づく請求からも，上記著作権者およびTOPPERSプロジェクトを
 *      免責すること．
 *
 *  本ソフトウェアは，AUTOSAR（AUTomotive Open System ARchitecture）仕
 *  様に基づいている．上記の許諾は，AUTOSARの知的財産権を許諾するもので
 *  はない．AUTOSARは，AUTOSAR仕様に基づいたソフトウェアを商用目的で利
 *  用する者に対して，AUTOSARパートナーになることを求めている．
 *
 *  本ソフトウェアは，無保証で提供されているものである．上記著作権者お
 *  よびTOPPERSプロジェクトは，本ソフトウェアに関して，特定の使用目的
 *  に対する適合性も含めて，いかなる保証も行わない．また，本ソフトウェ
 *  アの利用により直接的または間接的に生じたいかなる損害に関しても，そ
 *  の責任を負わない．
 *
 *  $Id: prc_config.h 832 2017-09-23 02:18:28Z ertl-honda $
 */

/*
 *		プロセッサ依存モジュール（V850用）
 *
 *  このインクルードファイルは，target_config.h（または，そこからインク
 *  ルードされるファイル）のみからインクルードされる．他のファイルから
 *  直接インクルードしてはならない
 */

#ifndef TOPPERS_PRC_CONFIG_H
#define TOPPERS_PRC_CONFIG_H

#include "prc_sil.h"

/*
 *  エラーチェック方法の指定
 */
#ifndef CHECK_STKSZ_ALIGN
#define CHECK_STKSZ_ALIGN	4   /* スタックサイズのアライン単位 */
#endif /* CHECK_STKSZ_ALIGN */

#ifndef CHECK_FUNC_ALIGN
#define CHECK_FUNC_ALIGN	2   /* 関数のアライン単位 */
#endif /* CHECK_FUNC_ALIGN */

#define CHECK_FUNC_NONNULL      /* 関数の非NULLチェック */

#ifndef CHECK_STACK_ALIGN
#define CHECK_STACK_ALIGN	4   /* スタック領域のアライン単位 */
#endif /* CHECK_STACK_ALIGN */

#define CHECK_STACK_NONNULL     /* スタック領域の非NULLチェック */

#define call_errorhook(ercd, svcid)		stack_change_and_call_func_2((void *) &internal_call_errorhook, (ercd), (svcid))
#define call_shutdownhook(ercd)			stack_change_and_call_func_1((void *) &internal_call_shtdwnhk, ((uint32) (ercd)))

/*
 *  割込み番号の範囲の判定
 */
#if TMIN_INTNO == 0
#define VALID_INTNO(intno)	((intno) <= (InterruptNumberType) (TMAX_INTNO))
#else /* TMIN_INTNO != 0 */
#define VALID_INTNO(intno)	(((InterruptNumberType) (TMIN_INTNO) <= (intno)) && ((intno) <= (InterruptNumberType) (TMAX_INTNO)))
#endif /* TMIN_INTNO == 0 */

#ifndef TOPPERS_MACRO_ONLY

/*
 *  非タスクコンテキスト用のスタック開始アドレス設定マクロ
 */
#define TOPPERS_OSTKPT(stk, stksz)	((StackType *) ((sint8 *) (stk) + (stksz)))

/*
 *  プロセッサの特殊命令のインライン関数定義
 */
#include <prc_insn.h>

/*
 *  TOPPERS標準割込み処理モデルの実現
 */

/*
 * 例外（割込み/CPU例外）のネスト回数のカウント
 *
 * コンテキスト参照のために使用
 */
extern uint32		except_nest_cnt;

/*
 *  カーネル管理外（カテゴリ1）の割込みとなる優先度のPMRのビット
 */
extern const uint16	pmr_isr1_mask;

/*
 *  カーネル管理内（カテゴリ2）の割込みを禁止するためのPMRの設定値
 */
extern const uint16	pmr_isr2_mask;


#ifdef __rh850v2__

/*
 *  カーネル管理内（カテゴリ2）の最高優先度（内部表現）
 */
extern const uint8	isr2max_iintpri;

#endif /* __rh850v2__ */

/*
 *  現在の割込み優先度の値（内部表現）
 */
extern uint8		current_iintpri;

#ifndef __rh850v2__
/*
 * 割り込み要求マスクテーブル（PMRへの設定値）
 */
extern const uint16	pmr_setting_tbl[];
#endif /* !__rh850v2__ */

/*
 *  無限ループ処理
 */
extern void infinite_loop(void) NoReturn;

/*
 *  割込み優先度の内部表現と外部表現の変更
 */
#ifndef TOPPERS_MACRO_ONLY
#define EXT_IPM(iipm)	((PriorityType) (iipm - (TNUM_INTPRI)))     /* 内部表現を外部表現に */
#define INT_IPM(ipm)	((uint32) (ipm + (TNUM_INTPRI)))            /* 外部表現を内部表現に */
#else /* TOPPERS_MACRO_ONLY */
#define EXT_IPM(iipm)	(iipm - (TNUM_INTPRI))              /* 内部表現を外部表現に */
#define INT_IPM(ipm)	(ipm + (TNUM_INTPRI))               /* 外部表現を内部表現に */
#endif /* TOPPERS_MACRO_ONLY */

/*
 *  全割込み禁止状態への移行
 */
LOCAL_INLINE void
x_lock_all_int(void)
{
	disable_int();
}

/*
 *  全割込み禁止状態の解除
 */
LOCAL_INLINE void
x_unlock_all_int(void)
{
	enable_int();
}

/*
 *  x_nested_lock_os_int()のネスト回数
 *  アクセスの順序が変化しないよう，volatile を指定
 */
extern volatile uint8 nested_lock_os_int_cnt;

/*
 *  OS割込み禁止
 *  API実行時に呼び出される
 *  割込み優先度マスクがISR2の優先度マスクの範囲より高い状態で呼び出される
 *  ことはない(ISR1から呼び出されることはない)
 */
LOCAL_INLINE void
x_nested_lock_os_int(void)
{
	/*
	 *  一段目なら割込みの禁止レベルに設定割込み優先度マスクの値を保存
	 */
	if (nested_lock_os_int_cnt == 0U) {
#ifdef __rh850v2__
		set_plmr(isr2max_iintpri);  /* OS割り込み禁止に設定 */
#else /* !__rh850v2__ */
		set_pmr(pmr_isr2_mask);  /* OS割り込み禁止に設定 */
#endif /* __rh850v2__ */
		
	}
	nested_lock_os_int_cnt++;

	V850_MEMORY_CHANGED
}

/*
 *  OS割込み解除
 *  API実行時に呼び出される
 *  割込み優先度マスクがISR2の優先度マスクの範囲より高い状態で呼び出される
 *  ことはない(ISR1から呼び出されることはない)
 */
LOCAL_INLINE void
x_nested_unlock_os_int(void)
{
	V850_MEMORY_CHANGED
	ASSERT(nested_lock_os_int_cnt > 0U);
	nested_lock_os_int_cnt--;

	/*
	 *  一番外側なら割込み優先度マスクを更新
	 */
	if (nested_lock_os_int_cnt == 0U) {
#ifdef __rh850v2__
		set_plmr(current_iintpri);
#else /* !__rh850v2__ */
		set_pmr(pmr_setting_tbl[current_iintpri]);
#endif /* __rh850v2__ */
	}
}

/*
 *  (モデル上の)割込み優先度マスクの設定
 *
 *  本OSでは次の二点が成り立つ
 *    * OS割込み禁止状態で呼び出される
 *    * OS割込み禁止時の優先度より高い値は指定されない
 *  OS割込み禁止はPMRで実現されており，ここでPMRを変更すると，OS割込み禁止
 *  が解除される場合があるため，内部変数(current_iintpri) の変更のみを行う．
 *  優先度マスクの変更は，APIの出口で呼び出される，x_nested_unlock_os_int()
 *  で実施される．
 */
LOCAL_INLINE void
x_set_ipm(PriorityType intpri)
{
	ASSERT(nested_lock_os_int_cnt > 0U);

	current_iintpri = INT_IPM(intpri);
}

/*
 *  (モデル上の)割込み優先度マスクの参照
 *
 *  本OSでは次の点が成り立つ
 *    * OS割込み禁止状態で呼び出される
 */
LOCAL_INLINE PriorityType
x_get_ipm(void)
{
	ASSERT(nested_lock_os_int_cnt > 0U);
	return(EXT_IPM(current_iintpri));
}

/*
 *  割込み要求禁止フラグのセット
 *
 *  割込み属性が設定されていない割込み要求ラインに対して割込み要求禁止
 *  フラグをクリアしようとした場合には，falseを返す．
 */
LOCAL_INLINE boolean
x_disable_int(InterruptNumberType intno)
{
#ifdef TOPPERS_ENABLE_EIP
	uint32 eeic_address = EEIC_ADDRESS(intno);
#else /* !TOPPERS_ENABLE_EIP */
	uint32 eic_address = EIC_ADDRESS(intno);
#endif /* TOPPERS_ENABLE_EIP */

	if (!VALID_INTNO(intno)) {
		return(FALSE);
	}

#ifdef TOPPERS_ENABLE_EIP
	/* 割込みの禁止(23bit目をセット) */
	sil_wrw_mem((void *) eeic_address,
				sil_rew_mem((void *) eeic_address) | (0x01U << 23));
#else /* !TOPPERS_ENABLE_EIP */
	/* 割込みの禁止(7bit目をセット) */
	sil_wrh_mem((void *) eic_address,
				sil_reh_mem((void *) eic_address) | (0x01U << 7));
#endif /* TOPPERS_ENABLE_EIP */

	return(TRUE);
}
/*
 *  割込み要求禁止フラグの解除
 *
 *  割込み属性が設定されていない割込み要求ラインに対して割込み要求禁止
 *  フラグをクリアしようとした場合には，falseを返す．
 */

LOCAL_INLINE boolean
x_enable_int(InterruptNumberType intno)
{
#ifdef TOPPERS_ENABLE_EIP
	uint32 eeic_address = EEIC_ADDRESS(intno);
#else /* !TOPPERS_ENABLE_EIP */
	uint32 eic_address = EIC_ADDRESS(intno);
#endif /* TOPPERS_ENABLE_EIP */

	if (!VALID_INTNO(intno)) {
		return(FALSE);
	}

#ifdef TOPPERS_ENABLE_EIP
	/* 23bit目をクリア */
	sil_wrw_mem((void *) eeic_address,
				sil_rew_mem((void *) eeic_address) & ~(0x01U << 23));
#else /* !TOPPERS_ENABLE_EIP */
	/* 7bit目をクリア */
	sil_wrh_mem((void *) eic_address,
				sil_reh_mem((void *) eic_address) & ~(0x01U << 7));
#endif /* TOPPERS_ENABLE_EIP */

	return(TRUE);
}

/*
 *  割込み要求のクリア
 */
LOCAL_INLINE boolean
x_clear_int(InterruptNumberType intno)
{
#ifdef TOPPERS_ENABLE_EIP
	uint32 eeic_address = EEIC_ADDRESS(intno);
#else /* !TOPPERS_ENABLE_EIP */
	uint32 eic_address = EIC_ADDRESS(intno);
#endif /* TOPPERS_ENABLE_EIP */

	if (!VALID_INTNO(intno)) {
		return(FALSE);
	}

#ifdef TOPPERS_ENABLE_EIP
	sil_wrw_mem((void *) eeic_address,
				sil_rew_mem((void *) eeic_address) & ~(0x01U << 28));
#else /* !TOPPERS_ENABLE_EIP */
	/* 割込みのクリア(12bit目をクリア) */
	sil_wrh_mem((void *) eic_address,
				sil_reh_mem((void *) eic_address) & ~(0x01U << 12));
#endif /* TOPPERS_ENABLE_EIP */

	return(TRUE);
}

/*
 *  割込み要求のチェック
 */
LOCAL_INLINE boolean
x_probe_int(InterruptNumberType intno)
{
	uint32	eic_address = EIC_ADDRESS(intno);
	uint16	tmp;

	if (!VALID_INTNO(intno)) {
		return(FALSE);
	}

	tmp = sil_reh_mem((void *) (eic_address));
	return((tmp & 0x1000) != 0);
}

/*
 *  割込み要求ラインの属性の設定
 */
extern void x_config_int(InterruptNumberType intno, AttributeType intatr, PriorityType intpri);

/*
 *  割込みハンドラの入り口で必要なIRC操作
 */
LOCAL_INLINE void
i_begin_int(InterruptNumberType intno)
{

}

/*
 *  割込みハンドラの出口で必要なIRC操作
 */
LOCAL_INLINE void
i_end_int(InterruptNumberType intno)
{

}

/*
 *  未定義の割込みが入った場合の処理
 */
extern void default_int_handler(void);

/*
 *  起動時のハードウェアの初期化
 */
extern void prc_hardware_initialize(void);

/*
 *  プロセッサ依存の初期化
 */
extern void prc_initialize(void);

/*
 *  プロセッサ依存の終了時処理
 */
extern void prc_terminate(void);

/*
 *  タスクディスパッチャ
 */

/*
 *  最高優先順位タスクへのディスパッチ（prc_support.S）
 *
 *  dispatch は，タスクコンテキストから呼び出されたサービスコール処理
 *  内で，OS割込み禁止状態で呼び出さなければならない
 */
extern void dispatch(void);

/*
 *  ディスパッチャの動作開始（prc_support.S）
 *
 *  start_dispatchは，カーネル起動時に呼び出すべきもので，すべての割込
 *  みを禁止した状態（全割込み禁止状態と同等の状態）で呼び出さなければ
 *  ならない
 */
extern void start_dispatch(void) NoReturn;

/*
 *  現在のコンテキストを捨ててディスパッチ（prc_support.S）
 *
 *  exit_and_dispatch は，OS割込み禁止状態で呼び出さなければならない
 */
extern void exit_and_dispatch(void) NoReturn;

/*
 *  タスクコンテキストブロックの定義
 */
typedef struct task_context_block {
	void			*sp;        /* スタックポインタ */
	FunctionRefType	pc;         /* プログラムカウンタ */
} TSKCTXB;

/*
 *  タスクコンテキストの初期化
 *
 *  タスクが休止状態から実行できる状態に移行する時に呼ばれる．この時点
 *  でスタック領域を使ってはならない
 *
 *  activate_contextを，インライン関数ではなくマクロ定義としているのは，
 *  この時点ではTCBが定義されていないためである
 */
extern void start_r(void);

#define activate_context(p_tcb) do {									  \
		(p_tcb)->tskctxb.sp = (void *) ((char8 *) ((p_tcb)->p_tinib->stk) \
										+ (p_tcb)->p_tinib->stksz);		  \
		(p_tcb)->tskctxb.pc = (FunctionRefType) start_r;				  \
} while (0)

/* 引数の型はハードウェアが扱えるサイズ（uint32）と同サイズを使用 */
extern void stack_change_and_call_func_1(void *func, uint32 arg1);
extern void stack_change_and_call_func_2(void *func, uint8 arg1, uint8 arg2);

#ifdef ENABLE_RETURN_MAIN
/*
 *  AKTSP用のmain()へのリターンコード（prc_support.S）
 */
extern void return_main(void);
#endif /* ENABLE_RETURN_MAIN */

/*
 *  特定の割込み要求ラインの有効/無効を制御可能かを調べる処理
 */
extern boolean target_is_int_controllable(InterruptNumberType intno);

#endif /* TOPPERS_MACRO_ONLY */

#endif /* TOPPERS_PRC_CONFIG_H */
