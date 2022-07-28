#ifndef _RCARS4_H_
#define _RCARS4_H_

/*
 *  物理コア数
 */
#define TNUM_PHYS_CORE    (2U)
  
/*
 *  R-Car S4共通部のインクルード
 */
#include "u2a.h"

#define PORT_GROUP4_BASE    0xffd90000
#define PORT_GROUP5_BASE    0xffd90800
#define PORT_GROUP6_BASE    0xffd91000
#define PORT_GROUP7_BASE    0xffd91800
#define PORT_SYSGROUP1_BASE 0xffd98000

/* 端子機能設定  (USE)*/
#define PORT_PMMR4(n)       ((uint32_t*)(PORT_GROUP4_BASE + 0x0000U + (n * 0x200U)))   /* GPIO group 4, LSI Multiplexed Pin Setting Mask Register [R/W 32] */
#define PORT_PMMR5(n)       ((uint32_t*)(PORT_GROUP5_BASE + 0x0000U + (n * 0x200U)))   /* GPIO group 5, LSI Multiplexed Pin Setting Mask Register [R/W 32] */
#define PORT_PMMR6(n)       ((uint32_t*)(PORT_GROUP6_BASE + 0x0000U + (n * 0x200U)))   /* GPIO group 6, LSI Multiplexed Pin Setting Mask Register [R/W 32] */
#define PORT_PMMR7(n)       ((uint32_t*)(PORT_GROUP7_BASE + 0x0000U + (n * 0x200U)))   /* GPIO group 7, LSI Multiplexed Pin Setting Mask Register [R/W 32] */
#define PORT_PMMER4(n)      ((uint32_t*)(PORT_GROUP4_BASE + 0x0004U + (n * 0x200U)))   /* GPIO group 4, LSI Multiplexed Pin Setting Mask Enable Register [R/W 32] */
#define PORT_PMMER5(n)      ((uint32_t*)(PORT_GROUP5_BASE + 0x0004U + (n * 0x200U)))   /* GPIO group 5, LSI Multiplexed Pin Setting Mask Enable Register [R/W 32] */
#define PORT_PMMER6(n)      ((uint32_t*)(PORT_GROUP6_BASE + 0x0004U + (n * 0x200U)))   /* GPIO group 6, LSI Multiplexed Pin Setting Mask Enable Register [R/W 32] */
#define PORT_PMMER7(n)      ((uint32_t*)(PORT_GROUP7_BASE + 0x0004U + (n * 0x200U)))   /* GPIO group 7, LSI Multiplexed Pin Setting Mask Enable Register [R/W 32] */
/* PORT_DM0PR */
#define PORT_DM1PR4(n)      ((uint32_t*)(PORT_GROUP4_BASE + 0x0024U + (n * 0x200U)))   /* GPIO group 4, Dmain Protection register 1 [R/W 32] */
#define PORT_DM1PR5(n)      ((uint32_t*)(PORT_GROUP5_BASE + 0x0024U + (n * 0x200U)))   /* GPIO group 5, Dmain Protection register 1 [R/W 32] */
#define PORT_DM1PR6(n)      ((uint32_t*)(PORT_GROUP6_BASE + 0x0024U + (n * 0x200U)))   /* GPIO group 6, Dmain Protection register 1 [R/W 32] */
#define PORT_DM1PR7(n)      ((uint32_t*)(PORT_GROUP7_BASE + 0x0024U + (n * 0x200U)))   /* GPIO group 7, Dmain Protection register 1 [R/W 32] */
#define PORT_DM2PR4(n)      ((uint32_t*)(PORT_GROUP4_BASE + 0x0028U + (n * 0x200U)))   /* GPIO group 4, Dmain Protection register 2 [R/W 32] */
#define PORT_DM2PR5(n)      ((uint32_t*)(PORT_GROUP5_BASE + 0x0028U + (n * 0x200U)))   /* GPIO group 5, Dmain Protection register 2 [R/W 32] */
#define PORT_DM2PR6(n)      ((uint32_t*)(PORT_GROUP6_BASE + 0x0028U + (n * 0x200U)))   /* GPIO group 6, Dmain Protection register 2 [R/W 32] */
#define PORT_DM2PR7(n)      ((uint32_t*)(PORT_GROUP7_BASE + 0x0028U + (n * 0x200U)))   /* GPIO group 7, Dmain Protection register 2 [R/W 32] */
/* PORT_DM3PR */
#define PORT_GPSR4(n)       ((uint32_t*)(PORT_GROUP4_BASE + 0x0040U + (n * 0x200U)))   /* GPIO group 4, GPIO/Peripheral Function Select register [R/W 32] */
#define PORT_GPSR5(n)       ((uint32_t*)(PORT_GROUP5_BASE + 0x0040U + (n * 0x200U)))   /* GPIO group 5, GPIO/Peripheral Function Select register [R/W 32] */
#define PORT_GPSR6(n)       ((uint32_t*)(PORT_GROUP6_BASE + 0x0040U + (n * 0x200U)))   /* GPIO group 6, GPIO/Peripheral Function Select register [R/W 32] */
#define PORT_GPSR7(n)       ((uint32_t*)(PORT_GROUP7_BASE + 0x0040U + (n * 0x200U)))   /* GPIO group 7, GPIO/Peripheral Function Select register [R/W 32] */
#define PORT_IP0SR4(n)      ((uint32_t*)(PORT_GROUP4_BASE + 0x0060U + (n * 0x200U)))   /* GPIO group 4, Peripheral Function Select register 0 [R/W 32] */
#define PORT_IP0SR5(n)      ((uint32_t*)(PORT_GROUP5_BASE + 0x0060U + (n * 0x200U)))   /* GPIO group 5, Peripheral Function Select register 0 [R/W 32] */
#define PORT_IP0SR6(n)      ((uint32_t*)(PORT_GROUP6_BASE + 0x0060U + (n * 0x200U)))   /* GPIO group 6, Peripheral Function Select register 0 [R/W 32] */
#define PORT_IP0SR7(n)      ((uint32_t*)(PORT_GROUP7_BASE + 0x0060U + (n * 0x200U)))   /* GPIO group 7, Peripheral Function Select register 0 [R/W 32] */
#define PORT_IP1SR4(n)      ((uint32_t*)(PORT_GROUP4_BASE + 0x0064U + (n * 0x200U)))   /* GPIO group 4, Peripheral Function Select register 1 [R/W 32] */
#define PORT_IP1SR5(n)      ((uint32_t*)(PORT_GROUP5_BASE + 0x0064U + (n * 0x200U)))   /* GPIO group 5, Peripheral Function Select register 1 [R/W 32] */
#define PORT_IP1SR6(n)      ((uint32_t*)(PORT_GROUP6_BASE + 0x0064U + (n * 0x200U)))   /* GPIO group 6, Peripheral Function Select register 1 [R/W 32] */
#define PORT_IP1SR7(n)      ((uint32_t*)(PORT_GROUP7_BASE + 0x0064U + (n * 0x200U)))   /* GPIO group 7, Peripheral Function Select register 1 [R/W 32] */
#define PORT_IP2SR4(n)      ((uint32_t*)(PORT_GROUP4_BASE + 0x0068U + (n * 0x200U)))   /* GPIO group 4, Peripheral Function Select register 2 [R/W 32] */
#define PORT_IP2SR5(n)      ((uint32_t*)(PORT_GROUP5_BASE + 0x0068U + (n * 0x200U)))   /* GPIO group 5, Peripheral Function Select register 2 [R/W 32] */
#define PORT_IP2SR6(n)      ((uint32_t*)(PORT_GROUP6_BASE + 0x0068U + (n * 0x200U)))   /* GPIO group 6, Peripheral Function Select register 2 [R/W 32] */
#define PORT_IP2SR7(n)      ((uint32_t*)(PORT_GROUP7_BASE + 0x0068U + (n * 0x200U)))   /* GPIO group 7, Peripheral Function Select register 2 [R/W 32] */
#define PORT_IP3SR4(n)      ((uint32_t*)(PORT_GROUP4_BASE + 0x006cU + (n * 0x200U)))   /* GPIO group 4, Peripheral Function Select register 3 [R/W 32] */
#define PORT_IP3SR7(n)      ((uint32_t*)(PORT_GROUP7_BASE + 0x006cU + (n * 0x200U)))   /* GPIO group 7, Peripheral Function Select register 3 [R/W 32] */
#define PORT_DRV0CTRL4(n)   ((uint32_t*)(PORT_GROUP4_BASE + 0x0080U + (n * 0x200U)))   /* GPIO group 4, DRV control register 0 [R/W 32] */
#define PORT_DRV0CTRL5(n)   ((uint32_t*)(PORT_GROUP5_BASE + 0x0080U + (n * 0x200U)))   /* GPIO group 5, DRV control register 0 [R/W 32] */
#define PORT_DRV0CTRL6(n)   ((uint32_t*)(PORT_GROUP6_BASE + 0x0080U + (n * 0x200U)))   /* GPIO group 6, DRV control register 0 [R/W 32] */
#define PORT_DRV0CTRL7(n)   ((uint32_t*)(PORT_GROUP7_BASE + 0x0080U + (n * 0x200U)))   /* GPIO group 7, DRV control register 0 [R/W 32] */
#define PORT_DRV1CTRL4(n)   ((uint32_t*)(PORT_GROUP4_BASE + 0x0084U + (n * 0x200U)))   /* GPIO group 4, DRV control register 1 [R/W 32] */
#define PORT_DRV1CTRL5(n)   ((uint32_t*)(PORT_GROUP5_BASE + 0x0084U + (n * 0x200U)))   /* GPIO group 5, DRV control register 1 [R/W 32] */
#define PORT_DRV1CTRL6(n)   ((uint32_t*)(PORT_GROUP6_BASE + 0x0084U + (n * 0x200U)))   /* GPIO group 6, DRV control register 1 [R/W 32] */
#define PORT_DRV1CTRL7(n)   ((uint32_t*)(PORT_GROUP7_BASE + 0x0084U + (n * 0x200U)))   /* GPIO group 7, DRV control register 1 [R/W 32] */
#define PORT_DRV2CTRL4(n)   ((uint32_t*)(PORT_GROUP4_BASE + 0x0088U + (n * 0x200U)))   /* GPIO group 4, DRV control register 2 [R/W 32] */
#define PORT_DRV2CTRL5(n)   ((uint32_t*)(PORT_GROUP5_BASE + 0x0088U + (n * 0x200U)))   /* GPIO group 5, DRV control register 2 [R/W 32] */
#define PORT_DRV2CTRL6(n)   ((uint32_t*)(PORT_GROUP6_BASE + 0x0088U + (n * 0x200U)))   /* GPIO group 6, DRV control register 2 [R/W 32] */
#define PORT_DRV2CTRL7(n)   ((uint32_t*)(PORT_GROUP7_BASE + 0x0088U + (n * 0x200U)))   /* GPIO group 7, DRV control register 2 [R/W 32] */
#define PORT_DRV3CTRL4(n)   ((uint32_t*)(PORT_GROUP4_BASE + 0x008cU + (n * 0x200U)))   /* GPIO group 4, DRV control register 3 [R/W 32] */
#define PORT_DRV3CTRL5(n)   ((uint32_t*)(PORT_GROUP5_BASE + 0x008cU + (n * 0x200U)))   /* GPIO group 5, DRV control register 3 [R/W 32] */
#define PORT_DRV3CTRL6(n)   ((uint32_t*)(PORT_GROUP6_BASE + 0x008cU + (n * 0x200U)))   /* GPIO group 6, DRV control register 3 [R/W 32] */
#define PORT_DRV3CTRL7(n)   ((uint32_t*)(PORT_GROUP7_BASE + 0x008cU + (n * 0x200U)))   /* GPIO group 7, DRV control register 3 [R/W 32] */
/* PORT_POC */
#define PORT_PUEN4(n)       ((uint32_t*)(PORT_GROUP4_BASE + 0x00c0U + (n * 0x200U)))   /* GPIO group 4, LSI pin pull-eneble register [R/W 32] */
#define PORT_PUEN5(n)       ((uint32_t*)(PORT_GROUP5_BASE + 0x00c0U + (n * 0x200U)))   /* GPIO group 5, LSI pin pull-eneble register [R/W 32] */
#define PORT_PUEN6(n)       ((uint32_t*)(PORT_GROUP6_BASE + 0x00c0U + (n * 0x200U)))   /* GPIO group 6, LSI pin pull-eneble register [R/W 32] */
#define PORT_PUEN7(n)       ((uint32_t*)(PORT_GROUP7_BASE + 0x00c0U + (n * 0x200U)))   /* GPIO group 7, LSI pin pull-eneble register [R/W 32] */
#define PORT_PUD4(n)        ((uint32_t*)(PORT_GROUP4_BASE + 0x00e0U + (n * 0x200U)))   /* GPIO group 4, LSI pin pull-up/down control register [R/W 32] */
#define PORT_PUD5(n)        ((uint32_t*)(PORT_GROUP5_BASE + 0x00e0U + (n * 0x200U)))   /* GPIO group 5, LSI pin pull-up/down control register [R/W 32] */
#define PORT_PUD6(n)        ((uint32_t*)(PORT_GROUP6_BASE + 0x00e0U + (n * 0x200U)))   /* GPIO group 6, LSI pin pull-up/down control register [R/W 32] */
#define PORT_PUD7(n)        ((uint32_t*)(PORT_GROUP7_BASE + 0x00e0U + (n * 0x200U)))   /* GPIO group 7, LSI pin pull-up/down control register [R/W 32] */
#define PORT_MODSEL4(n)     ((uint32_t*)(PORT_GROUP4_BASE + 0x0100U + (n * 0x200U)))   /* GPIO group 4, Module select register [R/W 32] */
/* PORT_TD0SEL */
/* PORT_TD1SEL */
#define PORT_PSER4(n)       ((uint32_t*)(PORT_GROUP4_BASE + 0x0160U + (n * 0x200U)))   /* GPIO group 4, Port Safe state Enable Register [R/W 32] */
#define PORT_PSER5(n)       ((uint32_t*)(PORT_GROUP5_BASE + 0x0160U + (n * 0x200U)))   /* GPIO group 5, Port Safe state Enable Register [R/W 32] */
#define PORT_PSER6(n)       ((uint32_t*)(PORT_GROUP6_BASE + 0x0160U + (n * 0x200U)))   /* GPIO group 6, Port Safe state Enable Register [R/W 32] */
#define PORT_PSER7(n)       ((uint32_t*)(PORT_GROUP7_BASE + 0x0160U + (n * 0x200U)))   /* GPIO group 7, Port Safe state Enable Register [R/W 32] */
#define PORT_PS0SR4(n)      ((uint32_t*)(PORT_GROUP4_BASE + 0x0164U + (n * 0x200U)))   /* GPIO group 4, Port Safe state Select Register 0 [R/W 32] */
#define PORT_PS0SR5(n)      ((uint32_t*)(PORT_GROUP5_BASE + 0x0164U + (n * 0x200U)))   /* GPIO group 5, Port Safe state Select Register 0 [R/W 32] */
#define PORT_PS0SR6(n)      ((uint32_t*)(PORT_GROUP6_BASE + 0x0164U + (n * 0x200U)))   /* GPIO group 6, Port Safe state Select Register 0 [R/W 32] */
#define PORT_PS0SR7(n)      ((uint32_t*)(PORT_GROUP7_BASE + 0x0164U + (n * 0x200U)))   /* GPIO group 7, Port Safe state Select Register 0 [R/W 32] */
#define PORT_PS1SR4(n)      ((uint32_t*)(PORT_GROUP4_BASE + 0x0168U + (n * 0x200U)))   /* GPIO group 4, Port Safe state Select Register 1 [R/W 32] */
#define PORT_PS1SR5(n)      ((uint32_t*)(PORT_GROUP5_BASE + 0x0168U + (n * 0x200U)))   /* GPIO group 5, Port Safe state Select Register 1 [R/W 32] */
#define PORT_PS1SR6(n)      ((uint32_t*)(PORT_GROUP6_BASE + 0x0168U + (n * 0x200U)))   /* GPIO group 6, Port Safe state Select Register 1 [R/W 32] */
#define PORT_PS1SR7(n)      ((uint32_t*)(PORT_GROUP7_BASE + 0x0168U + (n * 0x200U)))   /* GPIO group 7, Port Safe state Select Register 1 [R/W 32] */
/* PORT_IOINTSEL */
#define PORT_INOUTSEL4(n)   ((uint32_t*)(PORT_GROUP4_BASE + 0x0184U + (n * 0x200U)))   /* GPIO group 4, General input/output switching register [R/W 32] */
#define PORT_INOUTSEL5(n)   ((uint32_t*)(PORT_GROUP5_BASE + 0x0184U + (n * 0x200U)))   /* GPIO group 5, General input/output switching register [R/W 32] */
#define PORT_INOUTSEL6(n)   ((uint32_t*)(PORT_GROUP6_BASE + 0x0184U + (n * 0x200U)))   /* GPIO group 6, General input/output switching register [R/W 32] */
#define PORT_INOUTSEL7(n)   ((uint32_t*)(PORT_GROUP7_BASE + 0x0184U + (n * 0x200U)))   /* GPIO group 7, General input/output switching register [R/W 32] */
#define PORT_OUTDT4(n)      ((uint32_t*)(PORT_GROUP4_BASE + 0x0188U + (n * 0x200U)))   /* GPIO group 4, General output register [R/W 32] */
#define PORT_OUTDT5(n)      ((uint32_t*)(PORT_GROUP5_BASE + 0x0188U + (n * 0x200U)))   /* GPIO group 5, General output register [R/W 32] */
#define PORT_OUTDT6(n)      ((uint32_t*)(PORT_GROUP6_BASE + 0x0188U + (n * 0x200U)))   /* GPIO group 6, General output register [R/W 32] */
#define PORT_OUTDT7(n)      ((uint32_t*)(PORT_GROUP7_BASE + 0x0188U + (n * 0x200U)))   /* GPIO group 7, General output register [R/W 32] */
#define PORT_INDT4(n)       ((uint32_t*)(PORT_GROUP4_BASE + 0x018cU + (n * 0x200U)))   /* GPIO group 4, General input register [R 32] */
#define PORT_INDT5(n)       ((uint32_t*)(PORT_GROUP5_BASE + 0x018cU + (n * 0x200U)))   /* GPIO group 5, General input register [R 32] */
#define PORT_INDT6(n)       ((uint32_t*)(PORT_GROUP6_BASE + 0x018cU + (n * 0x200U)))   /* GPIO group 6, General input register [R 32] */
#define PORT_INDT7(n)       ((uint32_t*)(PORT_GROUP7_BASE + 0x018cU + (n * 0x200U)))   /* GPIO group 7, General input register [R 32] */
/* PORT_INTDT */
/* PORT_INTCLR */
/* PORT_INTMSK */
/* PORT_MSKCLR */
#define PORT_POSNEG4(n)     ((uint32_t*)(PORT_GROUP4_BASE + 0x01a0U + (n * 0x200U)))   /* GPIO group 4, Positive/negative logic select register [R/W 32] */
#define PORT_POSNEG5(n)     ((uint32_t*)(PORT_GROUP5_BASE + 0x01a0U + (n * 0x200U)))   /* GPIO group 5, Positive/negative logic select register [R/W 32] */
#define PORT_POSNEG6(n)     ((uint32_t*)(PORT_GROUP6_BASE + 0x01a0U + (n * 0x200U)))   /* GPIO group 6, Positive/negative logic select register [R/W 32] */
#define PORT_POSNEG7(n)     ((uint32_t*)(PORT_GROUP7_BASE + 0x01a0U + (n * 0x200U)))   /* GPIO group 7, Positive/negative logic select register [R/W 32] */
/* PORT_EDGLEVEL */
#define PORT_FILONOFF4(n)   ((uint32_t*)(PORT_GROUP4_BASE + 0x01a8U + (n * 0x200U)))   /* GPIO group 4, Chattering prevention on/off register [R/W 32] */
#define PORT_FILONOFF5(n)   ((uint32_t*)(PORT_GROUP5_BASE + 0x01a8U + (n * 0x200U)))   /* GPIO group 5, Chattering prevention on/off register [R/W 32] */
#define PORT_FILONOFF6(n)   ((uint32_t*)(PORT_GROUP6_BASE + 0x01a8U + (n * 0x200U)))   /* GPIO group 6, Chattering prevention on/off register [R/W 32] */
#define PORT_FILONOFF7(n)   ((uint32_t*)(PORT_GROUP7_BASE + 0x01a8U + (n * 0x200U)))   /* GPIO group 7, Chattering prevention on/off register [R/W 32] */
#define PORT_FILCLKSEL4(n)  ((uint32_t*)(PORT_GROUP4_BASE + 0x01acU + (n * 0x200U)))   /* GPIO group 4, Chattering prevention clock select register [R/W 32] */
#define PORT_FILCLKSEL5(n)  ((uint32_t*)(PORT_GROUP5_BASE + 0x01acU + (n * 0x200U)))   /* GPIO group 5, Chattering prevention clock select register [R/W 32] */
#define PORT_FILCLKSEL6(n)  ((uint32_t*)(PORT_GROUP6_BASE + 0x01acU + (n * 0x200U)))   /* GPIO group 6, Chattering prevention clock select register [R/W 32] */
#define PORT_FILCLKSEL7(n)  ((uint32_t*)(PORT_GROUP7_BASE + 0x01acU + (n * 0x200U)))   /* GPIO group 7, Chattering prevention clock select register [R/W 32] */
#define PORT_OUTDTSEL4(n)   ((uint32_t*)(PORT_GROUP4_BASE + 0x01c0U + (n * 0x200U)))   /* GPIO group 4, Output data select register [R/W 32] */
#define PORT_OUTDTSEL5(n)   ((uint32_t*)(PORT_GROUP5_BASE + 0x01c0U + (n * 0x200U)))   /* GPIO group 5, Output data select register [R/W 32] */
#define PORT_OUTDTSEL6(n)   ((uint32_t*)(PORT_GROUP6_BASE + 0x01c0U + (n * 0x200U)))   /* GPIO group 6, Output data select register [R/W 32] */
#define PORT_OUTDTSEL7(n)   ((uint32_t*)(PORT_GROUP7_BASE + 0x01c0U + (n * 0x200U)))   /* GPIO group 7, Output data select register [R/W 32] */
#define PORT_OUTDTH4(n)     ((uint32_t*)(PORT_GROUP4_BASE + 0x01c4U + (n * 0x200U)))   /* GPIO group 4, Output data high register [R/W 32] */
#define PORT_OUTDTH5(n)     ((uint32_t*)(PORT_GROUP5_BASE + 0x01c4U + (n * 0x200U)))   /* GPIO group 5, Output data high register [R/W 32] */
#define PORT_OUTDTH6(n)     ((uint32_t*)(PORT_GROUP6_BASE + 0x01c4U + (n * 0x200U)))   /* GPIO group 6, Output data high register [R/W 32] */
#define PORT_OUTDTH7(n)     ((uint32_t*)(PORT_GROUP7_BASE + 0x01c4U + (n * 0x200U)))   /* GPIO group 7, Output data high register [R/W 32] */
#define PORT_OUTDTL4(n)     ((uint32_t*)(PORT_GROUP4_BASE + 0x01c8U + (n * 0x200U)))   /* GPIO group 4, Output data low register [R/W 32] */
#define PORT_OUTDTL5(n)     ((uint32_t*)(PORT_GROUP5_BASE + 0x01c8U + (n * 0x200U)))   /* GPIO group 5, Output data low register [R/W 32] */
#define PORT_OUTDTL6(n)     ((uint32_t*)(PORT_GROUP6_BASE + 0x01c8U + (n * 0x200U)))   /* GPIO group 6, Output data low register [R/W 32] */
#define PORT_OUTDTL7(n)     ((uint32_t*)(PORT_GROUP7_BASE + 0x01c8U + (n * 0x200U)))   /* GPIO group 7, Output data low register [R/W 32] */
/* PORT_BOTHEDGE */
#define PORT_INEN4(n)       ((uint32_t*)(PORT_GROUP4_BASE + 0x01d0U + (n * 0x200U)))   /* GPIO group 4, General input enable register [R/W 32] */
#define PORT_INEN5(n)       ((uint32_t*)(PORT_GROUP5_BASE + 0x01d0U + (n * 0x200U)))   /* GPIO group 5, General input enable register [R/W 32] */
#define PORT_INEN6(n)       ((uint32_t*)(PORT_GROUP6_BASE + 0x01d0U + (n * 0x200U)))   /* GPIO group 6, General input enable register [R/W 32] */
#define PORT_INEN7(n)       ((uint32_t*)(PORT_GROUP7_BASE + 0x01d0U + (n * 0x200U)))   /* GPIO group 7, General input enable register [R/W 32] */

