/*
 *  TOPPERS ATK2
 *      Toyohashi Open Platform for Embedded Real-Time Systems
 *      Automotive Kernel Version 2
 *
 *  Copyright (C) 2012-2019 by Center for Embedded Computing Systems
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
 *  $Id: rh850_f1l.h 39 2014-07-19 06:33:15Z ertl-honda $
 */

/*
 *		R-CAR S4のハードウェア資源の定義
 */

#ifndef TOPPERS_RCAR_S4_H
#define TOPPERS_RCAR_S4_H

#define _RH850G4MH_

#define LOCK_VAL            0xA5A5A500   /* U2A */
#define UNLOCK_VAL          0xA5A5A501

#define CLKKCPROT1          0xFF980700
#define MOSCE               0xFF988000
#define MOSCS               0xFF988004
#define PLLE                0xFF980000
#define PLLS                0xFF980004
#define CLKD_PLLC           0xFF980120
#define CKSC_CPUC           0xFF980100
#define CKSC_CPUS           0xFF980108

#define PORT_GROUP4_BASE    0xffd90000  /* R-Car S4 */
#define PORT_GROUP5_BASE    0xffd90800
#define PORT_GROUP6_BASE    0xffd91000
#define PORT_GROUP7_BASE    0xffd91800
#define PORT_SYSGROUP1_BASE 0xffd98000

