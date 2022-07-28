#ifndef _TARGET_HV_IMPL_H
#define _TARGET_HV_IMPL_H

/*
 *  チップの定義のインクルード
 */
#include "rcars4.h"

/*
 *  ゲストでも参照する情報のインクルード
 */
#include "target_libguest.h"

/*
 *  システム周期タイマとして使用するタイマドライバの選択
 */
#define USE_TPTM_AS_SYSCTIMER
//#define USE_TAUD_AS_SYSCTIMER

/*
 *  システム周期タイマの割込み優先度
 */
#ifndef TPRI_SYSCTIMER
#define TPRI_SYSCTIMER    (15U)
#endif  /* TPRI_SYSCTIMER */

/*
 *  タイムウィンドウタイマとして使用するタイマドライバの選択
 */
#define USE_TPTM_AS_TWDTIMER
//#define USE_TAUD_AS_TWDTIMER

/*
 *  タイムウィンドウタイマの割込み優先度
 */
#ifndef TPRI_TWDTIMER
#define TPRI_TWDTIMER    (14U)
#endif /* TPRI_TWDTIMER */

/*
 *  usをTAUDのカウント値に変換
 */
#define TAUD_TO_CNT(us)    (((us) * TAUD_PCLK_MHZ) / (1U << TAUD_CLK_PRS))

#ifndef TOPPERS_MACRO_ONLY

/*
 *  デバッグ出力
 */
#ifdef ENABLE_DEBUGOUT
#define DEBUGOUT(str) uart_puts(0, str)
#else /* !ENABLE_DEBUGOUT */
#define DEBUGOUT(str)
#endif /* ENABLE_DEBUGOUT */

/*
 *  HV起動時の初期化
 */
void hardware_init_hook(void);

/*
 *  ターゲット依存の初期化
 */
extern void target_initialize(void);

#endif /* TOPPERS_MACRO_ONLY */

#endif /* _TARGET_HV_IMPL_H */
