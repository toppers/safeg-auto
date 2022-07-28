/*
 *  TOPPERS ATK2
 *      Toyohashi Open Platform for Embedded Real-Time Systems
 *      Automotive Kernel Version 2
 *
 *  Copyright (C) 2012-2019 by Center for Embedded Computing Systems
 *              Graduate School of Information Science, Nagoya Univ., JAPAN
 *
 *  �嵭����Ԥϡ��ʲ���(1)��(4)�ξ������������˸¤ꡤ�ܥ��եȥ���
 *  �����ܥ��եȥ���������Ѥ�����Τ�ޤࡥ�ʲ�Ʊ���ˤ���ѡ�ʣ������
 *  �ѡ������ۡʰʲ������ѤȸƤ֡ˤ��뤳�Ȥ�̵���ǵ������롥
 *  (1) �ܥ��եȥ������򥽡��������ɤη������Ѥ�����ˤϡ��嵭������
 *      ��ɽ�����������Ѿ�浪��Ӳ�����̵�ݾڵ��꤬�����Τޤޤη��ǥ���
 *      ����������˴ޤޤ�Ƥ��뤳�ȡ�
 *  (2) �ܥ��եȥ������򡤥饤�֥������ʤɡ�¾�Υ��եȥ�������ȯ�˻�
 *      �ѤǤ�����Ǻ����ۤ�����ˤϡ������ۤ�ȼ���ɥ�����ȡ�����
 *      �ԥޥ˥奢��ʤɡˤˡ��嵭�����ɽ�����������Ѿ�浪��Ӳ���
 *      ��̵�ݾڵ����Ǻܤ��뤳�ȡ�
 *  (3) �ܥ��եȥ������򡤵�����Ȥ߹���ʤɡ�¾�Υ��եȥ�������ȯ�˻�
 *      �ѤǤ��ʤ����Ǻ����ۤ�����ˤϡ����Τ����줫�ξ�����������
 *      �ȡ�
 *    (a) �����ۤ�ȼ���ɥ�����ȡ����Ѽԥޥ˥奢��ʤɡˤˡ��嵭����
 *        �ɽ�����������Ѿ�浪��Ӳ�����̵�ݾڵ����Ǻܤ��뤳�ȡ�
 *    (b) �����ۤη��֤��̤�������ˡ�ˤ�äơ�TOPPERS�ץ������Ȥ�
 *        ��𤹤뤳�ȡ�
 *  (4) �ܥ��եȥ����������Ѥˤ��ľ��Ū�ޤ��ϴ���Ū�������뤤���ʤ�»
 *      ������⡤�嵭����Ԥ����TOPPERS�ץ������Ȥ����դ��뤳�ȡ�
 *      �ޤ����ܥ��եȥ������Υ桼���ޤ��ϥ���ɥ桼������Τ����ʤ���
 *      ͳ�˴�Ť����ᤫ��⡤�嵭����Ԥ����TOPPERS�ץ������Ȥ�
 *      ���դ��뤳�ȡ�
 *
 *  �ܥ��եȥ������ϡ�AUTOSAR��AUTomotive Open System ARchitecture�˻�
 *  �ͤ˴�Ť��Ƥ��롥�嵭�ε����ϡ�AUTOSAR����Ū�⻺������������Τ�
 *  �Ϥʤ���AUTOSAR�ϡ�AUTOSAR���ͤ˴�Ť������եȥ�����������Ū����
 *  �Ѥ���Ԥ��Ф��ơ�AUTOSAR�ѡ��ȥʡ��ˤʤ뤳�Ȥ���Ƥ��롥
 *
 *  �ܥ��եȥ������ϡ�̵�ݾڤ��󶡤���Ƥ����ΤǤ��롥�嵭����Ԥ�
 *  ���TOPPERS�ץ������Ȥϡ��ܥ��եȥ������˴ؤ��ơ�����λ�����Ū
 *  ���Ф���Ŭ������ޤ�ơ������ʤ��ݾڤ�Ԥ�ʤ����ޤ����ܥ��եȥ���
 *  �������Ѥˤ��ľ��Ū�ޤ��ϴ���Ū�������������ʤ�»���˴ؤ��Ƥ⡤��
 *  ����Ǥ�����ʤ���
 *
 *  $Id: rh850_f1l.h 39 2014-07-19 06:33:15Z ertl-honda $
 */

/*
 *		RH850/F1L�Υϡ��ɥ������񸻤����
 */

