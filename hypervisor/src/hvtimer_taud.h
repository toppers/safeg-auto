#ifndef _HVTIMER_TAUD_
#define _HVTIMER_TAUD_

/*
 *  TAUD0を用いたシステム周期タイマ，タイムウィンドウタイマのドライバ
 */

/*
 *  HV内部で使用するデータ構造の定義ファイル
 */
#include "tool_stddef.h"

/*
 *  設定可能な最大値と最小値
 */
#define TAUD_MIN_NS    ((1000*TAUD_CLK_PRS)/TAUD_PCLK_HZ)
#define TAUD_MAX_NS    (0xFFFF*TAUD_MIN_NS)

#ifdef USE_TAUD_AS_SYSCTIMER
/*
 *  システム周期のusをTAUDIのカウント値に変換
 */
#define SYSCTIMER_TO_CYC(us)    TAUD_TO_CNT(us)

/*
 *  システム周期タイマの割込み番号
 */
#define SCYCTIM_INTNO(coreid)    (TAUD0_1_INTNO + coreid)
#endif /* USE_TAUD_AS_SYSCTIMER */

#ifdef USE_TAUD_AS_TWDTIMER
/*
 *  タイムウィンドウのusをTAUDのカウント値に変換
 */
#define TWDTIMER_TO_CYC(us)     TAUD_TO_CNT(us)

/*
 *  タイムウィンドウタイマの割込み番号
 */
#define TWDTIM_INTNO(coreid)    (TAUD0_0_INTNO + coreid)
#endif /* USE_TAUD_AS_TWDTIMER */

#ifndef TOPPERS_MACRO_ONLY

#ifdef USE_TAUD_AS_SYSCTIMER

/*
 *  システム周期タイマの実現
 *
 *  インターバルタイマモードで使用．
 *  カウントを開始すると，CDRの値をCNTにロードにしてカウント開始．
 */

/*
 *  コアに対するシステム周期タイマのチャネル取得
 */
#define SCYCTIM_CH(coreid)    (1 + (coreid * 2))

/*
 *  システム周期タイマの初期化
 */
LOCAL_INLINE void
sysctimer_init(uint32_t coreid)
{
    taud0_init(TAUD_CLK_PRS);

    /* CK0によるカウント，ソフトウェアトリガ，インターバルタイマ */
    sil_wrh_mem(TAUD0_CMOR(SCYCTIM_CH(coreid)), ((0x00U << 14U)| (0x00U << 12U)) | (0x00U << 8U) | (0x00U));
}

/*
 *  システム周期タイマのスタート
 */
LOCAL_INLINE void
sysctimer_start(uint32_t coreid, uint32_t duration)
{
    /* タイマ停止 */
    sil_wrh_mem(TAUD0_TT, (1U << SCYCTIM_CH(coreid)));
    /* 周期のカウント値の設定 */
    sil_wrh_mem(TAUD0_CDR(SCYCTIM_CH(coreid)), duration);
    /* タイマスタート */
    sil_wrh_mem(TAUD0_TS, (1U << SCYCTIM_CH(coreid)));
}

/*
 *  システム周期タイマの停止
 */
LOCAL_INLINE void
sysctimer_stop(uint32_t coreid)
{
    /* タイマ停止 */
    sil_wrh_mem(TAUD0_TT,  (1U << SCYCTIM_CH(coreid)));
}

/*
 *  システム周期タイマの割込みクリア
 */
LOCAL_INLINE void
sysctimer_clearint(uint32_t coreid)
{
    /* オーバーフローフラグのクリア */
    sil_wrh_mem(TAUD0_CSC(SCYCTIM_CH(coreid)),  0x01U);
}

LOCAL_INLINE void
sysctimer_raiseint(uint32 intno) {
    raise_exint(intno);    
}

/*
 *  ガードの設定
 */
