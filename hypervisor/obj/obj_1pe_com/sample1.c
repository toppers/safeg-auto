#include "hv.h"
#include "syslog.h"
#include "perf.h"

#define TAUD_TO_CNT(us)    (((us)*TAUD_PCLK_MHZ)/(1<<TAUD_CLK_PRS))

/*
 *  HVINTを使用するか
 */
#define USE_HVINT

/*
 *  VMのMIP/MDP例外発生時の挙動
 */
#define TO_MIP_RESET
//#define TO_MIP_FAKEFE

/*
 *  VMのSYSERR例外発生時の挙動
 */
#define TO_SYSERR_RESET
//#define TO_SYSERR_FAKEFE

/*
 *  HVINTとして使用するTAUDとOSTMの番号
 */
#define HVINT1_TAUDNO    9
#define HVINT2_OSTMNO    9

/*
 *  コア毎に使用するUARTの番号
 */
const uint uartno[] = {
    RLIN3_PORT0,  /* CORE0 */
    RLIN3_PORT1,  /* CORE1 */
#if TNUM_PHYS_CORE == 4
    RLIN3_PORT2,  /* CORE2 */
    RLIN3_PORT3   /* CORE3 */
#endif /* TNUM_PHYS_CORE == 4 */
};

/*
 *  HV割込みハンドラ0
 */
void
hvint0_handler(void)
{
    ID    coreid;

    GetCoreID(&coreid);

    syslog("HV%d : HVINT0 Handler.\n", coreid);
}

/*
 *  HV割込みハンドラ1
 */
void
hvint1_handler(void)
{
    ID    coreid;

    GetCoreID(&coreid);

    syslog("HV%d : HVINT1 Handler.\n", coreid);
}

/*
 *  タイムウィンドウトリガ割込みハンドラ
 */
void
twdint_handler(void)
{
    ID    coreid;

    GetCoreID(&coreid);

    syslog("\nHV%d : TWTGINT Handler.\n", coreid);
}

/*
 *  HVアイドル処理でのカウント変数
 */
volatile uint32 cnt_idle[TNUM_PHYS_CORE];

/*
 *  HVアイドル処理
 */
void
hv_idle(void)
{
    ID    coreid;
    uint    cnt = 0;

    GetCoreID(&coreid);

    syslog("HV%d : hv_idle : start!\n", coreid);

    while(1) {
        for(cnt_idle[coreid] = 0; cnt_idle[coreid] < 10000000U; cnt_idle[coreid]++);
        syslog("HV%d : hv_idle : running %d.\n", coreid, cnt++);
    }
}

/*
 *  HVタイムウィンドウ処理でのカウント変数
 */
volatile uint32 cnt_twd[TNUM_PHYS_CORE];

/*
 *  HVタイムウィンドウ処理
 */
void
hv_twd(void)
{
    ID    coreid;
    uint    cnt = 0;
    uint32    time_left;
      
    GetCoreID(&coreid);
    syslog("HV%d : hv_twd : start!\n", coreid);

    while(1) {
        for(cnt_twd[coreid] = 0; cnt_twd[coreid] < 10000000U; cnt_twd[coreid]++);
        GetHVTWTimeLeft(&time_left);
        syslog("HV%d : hv_twd : running %d : Time Left %d .\n", coreid, cnt++, time_left);
    }
}

/*
 *  HVIT0として使用するTAUD0の番号
 */
const uint hvint0_taud0no[] = {
    9,  /* CORE0 */
    11, /* CORE1 */
    13, /* CORE2 */
    15  /* CORE3 */
};

/*
 *  HVユーザーメイン関数
 */
void
rh850_main(void)
{
    /*
     *  ハイパーバイザーの呼び出し
     */    
    StartHV(SOMID_SOM1);
}

/*
 *  スタートアップフック関数
 */
void
startup_hook(void)
{
    ID        coreid;
    uint    taud0no;

    GetCoreID(&coreid);
    taud0no = hvint0_taud0no[coreid];

#ifdef USE_HVINT
    /*
     *  HVINT0用タイマのスタート
     */
    taud0_start_interval(taud0no, TAUD_TO_CNT((SYSTEM_INTERVAL_US/5)));
#endif /* USE_HVINT */

    /*
     *  パフォーマンスカウンタ初期化
     */
    perf_init();

    /*
     *  UARTの初期化
     */
    uart_init(uartno[coreid], RLIN3xLWBR_VAL, RLIN3xLBRP01_VAL);
    syslog_init(uartno[coreid]);

    /*
     *  各仮想マシンが使用する周辺回路へのアクセス許可設定
     *   VM0_0 : OSTM0 RLIN3_PORT0
     *   VM0_1 : OSTM1 RLIN3_PORT0
     */

    /*
     *  OSTM0へのアクセス権の設定
     */
    sil_wrw_mem(PBGERRSLV50_PBGKCPROT, UNLOCKKEY_VAL);
    sil_wrw_mem(PBG50_PBGPROT1_m(7), (TBIT_HV_SPID|VM0_0_SPIDLIST));
    sil_wrw_mem(PBGERRSLV50_PBGKCPROT, LOCKKEY_VAL);

    /*
     *  OSTM1へのアクセス権の設定
     */
    sil_wrw_mem(PBGERRSLV50_PBGKCPROT, UNLOCKKEY_VAL);
    sil_wrw_mem(PBG50_PBGPROT1_m(8), (TBIT_HV_SPID|VM0_1_SPIDLIST));
    sil_wrw_mem(PBGERRSLV50_PBGKCPROT, LOCKKEY_VAL);

#if RLIN3_PORT0 == 0    
    /*
     *  RLIN30へのアクセス権の設定
     */
    sil_wrw_mem(PBGERRSLV30_PBGKCPROT, UNLOCKKEY_VAL);
    sil_wrw_mem(PBG32_PBGPROT1_m(1), (TBIT_HV_SPID|VM0_0_SPIDLIST|VM0_1_SPIDLIST));
    sil_wrw_mem(PBGERRSLV30_PBGKCPROT, LOCKKEY_VAL);
#endif /* RLIN3_PORT0 == 0 */
    
#if RLIN3_PORT0 == 7
    /*
     *  RLIN37へのアクセス権の設定
     */
    sil_wrw_mem(PBGERRSLV50_PBGKCPROT, UNLOCKKEY_VAL);
    sil_wrw_mem(PBG52_PBGPROT1_m(14), (TBIT_HV_SPID|VM0_0_SPIDLIST|VM0_1_SPIDLIST));
    sil_wrw_mem(PBGERRSLV50_PBGKCPROT, LOCKKEY_VAL);    
#endif /* RLIN3_PORT0 == 7 */

    /*
     *  起動ログの出力
     */
    syslog("HV%d : RH850v2 HyperVisor start on Core%d!\n", coreid, coreid);
}

