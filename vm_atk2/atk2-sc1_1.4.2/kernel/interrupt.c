/*
 *  TOPPERS ATK2
 *      Toyohashi Open Platform for Embedded Real-Time Systems
 *      Automotive Kernel Version 2
 *
 *  Copyright (C) 2000-2003 by Embedded and Real-Time Systems Laboratory
 *                              Toyohashi Univ. of Technology, JAPAN
 *  Copyright (C) 2004-2017 by Center for Embedded Computing Systems
 *              Graduate School of Information Science, Nagoya Univ., JAPAN
 *  Copyright (C) 2011-2017 by FUJI SOFT INCORPORATED, JAPAN
 *  Copyright (C) 2011-2013 by Spansion LLC, USA
 *  Copyright (C) 2011-2017 by NEC Communication Systems, Ltd., JAPAN
 *  Copyright (C) 2011-2016 by Panasonic Advanced Technology Development Co., Ltd., JAPAN
 *  Copyright (C) 2011-2014 by Renesas Electronics Corporation, JAPAN
 *  Copyright (C) 2011-2016 by Sunny Giken Inc., JAPAN
 *  Copyright (C) 2011-2017 by TOSHIBA CORPORATION, JAPAN
 *  Copyright (C) 2004-2017 by Witz Corporation
 *  Copyright (C) 2014-2016 by AISIN COMCRUISE Co., Ltd., JAPAN
 *  Copyright (C) 2014-2016 by eSOL Co.,Ltd., JAPAN
 *  Copyright (C) 2014-2017 by SCSK Corporation, JAPAN
 *  Copyright (C) 2015-2017 by SUZUKI MOTOR CORPORATION
 *
 *  上記著作権者は，以下の(1)〜(4)の条件を満たす場合に限り，本ソフトウェ
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
 *  $Id: interrupt.c 727 2017-01-23 09:27:59Z witz-itoyo $
 */

/*
 *		割込み制御モジュール
 */

#include "kernel_impl.h"
#include "interrupt.h"

#ifdef TOPPERS_interrupt_initialize
/*
 *  実行中のC2ISR
 */
ISRCB	*p_runisr;

/*
 *  割込み管理機能内部で使用する変数の定義
 */

/*
 *  SuspendAllInterrupts のネスト回数
 */
uint8	sus_all_cnt = 0U;

/*
 *  SuspendOSInterrupts のネスト回数
 */
uint8	sus_os_cnt = 0U;

/*
 *  割込み管理機能の初期化
 */
#ifndef OMIT_INITIALIZE_INTERRUPT

void
interrupt_initialize(void)
{
	ISRType				i;
	InterruptNumberType	j;
	ISRCB				*p_isrcb;
	const INTINIB		*p_intinib;

	p_runisr = NULL;

	sus_all_cnt = 0U;
	sus_os_cnt = 0U;

	for (i = 0U; i < tnum_isr2; i++) {
		p_isrcb = &(isrcb_table[i]);
		p_isrcb->p_intinib = &(intinib_table[i]);
		p_isrcb->p_lastrescb = NULL;
	}

	for (j = 0U; j < tnum_intno; j++) {
		p_intinib = &(intinib_table[j]);
		x_config_int(p_intinib->intno, p_intinib->intatr, p_intinib->intpri);
	}
}

#endif /* OMIT_INITIALIZE_INTERRUPT */
#endif /* TOPPERS_interrupt_initialize */

/*
 *  割込み禁止の解除
 */
#ifdef TOPPERS_release_interrupts

void
release_interrupts(OSServiceIdType serviceId)
{
#ifdef CFG_USE_ERRORHOOK
	boolean call_error_hook = FALSE;
#endif /* CFG_USE_ERRORHOOK */

	if (sus_os_cnt > 0U) {
		sus_os_cnt = 0U;
		LEAVE_CALLEVEL(TSYS_SUSOSINT);
		x_nested_unlock_os_int();
#ifdef CFG_USE_ERRORHOOK
		call_error_hook = TRUE;
#endif /* CFG_USE_ERRORHOOK */
	}

	if (sus_all_cnt > 0U) {
		sus_all_cnt = 0U;
		LEAVE_CALLEVEL(TSYS_SUSALLINT);
		ASSERT((callevel_stat & TSYS_DISALLINT) == TSYS_NULL);
		x_unlock_all_int();
#ifdef CFG_USE_ERRORHOOK
		call_error_hook = TRUE;
#endif /* CFG_USE_ERRORHOOK */
	}

	/* C2ISRの場合のみDisAllを解除する */
	if (serviceId == OSServiceId_ISRMissingEnd) {
		if ((callevel_stat & TSYS_DISALLINT) != TSYS_NULL) {
			LEAVE_CALLEVEL(TSYS_DISALLINT);
			x_unlock_all_int();
#ifdef CFG_USE_ERRORHOOK
			call_error_hook = TRUE;
#endif /* CFG_USE_ERRORHOOK */
		}
	}

#ifdef CFG_USE_ERRORHOOK
	if ((serviceId != OSServiceId_Invalid) && (call_error_hook != FALSE)) {
		call_errorhook(E_OS_DISABLEDINT, serviceId);
	}
#endif /* CFG_USE_ERRORHOOK */
}

#endif /* TOPPERS_release_interrupts */

/*
 *  C2ISR終了時のチェック関数
 */
#ifdef TOPPERS_exit_isr2

LOCAL_INLINE void
release_isrresources(ISRCB *p_isrcb)
{

	/* OS割込み禁止状態以上で来るはず */
	if (p_isrcb->p_lastrescb != NULL) {
		do {
			x_set_ipm(p_isrcb->p_lastrescb->prevpri);
			p_isrcb->p_lastrescb->lockflg = FALSE;
			p_isrcb->p_lastrescb = p_isrcb->p_lastrescb->p_prevrescb;
		} while (p_isrcb->p_lastrescb != NULL);

#ifdef CFG_USE_ERRORHOOK
		call_errorhook(E_OS_RESOURCE, OSServiceId_ISRMissingEnd);
#endif /* CFG_USE_ERRORHOOK */
	}
}

void
exit_isr2(void)
{
	x_nested_lock_os_int();

	release_interrupts(OSServiceId_ISRMissingEnd);

	/* リソース確保状態の場合 */
	release_isrresources(p_runisr);

	x_nested_unlock_os_int();
}

#endif /* TOPPERS_exit_isr2 */
