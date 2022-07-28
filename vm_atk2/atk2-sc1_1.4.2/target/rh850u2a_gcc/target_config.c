/*
 *  TOPPERS ATK2
 *      Toyohashi Open Platform for Embedded Real-Time Systems
 *      Automotive Kernel Version 2
 *
 *  Copyright (C) 2012-2014 by Center for Embedded Computing Systems
 *              Graduate School of Information Science, Nagoya Univ., JAPAN
 *  Copyright (C) 2012-2014 by FUJISOFT INCORPORATED, JAPAN
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
 *  $Id: target_config.c 42 2014-07-19 07:10:58Z ertl-honda $
 */

/*
 *		ターゲット依存モジュール（RH850U2A_GCC用）
 */

#include "kernel_impl.h"
#include "v850_gcc/uart_rlin.h"
#include "v850_gcc/prc_sil.h"
#include "target_sysmod.h"
#ifdef ENABLE_RETURN_MAIN
#include "interrupt.h"
#endif /* ENABLE_RETURN_MAIN */
#ifdef TOPPERS_ENABLE_TRACE
#include "logtrace/trace_config.h"
#endif /* TOPPERS_ENABLE_TRACE */

#ifndef TARGET_RCARS4

/*
 *  RLIN30 Port Configration
 *   P10_4 : RLIN30TX (第2兼用Output)
 *   P10_5 : RLIN30RX (第4兼用Input)
 */
#define RLIN30_P10_MASK		((uint16)0x0030)

#define RLIN30_PMC10_INIT	((uint16)0x0030)

#define RLIN30_PFCAE10_INIT	((uint16)0x0000)
#define RLIN30_PFCE10_INIT	((uint16)0x0020)
#define RLIN30_PFC10_INIT	((uint16)0x0030)
#define RLIN30_PM10_INIT	((uint16)0x0020)

/*
 *  RLIN31 Port Configration
 *   P2_5 : RLIN31TX (第3兼用Output)
 *   P2_4 : RLIN31RX (第3兼用Input)
 */
#define RLIN31_P2_MASK		((uint16)0x0030)

#define RLIN31_PMC2_INIT	((uint16)0x0030)
#define RLIN31_PFCAE2_INIT	((uint16)0x0000)
#define RLIN31_PFCE2_INIT	((uint16)0x0030)
#define RLIN31_PFC2_INIT	((uint16)0x0000)
#define RLIN31_PM2_INIT		((uint16)0x0010)

/*
 *  RLIN32 Port Configration
 *   P2_0 : RLIN32TX (第4兼用Output)
 *   P2_1 : RLIN32RX (第4兼用Input)
 */
#define RLIN32_P2_MASK		((uint16)0x0003)

#define RLIN32_PMC2_INIT	((uint16)0x0003)
#define RLIN32_PFCAE2_INIT	((uint16)0x0000)
#define RLIN32_PFCE2_INIT	((uint16)0x0003)
#define RLIN32_PFC2_INIT	((uint16)0x0003)
#define RLIN32_PM2_INIT		((uint16)0x0002)

/*
 *  RLIN33 Port Configration
 *   P4_14 : RLIN33TX (第3兼用Output)
 *   P4_15 : RLIN33RX (第3兼用Input)
 */
#define RLIN33_P2_MASK		((uint16)0xC000)

#define RLIN33_PMC2_INIT	((uint16)0xC000)
#define RLIN33_PFCAE2_INIT	((uint16)0x0000)
#define RLIN33_PFCE2_INIT	((uint16)0xC000)
#define RLIN33_PFC2_INIT	((uint16)0x0000)
#define RLIN33_PM2_INIT		((uint16)0x8000)

#define FCLACTL0_RLIN3    ((uint8_t*)0xFFED4000)
#define FCLACTL1_RLIN3    ((uint8_t*)0xFFED4004)

#else /* TARGET_RCARS4 */

/*
 *  RLIN31 Port Configration
 *   AA6 : GP6_12 : RLIN31TX (Function 0：Output)
 *   AA5 : GP6_13 : RLIN31RX (Function 0：Input)
 */
