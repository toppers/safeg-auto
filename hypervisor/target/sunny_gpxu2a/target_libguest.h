#ifndef _TARGET_LIBGUEST_H
#define _TARGET_LIBGUEST_H

/*
 *  タイムウィンドウタイマとして使用するタイマドライバの選択
 */
#define USE_TPTM_AS_TWDTIMER
//#define USE_TAUD_AS_TWDTIMER

/*
 *  タイムウィンドウタイマとして使用するTPTUの番号
 */
#define TWDTIM_TPTMU_NO    0

/*
 *  TPTMに関する設定
 *   分周とカウント速度
 *    DIV 15 : 400Mhz/16 = 25Mhz(40ns) :   
 *    DIV 19 : 400Mhz/20 = 20Mhz(50ns) :
 *    DIV 39 : 400Mhz/40 = 10Mhz(100ns) :  
 */
#define TPTM_DIV        (39U)
#define TPTM_CLK_MHZ    (10U)

#endif /* _TARGET_LIBGUEST_H */