/* 端子機能設定  (USE)*/
#define PORT_PMMR4(n)       ((PORT_GROUP4_BASE) + 0x0000U + (n * 0x200U))   /* GPIO group 4, LSI Multiplexed Pin Setting Mask Register [R/W 32] */
#define PORT_PMMR5(n)       ((PORT_GROUP5_BASE) + 0x0000U + (n * 0x200U))   /* GPIO group 5, LSI Multiplexed Pin Setting Mask Register [R/W 32] */
#define PORT_PMMR6(n)       ((PORT_GROUP6_BASE) + 0x0000U + (n * 0x200U))   /* GPIO group 6, LSI Multiplexed Pin Setting Mask Register [R/W 32] */
#define PORT_PMMR7(n)       ((PORT_GROUP7_BASE) + 0x0000U + (n * 0x200U))   /* GPIO group 7, LSI Multiplexed Pin Setting Mask Register [R/W 32] */
#define PORT_PMMER4(n)      ((PORT_GROUP4_BASE) + 0x0004U + (n * 0x200U))   /* GPIO group 4, LSI Multiplexed Pin Setting Mask Enable Register [R/W 32] */
#define PORT_PMMER5(n)      ((PORT_GROUP5_BASE) + 0x0004U + (n * 0x200U))   /* GPIO group 5, LSI Multiplexed Pin Setting Mask Enable Register [R/W 32] */
#define PORT_PMMER6(n)      ((PORT_GROUP6_BASE) + 0x0004U + (n * 0x200U))   /* GPIO group 6, LSI Multiplexed Pin Setting Mask Enable Register [R/W 32] */
#define PORT_PMMER7(n)      ((PORT_GROUP7_BASE) + 0x0004U + (n * 0x200U))   /* GPIO group 7, LSI Multiplexed Pin Setting Mask Enable Register [R/W 32] */
/* PORT_DM0PR */
#define PORT_DM1PR4(n)      ((PORT_GROUP4_BASE) + 0x0024U + (n * 0x200U))   /* GPIO group 4, Dmain Protection register 1 [R/W 32] */
#define PORT_DM1PR5(n)      ((PORT_GROUP5_BASE) + 0x0024U + (n * 0x200U))   /* GPIO group 5, Dmain Protection register 1 [R/W 32] */
#define PORT_DM1PR6(n)      ((PORT_GROUP6_BASE) + 0x0024U + (n * 0x200U))   /* GPIO group 6, Dmain Protection register 1 [R/W 32] */
#define PORT_DM1PR7(n)      ((PORT_GROUP7_BASE) + 0x0024U + (n * 0x200U))   /* GPIO group 7, Dmain Protection register 1 [R/W 32] */
#define PORT_DM2PR4(n)      ((PORT_GROUP4_BASE) + 0x0028U + (n * 0x200U))   /* GPIO group 4, Dmain Protection register 2 [R/W 32] */
#define PORT_DM2PR5(n)      ((PORT_GROUP5_BASE) + 0x0028U + (n * 0x200U))   /* GPIO group 5, Dmain Protection register 2 [R/W 32] */
#define PORT_DM2PR6(n)      ((PORT_GROUP6_BASE) + 0x0028U + (n * 0x200U))   /* GPIO group 6, Dmain Protection register 2 [R/W 32] */
#define PORT_DM2PR7(n)      ((PORT_GROUP7_BASE) + 0x0028U + (n * 0x200U))   /* GPIO group 7, Dmain Protection register 2 [R/W 32] */
/* PORT_DM3PR */
#define PORT_GPSR4(n)       ((PORT_GROUP4_BASE) + 0x0040U + (n * 0x200U))   /* GPIO group 4, GPIO/Peripheral Function Select register [R/W 32] */
#define PORT_GPSR5(n)       ((PORT_GROUP5_BASE) + 0x0040U + (n * 0x200U))   /* GPIO group 5, GPIO/Peripheral Function Select register [R/W 32] */
#define PORT_GPSR6(n)       ((PORT_GROUP6_BASE) + 0x0040U + (n * 0x200U))   /* GPIO group 6, GPIO/Peripheral Function Select register [R/W 32] */
#define PORT_GPSR7(n)       ((PORT_GROUP7_BASE) + 0x0040U + (n * 0x200U))   /* GPIO group 7, GPIO/Peripheral Function Select register [R/W 32] */
#define PORT_IP0SR4(n)      ((PORT_GROUP4_BASE) + 0x0060U + (n * 0x200U))   /* GPIO group 4, Peripheral Function Select register 0 [R/W 32] */
#define PORT_IP0SR5(n)      ((PORT_GROUP5_BASE) + 0x0060U + (n * 0x200U))   /* GPIO group 5, Peripheral Function Select register 0 [R/W 32] */
#define PORT_IP0SR6(n)      ((PORT_GROUP6_BASE) + 0x0060U + (n * 0x200U))   /* GPIO group 6, Peripheral Function Select register 0 [R/W 32] */
#define PORT_IP0SR7(n)      ((PORT_GROUP7_BASE) + 0x0060U + (n * 0x200U))   /* GPIO group 7, Peripheral Function Select register 0 [R/W 32] */
#define PORT_IP1SR4(n)      ((PORT_GROUP4_BASE) + 0x0064U + (n * 0x200U))   /* GPIO group 4, Peripheral Function Select register 1 [R/W 32] */
#define PORT_IP1SR5(n)      ((PORT_GROUP5_BASE) + 0x0064U + (n * 0x200U))   /* GPIO group 5, Peripheral Function Select register 1 [R/W 32] */
#define PORT_IP1SR6(n)      ((PORT_GROUP6_BASE) + 0x0064U + (n * 0x200U))   /* GPIO group 6, Peripheral Function Select register 1 [R/W 32] */
#define PORT_IP1SR7(n)      ((PORT_GROUP7_BASE) + 0x0064U + (n * 0x200U))   /* GPIO group 7, Peripheral Function Select register 1 [R/W 32] */
#define PORT_IP2SR4(n)      ((PORT_GROUP4_BASE) + 0x0068U + (n * 0x200U))   /* GPIO group 4, Peripheral Function Select register 2 [R/W 32] */
#define PORT_IP2SR5(n)      ((PORT_GROUP5_BASE) + 0x0068U + (n * 0x200U))   /* GPIO group 5, Peripheral Function Select register 2 [R/W 32] */
#define PORT_IP2SR6(n)      ((PORT_GROUP6_BASE) + 0x0068U + (n * 0x200U))   /* GPIO group 6, Peripheral Function Select register 2 [R/W 32] */
#define PORT_IP2SR7(n)      ((PORT_GROUP7_BASE) + 0x0068U + (n * 0x200U))   /* GPIO group 7, Peripheral Function Select register 2 [R/W 32] */
#define PORT_IP3SR4(n)      ((PORT_GROUP4_BASE) + 0x006cU + (n * 0x200U))   /* GPIO group 4, Peripheral Function Select register 3 [R/W 32] */
#define PORT_IP3SR7(n)      ((PORT_GROUP7_BASE) + 0x006cU + (n * 0x200U))   /* GPIO group 7, Peripheral Function Select register 3 [R/W 32] */
#define PORT_DRV0CTRL4(n)   ((PORT_GROUP4_BASE) + 0x0080U + (n * 0x200U))   /* GPIO group 4, DRV control register 0 [R/W 32] */
#define PORT_DRV0CTRL5(n)   ((PORT_GROUP5_BASE) + 0x0080U + (n * 0x200U))   /* GPIO group 5, DRV control register 0 [R/W 32] */
#define PORT_DRV0CTRL6(n)   ((PORT_GROUP6_BASE) + 0x0080U + (n * 0x200U))   /* GPIO group 6, DRV control register 0 [R/W 32] */
#define PORT_DRV0CTRL7(n)   ((PORT_GROUP7_BASE) + 0x0080U + (n * 0x200U))   /* GPIO group 7, DRV control register 0 [R/W 32] */
#define PORT_DRV1CTRL4(n)   ((PORT_GROUP4_BASE) + 0x0084U + (n * 0x200U))   /* GPIO group 4, DRV control register 1 [R/W 32] */
#define PORT_DRV1CTRL5(n)   ((PORT_GROUP5_BASE) + 0x0084U + (n * 0x200U))   /* GPIO group 5, DRV control register 1 [R/W 32] */
#define PORT_DRV1CTRL6(n)   ((PORT_GROUP6_BASE) + 0x0084U + (n * 0x200U))   /* GPIO group 6, DRV control register 1 [R/W 32] */
#define PORT_DRV1CTRL7(n)   ((PORT_GROUP7_BASE) + 0x0084U + (n * 0x200U))   /* GPIO group 7, DRV control register 1 [R/W 32] */
#define PORT_DRV2CTRL4(n)   ((PORT_GROUP4_BASE) + 0x0088U + (n * 0x200U))   /* GPIO group 4, DRV control register 2 [R/W 32] */
#define PORT_DRV2CTRL5(n)   ((PORT_GROUP5_BASE) + 0x0088U + (n * 0x200U))   /* GPIO group 5, DRV control register 2 [R/W 32] */
#define PORT_DRV2CTRL6(n)   ((PORT_GROUP6_BASE) + 0x0088U + (n * 0x200U))   /* GPIO group 6, DRV control register 2 [R/W 32] */
#define PORT_DRV2CTRL7(n)   ((PORT_GROUP7_BASE) + 0x0088U + (n * 0x200U))   /* GPIO group 7, DRV control register 2 [R/W 32] */
#define PORT_DRV3CTRL4(n)   ((PORT_GROUP4_BASE) + 0x008cU + (n * 0x200U))   /* GPIO group 4, DRV control register 3 [R/W 32] */
#define PORT_DRV3CTRL5(n)   ((PORT_GROUP5_BASE) + 0x008cU + (n * 0x200U))   /* GPIO group 5, DRV control register 3 [R/W 32] */
#define PORT_DRV3CTRL6(n)   ((PORT_GROUP6_BASE) + 0x008cU + (n * 0x200U))   /* GPIO group 6, DRV control register 3 [R/W 32] */
#define PORT_DRV3CTRL7(n)   ((PORT_GROUP7_BASE) + 0x008cU + (n * 0x200U))   /* GPIO group 7, DRV control register 3 [R/W 32] */
/* PORT_POC */
#define PORT_PUEN4(n)       ((PORT_GROUP4_BASE) + 0x00c0U + (n * 0x200U))   /* GPIO group 4, LSI pin pull-eneble register [R/W 32] */
#define PORT_PUEN5(n)       ((PORT_GROUP5_BASE) + 0x00c0U + (n * 0x200U))   /* GPIO group 5, LSI pin pull-eneble register [R/W 32] */
#define PORT_PUEN6(n)       ((PORT_GROUP6_BASE) + 0x00c0U + (n * 0x200U))   /* GPIO group 6, LSI pin pull-eneble register [R/W 32] */
#define PORT_PUEN7(n)       ((PORT_GROUP7_BASE) + 0x00c0U + (n * 0x200U))   /* GPIO group 7, LSI pin pull-eneble register [R/W 32] */
#define PORT_PUD4(n)        ((PORT_GROUP4_BASE) + 0x00e0U + (n * 0x200U))   /* GPIO group 4, LSI pin pull-up/down control register [R/W 32] */
#define PORT_PUD5(n)        ((PORT_GROUP5_BASE) + 0x00e0U + (n * 0x200U))   /* GPIO group 5, LSI pin pull-up/down control register [R/W 32] */
#define PORT_PUD6(n)        ((PORT_GROUP6_BASE) + 0x00e0U + (n * 0x200U))   /* GPIO group 6, LSI pin pull-up/down control register [R/W 32] */
#define PORT_PUD7(n)        ((PORT_GROUP7_BASE) + 0x00e0U + (n * 0x200U))   /* GPIO group 7, LSI pin pull-up/down control register [R/W 32] */
#define PORT_MODSEL4(n)     ((PORT_GROUP4_BASE) + 0x0100U + (n * 0x200U))   /* GPIO group 4, Module select register [R/W 32] */
/* PORT_TD0SEL */
/* PORT_TD1SEL */
#define PORT_PSER4(n)       ((PORT_GROUP4_BASE) + 0x0160U + (n * 0x200U))   /* GPIO group 4, Port Safe state Enable Register [R/W 32] */
#define PORT_PSER5(n)       ((PORT_GROUP5_BASE) + 0x0160U + (n * 0x200U))   /* GPIO group 5, Port Safe state Enable Register [R/W 32] */
#define PORT_PSER6(n)       ((PORT_GROUP6_BASE) + 0x0160U + (n * 0x200U))   /* GPIO group 6, Port Safe state Enable Register [R/W 32] */
#define PORT_PSER7(n)       ((PORT_GROUP7_BASE) + 0x0160U + (n * 0x200U))   /* GPIO group 7, Port Safe state Enable Register [R/W 32] */
#define PORT_PS0SR4(n)      ((PORT_GROUP4_BASE) + 0x0164U + (n * 0x200U))   /* GPIO group 4, Port Safe state Select Register 0 [R/W 32] */
#define PORT_PS0SR5(n)      ((PORT_GROUP5_BASE) + 0x0164U + (n * 0x200U))   /* GPIO group 5, Port Safe state Select Register 0 [R/W 32] */
#define PORT_PS0SR6(n)      ((PORT_GROUP6_BASE) + 0x0164U + (n * 0x200U))   /* GPIO group 6, Port Safe state Select Register 0 [R/W 32] */
#define PORT_PS0SR7(n)      ((PORT_GROUP7_BASE) + 0x0164U + (n * 0x200U))   /* GPIO group 7, Port Safe state Select Register 0 [R/W 32] */
#define PORT_PS1SR4(n)      ((PORT_GROUP4_BASE) + 0x0168U + (n * 0x200U))   /* GPIO group 4, Port Safe state Select Register 1 [R/W 32] */
#define PORT_PS1SR5(n)      ((PORT_GROUP5_BASE) + 0x0168U + (n * 0x200U))   /* GPIO group 5, Port Safe state Select Register 1 [R/W 32] */
#define PORT_PS1SR6(n)      ((PORT_GROUP6_BASE) + 0x0168U + (n * 0x200U))   /* GPIO group 6, Port Safe state Select Register 1 [R/W 32] */
#define PORT_PS1SR7(n)      ((PORT_GROUP7_BASE) + 0x0168U + (n * 0x200U))   /* GPIO group 7, Port Safe state Select Register 1 [R/W 32] */
/* PORT_IOINTSEL */
#define PORT_INOUTSEL4(n)   ((PORT_GROUP4_BASE) + 0x0184U + (n * 0x200U))   /* GPIO group 4, General input/output switching register [R/W 32] */
#define PORT_INOUTSEL5(n)   ((PORT_GROUP5_BASE) + 0x0184U + (n * 0x200U))   /* GPIO group 5, General input/output switching register [R/W 32] */
#define PORT_INOUTSEL6(n)   ((PORT_GROUP6_BASE) + 0x0184U + (n * 0x200U))   /* GPIO group 6, General input/output switching register [R/W 32] */
#define PORT_INOUTSEL7(n)   ((PORT_GROUP7_BASE) + 0x0184U + (n * 0x200U))   /* GPIO group 7, General input/output switching register [R/W 32] */
#define PORT_OUTDT4(n)      ((PORT_GROUP4_BASE) + 0x0188U + (n * 0x200U))   /* GPIO group 4, General output register [R/W 32] */
#define PORT_OUTDT5(n)      ((PORT_GROUP5_BASE) + 0x0188U + (n * 0x200U))   /* GPIO group 5, General output register [R/W 32] */
#define PORT_OUTDT6(n)      ((PORT_GROUP6_BASE) + 0x0188U + (n * 0x200U))   /* GPIO group 6, General output register [R/W 32] */
#define PORT_OUTDT7(n)      ((PORT_GROUP7_BASE) + 0x0188U + (n * 0x200U))   /* GPIO group 7, General output register [R/W 32] */
#define PORT_INDT4(n)       ((PORT_GROUP4_BASE) + 0x018cU + (n * 0x200U))   /* GPIO group 4, General input register [R 32] */
#define PORT_INDT5(n)       ((PORT_GROUP5_BASE) + 0x018cU + (n * 0x200U))   /* GPIO group 5, General input register [R 32] */
#define PORT_INDT6(n)       ((PORT_GROUP6_BASE) + 0x018cU + (n * 0x200U))   /* GPIO group 6, General input register [R 32] */
#define PORT_INDT7(n)       ((PORT_GROUP7_BASE) + 0x018cU + (n * 0x200U))   /* GPIO group 7, General input register [R 32] */
/* PORT_INTDT */
/* PORT_INTCLR */
/* PORT_INTMSK */
/* PORT_MSKCLR */
#define PORT_POSNEG4(n)     ((PORT_GROUP4_BASE) + 0x01a0U + (n * 0x200U))   /* GPIO group 4, Positive/negative logic select register [R/W 32] */
#define PORT_POSNEG5(n)     ((PORT_GROUP5_BASE) + 0x01a0U + (n * 0x200U))   /* GPIO group 5, Positive/negative logic select register [R/W 32] */
#define PORT_POSNEG6(n)     ((PORT_GROUP6_BASE) + 0x01a0U + (n * 0x200U))   /* GPIO group 6, Positive/negative logic select register [R/W 32] */
#define PORT_POSNEG7(n)     ((PORT_GROUP7_BASE) + 0x01a0U + (n * 0x200U))   /* GPIO group 7, Positive/negative logic select register [R/W 32] */
/* PORT_EDGLEVEL */
#define PORT_FILONOFF4(n)   ((PORT_GROUP4_BASE) + 0x01a8U + (n * 0x200U))   /* GPIO group 4, Chattering prevention on/off register [R/W 32] */
#define PORT_FILONOFF5(n)   ((PORT_GROUP5_BASE) + 0x01a8U + (n * 0x200U))   /* GPIO group 5, Chattering prevention on/off register [R/W 32] */
#define PORT_FILONOFF6(n)   ((PORT_GROUP6_BASE) + 0x01a8U + (n * 0x200U))   /* GPIO group 6, Chattering prevention on/off register [R/W 32] */
#define PORT_FILONOFF7(n)   ((PORT_GROUP7_BASE) + 0x01a8U + (n * 0x200U))   /* GPIO group 7, Chattering prevention on/off register [R/W 32] */
#define PORT_FILCLKSEL4(n)  ((PORT_GROUP4_BASE) + 0x01acU + (n * 0x200U))   /* GPIO group 4, Chattering prevention clock select register [R/W 32] */
#define PORT_FILCLKSEL5(n)  ((PORT_GROUP5_BASE) + 0x01acU + (n * 0x200U))   /* GPIO group 5, Chattering prevention clock select register [R/W 32] */
#define PORT_FILCLKSEL6(n)  ((PORT_GROUP6_BASE) + 0x01acU + (n * 0x200U))   /* GPIO group 6, Chattering prevention clock select register [R/W 32] */
#define PORT_FILCLKSEL7(n)  ((PORT_GROUP7_BASE) + 0x01acU + (n * 0x200U))   /* GPIO group 7, Chattering prevention clock select register [R/W 32] */
#define PORT_OUTDTSEL4(n)   ((PORT_GROUP4_BASE) + 0x01c0U + (n * 0x200U))   /* GPIO group 4, Output data select register [R/W 32] */
#define PORT_OUTDTSEL5(n)   ((PORT_GROUP5_BASE) + 0x01c0U + (n * 0x200U))   /* GPIO group 5, Output data select register [R/W 32] */
#define PORT_OUTDTSEL6(n)   ((PORT_GROUP6_BASE) + 0x01c0U + (n * 0x200U))   /* GPIO group 6, Output data select register [R/W 32] */
#define PORT_OUTDTSEL7(n)   ((PORT_GROUP7_BASE) + 0x01c0U + (n * 0x200U))   /* GPIO group 7, Output data select register [R/W 32] */
#define PORT_OUTDTH4(n)     ((PORT_GROUP4_BASE) + 0x01c4U + (n * 0x200U))   /* GPIO group 4, Output data high register [R/W 32] */
#define PORT_OUTDTH5(n)     ((PORT_GROUP5_BASE) + 0x01c4U + (n * 0x200U))   /* GPIO group 5, Output data high register [R/W 32] */
#define PORT_OUTDTH6(n)     ((PORT_GROUP6_BASE) + 0x01c4U + (n * 0x200U))   /* GPIO group 6, Output data high register [R/W 32] */
#define PORT_OUTDTH7(n)     ((PORT_GROUP7_BASE) + 0x01c4U + (n * 0x200U))   /* GPIO group 7, Output data high register [R/W 32] */
#define PORT_OUTDTL4(n)     ((PORT_GROUP4_BASE) + 0x01c8U + (n * 0x200U))   /* GPIO group 4, Output data low register [R/W 32] */
#define PORT_OUTDTL5(n)     ((PORT_GROUP5_BASE) + 0x01c8U + (n * 0x200U))   /* GPIO group 5, Output data low register [R/W 32] */
#define PORT_OUTDTL6(n)     ((PORT_GROUP6_BASE) + 0x01c8U + (n * 0x200U))   /* GPIO group 6, Output data low register [R/W 32] */
#define PORT_OUTDTL7(n)     ((PORT_GROUP7_BASE) + 0x01c8U + (n * 0x200U))   /* GPIO group 7, Output data low register [R/W 32] */
/* PORT_BOTHEDGE */
#define PORT_INEN4(n)       ((PORT_GROUP4_BASE) + 0x01d0U + (n * 0x200U))   /* GPIO group 4, General input enable register [R/W 32] */
#define PORT_INEN5(n)       ((PORT_GROUP5_BASE) + 0x01d0U + (n * 0x200U))   /* GPIO group 5, General input enable register [R/W 32] */
#define PORT_INEN6(n)       ((PORT_GROUP6_BASE) + 0x01d0U + (n * 0x200U))   /* GPIO group 6, General input enable register [R/W 32] */
#define PORT_INEN7(n)       ((PORT_GROUP7_BASE) + 0x01d0U + (n * 0x200U))   /* GPIO group 7, General input enable register [R/W 32] */