#define RLIN31_GP6_MASK         ((uint32)0x00003000)
#define RLIN31_PMMER_INIT       ((uint32)0x00003000)
#define RLIN31_PMMR_INIT        ((uint32)0x00003000)
#define RLIN31_IPSR_MASK        ((uint32)0x00FF0000)
#define RLIN31_IPSR_INIT        ((uint32)0x00000000)
#define RLIN31_GPSR_INIT        ((uint32)0x00003000)
#define RLIN31_GP6_13_MASK      ((uint32)0x00002000)
#define RLIN31_POSNEG_INIT      ((uint32)0x00002000)
#define RLIN31_EDGLEVEL_INIT    ((uint32)0x00002000)
#define RLIN31_BOTHEDGE_INIT    ((uint32)0x00000000)
#define RLIN31_IOINTSEL_INIT    ((uint32)0x00002000)
#define RLIN31_INTCLR_INIT      ((uint32)0x00002000)
#define RLIN31_MSKCLR_INIT      ((uint32)0x00002000)

/*
 *  RLIN33 Port Configration
 *   AB6 : GP6_08 : RLIN33TX (Function 0：Output)
 *   AB4 : GP6_09 : RLIN33RX (Function 0：Input)
 */
#define RLIN33_GP6_MASK         ((uint32)0x00000300)
#define RLIN33_PMMER_INIT       ((uint32)0x00000300)
#define RLIN33_PMMR_INIT        ((uint32)0x00000300)
#define RLIN33_IPSR_MASK        ((uint32)0x000000FF)
#define RLIN33_IPSR_INIT        ((uint32)0x00000000)
#define RLIN33_GPSR_INIT        ((uint32)0x00000300)
#define RLIN33_GP6_09_MASK      ((uint32)0x00000200)
#define RLIN33_POSNEG_INIT      ((uint32)0x00000200)
#define RLIN33_EDGLEVEL_INIT    ((uint32)0x00000200)
#define RLIN33_BOTHEDGE_INIT    ((uint32)0x00000000)
#define RLIN33_IOINTSEL_INIT    ((uint32)0x00000200)
#define RLIN33_INTCLR_INIT      ((uint32)0x00000200)
#define RLIN33_MSKCLR_INIT      ((uint32)0x00000200)

/*
 *  RLIN35 Port Configration
 *   AC4 : GP6_04 : RLIN35TX (Function 0：Output)
 *   AC3 : GP6_05 : RLIN35RX (Function 0：Input)
 */
#define RLIN35_GP6_MASK         ((uint32)0x00000030)
#define RLIN35_PMMER_INIT       ((uint32)0x00000030)
#define RLIN35_PMMR_INIT        ((uint32)0x00000030)
#define RLIN35_IPSR_MASK        ((uint32)0x00FF0000)
#define RLIN35_IPSR_INIT        ((uint32)0x00000000)
#define RLIN35_GPSR_INIT        ((uint32)0x00000030)
#define RLIN35_GP6_05_MASK      ((uint32)0x00000020)
#define RLIN35_POSNEG_INIT      ((uint32)0x00000020)
#define RLIN35_EDGLEVEL_INIT    ((uint32)0x00000020)
#define RLIN35_BOTHEDGE_INIT    ((uint32)0x00000000)
#define RLIN35_IOINTSEL_INIT    ((uint32)0x00000020)
#define RLIN35_INTCLR_INIT      ((uint32)0x00000020)
#define RLIN35_MSKCLR_INIT      ((uint32)0x00000020)

/*
 *  RLIN37 Port Configration
 *   AD2 : GP6_00 : RLIN37TX (Function 0：Output)
 *   AD1 : GP6_01 : RLIN37RX (Function 0：Input)
 */
#define RLIN37_GP6_MASK         ((uint32)0x00000003)
#define RLIN37_PMMER_INIT       ((uint32)0x00000003)
#define RLIN37_PMMR_INIT        ((uint32)0x00000003)
#define RLIN37_IPSR_MASK        ((uint32)0x000000FF)
#define RLIN37_IPSR_INIT        ((uint32)0x00000000)
#define RLIN37_GPSR_INIT        ((uint32)0x00000003)
#define RLIN37_GP6_01_MASK      ((uint32)0x00000002)
#define RLIN37_POSNEG_INIT      ((uint32)0x00000002)
#define RLIN37_EDGLEVEL_INIT    ((uint32)0x00000002)
#define RLIN37_BOTHEDGE_INIT    ((uint32)0x00000000)
#define RLIN37_IOINTSEL_INIT    ((uint32)0x00000002)
#define RLIN37_INTCLR_INIT      ((uint32)0x00000002)
#define RLIN37_MSKCLR_INIT      ((uint32)0x00000002)