#ifndef TOPPERS_RH850_U2A_H
#define TOPPERS_RH850_U2A_H

#define _RH850G4MH_

#define LOCK_VAL   0xA5A5A500
#define UNLOCK_VAL 0xA5A5A501

#define CLKKCPROT1 0xFF980700
#define MOSCE      0xFF988000
#define MOSCS      0xFF988004
#define PLLE       0xFF980000
#define PLLS       0xFF980004
#define CLKD_PLLC  0xFF980120
#define CKSC_CPUC  0xFF980100
#define CKSC_CPUS  0xFF980108


#define PORT_BASE  0xffd90000

/* ü�ҵ�ǽ����  (USE)*/
#define P(n)		((PORT_BASE) + 0x0000 + (n * 0x40U))         /* �ݡ��ȡ��⡼�ɡ�����ȥ��롦�쥸���� */
#define PMC(n)		((PORT_BASE) + 0x0014 + (n * 0x40U))         /* �ݡ��ȡ��⡼�ɡ�����ȥ��롦�쥸���� */
#define PMCSR(n)	((PORT_BASE) + 0x0024 + (n * 0x40U))         /* �ݡ��ȡ��⡼�ɡ�����ȥ��롦���åȡ��ꥻ�åȡ��쥸���� */
#define PIPC(n)		((PORT_BASE) + 0x4008 + (n * 0x40U))         /* �ݡ���IP ����ȥ��롦�쥸���� */
#define PM(n)		((PORT_BASE) + 0x0010 + (n * 0x40U))         /* �ݡ��ȡ��⡼�ɡ��쥸���� */
#define PMSR(n)		((PORT_BASE) + 0x0020 + (n * 0x40U))         /* �ݡ��ȡ��⡼�ɡ����åȡ��ꥻ�åȡ��쥸���� */
#define PIBC(n)		((PORT_BASE) + 0x4000 + (n * 0x40U))         /* �ݡ������ϥХåե�������ȥ��롦�쥸���� */
#define PFC(n)		((PORT_BASE) + 0x0018 + (n * 0x40U))         /* �ݡ��ȵ�ǽ����ȥ��롦�쥸���� */
#define PFCE(n)		((PORT_BASE) + 0x001C + (n * 0x40U))         /* �ݡ��ȵ�ǽ����ȥ��롦�쥸���� */
#define PFCAE(n)	((PORT_BASE) + 0x0028 + (n * 0x40U))         /* �ݡ��ȵ�ǽ����ȥ��롦��ĥ�쥸���� */
#define PISA(n)		((PORT_BASE) + 0x4024 + (n * 0x40U))

#define PKCPROT		((PORT_BASE) + 0x2F40)
#define PWE			((PORT_BASE) + 0x2F44)

#define RLIN30_BASE  (0xFFD28000)
#define RLIN31_BASE  (0xFFC7C000)
#define RLIN32_BASE  (0xFFD28200)
#define RLIN33_BASE  (0xFFC7C080)
#define RLIN34_BASE  (0xFFD28400)
#define RLIN35_BASE  (0xFFC7C100)
#define RLIN36_BASE  (0xFFD28600)
#define RLIN37_BASE  (0xFFC7C180)
#define RLIN38_BASE  (0xFFD28800)
#define RLIN39_BASE  (0xFFC7C200)
#define RLIN310_BASE (0xFFD28C00)
#define RLIN311_BASE (0xFFC7C280)

/* P-Bus Group 3 */

#define RLIN3xLWBR_B		0x00000001
#define RLIN3xLBRP01_H		0x00000002
#define RLIN3xLBRP0_B		0x00000002
#define RLIN3xLBRP1_B		0x00000003
#define RLIN3xLSTC_B		0x00000004
#define RLIN3xLMD_B			0x00000008
#define RLIN3xLBFC_B		0x00000009
#define RLIN3xLSC_B			0x0000000a
#define RLIN3xLWUP_B		0x0000000b
#define RLIN3xLIE_B			0x0000000c
#define RLIN3xLEDE_B		0x0000000d
#define RLIN3xLCUC_B		0x0000000e
#define RLIN3xLTRC_B		0x00000010
#define RLIN3xLMST_B		0x00000011
#define RLIN3xLST_B			0x00000012
#define RLIN3xLEST_B		0x00000013
#define RLIN3xLDFC_B		0x00000014
#define RLIN3xLIDB_B		0x00000015
#define RLIN3xLCBR_B		0x00000016
#define RLIN3xLUDB0_B		0x00000017
#define RLIN3xLDBR1_B		0x00000018
#define RLIN3xLDBR2_B		0x00000019
#define RLIN3xLDBR3_B		0x0000001a
#define RLIN3xLDBR4_B		0x0000001b
#define RLIN3xLDBR5_B		0x0000001c
#define RLIN3xLDBR6_B		0x0000001d
#define RLIN3xLDBR7_B		0x0000001e
#define RLIN3xLDBR8_B		0x0000001f
#define RLIN3xLUOER_B		0x00000020
#define RLIN3xLUOR1_B		0x00000021
#define RLIN3xLUTDR_H		0x00000024
#define RLIN3xLUTDRL_B		0x00000024
#define RLIN3xLUTDRH_B		0x00000025
#define RLIN3xLURDR_H		0x00000026
#define RLIN3xLURDRL_B		0x00000026
#define RLIN3xLURDRH_B		0x00000027
#define RLIN3xLUWTDR_H		0x00000028
#define RLIN3xLUWTDRL_B		0x00000028
#define RLIN3xLUWTDRH_B		0x00000029

