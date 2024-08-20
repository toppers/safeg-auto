#ifndef _HV_IMPL_H_
#define _HV_IMPL_H_

#include "common_def.h"
#include "target_hv_impl.h"
#include "hv_cfg.h"
#include "hv.h"
#include "hv_asm.h"

/*
 *  割込み番号のマスク（下位16bitのみが有効）
 */
#define INTNO_MASK(intno)    (intno & 0xffffU)

/*
 *  割込み番号の接続プロセッサのコアID（上位16bit）
 */
#define INTNO_COREID(intno)     ((intno - 1U) >> 16U)

/*
 *  コアIDの最小値と最大値の定義
 */
#define TMIN_COREID    (0U)                    /* コアIDの最小値 */
#define TMAX_COREID    (TNUM_PHYS_CORE - 1U)    /* コアIDの最大値 */

/*
 * VMIDの最小値
 */
#define TMIN_VMID    (1U)

/*
 *  SOMIDの最小値
 */
#define TMIN_SOMID    (1U)

/*
 *  VM切替時のMPUのホストエントリの開始番号
 */
#define TMIN_HEMPU    0

/*
 *  タイマドライバ
 */
#include "hvtimer_tptm.h"
#include "hvtimer_taud.h"

/*
 *  タイムウィンドウ初期化ブロックに設定する特殊なVMID値
 */
#define VMID_HVTWD    (0)
#define VMID_IDLE     (-1)

#ifndef TOPPERS_MACRO_ONLY

/*
 *  SPIDテーブル(hv_cfg.c)
 */
extern const uint8 hvspid_table[];

/* 
 * INTC2GMPIDの使用方法
 *  VMの数が(GMPID-4)より大きい場合（VM毎にINTC2GMPIDを割付け可能の場合）
 *    GMPID0～3に各コアで実行するVMのSPIDを設定
 *    タイムウィンドウ切替時にGPPIDの内容を実行するVMのSPIDに設定
 *  VMの数が(GMPID-4)以下の場合
 *    GMPIDに各VMのSPIDを設定
 */
#if (TNUM_INTC2GMPID - 4) < TNUM_VM
#define USE_DYNAMIC_INTC2GMPID

/*  
 *  VMが使用するINTC2GMPのID
 */
#define CORE0_VM_INTC2GMPID  0U
#define CORE1_VM_INTC2GMPID  1U
#define CORE2_VM_INTC2GMPID  2U
#define CORE3_VM_INTC2GMPID  3U
#endif /* (TNUM_INTC2GMPID - 4) < TNUM_VM */

/*  
 *  HVが使用するINTC2GMPのID
 */
#define CORE0_HV_INTC2GMPID  4U
#define CORE1_HV_INTC2GMPID  5U
#define CORE2_HV_INTC2GMPID  6U
#define CORE3_HV_INTC2GMPID  7U

/*
 *  HVが使用するINTC2GMPのIDのビットマスク
 */
#define HV_INTC2GMPID_BITMASK ((1U << CORE0_HV_INTC2GMPID)|(1U << CORE1_HV_INTC2GMPID)|(1U << CORE2_HV_INTC2GMPID)|(1U << CORE3_HV_INTC2GMPID))

/*
 *  前方参照
 */
typedef struct hempu_initialization_block HEMPUINIB;
typedef struct vm_control_block VMCB;
typedef struct twd_initialization_block TWDINIB;
typedef struct core_control_block CCB;
typedef struct system_operation_mode_initialization_block SOMINIB;
typedef struct vmint_initialization_block VMINTINIB;

/*
 *  コア管理ブロック（CCB）の定義
 *  アセンブラコードからアクセスしているため新しいエントリは最後に追加すること 
 */
struct core_control_block {
    /*
     *  実行中のVMのコンテキスト
     *  HVタイムウィンドウとアイドルタイムウィンドウ実行中はNULLに設定 
     */
    VMCB *p_runvm;

    /*
     *  実行中のタイムウィンドウの
     *  アイドルタイムウィンドウ実行中はNULLに設定 
     */
    TWDINIB const *p_runtwd;