/* PORT_DM0PRSYS0 */
/* PORT_DM1PRSYS0 */
/* PORT_DM2PRSYS0 */
/* PORT_DM3PRSYS0 */
/* PORT_DRV0CTRLSYS0 */
#define PORT_DRV1CTRLSYS1(n)    ((PORT_SYSGROUP1_BASE) + 0x0084U + (n * 0x200U))    /* DRV control register 1 [R/W 32] */
#define PORT_DRV2CTRLSYS1(n)    ((PORT_SYSGROUP1_BASE) + 0x0088U + (n * 0x200U))    /* DRV control register 2 [R/W 32] */
#define PORT_PUENSYS1(n)        ((PORT_SYSGROUP1_BASE) + 0x00c0U + (n * 0x200U))    /* LSI pin pull-eneble register [R/W 32] */
#define PORT_PUDSYS1(n)         ((PORT_SYSGROUP1_BASE) + 0x00e0U + (n * 0x200U))    /* LSI pin pull-up/down control register [R/W 32] */

 #if 0
 PMMERSYS1_B0A0    IOR[R/W 32](4)    0xffd98004  
 PMMERSYS1_B0A1    IOR[R/W 32](4)    0xffd98204  
 PMMERSYS1_B0A2    IOR[R/W 32](4)    0xffd98404  
 PMMRSYS1_B0A0     IOR[R/W 32](4)    0xffd98000  
 PMMRSYS1_B0A1     IOR[R/W 32](4)    0xffd98200  
 PMMRSYS1_B0A2     IOR[R/W 32](4)    0xffd98400  
 #endif