#define PORT_DRV1CTRLSYS1(n)    ((uint32_t*)(PORT_SYSGROUP1_BASE + 0x0084U + (n * 0x200U)))    /* DRV control register 1 [R/W 32] */
#define PORT_DRV2CTRLSYS1(n)    ((uint32_t*)(PORT_SYSGROUP1_BASE + 0x0088U + (n * 0x200U)))    /* DRV control register 2 [R/W 32] */
#define PORT_PUENSYS1(n)        ((uint32_t*)(PORT_SYSGROUP1_BASE + 0x00c0U + (n * 0x200U)))    /* LSI pin pull-eneble register [R/W 32] */
#define PORT_PUDSYS1(n)         ((uint32_t*)(PORT_SYSGROUP1_BASE + 0x00e0U + (n * 0x200U)))    /* LSI pin pull-up/down control register [R/W 32] */

#define PORT_PNOT4(n)       ((uint32_t*)(PORT_GROUP4_BASE + 0x01d4U + (n * 0x200U)))   /* GPIO group 4, Port NOT Register [R/W 32] */
#define PORT_PNOT5(n)       ((uint32_t*)(PORT_GROUP5_BASE + 0x01d4U + (n * 0x200U)))   /* GPIO group 5, Port NOT Register [R/W 32] */
#define PORT_PNOT6(n)       ((uint32_t*)(PORT_GROUP6_BASE + 0x01d4U + (n * 0x200U)))   /* GPIO group 6, Port NOT Register [R/W 32] */
#define PORT_PNOT7(n)       ((uint32_t*)(PORT_GROUP7_BASE + 0x01d4U + (n * 0x200U)))   /* GPIO group 7, Port NOT Register [R/W 32] */
#define PORT_PINV4(n)       ((uint32_t*)(PORT_GROUP4_BASE + 0x016cU + (n * 0x200U)))   /* GPIO group 4, Port Output Value Inversion Register [R/W 32] */
#define PORT_PINV5(n)       ((uint32_t*)(PORT_GROUP5_BASE + 0x016cU + (n * 0x200U)))   /* GPIO group 5, Port Output Value Inversion Register [R/W 32] */
#define PORT_PINV6(n)       ((uint32_t*)(PORT_GROUP6_BASE + 0x016cU + (n * 0x200U)))   /* GPIO group 6, Port Output Value Inversion Register [R/W 32] */
#define PORT_PINV7(n)       ((uint32_t*)(PORT_GROUP7_BASE + 0x016cU + (n * 0x200U)))   /* GPIO group 7, Port Output Value Inversion Register [R/W 32] */
#define PORT_PIS5(n)        ((uint32_t*)(PORT_GROUP5_BASE + 0x0170U + (n * 0x200U)))   /* GPIO group 5, Port Input Buffer Selection Register [R/W 32] */

#endif /* _RCARS4_H_ */