    /*
     *  HVスタック（HVアイドル処理とHV割込み）の現在値
     */
    uint32 sp;

    /*
     *  HVC実行中か
     */
    boolean runhvc;

    /*
     *  HVINT実行中か
     */
    boolean runhvint;

    /*
     *  実行中のVMのVMID
     */
    uint8 runvmid; 

    /*
     *  IDLE処理実行中か
     */
    boolean runidle;

    /*
     *  現在のシステム動作モード
     */
    const SOMINIB *p_cursom;
};

#endif /* TOPPERS_MACRO_ONLY */

#ifndef TOPPERS_MACRO_ONLY

/*
 *  CCBへのアクセステーブル（hv_cfg.c）
 */
extern CCB *const  p_ccb_table[];

/*
 *  自プロセッサのCCBの取得
 *
 */
#ifndef get_my_ccb
#define get_my_ccb()    (p_ccb_table[get_my_coreid()])
#endif /* get_my_pcb */

/*
 *  HVタイムウィンドウ処理コンテキストブロック
 */
typedef struct {
    uint32 sp;
    uint32 pc;
    uint32 psw;
} HVTWDCB;

#endif /* TOPPERS_MACRO_ONLY */

#ifndef TOPPERS_MACRO_ONLY
/*
 *  HVTWDCBアクセステーブル（hv_cfg.c）
 */
extern HVTWDCB *const p_hvtwdcb_table[TNUM_PHYS_CORE];

/*
 *  VM初期化ブロック
 */
typedef struct {
    uint8   coreid;         /* コアID */
    uint32  rbase;          /* リセットベクタ */
    uint8   gpid;           /* GPID */
    uint32  spidlist;       /* 設定可能なspidのリスト */
    uint8   initspid;       /* spidの初期値 */
    uint32  num_vmint;      /* 割り当てられたVMINTの個数 */
    VMINTINIB const *p_vmintinb;  /* VMINT初期化ブロックへのポインタ */
} VMINIB;

/*
 *  VM初期化ブロックテーブル(hv_cfg.c)
 */
extern const VMINIB vminib_table[];

#define GET_VMINIB(id) (&vminib_table[id - 1U])

#endif /* TOPPERS_MACRO_ONLY */

/*
 *  多重化されているシステムレジスタのVMCBでの番号
 */
#define GMEIPC_NO       (0U)
#define GMEIPSW_NO      (1U)
#define GMFEPC_NO       (2U)
#define GMFEPSW_NO      (3U)
#define GMPSW_NO        (4U)
#define GMMEA_NO        (5U)
#define GMMEI_NO        (6U)
#define GMEIIC_NO       (7U)
#define GMFEIC_NO       (8U)
#define GMSPID_NO       (9U)
#define GMSPIDLIST_NO   (10U)
#define GMEBASE_NO      (11U)
#define GMINTBP_NO      (12U)
#define GMINTCFG_NO     (13U)
#define GMPLMR_NO       (14U)
#define GMSVLOCK_NO     (15U)
#define GMMPM_NO        (16U)
#define GMEIWR_NO       (17U)
#define GMFEWR_NO       (18U)

/*
 *   多重化されているシステムレジスタの初期値
 */
#define GMEIPSW_INIT    0x00000020
#define GMFEPSW_INIT    0x00000020
#define GMPSW_INIT      0x00000020
#define GMEIIC_INIT     0x00000000
#define GMFEIC_INIT     0x00000000
#define GMINTCFG_INIT   0x000F0000
#define GMPLMR_INIT     0x00000010
#define GMSVLOCK_INIT   0x00000000
#define GMMPM_INIT      0x00000004  /* ホストエントリによる保護を有効化(GMPE = 1) */

/*
 *  多重化されてないシステムレジスタの番号
 */