#define PORT_PNOT4(n)       ((PORT_GROUP4_BASE) + 0x01d4U + (n * 0x200U))   /* GPIO group 4, Port NOT Register [R/W 32] */
#define PORT_PNOT5(n)       ((PORT_GROUP5_BASE) + 0x01d4U + (n * 0x200U))   /* GPIO group 5, Port NOT Register [R/W 32] */
#define PORT_PNOT6(n)       ((PORT_GROUP6_BASE) + 0x01d4U + (n * 0x200U))   /* GPIO group 6, Port NOT Register [R/W 32] */
#define PORT_PNOT7(n)       ((PORT_GROUP7_BASE) + 0x01d4U + (n * 0x200U))   /* GPIO group 7, Port NOT Register [R/W 32] */
#define PORT_PINV4(n)       ((PORT_GROUP4_BASE) + 0x016cU + (n * 0x200U))   /* GPIO group 4, Port Output Value Inversion Register [R/W 32] */
#define PORT_PINV5(n)       ((PORT_GROUP5_BASE) + 0x016cU + (n * 0x200U))   /* GPIO group 5, Port Output Value Inversion Register [R/W 32] */
#define PORT_PINV6(n)       ((PORT_GROUP6_BASE) + 0x016cU + (n * 0x200U))   /* GPIO group 6, Port Output Value Inversion Register [R/W 32] */
#define PORT_PINV7(n)       ((PORT_GROUP7_BASE) + 0x016cU + (n * 0x200U))   /* GPIO group 7, Port Output Value Inversion Register [R/W 32] */
#define PORT_PIS5(n)        ((PORT_GROUP5_BASE) + 0x0170U + (n * 0x200U))   /* GPIO group 5, Port Input Buffer Selection Register [R/W 32] */


#define ECCCNT_A_TPTM_BASE  0xffc4d380
#define ECCCNT_D_TPTM_BASE  0xffc4f980
#define GUARD_TPTM_BASE     0xffc6a600
#define TPTM_BASE           0xfffbb000

/* ECCCNT_A_TPTM */
#define ECCCNT_A_TPTM_BUSAECCCTL        ((ECCCNT_A_TPTM_BASE) + 0x00U)      /* ECC control register [R/W 32] */
#define ECCCNT_A_TPTM_BUSAECCTSTCTL     ((ECCCNT_A_TPTM_BASE) + 0x10U)      /* ECC test control register [R/W 32] */
#define ECCCNT_A_TPTM_BUSAECCTSTDIN0    ((ECCCNT_A_TPTM_BASE) + 0x30U)      /* Test data input register [R/W 32] */
#define ECCCNT_A_TPTM_BUSAECCTSTEIN     ((ECCCNT_A_TPTM_BASE) + 0x40U)      /* Test ECC input register [R/W 32] */

/* ECCCNT_D_TPTM */
#define ECCCNT_D_TPTM_BUSDECCCTL        ((ECCCNT_D_TPTM_BASE) + 0x00U)      /* ECC control register [R/W 32] */

/* GUARD_TPTM */
#define GUARD_TPTM_TPTGOVFCLR           ((GUARD_TPTM_BASE) + 0x00U)         /* Guad error overflow clear register [R/W 32] */
#define GUARD_TPTM_TPTGOVFSTAT          ((GUARD_TPTM_BASE) + 0x04U)         /* Guad error overflow status register [R 32] */
#define GUARD_TPTM_TPTGERRADDR          ((GUARD_TPTM_BASE) + 0x08U)         /* Guad error adddress register [R 32] */
#define GUARD_TPTM_TPTGERRTYPE          ((GUARD_TPTM_BASE) + 0x0cU)         /* Guad error access information register [R 32] */
#define GUARD_TPTM_TPTGSPIDERRCLR       ((GUARD_TPTM_BASE) + 0x10U)         /* Guad SPID error clear register [W 32] */
#define GUARD_TPTM_TPTGSPIDERRSTAT      ((GUARD_TPTM_BASE) + 0x14U)         /* Guad SPID error status register [R 32] */
#define GUARD_TPTM_TPTGKCPROT           ((GUARD_TPTM_BASE) + 0x18U)         /* Key code protection register [R/W 32] */
#define GUARD_TPTM_TPTGPROT0_n(n)       ((GUARD_TPTM_BASE) + 0x80U + (n * 0x8U))    /* Channel protection control register [R/W 32] */
#define GUARD_TPTM_TPTGPROT1_n(n)       ((GUARD_TPTM_BASE) + 0x84U + (n * 0x8U))    /* Channel SPID stting register [R/W 32] *//