#define MSR_TAUJ_ISO 0xFF981140
#define MSR_TAUJ_AWO 0xFF988E20
#define MSR_RLIN3    0xFF981060
#define MSR_OSTM     0xFF981180
#define MSRKCPROT    0xFF981710

#define OSTM0_BASE	0xFFBF0000
#define OSTM1_BASE	0xFFBF0100
#define OSTM2_BASE	0xFFBF0200
#define OSTM3_BASE	0xFFBF0300
#define OSTM4_BASE	0xFFBF0400
#define OSTM5_BASE	0xFFBF0500
#define OSTM6_BASE	0xFFBF0600
#define OSTM7_BASE	0xFFBF0700
#define OSTM8_BASE	0xFFBF0800
#define OSTM9_BASE	0xFFBF0900

/*
 *  INTNO
 */
#define TAUJ0I0_INTNO		UINT_C(360)
#define TAUJ0I1_INTNO		UINT_C(361)
#define TAUJ0I2_INTNO		UINT_C(362)
#define TAUJ0I3_INTNO		UINT_C(363)

#define TAUJ1I0_INTNO		UINT_C(364)
#define TAUJ1I1_INTNO		UINT_C(365)
#define TAUJ1I2_INTNO		UINT_C(366)
#define TAUJ1I3_INTNO		UINT_C(367)

#define TAUJ2I0_INTNO		UINT_C(368)
#define TAUJ2I1_INTNO		UINT_C(369)
#define TAUJ2I2_INTNO		UINT_C(370)
#define TAUJ2I3_INTNO		UINT_C(371)

#define TAUJ3I0_INTNO		UINT_C(372)
#define TAUJ3I1_INTNO		UINT_C(373)
#define TAUJ3I2_INTNO		UINT_C(374)
#define TAUJ3I3_INTNO		UINT_C(375)

#define RLIN30_RX_INTNO 418
#define RLIN31_RX_INTNO 422
#define RLIN32_RX_INTNO 426
#define RLIN33_RX_INTNO 430
#define RLIN34_RX_INTNO 434
#define RLIN35_RX_INTNO 438
#define RLIN36_RX_INTNO 442
#define RLIN37_RX_INTNO 446
#define RLIN38_RX_INTNO 450
#define RLIN39_RX_INTNO 454
#define RLIN310_RX_INTNO 458
#define RLIN311_RX_INTNO 462

/*
 *  INTC
 */
#define INTC1_BASE  0xFFFC0000
#define INTC2_BASE  0xFFF80000

#define INTC2_EIC   0x0000
#define INTC2_EEIC  0x4000
#define INTC2_EIBD  0x2000
#define INTC2_INTNO_OFFSET 32

/* intno �� unsigned ������ */
#define EIC_ADDRESS(intno)	(intno <= 31)? (INTC1_BASE + (intno * 2)) : (INTC2_BASE + INTC2_EIC  + (intno * 2))
#define EEIC_ADDRESS(intno)	(intno <= 31)? (INTC1_BASE + (intno * 2)) : (INTC2_BASE + INTC2_EEIC  + (intno * 4))

#define EIBD_ADDRESS(intno) (INTC2_BASE + INTC2_EIBD  + (intno * 4))

#define TMIN_INTNO	UINT_C(0)
#define TMAX_INTNO	UINT_C(767)
#define TNUM_INT	UINT_C(768)

#include "v850.h"

#endif /* TOPPERS_RH850_U2A_H */