LOCAL_INLINE
sysctimer_sguard_init(void)
{
    sil_wrw_mem(PBGERRSLV50_PBGKCPROT, UNLOCKKEY_VAL);
    sil_wrw_mem(PBG51_PBGPROT0_m(1), CPCR_GEN|CPCR_DBG|CPCR_RG);
    sil_wrw_mem(PBG51_PBGPROT1_m(1), TBIT_HV_SPID);
    sil_wrw_mem(PBGERRSLV50_PBGKCPROT, LOCKKEY_VAL);
}
#endif /* USE_TAUD_AS_SYSCTIMER */

#ifdef USE_TAUD_AS_TWDTIMER

/*
 *  タイムウィンドウタイマの実現
 *
 *  インターバルタイマモードで使用．
 *  割込みが入るとタイマを停止する．
 */

/*
 *  コアに対するタイムウィンドウタイマのチャネル取得
 */
#define TWDTIM_CH(coreid)    (2 + (coreid * 2))

/*
 *  タイムウィンドウタイマの初期化
 */
LOCAL_INLINE void
twdtimer_init(uint32_t coreid)
{
    taud0_init(TAUD_CLK_PRS);

    /* CK0によるカウント，ソフトウェアトリガ，インターバルタイマ，割込み許可  */
    sil_wrh_mem(TAUD0_CMOR(TWDTIM_CH(coreid)), ((0x00U << 14U)| (0x00U << 12U)) | (0x00U << 8U) | (0x00U));
}

/*
 *  タイムウィンドウタイマのスタート
 */
LOCAL_INLINE void
twdtimer_start(uint32_t coreid, uint32_t duration)
{
    /* タイマ停止 */
    sil_wrh_mem(TAUD0_TT, (1U << TWDTIM_CH(coreid)));
    /* 周期のカウント値の設定 */
    sil_wrh_mem(TAUD0_CDR(TWDTIM_CH(coreid)), duration);
    /* タイマスタート */
    sil_wrh_mem(TAUD0_TS, (1U << TWDTIM_CH(coreid)));
}

/*
 *  タイムウィンドウタイマの停止
 */
LOCAL_INLINE void
twdtimer_stop(uint32_t coreid)
{
    /* タイマ停止 */
    sil_wrh_mem(TAUD0_TT, (1U << TWDTIM_CH(coreid)));
}

/*
 *  タイムウィンドウタイマの割込みクリア
 */
LOCAL_INLINE void
twdtimer_clearint(uint32_t coreid)
{
    /* タイマ停止 */
    sil_wrh_mem(TAUD0_TT, (1U << TWDTIM_CH(coreid)));
    /* オーバーフローフラグのクリア */
    sil_wrh_mem(TAUD0_CSC(TWDTIM_CH(coreid)), 0x01U);
}

/*
 *  タイムウィンドウタイマの一時停止
 */
LOCAL_INLINE uint32_t
twdtimer_pause(uint32_t coreid)
{
    /* タイマ停止 */
    sil_wrh_mem(TAUD0_TT,  (1U << TWDTIM_CH(coreid)));
                return sil_reh_mem(TAUD0_CNT(TWDTIM_CH(coreid)));
}

/*
 *  タイムウィンドウタイマの再開
 */
LOCAL_INLINE void
twdtimer_continue(uint32_t coreid, uint32_t duration)
{
    sil_wrh_mem(TAUD0_CDR(TWDTIM_CH(coreid)), duration);
    /* タイマスタート */
    sil_wrh_mem(TAUD0_TS, (1U << TWDTIM_CH(coreid)));
}

/*
 *  ガードの設定
 */
LOCAL_INLINE void
twdtimer_sguard_init(void)
{
    sil_wrw_mem(PBGERRSLV50_PBGKCPROT, UNLOCKKEY_VAL);    
    sil_wrw_mem(PBG51_PBGPROT0_m(1), (CPCR_GEN|CPCR_DBG|CPCR_RG));
    sil_wrw_mem(PBG51_PBGPROT1_m(1), TBIT_HV_SPID);
    sil_wrw_mem(PBGERRSLV50_PBGKCPROT, LOCKKEY_VAL);
}
#endif /* USE_TAUD_AS_TWDTIMER */

#endif /* TOPPERS_MACRO_ONLY */

#endif /* _HVTIMER_TAUD_ */