/* TPTMG0 */
#define TPTM_TPTMG0URUN     ((TPTM_BASE) + 0x940U)                          /* Simultaneous Counter Start Register of Up Timer 0 [R/W 32] */
#define TPTM_TPTMG0URRUN    ((TPTM_BASE) + 0x944U)                          /* Simultaneous Counter Restart Registerof Up Timer 0 [R/W 32] */
#define TPTM_TPTMG0USTP     ((TPTM_BASE) + 0x948U)                          /* Simultaneous Counter Stop Register of Up Timer 0 [R/W 32] */
/* TPTMG1 */
#define TPTM_TPTMG1URUN     ((TPTM_BASE) + 0xa40U)                          /* Simultaneous Counter Start Register of Up Timer 1 [R/W 32] */
#define TPTM_TPTMG1URRUN    ((TPTM_BASE) + 0xa44U)                          /* Simultaneous Counter Restart Registerof Up Timer 1 [R/W 32] */
#define TPTM_TPTMG1USTP     ((TPTM_BASE) + 0xa48U)                          /* Simultaneous Counter Stop Register of Up Timer 1 [R/W 32] */
/* TPTMPE0, TPTMPE1 */
#define TPTM_TPTMnIRUN(n)   ((TPTM_BASE) + (n * 0x100U) + 0x0100U + 0x00U)  /* Counter Start Register of Interval Timer for PEn [R/W 32] */
#define TPTM_TPTMnIRRUN(n)  ((TPTM_BASE) + (n * 0x100U) + 0x0100U + 0x04U)  /* Counter Restart Register of Interval Timer for PEn [R/W 32] */
#define TPTM_TPTMnISTP(n)   ((TPTM_BASE) + (n * 0x100U) + 0x0100U + 0x08U)  /* Counter Stop Register of Interval Timer for PEn [R/W 32] */
#define TPTM_TPTMnISTR(n)   ((TPTM_BASE) + (n * 0x100U) + 0x0100U + 0x0cU)  /* Counter Status Register of Interval Timer for PEn [R 32] */
#define TPTM_TPTMnIIEN(n)   ((TPTM_BASE) + (n * 0x100U) + 0x0100U + 0x10U)  /* Interrupt Enable Register of Interval Timer for PEn [R/W 32] */
#define TPTM_TPTMnIUSTR(n)  ((TPTM_BASE) + (n * 0x100U) + 0x0100U + 0x14U)  /* Underflow Status Register of Interval Timer for PEn [R/W 32] */
#define TPTM_TPTMnIDIV(n)   ((TPTM_BASE) + (n * 0x100U) + 0x0100U + 0x18U)  /* Divider Register of Interval Timer for PEn [R/W 32] */
#define TPTM_TPTMnFRUN(n)   ((TPTM_BASE) + (n * 0x100U) + 0x0100U + 0x20U)  /* Counter Start Register of Free-run Timer for PEn [R/W 32] */
#define TPTM_TPTMnFRRUN(n)  ((TPTM_BASE) + (n * 0x100U) + 0x0100U + 0x24U)  /* Counter Restart Register of Free-run Timer for PEn [R/W 32] */
#define TPTM_TPTMnFSTP(n)   ((TPTM_BASE) + (n * 0x100U) + 0x0100U + 0x28U)  /* Counter Stop Register of Free-run Timer for PEn [R/W 32] */
#define TPTM_TPTMnFSTR(n)   ((TPTM_BASE) + (n * 0x100U) + 0x0100U + 0x2cU)  /* Counter Status Register of Free-run Timer for PEn [R 32] */
#define TPTM_TPTMnFDIV(n)   ((TPTM_BASE) + (n * 0x100U) + 0x0100U + 0x30U)  /* Divider Register of Free-run Timer for PEn [R/W 32] */
#define TPTM_TPTMnURUN(n)   ((TPTM_BASE) + (n * 0x100U) + 0x0100U + 0x40U)  /* Counter Start Register of Up Timer for PEn [R/W 32] */
#define TPTM_TPTMnURRUN(n)  ((TPTM_BASE) + (n * 0x100U) + 0x0100U + 0x44U)  /* Counter Restart Register of Up Timer for PEn [R/W 32] */
#define TPTM_TPTMnUSTP(n)   ((TPTM_BASE) + (n * 0x100U) + 0x0100U + 0x48U)  /* Counter Stop Register of Up Timer for PEn [R/W 32] */
#define TPTM_TPTMnUSTR(n)   ((TPTM_BASE) + (n * 0x100U) + 0x0100U + 0x4cU)  /* Counter Status Register of Up Timer for PEn [R 32] */
#define TPTM_TPTMnUIEN(n)   ((TPTM_BASE) + (n * 0x100U) + 0x0100U + 0x50U)  /* Interrupt Enable Register of Up Timer for PEn [R/W 32] */
#define TPTM_TPTMnUDIV(n)   ((TPTM_BASE) + (n * 0x100U) + 0x0100U + 0x58U)  /* Divider Register of Up Timer for PEn [R/W 32] */
#define TPTM_TPTMnUTRG(n)   ((TPTM_BASE) + (n * 0x100U) + 0x0100U + 0x5cU)  /* Triggrer Select Register of Up Timer for PEn [R/W 32] */
#define TPTM_TPTMnICNT0(n)  ((TPTM_BASE) + (n * 0x100U) + 0x0100U + 0x80U)  /* Counter Channel 0 Register of Interval Timer for PE0 [R/W 32] */
#define TPTM_TPTMnILD0(n)   ((TPTM_BASE) + (n * 0x100U) + 0x0100U + 0x84U)  /* Load Channel 0 Register of Interval Timer for PE0 [R/W 32] */
#define TPTM_TPTMnICNT1(n)  ((TPTM_BASE) + (n * 0x100U) + 0x0100U + 0x88U)  /* Counter Channel1 Register of Interval Timer for PE0 [R/W 32] */
#define TPTM_TPTMnILD1(n)   ((TPTM_BASE) + (n * 0x100U) + 0x0100U + 0x8cU)  /* Load Channel 1 Register of Interval Timer for PE0 [R/W 32] */
#define TPTM_TPTMnICNT2(n)  ((TPTM_BASE) + (n * 0x100U) + 0x0100U + 0x90U)  /* Counter Channel 2 Register of Interval Timer for PE0 [R/W 32] */
#define TPTM_TPTMnILD2(n)   ((TPTM_BASE) + (n * 0x100U) + 0x0100U + 0x94U)  /* Load Channel 2 Register of Interval Timer for PE0 [R/W 32] */
#define TPTM_TPTMnICNT3(n)  ((TPTM_BASE) + (n * 0x100U) + 0x0100U + 0x98U)  /* Counter Channel 3 Register of Interval Timer for PE0 [R/W 32] */
#define TPTM_TPTMnILD3(n)   ((TPTM_BASE) + (n * 0x100U) + 0x0100U + 0x9cU)  /* Load Channel 3 Register of Interval Timer for PE0 [R/W 32] */
#define TPTM_TPTMnFCNT(n)   ((TPTM_BASE) + (n * 0x100U) + 0x0100U + 0xa0U)  /* Counter Register of Free-run Timer for PEn [R/W 32] */
#define TPTM_TPTMnUCNT0(n)  ((TPTM_BASE) + (n * 0x100U) + 0x0100U + 0xc0U)  /* Counter Channel 0 Register of Up Timer for PEn [R/W 32] */
#define TPTM_TPTMnUCMP00(n) ((TPTM_BASE) + (n * 0x100U) + 0x0100U + 0xc4U)  /* Compare Value 0 Register of Up Timer 0 for PEn [R/W 32] */
#define TPTM_TPTMnUCMP01(n) ((TPTM_BASE) + (n * 0x100U) + 0x0100U + 0xc8U)  /* Compare Value 1 Register of Up Timer 0 for PEn [R/W 32] */
#define TPTM_TPTMnUCMP02(n) ((TPTM_BASE) + (n * 0x100U) + 0x0100U + 0xccU)  /* Compare Value 2 Register of Up Timer 0 for PEn [R/W 32] */
#define TPTM_TPTMnUCMP03(n) ((TPTM_BASE) + (n * 0x100U) + 0x0100U + 0xd0U)  /* Compare Value 3 Register of Up Timer 0 for PEn [R/W 32] */
#define TPTM_TPTMnUCNT1(n)  ((TPTM_BASE) + (n * 0x100U) + 0x0100U + 0xe0U)  /* Counter Channel 1 Register of Up Timer for PEn [R/W 32] */
#define TPTM_TPTMnUCMP10(n) ((TPTM_BASE) + (n * 0x100U) + 0x0100U + 0xe4U)  /* Compare Value 0 Register of Up Timer 1 for PEn [R/W 32] */
#define TPTM_TPTMnUCMP11(n) ((TPTM_BASE) + (n * 0x100U) + 0x0100U + 0xe8U)  /* Compare Value 1 Register of Up Timer 1 for PEn [R/W 32] */
#define TPTM_TPTMnUCMP12(n) ((TPTM_BASE) + (n * 0x100U) + 0x0100U + 0xecU)  /* Compare Value 2 Register of Up Timer 1 for PEn [R/W 32] */
#define TPTM_TPTMnUCMP13(n) ((TPTM_BASE) + (n * 0x100U) + 0x0100U + 0xf0U)  /* Compare Value 3 Register of Up Timer 1 for PEn [R/W 32] */