#endif /* TARGET_RCARS4 */

/*
 *   ボーレートレジスタへの設定値 PCLK=80MHz, 115200bps
 */
#define RLIN3xLWBR_VAL		0xf4
#define RLIN3xLBRP01_VAL	10

/*
 *  文字列の出力
 */
void
target_fput_str(const char8 *c)
{
	while (*c != '\0') {
		uart_putc(*c);
		c++;
	}
	uart_putc('\n');
}


#ifndef TARGET_RCARS4

/*
 *  ポートの初期設定
 */
void
target_port_initialize(void)
{
	uint16 wk;

	sil_wrw_mem((void *) PKCPROT, 0xA5A5A501);

#if RLIN3_PORT_NO == 0
	/*
	 * Enable write
	 */
	sil_wrw_mem((void *) PWE, 1U << 10);
    
	/* Bypass filter*/
	sil_wrb_mem((void *) FCLACTL0_RLIN3, 0x80);
	/* TTL level */
	sil_wrh_mem((void *) PISA(10), 0x20);

	/*
	 * Select Alternative mode
	 */
	wk = sil_reh_mem((void *) PMC(10));
	wk &= ~RLIN30_P10_MASK;
	wk |= (RLIN30_PMC10_INIT & RLIN30_P10_MASK);
	sil_wrh_mem((void *) PMC(10), wk);

	/*
	 * Select Function
	 */
	/* Set PFCAE10 */
	wk = sil_reh_mem((void *) PFCAE(10));
	wk &= ~RLIN30_P10_MASK;
	wk |= (RLIN30_PFCAE10_INIT & RLIN30_P10_MASK);
	sil_wrh_mem((void *) PFCAE(10), wk);

	/* Set PFCE10 */
	wk = sil_reh_mem((void *) PFCE(10));
	wk &= ~RLIN30_P10_MASK;
	wk |= (RLIN30_PFCE10_INIT & RLIN30_P10_MASK);
	sil_wrh_mem((void *) PFCE(10), wk);

	/* Set PFC10 */
	wk = sil_reh_mem((void *) PFC(10));
	wk &= ~RLIN30_P10_MASK;
	wk |= (RLIN30_PFC10_INIT & RLIN30_P10_MASK);
	sil_wrh_mem((void *) PFC(10), wk);

	/* Set PM10 */
	wk = sil_reh_mem((void *) PM(10));
	wk &= ~RLIN30_P10_MASK;
	wk |= (RLIN30_PM10_INIT & RLIN30_P10_MASK);
	sil_wrh_mem((void *) PM(10), wk);
    
#elif RLIN3_PORT_NO == 1

	/*
	 * Enable write
	 */
	sil_wrw_mem((void *) PWE, 1U << 3);

	sil_wrb_mem((void *) FCLACTL1_RLIN3, 0x80);

	/*
	 * Select Alternative mode
	 */
	wk = sil_reh_mem((void *) PMC(2));
	wk &= ~RLIN31_P2_MASK;
	wk |= (RLIN31_PMC2_INIT & RLIN31_P2_MASK);
	sil_wrh_mem((void *) PMC(2), wk);

	/*
	 * Select Function
	 */
	/* Set PFCAE2 */
	wk = sil_reh_mem((void *) PFCAE(2));
	wk &= ~RLIN31_P2_MASK;
	wk |= (RLIN31_PFCAE2_INIT & RLIN31_P2_MASK);
	sil_wrh_mem((void *) PFCAE(2), wk);

	/* Set PFCE2 */
	wk = sil_reh_mem((void *) PFCE(2));
	wk &= ~RLIN31_P2_MASK;
	wk |= (RLIN31_PFCE2_INIT & RLIN31_P2_MASK);
	sil_wrh_mem((void *) PFCE(2), wk);

	/* Set PFC2 */
	wk = sil_reh_mem((void *) PFC(2));
	wk &= ~RLIN31_P2_MASK;
	wk |= (RLIN31_PFC2_INIT & RLIN31_P2_MASK);
	sil_wrh_mem((void *) PFC(2), wk);

	/* Set PM2 */
	wk = sil_reh_mem((void *) PM(2));
	wk &= ~RLIN31_P2_MASK;
	wk |= (RLIN31_PM2_INIT & RLIN31_P2_MASK);
	sil_wrh_mem((void *) PM(2), wk);
    
#elif RLIN3_PORT_NO == 2

	/*
	 * Enable write
	 */
	sil_wrw_mem((void *) PWE, 1U << 3);
    
	/*
	 * Select Alternative mode
	 */
	wk = sil_reh_mem((void *) PMC(2));
	wk &= ~RLIN32_P2_MASK;
	wk |= (RLIN32_PMC2_INIT & RLIN32_P2_MASK);
	sil_wrh_mem((void *) PMC(2), wk);

	/*
	 * Select Function
	 */
	/* Set PFCAE2 */
	wk = sil_reh_mem((void *) PFCAE(2));
	wk &= ~RLIN32_P2_MASK;
	wk |= (RLIN32_PFCAE2_INIT & RLIN32_P2_MASK);
	sil_wrh_mem((void *) PFCAE(2), wk);

	/* Set PFCE2 */
	wk = sil_reh_mem((void *) PFCE(2));
	wk &= ~RLIN32_P2_MASK;
	wk |= (RLIN32_PFCE2_INIT & RLIN32_P2_MASK);
	sil_wrh_mem((void *) PFCE(2), wk);

	/* Set PFC2 */
	wk = sil_reh_mem((void *) PFC(2));
	wk &= ~RLIN32_P2_MASK;
	wk |= (RLIN32_PFC2_INIT & RLIN32_P2_MASK);
	sil_wrh_mem((void *) PFC(2), wk);

	/* Set PM2 */
	wk = sil_reh_mem((void *) PM(2));
	wk &= ~RLIN32_P2_MASK;
	wk |= (RLIN32_PM2_INIT & RLIN32_P2_MASK);
	sil_wrh_mem((void *) PM(2), wk);

#elif RLIN3_PORT_NO == 3
	/*
	 * Enable write
	 */
	sil_wrw_mem((void *) PWE, 1U << 5);
    
	/*
	 * Select Alternative mode
	 */
	wk = sil_reh_mem((void *) PMC(4));
	wk &= ~RLIN33_P2_MASK;
	wk |= (RLIN33_PMC2_INIT & RLIN33_P2_MASK);
	sil_wrh_mem((void *) PMC(4), wk);

	/*
	 * Select Function
	 */
	/* Set PFCAE2 */
	wk = sil_reh_mem((void *) PFCAE(4));
	wk &= ~RLIN33_P2_MASK;
	wk |= (RLIN33_PFCAE2_INIT & RLIN33_P2_MASK);
	sil_wrh_mem((void *) PFCAE(4), wk);

	/* Set PFCE2 */
	wk = sil_reh_mem((void *) PFCE(4));
	wk &= ~RLIN33_P2_MASK;
	wk |= (RLIN33_PFCE2_INIT & RLIN33_P2_MASK);
	sil_wrh_mem((void *) PFCE(4), wk);

	/* Set PFC2 */
	wk = sil_reh_mem((void *) PFC(4));
	wk &= ~RLIN33_P2_MASK;
	wk |= (RLIN33_PFC2_INIT & RLIN33_P2_MASK);
	sil_wrh_mem((void *) PFC(4), wk);

	/* Set PM2 */
	wk = sil_reh_mem((void *) PM(4));
	wk &= ~RLIN33_P2_MASK;
	wk |= (RLIN33_PM2_INIT & RLIN33_P2_MASK);
	sil_wrh_mem((void *) PM(4), wk);

#endif /* RLIN3_PORT_NO == 3 */

	sil_wrw_mem((void *) PWE, 0x00000000);
	sil_wrw_mem((void *) PKCPROT, 0xA5A5A500);
}

