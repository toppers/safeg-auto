#include "hv.h"
#include "ivccom.h"
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
 *  ハイパーバイザーの動作モード
 */
#define HV_MODE        1

/*
 *  HVユーザーメイン関数
 */
void
rh850_main(void)
{
    /*
     *  IVC関連の初期化
     */

    /*
     *  IVC状態変数初期化
     */
    initialize_ivcstatevariable();

    /*
     *  IVCメッセージキュー初期化
     */
    initialize_ivcmessagequeue();
    
    /*
     *  ハイパーバイザーの呼び出し
     */    
    StartHV(HV_MODE);
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

#if 0
    /*
     *  HVのSPIDの設定
     */
    set_spid(0x00);

    /* 
     *  PBGの設定
     *  CRAMのプロテクションレジスタへアクセス可能とする． 
     */
    /* 書き込みロックの解除 */
    *PBGERRSLV00_PBGKCPROT = PBGERRSLV00_UNLOCK_KEY;
    /* 書き込み待ち */
    (void)*PBGERRSLV00_PBGKCPROT; Asm("synci");

    /* HVからCRAM0のガードレジスタにアクセス可能とする */
    *PGB00_PBGPROT1n(PGB00_C0RAMG_NO) = 1U << get_spid();

    /* 書き込みロックの設定 */
    *PBGERRSLV00_PBGKCPROT = PBGERRSLV00_LOCK_KEY;
    (void)*PBGERRSLV00_PBGKCPROT; Asm("synci");

    /* ここまではOK */

    /*
     *  CRGの設定
     *    
     */
    /* 書き込みロックの解除 */
    *CRAMCRGn0_CRGKCPROT(0) = CRGKCPROT_UNLOCK_KEY;
    *CRAMCRGn0_CRGKCPROT(0); Asm("synci"); 

    /* VM0_0用のCRAM領域の設定 */
    *CRAMCSGn0_CSGBADm(0, 0) = 0x00020000;
    *CRAMCSGn0_CSGADVm(0, 0) = 0x00070000; /* 0x00020000 - 0x0002ffff */
    *CRAMCSGn0_CSGSPIDm(0, 0) = VM0_0_SPIDLIST;
    *CRAMCSGn0_CSGPROTm(0, 0) = 0x00000150;
    
    /* VM0_1用のCRAM領域の設定 */
    *CRAMCSGn0_CSGBADm(0, 1) = 0x00030000;
    *CRAMCSGn0_CSGADVm(0, 1) = 0x00070000; /* 0x00020000 - 0x0002ffff */
    *CRAMCSGn0_CSGSPIDm(0, 1) = VM0_1_SPIDLIST;
    *CRAMCSGn0_CSGPROTm(0, 1) = 0x00000150;

    /* HV用のCRAM領域の設定 */
    *CRAMCSGn0_CSGBADm(0, 2) = 0x00000000;
    *CRAMCSGn0_CSGADVm(0, 2) = 0x00060000; /* 0x00000000 - 0x0001ffff */
    *CRAMCSGn0_CSGSPIDm(0, 2) = 0x00000001;
    *CRAMCSGn0_CSGPROTm(0, 2) = 0x00000150;

    /* VM0_0を初期オーナーとする */
    *CRAMCRGn0_CRGSPIDm(0, 0) = 1U << VM0_0_SPID;
    /* VM0_0とVM0_1からアクセス可能とする */
    *CRAMCRGn0_CRGIVCSPIDm(0, 0) = (1U << VM0_0_SPID) | (1U << VM0_1_SPID);
    /* IVCで使用するメモリ領域の先頭アドレスを指定 */
    *CRAMCRGn0_CRGBADm(0, 0) = 0x00070000;
    /*
     *  IVCで使用するメモリ領域のサイズを指定 1:CRGBADの内容を使用する，0:0/1共に有効とする
     *  設定可能なビット 0x0007ff00
     *  使用したいアドレス範囲  0x00010000 ～ 0x0002ffff
     *   CRGBADmへの設定値 : 0x00010000
     *   CRGADVmへの設定値 : 0x00070000
     */
    /* ToDo 計算が面倒なので決め打ちで */
    *CRAMCRGn0_CRGADVm(0, 0) = 0x0007e000;

    /*
     *  属性の設定
     *  OW : 1 IVC有効
     *  GEN : 1 Enable guard setting
     *  DBG : 1 Debug master Enable R/W
     *  UM  : 1 User Mode Enable R/W
     *  WG  : 0 Write Global Disable
     *  RG  : 0 Read  Global Disable
     */
    *CRAMCRGn0_CRGPROTm(0, 0) = 0x00010150;

    /* CRAM0 チャネル1 */
    /* VM0_0を初期オーナーとする */
    *CRAMCRGn0_CRGSPIDm(0, 1) = 1U << VM0_0_SPID;
    *CRAMCRGn0_CRGIVCSPIDm(0, 1) = (1U << VM0_0_SPID) | (1U << VM0_1_SPID);
    *CRAMCRGn0_CRGBADm(0, 1) = 0x00072000;
    *CRAMCRGn0_CRGADVm(0, 1) = 0x0007e000;
    *CRAMCRGn0_CRGPROTm(0, 1) = 0x00010150;

    /* 書き込みロックの設定 */
    *CRAMCRGn0_CRGKCPROT(0) = CRGKCPROT_LOCK_KEY;
    /* 書き込み完了待ち */
    (void)*CRAMCRGn0_CRGKCPROT(0);
    Asm("synci");
#endif


    /*
     *  CRGの設定
     *    
     */
    /* 書き込みロックの解除 */
    sil_wrw_mem(CRAMCRGn0_CRGKCPROT(0), UNLOCKKEY_VAL);
    (void)sil_rew_mem(CRAMCRGn0_CRGKCPROT(0));
    Asm("synci"); 

    /* CRAM0 チャネル0 */
    /* VM0_0を初期オーナーとする */
    sil_wrw_mem(CRAMCRGn0_CRGSPIDm(0, 1), (1U << VM0_0_SPID));
    /* VM0_0とVM0_1からアクセス可能とする */
    sil_wrw_mem(CRAMCRGn0_CRGIVCSPIDm(0, 1), ((1U << VM0_0_SPID) | (1U << VM0_1_SPID)));
    /* IVCで使用するメモリ領域の先頭アドレスを指定 */
    sil_wrw_mem(CRAMCRGn0_CRGBADm(0, 1), 0x00070000);
    /*
     *  IVCで使用するメモリ領域のサイズを指定 1:CRGBADの内容を使用する，0:0/1共に有効とする
     *  設定可能なビット 0x0007ff00
     *  使用したいアドレス範囲  0x00010000 ～ 0x0002ffff
     *   CRGBADmへの設定値 : 0x00010000
     *   CRGADVmへの設定値 : 0x00070000
     */
    /* ToDo 計算が面倒なので決め打ちで */
    sil_wrw_mem(CRAMCRGn0_CRGADVm(0, 1), 0x0007e000);

    /*
     *  属性の設定
     *  OW : 1 IVC有効
     *  GEN : 1 Enable guard setting
     *  DBG : 1 Debug master Enable R/W
     *  UM  : 1 User Mode Enable R/W
     *  WG  : 0 Write Global Disable
     *  RG  : 0 Read  Global Disable
     */
    sil_wrw_mem(CRAMCRGn0_CRGPROTm(0, 1), 0x00010150);

    /* CRAM0 チャネル2 */
    /* VM0_0を初期オーナーとする */
    sil_wrw_mem(CRAMCRGn0_CRGSPIDm(0, 2), 1U << VM0_0_SPID);
    sil_wrw_mem(CRAMCRGn0_CRGIVCSPIDm(0, 2), (1U << VM0_0_SPID) | (1U << VM0_1_SPID));
    sil_wrw_mem(CRAMCRGn0_CRGBADm(0, 2), 0x00072000);
    sil_wrw_mem(CRAMCRGn0_CRGADVm(0, 2), 0x0007e000);
    sil_wrw_mem(CRAMCRGn0_CRGPROTm(0, 2), 0x00010150);

    /* 書き込みロックの設定 */
    sil_wrw_mem(CRAMCRGn0_CRGKCPROT(0), LOCKKEY_VAL);
    /* 書き込み完了待ち */
    (void)sil_rew_mem(CRAMCRGn0_CRGKCPROT(0));
    Asm("synci");
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