/* TPTMSELF */
#define TPTM_TPTMSIRUN      ((TPTM_BASE) + 0x00U)                           /* Counter Start Register of Interval Timer Self [R/W 32] */
#define TPTM_TPTMSIRRUN     ((TPTM_BASE) + 0x04U)                           /* Counter Restart Register of Interval Timer Self [R/W 32] */
#define TPTM_TPTMSISTP      ((TPTM_BASE) + 0x08U)                           /* Counter Stop Register of Interval Timer Self [R/W 32] */
#define TPTM_TPTMSISTR      ((TPTM_BASE) + 0x0cU)                           /* Counter Status Register of Interval Timer Self [R 32] */
#define TPTM_TPTMSIIEN      ((TPTM_BASE) + 0x10U)                           /* Interrupt Enable Register of Interval Timer Self [R/W 32] */
#define TPTM_TPTMSIUSTR     ((TPTM_BASE) + 0x14U)                           /* Underflow Status Register of Interval Timer Self [R/W 32] */
#define TPTM_TPTMSIDIV      ((TPTM_BASE) + 0x18U)                           /* Divider Register of Interval Timer Self [R/W 32] */
#define TPTM_TPTMSFRUN      ((TPTM_BASE) + 0x20U)                           /* Counter Start Register of Free-run Timer Self [R/W 32] */
#define TPTM_TPTMSFRRUN     ((TPTM_BASE) + 0x24U)                           /* Counter Restart Register of Free-run Timer Self [R/W 32] */
#define TPTM_TPTMSFSTP      ((TPTM_BASE) + 0x28U)                           /* Counter Stop Register of Free-run Timer Self [R/W 32] */
#define TPTM_TPTMSFSTR      ((TPTM_BASE) + 0x2cU)                           /* Counter Status Register of Free-run Timer Self [R 32] */
#define TPTM_TPTMSFDIV      ((TPTM_BASE) + 0x30U)                           /* Divider Register of Free-run Timer Self [R/W 32] */
#define TPTM_TPTMSURUN      ((TPTM_BASE) + 0x40U)                           /* Counter Start Register of Up Timer Self [R/W 32] */
#define TPTM_TPTMSURRUN     ((TPTM_BASE) + 0x44U)                           /* Counter Stop Register of Up Timer Self [R/W 32] */
#define TPTM_TPTMSUSTP      ((TPTM_BASE) + 0x48U)                           /* Counter Stop Register of Up Timer Self [R/W 32] */
#define TPTM_TPTMSUSTR      ((TPTM_BASE) + 0x4cU)                           /* Counter Status Register of Up Timer Self [R 32] */
#define TPTM_TPTMSUIEN      ((TPTM_BASE) + 0x50U)                           /* Interrupt Enable Register of Up Timer Self [R/W 32] */
#define TPTM_TPTMSUDIV      ((TPTM_BASE) + 0x58U)                           /* Divider Register of Up Timer Self [R/W 32] */
#define TPTM_TPTMSUTRG      ((TPTM_BASE) + 0x5cU)                           /* Triggrer Select Register of Up Timer Self [R/W 32] */
#define TPTM_TPTMSICNT0     ((TPTM_BASE) + 0x80U)                           /* Counter Channel 0 Register of Interval Timer Self [R/W 32] */
#define TPTM_TPTMSILD0      ((TPTM_BASE) + 0x84U)                           /* Load Channel 0 Register of Interval Timer Self [R/W 32] */
#define TPTM_TPTMSICNT1     ((TPTM_BASE) + 0x88U)                           /* Counter Channel1 Register of Interval Timer Self [R/W 32] */
#define TPTM_TPTMSILD1      ((TPTM_BASE) + 0x8cU)                           /* Load Channel 1 Register of Interval Timer Self [R/W 32] */
#define TPTM_TPTMSICNT2     ((TPTM_BASE) + 0x90U)                           /* Counter Channel 2 Register of Interval Timer Self [R/W 32] */
#define TPTM_TPTMSILD2      ((TPTM_BASE) + 0x94U)                           /* Load Channel 2 Register of Interval Timer Self [R/W 32] */
#define TPTM_TPTMSICNT3     ((TPTM_BASE) + 0x98U)                           /* Counter Channel 3 Register of Interval Timer Self [R/W 32] */
#define TPTM_TPTMSILD3      ((TPTM_BASE) + 0x9cU)                           /* Load Channel 3 Register of Interval Timer Self [R/W 32] */
#define TPTM_TPTMSFCNT      ((TPTM_BASE) + 0xa0U)                           /* Counter Register of Free-run Timer Self [R/W 32] */
#define TPTM_TPTMSUCNT0     ((TPTM_BASE) + 0xc0U)                           /* Counter Channel 0 Register of Up Timer Self [R/W 32] */
#define TPTM_TPTMSUCMP00    ((TPTM_BASE) + 0xc4U)                           /* Compare Value 0 Register of Up Timer 0 Self [R/W 32] */
#define TPTM_TPTMSUCMP01    ((TPTM_BASE) + 0xc8U)                           /* Compare Value 1 Register of Up Timer 0 Self [R/W 32] */
#define TPTM_TPTMSUCMP02    ((TPTM_BASE) + 0xccU)                           /* Compare Value 2 Register of Up Timer 0 Self [R/W 32] */
#define TPTM_TPTMSUCMP03    ((TPTM_BASE) + 0xd0U)                           /* Compare Value 3 Register of Up Timer 0 Self [R/W 32] */
#define TPTM_TPTMSUCNT1     ((TPTM_BASE) + 0xe0U)                           /* Counter Channel 1 Register of Up Timer Self [R/W 32] */
#define TPTM_TPTMSUCMP10    ((TPTM_BASE) + 0xe4U)                           /* Compare Value 0 Register of Up Timer 1 Self [R/W 32] */
#define TPTM_TPTMSUCMP11    ((TPTM_BASE) + 0xe8U)                           /* Compare Value 1 Register of Up Timer 1 Self [R/W 32] */
#define TPTM_TPTMSUCMP12    ((TPTM_BASE) + 0xecU)                           /* Compare Value 2 Register of Up Timer 1 Self [R/W 32] */
#define TPTM_TPTMSUCMP13    ((TPTM_BASE) + 0xf0U)                           /* Compare Value 3 Register of Up Timer 1 Self [R/W 32] */

