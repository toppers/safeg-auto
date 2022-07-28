/*
 *  TOPPERS ATK2
 *      Toyohashi Open Platform for Embedded Real-Time Systems
 *      Automotive Kernel Version 2
 *
 *  Copyright (C) 2012-2014 by Center for Embedded Computing Systems
 *              Graduate School of Information Science, Nagoya Univ., JAPAN
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
 *  $Id: prc_insn.h 540 2015-12-29 01:00:01Z ertl-honda $
 */

/*
 *		プロセッサの特殊命令のインライン関数定義（V850用）
 */
#ifndef TOPPERS_PRC_INSN_H
#define TOPPERS_PRC_INSN_H

#include <prc_tool.h>

/*
 *  V850E3V5 or RH850V2のみサポート
 */
#if !(defined(__v850e3v5__) || defined(__rh850v2__))
#error Only support v850e3v5 or rh850v2 architecture!!
#endif /* __v850e3v5__ */

#ifndef TOPPERS_MACRO_ONLY

#define SYNCM	__syncm();

LOCAL_INLINE void
disable_int(void)
{
	__DI();
}

LOCAL_INLINE void
enable_int(void)
{
	__EI();
}

LOCAL_INLINE uint32
current_psw(void)
{
	return(__stsr_rh(5, 0));
}

LOCAL_INLINE void
set_psw(uint32 psw)
{
	__ldsr_rh(5, 0, psw);
}

LOCAL_INLINE void
set_rbase(uint32 rbase)
{
	__ldsr_rh(2, 1, rbase);
}

LOCAL_INLINE void
set_ebase(uint32 ebase)
{
	__ldsr_rh(3, 1, ebase);
}

#ifdef __v850e3v5__
/*
 *  V850E3V5用の割込みコントローラ操作ルーチン
 */
LOCAL_INLINE void
set_pmr(uint16 pmr)
{
	uint32 psw;

	/* PMR must be set in di sate(PSW.ID = 1) */
	psw = current_psw();
	disable_int();

	__ldsr_rh(11, 2, pmr);

	set_psw(psw);
}
#endif /* __v850e3v5__ */

#ifdef __rh850v2__
LOCAL_INLINE void
set_plmr(uint16 plm)
{
	uint32 psw;

	/* PMR must be set in di sate(PSW.ID = 1) */
	psw = current_psw();
	disable_int();

	__ldsr_rh(14, 2, plm);

	set_psw(psw);
}
#endif /* __rh850v2__ */

LOCAL_INLINE uint32
get_intcfg(void)
{
	return(__stsr_rh(13, 2));
}

LOCAL_INLINE void
set_intcfg(uint32 intcfg)
{
	__ldsr_rh(13, 2, intcfg);
}

LOCAL_INLINE uint16
get_ispr(void)
{
	return(__stsr_rh(10, 2));
}

LOCAL_INLINE void
clear_ispr(void)
{
	uint32 psw;

	/* ISPR must be set in di sate(PSW.ID = 1) */
	psw = current_psw();
	disable_int();

	__ldsr_rh(13, 2, 1);    /* INTCFG = 1; ISPR を書き換え可能に */
	__ldsr_rh(10, 2, 0);    /* ISPR = 0 */
	__ldsr_rh(13, 2, 0);    /* INTCFG = 0; ISPR を書き換え禁止に(自動更新に) */
	__syncp();

	set_psw(psw);
}

LOCAL_INLINE void
set_intbp(uint32 intbp)
{
	uint32 psw;

	/* INTBP must be set in di sate(PSW.ID = 1) */
	psw = current_psw();
	disable_int();

	__ldsr_rh(4, 1, intbp);

	set_psw(psw);
}

LOCAL_INLINE uint32
get_peid(void)
{
	return(__stsr_rh(0, 2));
}

/*
 *  Performace counter
 */
LOCAL_INLINE uint64
get_tscount(void)
{
    uint32 tmpl, tmph1, tmph2;
    uint64 tmp;

    tmph1 = __stsr_rh(1, 11);
    tmpl = __stsr_rh(0, 11);
    tmph2 = __stsr_rh(1, 11);
    
    if (tmph1 != tmph2) {
        tmpl = __stsr_rh(0, 11);
    }
    tmp = ((uint64)tmph2 << 32);
    tmp |= tmpl;
    return tmp;
}

LOCAL_INLINE void
start_tscount(void)
{
    __ldsr_rh(2, 11, 0x01);
}

LOCAL_INLINE void
stop_tscount(void)
{
    __ldsr_rh(2, 11, 0x00);
}

#define get_pmcount(no) (__stsr_rh((16 + no), 14))

#define set_pmcount(no, val) (__ldsr_rh((16 + no), 14, val))

/*
 *  Performace counter
 */
#define TNUM_PMCOUNT         (8U)

#define PMCTRL_CND_ALL       (0x00U)
#define PMCTRL_CND_INST      (0x10U)
#define PMCTRL_CND_BRA       (0x18U)
#define PMCTRL_CND_CBRA      (0x19U)
#define PMCTRL_CND_CBRAMISS  (0x1AU)
#define PMCTRL_CND_EIINT     (0x20U)
#define PMCTRL_CND_FEINT     (0x21U)
#define PMCTRL_CND_PEXC      (0x22U)
#define PMCTRL_CND_RCEXCT    (0x23U)
#define PMCTRL_CND_BGEIINT   (0x24U)
#define PMCTRL_CND_BGREINT   (0x25U)
#define PMCTRL_CND_NOINT     (0x28U)
#define PMCTRL_CND_NOINTDIS  (0x29U)
#define PMCTRL_CND_IFETCH    (0x30U)
#define PMCTRL_CND_ICACHE    (0x31U)
#define PMCTRL_CND_ISTALL    (0x40U)
#define PMCTRL_CND_FLASHI    (0x50U)
#define PMCTRL_CND_FLASHD    (0x51U)

#define PMCTRL_CNT_HOST     (1U << 24)
#define PMCTRL_CNT_GPID7    (1U << 23)
#define PMCTRL_CNT_GPID6    (1U << 22)
#define PMCTRL_CNT_GPID5    (1U << 21)
#define PMCTRL_CNT_GPID4    (1U << 20)
#define PMCTRL_CNT_GPID3    (1U << 19)
#define PMCTRL_CNT_GPID2    (1U << 18)
#define PMCTRL_CNT_GPID1    (1U << 17)
#define PMCTRL_CNT_GPID0    (1U << 16)
#define PMCTRL_CNT_ALL      (0x1ffU << 16)

#define PMCTRL_CND_SHIFT    (8U)

#define start_pmcount(no, cnd, mode) (__ldsr_rh(no, 14, (0x01 | (cnd << PMCTRL_CND_SHIFT) | mode)))

#define stop_pmcount(no)  (__ldsr_rh(no, 14, 0))

LOCAL_INLINE void
set_pmgmctrl(uint32 val) 
{
    __ldsr_rh(9, 11, val);
}

#endif /* TOPPERS_MACRO_ONLY */

#endif /* TOPPERS_PRC_INSN_H */
