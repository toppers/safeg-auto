#ifndef _LIBGUEST_H_
#define _LIBGUEST_H_

/*
 *  ゲストライブラリヘッダーファイル
 */

/*
 *  ゲスト処理・ユーザーホスト処理・ハイパーバイザー共通の定義
 */
#include "../include/common_def.h"

/*
 *  ターゲット依存のゲスト用定義
 */
#include "target_libguest.h"

#ifndef TOPPERS_MACRO_ONLY


#ifdef USE_TPTM_AS_TWDTIMER

LOCAL_INLINE ER
GetVMTWTimeLeft(uint32* pTimeLeft)
{
    uint32 cmp;
    uint32 cnt;
    cmp = tptmu_get_cmp(0);
    cnt = tptmu_getcnt(TWDTIM_TPTMU_NO);

    *pTimeLeft = (cmp -  cnt)/TPTM_CLK_MHZ;

    return E_OK;
}

#endif /* USE_TPTM_AS_TWDTIMER */

#endif /* TOPPERS_MACRO_ONLY */

#endif /* _LIBGUEST_H_ */
