#ifndef _HVTIMER_TPTM_
#define _HVTIMER_TPTM_

/*
 *  HV内部で使用するデータ構造の定義ファイル
 */
#include "tool_stddef.h"

/*
 *  システム周期タイマとして試用するTPTIの番号
 */
#define SCYCTIM_TPTMI_NO        0

#ifdef USE_TPTM_AS_SYSCTIMER
/*
 *  システム周期のusをTPTMIのカウント値に変換
 */
#define SYSCTIMER_TO_CYC(us)        ((uint32)(us * TPTM_CLK_MHZ))

/*
 *  システム周期タイマの割込み番号
 */
#define SCYCTIM_INTNO(coreid)        TPTMI_INTNO
#endif /* USE_TPTM_AS_SYSCTIMER */

#ifdef USE_TPTM_AS_TWDTIMER
/*
 *  タイムウィンドウのusをTPTMUのカウント値に変換
 */
#define TWDTIMER_TO_CYC(us)        ((uint32)(us * TPTM_CLK_MHZ))

/*
 *  タイムウィンドウタイマの割込み番号
 */
#define TWDTIM_INTNO(coreid)        TPTMU_INTNO(coreid, TWDTIM_TPTMU_NO)
#endif /* USE_TPTM_AS_TWDTIMER */

#ifndef TOPPERS_MACRO_ONLY

#ifdef USE_TPTM_AS_SYSCTIMER
/*
 *  システム周期タイマの初期化
 */
LOCAL_INLINE void
sysctimer_init(uint32 coreid)
{
    tptmi_seteiint();
    tptmi_enaint(SCYCTIM_TPTMI_NO);
    tptmi_setdiv(TPTM_DIV);
}

/*
 *  システム周期タイマのスタート
 */
LOCAL_INLINE void
sysctimer_start(uint32 coreid, uint32 duration)
{
    duration = duration != 0? (duration - 1) : 0;
    tptmi_start(SCYCTIM_TPTMI_NO, duration);
}

/*
 *  システム周期タイマの停止
 */
LOCAL_INLINE void
sysctimer_stop(uint32 coreid)
{
    tptmi_stop(SCYCTIM_TPTMI_NO);
    tptmi_clearint(SCYCTIM_TPTMI_NO);
}

/*
 *  システム周期タイマの割込みクリア
 */
LOCAL_INLINE void
sysctimer_clearint(uint32 coreid)
{
    tptmi_clearint(SCYCTIM_TPTMI_NO);
}

LOCAL_INLINE void
sysctimer_raiseint(uint32 intno) {
#if 0
    tptmi_setcnt(SCYCTIM_TPTMI_NO, 0);
#else
    if (tptmi_isrunning(SCYCTIM_TPTMI_NO)) {
        tptmi_setcnt(SCYCTIM_TPTMI_NO, 0);
    }
    else {
        tptmi_start(SCYCTIM_TPTMI_NO, 1);
        while(!tptmi_isuderflow(SCYCTIM_TPTMI_NO));
        tptmi_stop(SCYCTIM_TPTMI_NO);
    }
#endif    
}

/*
 *  ガードの設定
 */
LOCAL_INLINE void
sysctimer_sguard_init(void) {
    sil_wrw_mem(TPTGKCPROT, LOCKKEY_VAL);
    sil_wrw_mem(TPTGPROT0_n(0), (CPCR_GEN|CPCR_DBG|CPCR_RG));
    sil_wrw_mem(TPTGPROT1_n(0), TBIT_HV_SPID);
    sil_wrw_mem(TPTGKCPROT, UNLOCKKEY_VAL);
}    
#endif /* USE_TPTM_AS_SYSCTIMER */

#ifdef USE_TPTM_AS_TWDTIMER

/*
 *  タイムウィンドウタイマの初期化
 */
LOCAL_INLINE void
twdtimer_init(uint32 coreid)
{
    tptmu_setdiv(TPTM_DIV);
    tptmu_enaint(TWDTIM_TPTMU_NO, 0);
}

/*
 *  タイムウィンドウタイマのスタート
 */
LOCAL_INLINE void
twdtimer_start(uint32 coreid, uint32 duration)
{
    duration = duration != 0? (duration - 1) : 0;
    tptmu_set_cmp(0, duration);
    tptmu_start(TWDTIM_TPTMU_NO);
}

/*
 *  タイムウィンドウタイマの停止
 */
LOCAL_INLINE void
twdtimer_stop(uint32 coreid)
{
    tptmu_stop(TWDTIM_TPTMU_NO);
}

/*
 *  タイムウィンドウタイマの割込みクリア
 */
LOCAL_INLINE void
twdtimer_clearint(uint32 coreid)
{
}

/*
 *  タイムウィンドウタイマの一時停止
 */
LOCAL_INLINE uint32
twdtimer_pause(uint32 coreid)
{
    tptmu_stop(TWDTIM_TPTMU_NO);
    return tptmu_getcnt(TWDTIM_TPTMU_NO);
}

/*
 *  タイムウィンドウタイマの再開
 */
LOCAL_INLINE void
twdtimer_continue(uint32 coreid, uint32 duration)
{
    tptmu_restart(TWDTIM_TPTMU_NO);
}

/*
 *  ガードの設定
 */
LOCAL_INLINE void
twdtimer_sguard_init(void){
    sil_wrw_mem(TPTGKCPROT, LOCKKEY_VAL);
    sil_wrw_mem(TPTGPROT0_n(0), (CPCR_GEN|CPCR_DBG|CPCR_RG));
    sil_wrw_mem(TPTGPROT1_n(0), TBIT_HV_SPID);
    sil_wrw_mem(TPTGKCPROT, UNLOCKKEY_VAL);
}

#endif /* USE_TPTM_AS_TWDTIMER */

#endif /* TOPPERS_MACRO_ONLY */

#endif /* _HVTIMER_TPTM_ */