#else /* TARGET_RCARS4 */

/*
 *  ポートの初期設定
 */
void
target_port_initialize(void)
{
	uint16 wk;

#if RLIN3_PORT_NO == 1

    wk = sil_rew_mem((void *)PORT_PMMER6(0));
    wk &= ~RLIN31_PMMER_INIT;
    sil_wrw_mem((void *)PORT_PMMER6(0), wk);

    /*
     * Select Function
     */
    sil_wrw_mem((void *)PORT_PMMR6(0), RLIN31_PMMR_INIT);
    wk = sil_rew_mem((void *)PORT_IP1SR6(0));
    wk &= ~RLIN31_IPSR_MASK;
    wk |= (RLIN31_IPSR_INIT & RLIN31_IPSR_MASK);
    sil_wrw_mem((void *)PORT_IP1SR6(0), wk);

    sil_wrw_mem((void *)PORT_PMMR6(0), RLIN31_PMMR_INIT);
    wk = sil_rew_mem((void *)PORT_GPSR6(0));
    wk &= ~RLIN31_GP6_MASK;
    wk |= (RLIN31_GPSR_INIT & RLIN31_GP6_MASK);
    sil_wrw_mem((void *)PORT_GPSR6(0), wk);

    wk = sil_rew_mem((void *)PORT_PMMER6(0));
    wk |= RLIN31_PMMER_INIT;
    sil_wrw_mem((void *)PORT_PMMER6(0), wk);
    
#elif RLIN3_PORT_NO == 3

    wk = sil_rew_mem((void *)PORT_PMMER6(0));
    wk &= ~RLIN33_PMMER_INIT;
    sil_wrw_mem((void *)PORT_PMMER6(0), wk);

    /*
     * Select Function
     */
    sil_wrw_mem((void *)PORT_PMMR6(0), RLIN33_PMMR_INIT);
    wk = sil_rew_mem((void *)PORT_IP1SR6(0));
    wk &= ~RLIN33_IPSR_MASK;
    wk |= (RLIN33_IPSR_INIT & RLIN33_IPSR_MASK);
    sil_wrw_mem((void *)PORT_IP1SR6(0), wk);

    sil_wrw_mem((void *)PORT_PMMR6(0), RLIN33_PMMR_INIT);
    wk = sil_rew_mem(PORT_GPSR6(0));
    wk &= ~RLIN33_GP6_MASK;
    wk |= (RLIN33_GPSR_INIT & RLIN33_GP6_MASK);
    sil_wrw_mem((void *)PORT_GPSR6(0), wk);

    wk = sil_rew_mem((void *)PORT_PMMER6(0));
    wk |= RLIN33_PMMER_INIT;
    sil_wrw_mem((void *)PORT_PMMER6(0), wk);
    
#elif RLIN3_PORT_NO == 5

    wk = sil_rew_mem((void *)PORT_PMMER6(0));
    wk &= ~RLIN35_PMMER_INIT;
    sil_wrw_mem((void *)PORT_PMMER6(0), wk);

    /*
     * Select Function
     */
    sil_wrw_mem((void *)PORT_PMMR6(0), RLIN35_PMMR_INIT);
    wk = sil_rew_mem((void *)PORT_IP0SR6(0));
    wk &= ~RLIN35_IPSR_MASK;
    wk |= (RLIN35_IPSR_INIT & RLIN35_IPSR_MASK);
    sil_wrw_mem((void *)PORT_IP0SR6(0), wk);

    sil_wrw_mem((void *)PORT_PMMR6(0), RLIN35_PMMR_INIT);
    wk = sil_rew_mem((void *)PORT_GPSR6(0));
    wk &= ~RLIN35_GP6_MASK;
    wk |= (RLIN35_GPSR_INIT & RLIN35_GP6_MASK);
    sil_wrw_mem((void *)PORT_GPSR6(0), wk);

    wk = sil_rew_mem((void *)PORT_PMMER6(0));
    wk |= RLIN35_PMMER_INIT;
    sil_wrw_mem((void *)PORT_PMMER6(0), wk);

#elif RLIN3_PORT_NO == 7

    wk = sil_rew_mem((void *)PORT_PMMER6(0));
    wk &= ~RLIN37_PMMER_INIT;
    sil_wrw_mem((void *)PORT_PMMER6(0), wk);

    /*
     * Select Function
     */
    sil_wrw_mem((void *)PORT_PMMR6(0), RLIN37_PMMR_INIT);
    wk = sil_rew_mem((void *)PORT_IP0SR6(0));
    wk &= ~RLIN37_IPSR_MASK;
    wk |= (RLIN37_IPSR_INIT & RLIN37_IPSR_MASK);
    sil_wrw_mem((void *)PORT_IP0SR6(0), wk);

    sil_wrw_mem((void *)PORT_PMMR6(0), RLIN37_PMMR_INIT);
    wk = sil_rew_mem((void *)PORT_GPSR6(0));
    wk &= ~RLIN37_GP6_MASK;
    wk |= (RLIN37_GPSR_INIT & RLIN37_GP6_MASK);
    sil_wrw_mem((void *)PORT_GPSR6(0), wk);

    wk = sil_rew_mem((void *)PORT_PMMER6(0));
    wk |= RLIN37_PMMER_INIT;
    sil_wrw_mem((void *)PORT_PMMER6(0), wk);

#endif /* RLIN3_PORT_NO == 7 */

}