#define FPSR_NO     0U
#define FPEPC_NO    1U
#define CTPC_NO     2U
#define CTPSW_NO    3U
#define CTBP_NO     4U
#define MCTL_NO     5U
#define SCCFG_NO    6U
#define SCBP_NO     7U
#define RBCR0_NO    9U
#define RBCR1_NO    9U
#define RBNR_NO     10U
#define RBIP_NO     11U
#define MCA_NO      12U
#define MCS_NO      13U
#define MCR_NO      14U
#define MCI_NO      15U
#define MPIDX_NO    16U

/*
 *  多重化されてないシステムレジスタの初期値
 */
#define FPSR_INIT     0x00220000
#define CTPSW_INIT    0x00000000
#define CTBP_INIT     0x00000000
#define MCTL_INIT     0x00000000
#define SCCFG_INIT    0x00000000
#define SCBP_INIT     0x00000000
#define RBCR0_INIT    0x00000000
#define RBCR1_INIT    0x0000FFFF
#define RBNR_INIT     0x00000000

#ifndef TOPPERS_MACRO_ONLY
/*
 *  HEMPU初期化ブロック
 */
struct hempu_initialization_block {
    uint32 mpla;
    uint32 mpua;
    uint32 mpat;
};

/*
 *  HEMPU初期化ブロックテーブル(hv_cfg.c)
 */
extern const HEMPUINIB hempuinib_table[][TNUM_HEMPU];

/*
 *  VMコントロールブロック（VMCB）
 */
struct vm_control_block {
    uint32 reg[TNUM_REG];            /* 汎用レジスタ保存領域 */
    uint32 pc;                       /* PC保存領域 */
    uint32 s_sysreg[TNUM_S_SYSREG];  /* 多重化されていないシステムレジスタ保存領域 */
    uint32 d_sysreg[TNUM_D_SYSREG];  /* 多重化されているシステムレジスタ保存領域 */
    VMINIB const *p_vminib;          /* VM初期化ブロックへのポインタ */
    HEMPUINIB const *p_hempuinib;    /* MPU初期化ブロックへのポインタ */
#ifdef SUPPORT_SBUF
    HEMPUINIB sbufmpu[TNUM_HEMPU_SBUF];
#endif /* SUPPORT_SBUF */
};

#endif /* TOPPERS_MACRO_ONLY */

#ifndef TOPPERS_MACRO_ONLY

/*
 *  VMCBアクセステーブル(hv_cfg.c)
 */
extern VMCB *const p_vmcb_table[];

/*
 *  VMIDからVMCBを取り出すためのマクロ
 */
#define INDEX_VM(vmid)    ((uint_t)((vmid) - TMIN_VMID))
#define get_vmcb(vmid)    (p_vmcb_table[vmid - 1])

/*
 *  VMCBからVMIDを取り出すためのマクロ
 */
#define VMID(p_vmcb)    ((ID)(((p_vmcb)->p_vminib - vminib_table) + TMIN_VMID))

/*
 *  VMIDのチェック
 */
#define VALID_VMID(vmid)    (TMIN_VMID <= (vmid) && (vmid) <= TNUM_VM)

/*
 * VM割込み初期化ブロック
 */
struct vmint_initialization_block {
    uint32 intno;
};

/*
 *  タイムウィンドウ初期化ブロック
 */
struct twd_initialization_block {
    sint32  vmid;         /* 割り付けるVMのVMID */
    uint32  duration;     /* 区間長 */
    uint32  twtgintno;    /* タイムウィンドウトリガ割込み番号   */
    uint32  twtgintcyc;   /* タイムウィンドウトリガ割込みの周期 */
    uint32  *p_twtgcnt;   /* タイムウィンドウトリガ割込みのカウンタへのポインタ */
};

/*
 *  タイムウィンドウ初期化ブロックアクセステーブル（hv_cfg.c）
 */
extern const TWDINIB *const p_twdinib_table[];

#define get_my_twdinib()    (p_twdinib_table[get_my_coreid()])

/*
 *  VMモードへの遷移（hv.c）
 */
extern void vm_start(uint32 vmid);

/*
 *  hvtrap呼び出し関数（hv_asm.S）
 */
extern void call_hvtrap(void);

/*
 *  HVの初期化（hv.c）
 */
extern void hv_init(void);

