/*
 *  ハイパーバイザーサービス
 */
#include "hv_impl.h"

/*
 *  コアIDの取得
 */
ER
GetCoreID(ID* pCoreID)
{
    *pCoreID = get_my_coreid();

    return E_OK;
}

/*
 *  HVタイムウィンドウの残り時間の取得
 */
ER
GetHVTWTimeLeft(uint32* pTimeLeft)
{
    ER  ercd;
    CCB *p_my_ccb = get_my_ccb();

    if (!(p_my_ccb->runhvc) && !(p_my_ccb->runhvint)) {
        *pTimeLeft = (tptmu_get_cmp(0) -  tptmu_getcnt(TWDTIM_TPTMU_NO))/TPTM_CLK_MHZ;
        ercd = E_OK;
    }
    else {
        ercd = E_CTX;
    }

    return ercd;
}