#if 0
#define INTC1_SELF_BASE     0xfffc0000
#define INTC1_PE0_BASE      0xfffc4000
#define INTC1_PE1_BASE      0xfffc8000
#define INTC2_BASE          0xfff80000
#define INTIF_BASE          0xff090000
#define EINT_BASE           0xffc00000
#define FENC_BASE           0xff9a3a00
#define FEINC_PE0_BASE      0xff9a3b00
#define FEINC_PE1_BASE      0xff9a3c00
#define INTSWCD_BASE        0xf8f11000

/* INTC1_SELF */
#define INTC1_SELF_EIC(n)   ((INTC1_SELF_BASE) + 0x0000U + (n * 0x2U))      /* EI level Interrupt Control Register [R/W 16] (n=0～31) */
#define INTC1_SELF_IMR0     ((INTC1_SELF_BASE) + 0x00f0U)                   /* EI level Interrupt Mask Register [R/W 32] */
#define INTC1_SELF_EIBD(n)  ((INTC1_SELF_BASE) + 0x0100U + (n * 0x4U))      /* EI lebel Interrupt Bind Register [R 32] (n=0～31) */
#define INTC1_SELF_FIBD     ((INTC1_SELF_BASE) + 0x01c0U)                   /* FE lebel Interrupt Bind Register [R/W 32] */
    /*  INTC1_SELF.FIBDHL          [R 16](2)      0xfffc01c2  */
    /*  INTC1_SELF.EITB            [R 1.8](1)     0xfffc01c2  */
    /*  INTC1_SELF.EIMK            [R 1.8](1)     0xfffc01c3  */
#define INTC1_SELF_EEIC(n)  ((INTC1_SELF_BASE) + 0x0200U + (n * 0x4U))      /* Extended EI level Interrupt Control Register [R/W 32] (n=0～31) */
#define INTC1_SELF_EIBG     ((INTC1_SELF_BASE) + 0x0280U)                   /* BGEIINT Priority Lebel Setting Register [R/W 32] */
#define INTC1_SELF_FIBG     ((INTC1_SELF_BASE) + 0x02c0U)                   /* BGFEINT Channel Mask Setting Register [R/W 32] */
#define INTC1_SELF_IHVCFG   ((INTC1_SELF_BASE) + 0x02f0U)                   /* INTC1 Virtualization Configuration Register [R/W 32] */
/* INTC1_PE0 */
#define INTC1_PE0_EIC(n)    ((INTC1_PE0_BASE) + 0x0000U + (n * 0x2U))       /* EI level Interrupt Control Register [R/W 16] (n=0～31) */
#define INTC1_PE0_IMR0      ((INTC1_PE0_BASE) + 0x00f0U)                    /* EI level Interrupt Mask Register [R/W 32] */
#define INTC1_PE0_EIBD(n)   ((INTC1_PE0_BASE) + 0x0100U + (n * 0x4U))       /* EI lebel Interrupt Bind Register [R 32] (n=0～31) */
#define INTC1_PE0_FIBD      ((INTC1_PE0_BASE) + 0x01c0U)                    /* FE lebel Interrupt Bind Register [R/W 32] */
    /*  INTC1_PE0.FIBDHL           [R 16](2)      0xfffc41c2  */
    /*  INTC1_PE0.EITB             [R 1.8](1)     0xfffc41c2  */
    /*  INTC1_PE0.EIMK             [R 1.8](1)     0xfffc41c3  */
#define INTC1_PE0_EEIC(n)   ((INTC1_PE0_BASE) + 0x0200U + (n * 0x4U))       /* Extended EI level Interrupt Control Register [R/W 32] (n=0～31) */
#define INTC1_PE0_EIBG      ((INTC1_PE0_BASE) + 0x0280U)                    /* BGEIINT Priority Lebel Setting Register [R/W 32] */
#define INTC1_PE0_FIBG      ((INTC1_PE0_BASE) + 0x02c0U)                    /* BGFEINT Channel Mask Setting Register [R/W 32] */
#define INTC1_PE0_IHVCFG    ((INTC1_PE0_BASE) + 0x02f0U)                    /* INTC1 Virtualization Configuration Register [R/W 32] */
/* INTC1_PE1 */
#define INTC1_PE1_EIC(n)    ((INTC1_PE1_BASE) + 0x0000U + (n * 0x2U))       /* EI level Interrupt Control Register [R/W 16] (n=0～31) */
#define INTC1_PE1_IMR0      ((INTC1_PE1_BASE) + 0x00f0U)                    /* EI level Interrupt Mask Register [R/W 32] */
#define INTC1_PE1_EIBD(n)   ((INTC1_PE1_BASE) + 0x0100U + (n * 0x4U))       /* EI lebel Interrupt Bind Register [R 32] (n=0～31) */
#define INTC1_PE1_FIBD      ((INTC1_PE1_BASE) + 0x01c0U)                    /* FE lebel Interrupt Bind Register [R/W 32] */
    /*  INTC1_PE1.FIBDHL           [R 16](2)      0xfffc81c2  */
    /*  INTC1_PE1.EITB             [R 1.8](1)     0xfffc81c2  */
    /*  INTC1_PE1.EIMK             [R 1.8](1)     0xfffc81c3  */
#define INTC1_PE1_EEIC(n)   ((INTC1_PE1_BASE) + 0x0200U + (n * 0x4U))       /* Extended EI level Interrupt Control Register [R/W 32] (n=0～31) */
#define INTC1_PE1_EIBG      ((INTC1_PE1_BASE) + 0x0280U)                    /* BGEIINT Priority Lebel Setting Register [R/W 32] */
#define INTC1_PE1_FIBG      ((INTC1_PE1_BASE) + 0x02c0U)                    /* BGFEINT Channel Mask Setting Register [R/W 32] */
#define INTC1_PE1_IHVCFG    ((INTC1_PE1_BASE) + 0x02f0U)                    /* INTC1 Virtualization Configuration Register [R/W 32] */
/* INTC2 */
#define INTC2_EIC(n)        ((INTC2_BASE) + 0x0000U + (n * 0x2U))           /* EI level Interrupt Control Register [R/W 16] (n=32～767) */
#define INTC2_IMR(n)        ((INTC2_BASE) + 0x1000U + (n * 0x4U))           /* EI level Interrupt Mask Register [R/W 32] (n=1～23) */
#define INTC2_I2EIBG(n)     ((INTC2_BASE) + 0x1fe0U + (n * 0x4U))           /* INTC2 BGEIINT Priority Lebel Setting Register [R/W 32] (n=0～1) */
    /*  INTC2.I2EIBG2              [R/W 32](4)    0xfff81fe8  */
    /*  INTC2.I2EIBG3              [R/W 32](4)    0xfff81fec  */