/*
 *  TDMAスケジューリング初期化
 */
extern void tdmas_init(void);

/*
 *  システム周期切換え処理
 */
extern void scyc_switch(void);

/*
 *  タイムウィンドウ切換え処理
 */
extern void twd_switch(void);

/*
 *  HVタイムウィンドウ初期化
 */
extern void hvtwd_init(uint32 hv_twd, uint32 psw, uint32 sp);

/*
 * スレーブガードの初期化
 */
extern void sguard_init(void);

/*
 *  コアINDEXの取得
 */
#define get_my_coreid get_peid 

/*
 *  リーダコアの判定
 */
LOCAL_INLINE boolean
is_leader(void)
{
    if (LEADER_COREID == get_my_coreid()) {
        return true;
    }
    else {
        return false;
    }
}

/*
 *  ハイパーバイザの起動／終了に用いるバリア同期
 */
extern void barrier_sync(uint phase);

/*
 *  システム周期の同期に用いるバリア同期
 */
extern void scyc_barrier_sync(uint phase);

/*
 *  スピンロック取得
 */
LOCAL_INLINE void
acquire_lock(uint32 *p_lock)
{
    while (1) {
        if (acquire_lock_ldlstc(p_lock) != false) {
            /* ロック取得成功 */
            MEMORY_CHANGED;
            return;
        }
    }
}

/*
 *  スピンロックの解放
 */
LOCAL_INLINE void
release_lock(uint32 *p_lock)
{
    MEMORY_CHANGED;
    release_lock_ldlstc(p_lock);
}

/*
 *  HV割込み初期化ブロック
 */
typedef struct hvint_initialization_block {
    uint32  intno;     /* 割込み番号 */
    uint8   coreid;    /* 割付けコア */
    uint8   intpri;    /* 割込み優先度 */
    boolean enaint;    /* 起動時の割込み許可状態 */
} HVINTINIB;

/*
 *  HV割込み初期化ブロックのエリア（hv_cfg.c）
 */
extern HVINTINIB hvinib_table[];

/*
 *  HV割込みハンドラのINTC1用の登録テーブル用のアクセステーブル（hv_cfg.c）
 */
extern const FP *const p_hvint_intc1_table[TNUM_PHYS_CORE];

/*
 *  HV割込みハンドラのINTC2用の登録テーブル（hv_cfg.c）
 */
extern const FP hvint_intc2_table[];

/*
 *  HV割込みの初期化
 */
extern void hvint_init(void);

/*
 *  HVタイムウィンドウのスタックのアクセステーブル
 */
extern uint8 *const hvtwd_stkpt_table[TNUM_PHYS_CORE];
#endif /* TOPPERS_MACRO_ONLY */

#ifndef TOPPERS_MACRO_ONLY

/*
 *  HVC登録テーブル(hv_cfg.c)
 */
extern void *const hvc_table[];

/*
 *  起動時の同期用変数 
 */
extern uint32 bootsync;

#if TNUM_SUPPORT_CORE > 1
/*
 *  ジャイアントロック変数
 */
extern uint32 giant_lock;

INLINE void
acquire_giant_lock() {
    acquire_lock(&giant_lock);
}

INLINE void
release_giant_lock() {
    release_lock(&giant_lock);
}
#else  /* !TNUM_SUPPORT_CORE > 1 */
INLINE void
acquire_giant_lock() {
}

INLINE void
release_giant_lock() {

}
#endif /* TNUM_SUPPORT_CORE > 1 */

/*
 *  VMタイムウィンドウへの遷移（hv_support.S）
 */
extern void vmtwd_start_asm(uint32 *arg0, uint32 *arg1);

/*
 *  HVタイムウィンドウへの遷移（hv_support.S）
 */
extern void hvtwd_start_asm(void);

/*
 *  スタートアップフック関数（ユーザーコード）
 */
extern void startup_hook(void);

/*
 *  システム周期切り替えフック関数（ユーザーコード）
 */
extern void scyc_switch_hook(void);

/*
 *  タイムウィンドウ切り替えフック関数
 */