/*
 *  HVC関数
 */
int
hvc_test0(void)
{
    ID    coreid;

    GetCoreID(&coreid);
//    syslog("HV%d : hvc_test0 : clear cache!\n", coreid);
    set_icctrl(get_icctrl()|ICCTRL_ICHCLR);
    return 1;
}

int
hvc_test1(int arg1)
{
    ID    coreid;

    GetCoreID(&coreid);
//    syslog("HV%d : hvc_test1 : arg1 = %d.\n", coreid, arg1);
    return arg1*2;
}

int
hvc_test2(int arg1, int arg2)
{
    ID    coreid;

    GetCoreID(&coreid);
//    syslog("HV%d : hvc_test2 :  arg1 = %d,  arg2 = %d.\n", coreid, arg1, arg2);
    return (arg1 + arg2)*2;
}

int
hvc_test3(int arg1, int arg2, int arg3)
{
    ID    coreid;

    GetCoreID(&coreid);
//    syslog("HV%d : hvc_test3 :  arg1 = %d,  arg2 = %d,  arg3 = %d.\n", coreid, arg1, arg2, arg3);
    return (arg1 + arg2 + arg3)*2;
}

/*
 *  VM発生のMIP/MDP例外ハンドラ
 */
void
vm_mip_handler(VMEXC_INFO *pVmexcInfo)
{
    ID coreid;
    
    GetCoreID(&coreid);
    
    syslog("HV%d : MIP Handler cause 0x%x, pc 0x%x, sp 0x%x. \n",
           coreid, pVmexcInfo->cause, pVmexcInfo->pc,
           *((uint32*)(pVmexcInfo->regbase + VMEXC_REGBASE_SP)));

    syslog("HV%d : MIP Handler : reboot VM %d .\n", coreid, pVmexcInfo->vmid);

#ifdef TO_MIP_RESET    
    ResetVM(pVmexcInfo->vmid, 1, 0, 0, 0);
#endif /* TO_MIP_RESET */
#ifdef TO_MIP_FAKEFE
    RaiseVMFakeFE(pVmexcInfo->vmid, 0x90, pVmexcInfo->cause);
#endif /* TO_MIP_FAKEFE */    
}

/*
 *  VM発生のSYSERR例外ハンドラ
 */
void
vm_syserr_handler(VMEXC_INFO *pVmexcInfo)
{
    ID coreid;
    
    GetCoreID(&coreid);
    
    syslog("HV%d : SYSERR Handler cause 0x%x, pc 0x%x, sp 0x%x. \n",
           coreid, pVmexcInfo->cause, pVmexcInfo->pc,
           *((uint32*)(pVmexcInfo->regbase + VMEXC_REGBASE_SP)));

    syslog("HV%d : SYSERR Handler : reboot VM %d .\n", coreid, pVmexcInfo->vmid);

#ifdef TO_SYSERR_RESET
    ResetVM(pVmexcInfo->vmid, 2, 0, 0, 0);
#endif /* TO_SYSERR_RESET */
#ifdef TO_SYSERR_FAKEFE    
    RaiseVMFakeFE(pVmexcInfo->vmid, 0x10, pVmexcInfo->cause);
#endif /* TO_SYSERR_FAKEFE */        
}

/*
 *  HV発生のFE例外ハンドラ（ユーザーコード）
 */
void
hv_fe_handler(HVEXC_INFO *pHvexcInfo)
{
    ID coreid;
    
    GetCoreID(&coreid);

    syslog("HV%d : HV FE Handler cause 0x%x, pc 0x%x, sp 0x%x. \n",
           coreid, pHvexcInfo->cause, pHvexcInfo->pc,
           *((uint32*)(pHvexcInfo->regbase + HVEXC_REGBASE_SP)));
}

/*
 *  HV発生のEI例外ハンドラ（ユーザーコード）
 */
void
hv_ei_handler(HVEXC_INFO *pHvexcInfo)
{
    ID coreid;
    
    GetCoreID(&coreid);
    
    syslog("HV%d : HV EI Handler cause 0x%x, pc 0x%x, sp 0x%x. \n",
           coreid, pHvexcInfo->cause, pHvexcInfo->pc,
           *((uint32*)(pHvexcInfo->regbase + HVEXC_REGBASE_SP)));
}