#define INTC2_EIBD(n)       ((INTC2_BASE) + 0x2000U + (n * 0x4U))           /* EI lebel Interrupt Bind Register [R/W 32] (n=32～767) */
#define INTC2_EEIC(n)       ((INTC2_BASE) + 0x4000U + (n * 0x4U))           /* Extended EI level Interrupt Control Register [R/W 32] (n=32～767) */
/* INTIF */
#define INTIF_PINT(n)       ((INTIF_BASE) + 0x0000U + (n * 0x4U))           /* Peipheral Interrupt Status Register [R 32] (n=0～7) */
#define INTIF_PINTCLR(n)    ((INTIF_BASE) + 0x0020U + (n * 0x4U))           /* Peipheral Interrupt Status Clear Register [W 32] (n=0～7) */
#define INTIF_TPTMSEL       ((INTIF_BASE) + 0x0200U)                        /* TPTM Interrupt FE EI Select Register [R/W 32] */
/* INTSWCD */
#define INTSWCD_APPLINTSTSR(n)  ((INTSWCD_BASE) + 0x00U + (n * 0x4U))       /* INT_to_APP_L interrupt status register [R 32] (n=0～9,11～13) */
#define INTSWCD_APPLINTMSKR(n)  ((INTSWCD_BASE) + 0x40U + (n * 0x4U))       /* INT_to_APP_L interrupt mask register [R/W 32] (n=0～9,11～13) */
#define INTSWCD_INTSWCDCOMPCR   ((INTSWCD_BASE) + 0x80U)                    /* INTSW Control Domain Compare function Control register [R/W 32] */
#define INTSWCD_INTSWCDCOMPSR   ((INTSWCD_BASE) + 0x84U)                    /* INTSW Control Domain Compare function Status register [R 32] */
#define INTSWCD_INTSWCDCOMPCLR  ((INTSWCD_BASE) + 0x88U)                    /* INTSW Control Domain Compare function Clear register [R/W 32] */
#define INTSWCD_APPPINTSTSR(n)  ((INTSWCD_BASE) + 0x200U + (n * 0x4U))      /* INT_to_APP_P interrupt status register [R 32] (n=0～16) */
#define INTSWCD_APPPINTMSKR(n)  ((INTSWCD_BASE) + 0x250U + (n * 0x4U))      /* INT_to_APP_P interrupt mask register [R/W 32] (n=0～16) */
#define INTSWCD_APPPINTCLRR(n)  ((INTSWCD_BASE) + 0x2a0U + (n * 0x4U))      /* INT_to_APP_P interrupt clear register [R/W 32] (n=0～16) */

/* EINT */
#define EINT_SINTR(n)           ((EINT_BASE) + 0x00U + (n * 0x4U))          /* Software Interrupt Register [R/W 1.8] (n=0～3) */
/* FENC */
#define FENC_FENMIF             ((FENC_BASE) + 0x0U)                        /* [R/W 32] */
#define FENC_FENMIC             ((FENC_BASE) + 0x8U)                        /* [R/W 32] */
/* FEINC_PE0 */
#define FEINC_PE0_FEINTF        ((FEINC_PE0_BASE) + 0x0U)                   /* [R/W 32] */
#define FEINC_PE0_FEINTMSK      ((FEINC_PE0_BASE) + 0x4U)                   /* [R/W 32] */
#define FEINC_PE0_FEINTC        ((FEINC_PE0_BASE) + 0x8U)                   /* [R/W 32] */
/* FEINC_PE1 */
#define FEINC_PE1_FEINTF        ((FEINC_PE1_BASE) + 0x0U)                   /* [R/W 32] */
#define FEINC_PE1_FEINTMSK      ((FEINC_PE1_BASE) + 0x4U)                   /* [R/W 32] */
#define FEINC_PE1_FEINTC        ((FEINC_PE1_BASE) + 0x8U)                   /* [R/W 32] */
#endif

#define RLIN30_BASE         (0xFFD28000)
#define RLIN31_BASE         (0xFFC7C000)
#define RLIN32_BASE         (0xFFD28200)
#define RLIN33_BASE         (0xFFC7C080)
#define RLIN34_BASE         (0xFFD28400)
#define RLIN35_BASE         (0xFFC7C100)
#define RLIN36_BASE         (0xFFD28600)
#define RLIN37_BASE         (0xFFC7C180)
#define RLIN38_BASE         (0xFFD28800)
#define RLIN39_BASE         (0xFFC7C200)
#define RLIN310_BASE        (0xFFD28C00)
#define RLIN311_BASE        (0xFFC7C280)

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

#define MSR_TAUJ_ISO        0xFF981140
#define MSR_TAUJ_AWO        0xFF988E20
#define MSR_RLIN3           0xFF981060
#define MSR_OSTM            0xFF981180
#define MSRKCPROT           0xFF981710

#define OSTM0_BASE          0xFFBF0000
#define OSTM1_BASE          0xFFBF0100
#define OSTM2_BASE          0xFFBF0200
#define OSTM3_BASE          0xFFBF0300
#define OSTM4_BASE          0xFFBF0400
#define OSTM5_BASE          0xFFBF0500
#define OSTM6_BASE          0xFFBF0600
#define OSTM7_BASE          0xFFBF0700
#define OSTM8_BASE          0xFFBF0800
#define OSTM9_BASE          0xFFBF0900

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

#define RLIN30_RX_INTNO     418
#define RLIN31_RX_INTNO     422
#define RLIN32_RX_INTNO     426
#define RLIN33_RX_INTNO     430
#define RLIN34_RX_INTNO     434
#define RLIN35_RX_INTNO     438
#define RLIN36_RX_INTNO     442
#define RLIN37_RX_INTNO     446
#define RLIN38_RX_INTNO     450
#define RLIN39_RX_INTNO     454
#define RLIN310_RX_INTNO    458
#define RLIN311_RX_INTNO    462

/*
 *  INTC
 */
#define INTC1_BASE          0xFFFC0000
#define INTC2_BASE          0xFFF80000

#define INTC2_EIC           0x0000
#define INTC2_EEIC          0x4000
#define INTC2_EIBD          0x2000
#define INTC2_INTNO_OFFSET  32

/* intno は unsigned を想定 */
#define EIC_ADDRESS(intno)	(intno <= 31)? (INTC1_BASE + (intno * 2)) : (INTC2_BASE + INTC2_EIC  + (intno * 2))
#define EEIC_ADDRESS(intno)	(intno <= 31)? (INTC1_BASE + (intno * 2)) : (INTC2_BASE + INTC2_EEIC  + (intno * 4))

#define EIBD_ADDRESS(intno) (INTC2_BASE + INTC2_EIBD  + (intno * 4))

#define TMIN_INTNO          UINT_C(0)
#define TMAX_INTNO          UINT_C(767)
#define TNUM_INT            UINT_C(768)

#include "v850.h"

#endif /* TOPPERS_RCAR_S4_H */