extern void twd_switch_hook(void);

/*
 *  HVタイムウィンドウ処理（ユーザーコード）
 */
extern void hv_twd(void);

/*
 *  HVアイドル処理（ユーザーコード）
 */
extern void hv_idle(void);

/*
 *  VM発生のMIP/MDP例外ハンドラ（ユーザーコード）
 */
extern void vm_mip_handler(VMEXC_INFO *pVmexcInfo);

/*
 *  VM発生のSYSERR例外ハンドラ（ユーザーコード）
 */
extern void vm_syserr_handler(VMEXC_INFO *pVmexcInfo);

/*
 *  HV発生のFE例外ハンドラ（ユーザーコード）
 */
extern void hv_fe_handler(HVEXC_INFO *pHvexcInfo);

/*
 *  HV発生のEI例外ハンドラ（ユーザーコード）
 */
extern void hv_ei_handler(HVEXC_INFO *pHvexcInfo);

#endif /* TOPPERS_MACRO_ONLY */

/*
 *  VMTWに割り込んだHVINTでのコンテキストのオフセット
 */
#define HVINT_REGBASE_SP   (0 * 4)
#define HVINT_REGBASE_FPSR (1 * 4)
#define HVINT_REGBASE_R31  (2 * 4)
#define HVINT_REGBASE_R30  (3 * 4)
#define HVINT_REGBASE_R19  (4 * 4)
#define HVINT_REGBASE_R18  (5 * 4)
#define HVINT_REGBASE_R17  (6 * 4)
#define HVINT_REGBASE_R16  (7 * 4)
#define HVINT_REGBASE_R15  (8 * 4)
#define HVINT_REGBASE_R14  (9 * 4)
#define HVINT_REGBASE_R13  (10 * 4)
#define HVINT_REGBASE_R12  (11 * 4)
#define HVINT_REGBASE_R11  (12 * 4)
#define HVINT_REGBASE_R10  (13 * 4)
#define HVINT_REGBASE_R9   (14 * 4)
#define HVINT_REGBASE_R8   (15 * 4)
#define HVINT_REGBASE_R7   (16 * 4)
#define HVINT_REGBASE_R6   (17 * 4)
#define HVINT_REGBASE_R5   (18 * 4)
#define HVINT_REGBASE_R4   (19 * 4)
#define HVINT_REGBASE_R1   (20 * 4)

/*
 *  TRAP0呼び出し（引数1）
 */
extern void cal_trap0_1(int no);

/*
 *  システム動作モード初期化ブロック
 */
struct system_operation_mode_initialization_block {
    const TWDINIB *p_twdinib[TNUM_PHYS_CORE]; /* タイムウィンドウ初期化ブロック */
};

/*
 *  現在のシステム動作モード
 *
 *  現在のシステム全体でのシステム動作モードの初期化ブロックを指すポイ
 *  ンタ．
 */
extern const SOMINIB *p_global_cursom;

/*
 *  次のシステム動作モード
 */
extern const SOMINIB *p_global_nxtsom;

/*
 *  システム周期切換えを実行したプロセッサ（ビットマップ）
 */
extern uint32_t scycprc_bitmap;

/*
 *  システム動作モード初期化ブロックのエリア（hv_cfg.c）
 */
extern const SOMINIB	sominib_table[];

/*
 *  システム動作モードIDからシステム動作モード初期化ブロックを取り出す
 *  ためのマクロ
 */
#define INDEX_SOM(somid)	((uint32)((somid) - TMIN_SOMID))
#define get_sominib(somid)	(&(sominib_table[INDEX_SOM(somid)]))

/*
 *  システム動作モード初期化ブロックからシステム動作モードIDを取り出す
 *  ためのマクロ
 */
#define	SOMID(p_sominib)	((ID)(((p_sominib) - sominib_table) + TMIN_SOMID))

/*
 *  SOMIDのチェック
 */
#define VALID_SOMID(somid)	(TMIN_SOMID <= (somid) && (somid) <= TNUM_SOM)

#endif /* _HV_IMPL_H_ */