#endif /* TARGET_RCARS4 */


/*
 *  クロック関係の初期化
 */
void
target_clock_initialize(void)
{
	uint32 wk;

	/* Unlock */
	sil_wrw_mem((void *) CLKKCPROT1, UNLOCK_VAL);

	/* Start Main OSC */
	sil_wrw_mem((void *) MOSCE, 0x01);
	do {
		wk = sil_rew_mem((void *) MOSCS);
	} while(!((wk & 0x02) == 0x02));

	/* Start PLL */
	sil_wrw_mem((void *) PLLE, 0x01);
	do {
		wk = sil_rew_mem((void *) PLLS);
	} while(!((wk & 0x02) == 0x02));

	/* Select CPU Clock */
	/* Divided by 2 */
	sil_wrw_mem((void *) CLKD_PLLC, 0x02); 
	do {
		wk = sil_rew_mem((void *) CLKD_PLLC);
	} while(!((wk & 0x07) == 0x02));
	/* Select PLL0 */
	sil_wrw_mem((void *) CKSC_CPUC, 0x00);
	do {
		wk = sil_rew_mem((void *) CKSC_CPUS);
	} while(!((wk & 0x01) == 0x00));
	/* Divided by 1 */
	sil_wrw_mem((void *) CLKD_PLLC, 0x01); 
	do {
		wk = sil_rew_mem((void *) CLKD_PLLC);
	} while(!((wk & 0x07) == 0x01));
}

