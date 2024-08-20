/*
 *  起動時処理
 */
#include "tool_stddef.h"
#include "rh850.h"
#include "hv_impl.h"
#include "hv_cfg.h"
#include "vmcom.h"

/*
 *  アイドルタイムウィンドウ実行時のベクタベース（start.S）
 */
extern void EBaseIDLE(void);

/*
 *  起動処理
 */
void
StartHV(ID SystemModeID)
{
    uint  coreid = get_my_coreid();
    uint  my_scyctim_intno = SCYCTIM_INTNO(coreid);
    uint  my_twdtim_intno = TWDTIM_INTNO(coreid);

    /* サポートコアでない場合はリターン */
    if (((TBIT_SUPPORT_CORE) & (1U << coreid)) == 0) {
        return;
    }

    /*
     *  サポートする全てのコアの起動待ち
     */
    barrier_sync(1);

    /*
     *  マスターコアの初期化
     */
    if (is_leader()) {
        /* 次回起動用に同期用変数を0に初期化 */
        bootsync = 0;

#if TNUM_SUPPORT_CORE > 1
        giant_lock = 0;
#endif /* TNUM_SUPPORT_CORE > 1 */

        /* システム動作モード関連の初期化 */
        p_global_cursom = NULL;
        if (VALID_SOMID(SystemModeID)) {
            p_global_nxtsom = get_sominib(SystemModeID);
        }
        else {
            p_global_nxtsom = get_sominib(TMIN_SOMID);
        }
#if TNUM_SUPPORT_CORE > 1
        scycprc_bitmap = 0;
#endif /* TNUM_SUPPORT_CORE > 1 */
    }

    /*
     *  ターゲット依存の初期化
     */
    target_initialize();
    barrier_sync(2);

    /*
     *  HVの初期化
     */
    hv_init();

    /*
     *  TDMAスケジューリング初期化
     */
    tdmas_init();

    /*
     *  HV割込みの初期化     
     */
    hvint_init();

    /*
     *  HVタイムウィンドウ処理初期化
     *
     *  エントリ    : hv_twd
     *  PSWの初期値 : 割込み許可,EBASE有効(EBV=1),FPU有効
     *  スタック    : hvtwd_stkpt_table[] 
     */
    hvtwd_init((uint32)hv_twd,
               ((PSW_EIMASK_ENALL << PSW_EIMASK_BIT)|PSW_EBV_MASK|PSW_CU0_MASK),
               (uint32)(hvtwd_stkpt_table[coreid]));

#ifdef SUPPORT_VMCOM
    /*
     *  VM間通信モジュールの初期化
     */
    vmcom_init();
#endif /* SUPPORT_VMCOM */

    /*
     *  スレーブガードの初期化
     */
    barrier_sync(3);
    if (is_leader()) {
        sguard_init();
    }
    barrier_sync(4);

    /* 拡張割込みを有効化 */
    set_intcfg(INTCFG_EPL);

    /*
     *  シングルVMコアでも起動時の1回はシステム周期用タイマの割込みは発生させるため
     *  初期化及び割込みの設定を行う．
     */
    sysctimer_init(coreid);
    bind_int_to_host(my_scyctim_intno, coreid);
    set_exint_vector(my_scyctim_intno, INTVECTOR_PRI);
    set_exint_pri(my_scyctim_intno, TPRI_SYSCTIMER);
    ena_exint(my_scyctim_intno);
    
    /*
     *  シングルVMコアでなければタイムウィンドウ周期用タイマを初期化
     */
    if ((TBIT_SINGLEVM_CORE & (1 << coreid)) == 0) {
        /*  タイムウィンドウ周期用タイマの初期化 */
        twdtimer_init(coreid);
        bind_int_to_host(my_twdtim_intno, coreid);
        set_exint_vector(my_twdtim_intno, INTVECTOR_PRI);
        set_exint_pri(my_twdtim_intno, TPRI_TWDTIMER);
        ena_exint(my_twdtim_intno);
    }   

    /*
     *  IDLE処理用のPSWとEBASEの設定
     *  この時点でのPSWがIDLE処理で使われる 
     */
    set_ebase((uint32)EBaseIDLE);
    set_psw(get_psw()|PSW_EBV_MASK);

    /*
     *  スタートアップフックの呼び出し
     */
    barrier_sync(5);
    startup_hook();
    barrier_sync(6);

    /*
     *  シングルVMでなければシステム周期用タイマをスタート
     */
    if ((TBIT_SINGLEVM_CORE & (1 << coreid)) == 0) {
        sysctimer_start(coreid, SYSCTIMER_TO_CYC(SYSTEM_INTERVAL_US));
    }

    /*
     *  システム周期割込みを発生させる
     */
    sysctimer_raiseint(my_scyctim_intno);
 
    /*
     *  割込み許可
     *  直後にシステム周期割込みが受け付けられる 
     */
    set_plmr(PSW_EIMASK_ENALL);
    x_set_exipm(PSW_EIMASK_ENALL);
    enable_ei();

    /*
     *  HVアイドル処理の呼び出し
     */
    hv_idle();

    /*
     *  無限ループ
     */
    while(1){}
}