void
uart_enable(uint32 enable) {
	uint32 wk;
	sil_wrw_mem((void *) MSRKCPROT, 0xA5A5A501);
	wk = sil_rew_mem((void *) MSR_RLIN3);
	wk &= ~enable;
	wk |= 0x10000000U;
	sil_wrw_mem((void *) MSR_RLIN3, wk);
	sil_wrw_mem((void *) MSRKCPROT, 0xA5A5A500);
}

void
tauj_enable(uint32 enable) {
	uint32 wk;
	if ((enable == 1) || (enable == 2)) {
		sil_wrw_mem((void *) MSRKCPROT, 0xA5A5A501);
		wk = sil_rew_mem((void *) MSR_TAUJ_ISO);
		wk &= ~enable;
		sil_wrw_mem((void *) MSR_TAUJ_ISO, wk);
		sil_wrw_mem((void *) MSRKCPROT, 0xA5A5A500);
	}

	if ((enable == 3) || (enable == 4)) {
		sil_wrw_mem((void *) MSRKCPROT, 0xA5A5A501);
		wk = sil_rew_mem((void *) MSR_TAUJ_AWO);
		wk &= ~(1 << (enable-3));
		sil_wrw_mem((void *) MSR_TAUJ_AWO, wk);
		sil_wrw_mem((void *) MSRKCPROT, 0xA5A5A500);
	}
}

extern void _reset(void);

/*
 *  ターゲット依存のハードウェアの初期化
 *  スターアップルーチンから呼び出される． 
 */
void
target_hardware_initialize(void)
{
#ifndef OMIT_CLOCK_INIT
	/* クロックの初期設定 */
	target_clock_initialize();
#else /* OMIT_CLOCK_INIT */
	/* ToDo クロック初期化待ち */
//	while(sil_rew_mem((void *)CKSC_CPUCLKS_CTL) != 0x03){}
#endif /* !OMIT_CLOCK_INIT */

	/* リセットベクタの初期化 */
	set_rbase((uint32)_reset);

	/* ポートの初期設定 */
#ifndef OMIT_PORT_INIT
	target_port_initialize();
#endif /* OMIT_PORT_INIT */
}

/* In prc_support.S */
extern void _reset(void);

/*
 *  ターゲット依存の初期化
 */
void
target_initialize(void)
{
	/*
	 *  V850依存の初期化
	 */
	prc_initialize();

#ifdef USE_EBASE
	/* Set EBASE */
	set_ebase((uint32)_reset);
	/* Enable EBASE */
	set_psw(current_psw()|(1U << PSW_EBV_BIT));
#endif /* USE_EBASE */

#ifdef TOPPERS_ENABLE_TRACE
	/*
	 *  トレースログ機能の初期化
	 */
	trace_initialize((uintptr) (TRACE_AUTOSTOP));
#endif /* TOPPERS_ENABLE_TRACE */

#if RLIN3_PORT_NO == 0
	uart_enable(0x00000001U);
#elif RLIN3_PORT_NO == 1
	uart_enable(0x00000002U);
#elif RLIN3_PORT_NO == 2
	uart_enable(0x00000004U);
#elif RLIN3_PORT_NO == 3
	uart_enable(0x00000008U);
#elif RLIN3_PORT_NO == 4
	uart_enable(0x00000010U);
#elif RLIN3_PORT_NO == 5
	uart_enable(0x00000020U);
#elif RLIN3_PORT_NO == 6
	uart_enable(0x00000040U);
#elif RLIN3_PORT_NO == 7
	uart_enable(0x00000080U);
#elif RLIN3_PORT_NO == 8
	uart_enable(0x00000100U);
#elif RLIN3_PORT_NO == 9
	uart_enable(0x00000200U);
#elif RLIN3_PORT_NO == 10
	uart_enable(0x00000400U);
#elif RLIN3_PORT_NO == 11
	uart_enable(0x00000800U);
#endif /* RLIN3_PORT_NO == 11 */

#ifdef HWC_USE_UNIT0
	tauj_enable(0x01);
#elif defined(HWC_USE_UNIT1)
	tauj_enable(0x02);
#elif defined(HWC_USE_UNIT2)
	tauj_enable(0x03);
#elif defined(HWC_USE_UNIT3)
	tauj_enable(0x04);
#endif /* HWC_USE_UNIT0 */
}

/*
 *  ターゲット依存の終了処理
 */
void
target_exit(void)
{
#ifdef TOPPERS_ENABLE_TRACE
	/*
	 *  トレースログのダンプ
	 */
	trace_dump(target_fput_log);
#endif /* TOPPERS_ENABLE_TRACE */

#ifndef ENABLE_RETURN_MAIN
	/*
	 *  シャットダウン処理の出力
	 */
	target_fput_str("Kernel Exit...");
#else
	target_fput_str("Kernel Shutdown...");
#endif /* ENABLE_RETURN_MAIN */

	prc_terminate();

#ifdef ENABLE_RETURN_MAIN
	kerflg = FALSE;
	except_nest_cnt = 0U;
	nested_lock_os_int_cnt = 0U;
	sus_all_cnt = 0U;
	sus_all_cnt_ctx = 0U;
	sus_os_cnt = 0U;
	sus_os_cnt_ctx = 0U;

	/* スタックポインタの初期化とmain()の呼び出し */
	return_main();
#endif /* ENABLE_RETURN_MAIN */

	infinite_loop();
}

/*
 *  ターゲット依存の文字出力
 */
void
target_fput_log(char8 c)
{
	if (c == '\n') {
		uart_putc('\r');
	}
	uart_putc(c);
}
