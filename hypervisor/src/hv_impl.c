#include "hv_impl.h"
#include "hv_cfg.h"
#include "Compiler_hv_impl.h"

/*
 *  起動時の同期変数
 */
uint32_t bootsync;

#if TNUM_SUPPORT_CORE > 1
/*
 *  ジャイアントロック変数
 */
uint32 giant_lock;
#endif /* TNUM_SUPPORT_CORE > 1 */

/*
 *  バリア同期用変数
 */
static volatile uint32_t    core_phase[TNUM_PHYS_CORE];
static volatile uint32_t    sys_phase;

/*
 *  現在のシステム動作モード
 */
const SOMINIB *p_global_cursom;

/*
 *  次のシステム動作モード
 */
const SOMINIB *p_global_nxtsom;

#if TNUM_SUPPORT_CORE > 1
/*
 *  システム周期切換えを実行したプロセッサ（ビットマップ）
 */
uint32_t scycprc_bitmap;
#endif /* TNUM_SUPPORT_CORE > 1 */


#ifdef USE_DYNAMIC_INTC2GMPID

/*
 *  CORE毎のVMのspidを設定するINTC2GMPのID
 */
const uint8_t core_vm_intc2gmpid_table[] = {
    CORE0_VM_INTC2GMPID,
    CORE1_VM_INTC2GMPID,
    CORE2_VM_INTC2GMPID,
    CORE3_VM_INTC2GMPID
};

#endif /* USE_DYNAMIC_INTC2GMPID */

/*
 *  バリア同期
 */
void
barrier_sync(uint32_t phase)
{
    uint32  count;
    uint32  i;

    core_phase[get_my_coreid()] = phase;

    if (is_leader()) {
        /* リーダコアの処理 */
        do {
            count = 0;
            for (i = 0; i < TNUM_PHYS_CORE; i++) {
                if (core_phase[i] == phase) {
                    count++;
                }
            }
        } while (count < TNUM_SUPPORT_CORE);
        sys_phase = phase;
    }
    else {
        /* 他のコアの処理 */
        while (sys_phase != phase) {
        }
    }
}

/*
 *  HVの初期化 : 各コアで実行
 */
void
hv_init(void)
{
    uint32        lp;
    const VMINIB  *p_vminib;
    VMCB          *p_vmcb;
    ID            my_coreid = get_my_coreid();
    CCB           *p_my_ccb = get_my_ccb();
    uint32        reg_lp;
    uint32        vmintlp;
#ifdef SUPPORT_SBUF
    uint32        sbuflp;
#endif /* SUPPORT_SBUF */
    
    /*
     *  SPIDの設定
     */
    set_spid(hvspid_table[my_coreid]);

    /*
     *  仮想化支援機能の有効化
     */
    set_hvcfg(HVCFG_HVE_MASK);
    set_ihvcfg(INTC1_IHVCFG_IHVE_MASK);
    
    /*
     *  CCB関連の初期化
     */
    p_my_ccb->runhvc = false;
    p_my_ccb->runhvint = false;
    p_my_ccb->runidle = false;

    /*
     *  各VMの初期化
     */
    for (lp = 0; lp < TNUM_VM; lp++) {
        /* 自コアに割り付けのVMのみ初期化する */
        if (vminib_table[lp].coreid == my_coreid) {
            p_vmcb = p_vmcb_table[lp];
            p_vminib = &(vminib_table[lp]);

            /* 初期化ブロックの設定 */
            p_vmcb->p_vminib = p_vminib;
            p_vmcb->p_hempuinib = hempuinib_table[lp];

            /* レジスタの初期値の設定 */
            p_vmcb->pc = p_vminib->rbase;
            p_vmcb->d_sysreg[GMSPID_NO] = p_vminib->initspid;
            p_vmcb->d_sysreg[GMSPIDLIST_NO] = p_vminib->spidlist;
            p_vmcb->d_sysreg[GMEBASE_NO] = p_vminib->rbase & ~0xff;
            
            p_vmcb->d_sysreg[GMEIPSW_NO] = GMEIPSW_INIT;
            p_vmcb->d_sysreg[GMFEPSW_NO] = GMFEPSW_INIT;
            p_vmcb->d_sysreg[GMPSW_NO] = GMPSW_INIT;

            p_vmcb->d_sysreg[GMEIIC_NO] = GMEIIC_INIT;
            p_vmcb->d_sysreg[GMFEIC_NO] = GMFEIC_INIT;
            p_vmcb->d_sysreg[GMINTCFG_NO] = GMINTCFG_INIT;
            p_vmcb->d_sysreg[GMPLMR_NO] = GMPLMR_INIT;
            p_vmcb->d_sysreg[GMSVLOCK_NO] = GMSVLOCK_INIT;
            p_vmcb->d_sysreg[GMMPM_NO] = GMMPM_INIT;

            p_vmcb->s_sysreg[FPSR_NO] = FPSR_INIT;
            p_vmcb->s_sysreg[CTPSW_NO] = CTPSW_INIT;
            p_vmcb->s_sysreg[CTBP_NO] = CTBP_INIT;
            p_vmcb->s_sysreg[MCTL_NO] = MCTL_INIT;
            p_vmcb->s_sysreg[SCCFG_NO] = SCCFG_INIT;
            p_vmcb->s_sysreg[SCBP_NO] = SCBP_INIT;
            p_vmcb->s_sysreg[RBCR0_NO] = RBCR0_INIT;
            p_vmcb->s_sysreg[RBCR1_NO] = RBCR1_INIT;
            p_vmcb->s_sysreg[RBNR_NO] = RBNR_INIT;

            for (reg_lp = 0; reg_lp < TNUM_REG; reg_lp++) {
                p_vmcb->reg[reg_lp] = 0U;
            }
#ifdef SUPPORT_SBUF
            for (sbuflp = 0; sbuflp < TNUM_HEMPU_SBUF; sbuflp++) {
                p_vmcb->sbufmpu[sbuflp].mpat = 0U;
            }
#endif /* SUPPORT_SBUF */

            /*
             *  VM割込みの初期化
             *  自コアのINTC1と自コアで実行するVMのINTC2のみ初期化する
             */            
            for (vmintlp = 0; vmintlp < p_vminib->num_vmint; vmintlp++) {                
                bind_int_to_gm(INTNO_MASK((p_vminib->p_vmintinb)[vmintlp].intno),
                               p_vminib->gpid,
                               p_vminib->coreid);
            }         
        }
    }

    /*
     *  MPUの初期化
     */
    /* 全てホストエントリに */
    set_mpcfg((get_mpcfg() & ~MPCFG_HBE_MASK) | (0U << MPCFG_HBE_BIT));
    /* 例外発生時はホストに遷移 */
    set_gmcfg(get_gmcfg()|GMCFG_HMP_MASK);
    /* 全領域を無効に */
    for (lp = 0; lp < TNUM_MPU; lp++) {
        set_mpidx(lp);
        set_mpat(0);
    }
    /*
     *  メモリ保護機能の有効化
     *   SVモードでは保護は無効
     *   ハイパーバイザは常にSVモードで動作するが，VCOM通信で
     *   MPUのチェック機構を使用するため使用する．
     */
    set_mpm(MPM_MPE_MASK);

    /* 
     *  コプロセッサの初期化
     *   GCPU0=1 : 仮想マシンからFPUのみアクセス可能に
     */
    set_gmcfg(get_gmcfg()|GMCFG_GCU0_MASK);

    /*
     *  SYSERR例外関連の初期化
     */
    set_decfg(DECFG_EHE|DECFG_EGE|DECFG_ESE);
    /* SYSERR例外発生時はホストに遷移 */      
    set_gmcfg(get_gmcfg()|GMCFG_GSYSE_MASK);
}

/*
 *  TDMAスケジューリング初期化
 */
void
tdmas_init(void)
{
    CCB *p_my_ccb = get_my_ccb();

    /*
     *  起動時はアイドルタイムウィンドウ実行中とする
     */
    p_my_ccb->p_runtwd = NULL;
    p_my_ccb->p_runvm = NULL;
    p_my_ccb->runvmid = 0;
}

/*
 *  HVタイムウィンドウ初期化
 */
void
hvtwd_init(uint32_t hv_twd, uint32_t psw, uint32_t sp)
{
    HVTWDCB *hvtwdcb = p_hvtwdcb_table[get_my_coreid()];

    hvtwdcb->pc = (uint32)hv_twd;
    hvtwdcb->psw = psw;

    /* 
     *  スタック上にダミーフレームを生成 
     *   HVタイムウィンドウの初回呼び出しに復帰する内容をスタックに格納．
     *   fpsr以外のレジスタは不定で良いので特に初期化しない．
     */
    hvtwdcb->sp = sp - (4*(1+1+14+2+10));
    *(uint32_t *)(sp - (4*2)) = 0x000000002U; /* fpsrの初期値 */
}

/*
 *  HV割込みの初期化
 */
void 
hvint_init(void) 
{
    uint32      lp;
    uint        my_coreid = get_my_coreid();
    HVINTINIB   *p_hvintinib;

    for(lp = 0; lp < TNUM_HVINT; lp++) {
        p_hvintinib = &hvinib_table[lp];
        if (p_hvintinib->coreid == my_coreid) {
            bind_int_to_host(p_hvintinib->intno, my_coreid);
            set_exint_vector(p_hvintinib->intno, INTVECTOR_PRI);
            set_exint_pri(p_hvintinib->intno, p_hvintinib->intpri);
            if (p_hvintinib->enaint) {
                ena_exint(p_hvintinib->intno);
            }
        }
    }
}

/*
 *  スレーブガードの初期化 : リーダコアでのみ実行
 */
void 
sguard_init(void)
{
    uint32    lp;

    /*
     *  LRAMのガード設定
     *   他コアのHVと仮想マシンからアクセス可能とする
     */
    for(lp = 0; lp < TNUM_PHYS_CORE; lp++) {
        if (TBIT_SUPPORT_CORE & (1U << lp)) {
            sil_wrw_mem(PEGKCPROTm(lp), UNLOCKKEY_VAL);
            sil_wrw_mem(PEGPROTm(lp, 0), CPCR_GEN|CPCR_DBG|CPCR_RG);
            sil_wrw_mem(PEGSPIDm(lp, 0), TBIT_SPID);
            sil_wrw_mem(PEGBADm(lp, 0), LRAM_BASE_COREn(lp));
            sil_wrw_mem(PEGADVm(lp, 0), ~LRAM_SIZE);
            sil_wrw_mem(PEGKCPROTm(lp), LOCKKEY_VAL);
        }
    }

    /*
     *  CRAMのガード設定
     *   他コアのHVと仮想マシンからアクセス可能とする．
     */
#if (TBIT_SUPPORT_CORE & 0x03) != 0
    sil_wrw_mem(CRAMCRGn0_CRGKCPROT(0), UNLOCKKEY_VAL);    
    sil_wrw_mem(CRAMCRGn0_CRGPROTm(0, 0), CPCR_GEN|CPCR_DBG|CPCR_RG);
    sil_wrw_mem(CRAMCRGn0_CRGSPIDm(0, 0), TBIT_SPID);
    sil_wrw_mem(CRAMCRGn0_CRGBADm(0, 0), CRAM_BASE_0);
    sil_wrw_mem(CRAMCRGn0_CRGADVm(0, 0), ~CRAM_USAGE_SIZE_0);
    sil_wrw_mem(CRAMCRGn0_CRGKCPROT(0), LOCKKEY_VAL);
#endif /* (TBIT_SUPPORT_CORE & 0x03) != 0 */
#if (TBIT_SUPPORT_CORE & 0x0C) != 0                
    sil_wrw_mem(CRAMCRGn0_CRGKCPROT(1),  UNLOCKKEY_VAL);
    sil_wrw_mem(CRAMCRGn0_CRGPROTm(1, 0), CPCR_GEN|CPCR_DBG|CPCR_RG);
    sil_wrw_mem(CRAMCRGn0_CRGSPIDm(1, 0), TBIT_SPID);
    sil_wrw_mem(CRAMCRGn0_CRGBADm(1, 0), CRAM_BASE_0);
    sil_wrw_mem(CRAMCRGn0_CRGADVm(0, 0), ~CRAM_USAGE_SIZE_1);
    sil_wrw_mem(CRAMCRGn0_CRGKCPROT(1), LOCKKEY_VAL);
#endif /* (TBIT_SUPPORT_CORE & 0x0C) == 0 */

    /*
     *  I-Busのガード設定
     *   全てのHVからのみアクセス可能
     */
    /* IPIR */
    sil_wrw_mem(IPIGGKCPROT, UNLOCKKEY_VAL);
    for(lp = 0; lp < TNUM_PHYS_CORE; lp++) {
        if (TBIT_SUPPORT_CORE & (1U << lp)) {
            sil_wrw_mem(IPIGPROT0_Rn(lp), CPCR_GEN|CPCR_DBG|CPCR_RG);
            sil_wrw_mem(IPIGPROT1_Rn(lp), TBIT_HV_SPID);
            sil_wrw_mem(IPIGPROT0_Tn(lp), CPCR_GEN|CPCR_DBG|CPCR_RG);
            sil_wrw_mem(IPIGPROT1_Tn(lp), TBIT_HV_SPID);
        }
    }
    /* ToDo 扱いが課題 */
    sil_wrw_mem(IPIGPROT0_4, CPCR_GEN|CPCR_DBG|CPCR_RG);
    sil_wrw_mem(IPIGPROT1_4, TBIT_HV_SPID);
    sil_wrw_mem(IPIGGKCPROT, LOCKKEY_VAL);

    /* BARR */
    sil_wrw_mem(BRGKCPROT, UNLOCKKEY_VAL);
    for(lp = 0; lp < TNUM_PHYS_CORE; lp++) {
        if (TBIT_SUPPORT_CORE & (1U << lp)) {
            sil_wrw_mem(BRGPROT0_n(lp), CPCR_GEN|CPCR_DBG|CPCR_RG);
            sil_wrw_mem(BRGPROT1_n(lp), TBIT_HV_SPID);
        }
    }
    /* ToDo 扱いが課題 */
    sil_wrw_mem(BRGPROT0_16, CPCR_GEN|CPCR_DBG|CPCR_RG);
    sil_wrw_mem(BRGPROT1_16, TBIT_HV_SPID);
    sil_wrw_mem(BRGKCPROT, LOCKKEY_VAL);

    /* TPTM */
    sil_wrw_mem(TPTGKCPROT, UNLOCKKEY_VAL);
    for(lp = 0; lp < TNUM_PHYS_CORE; lp++) {
        if (TBIT_SUPPORT_CORE & (1U << lp)) {
            sil_wrw_mem(TPTGPROT0_n(lp), CPCR_GEN|CPCR_DBG|CPCR_RG);
            sil_wrw_mem(TPTGPROT1_n(lp), TBIT_HV_SPID);
        }
    }
    /* ToDo 扱いが課題 */
    sil_wrw_mem(TPTGPROT0_n(8), CPCR_GEN|CPCR_DBG|CPCR_RG);
    sil_wrw_mem(TPTGPROT1_n(8), TBIT_HV_SPID);
    sil_wrw_mem(TPTGPROT0_n(9), CPCR_GEN|CPCR_DBG|CPCR_RG);
    sil_wrw_mem(TPTGPROT1_n(9), TBIT_HV_SPID);
    sil_wrw_mem(TPTGKCPROT, LOCKKEY_VAL);

    /*
     *  INTC1のガード
     *   他コアからの書き込みを禁止 
     */
    for(lp = 0; lp < TNUM_PHYS_CORE; lp++) {
        if (TBIT_SUPPORT_CORE & (1U << lp)) {
            sil_wrw_mem(PEGKCPROTm(lp) , UNLOCKKEY_VAL);
            sil_wrw_mem(PEGPROTm(lp, 1), CPCR_GEN|CPCR_DBG|CPCR_RG);
            sil_wrw_mem(PEGSPIDm(lp, 1), 0); /* 自コアはPEGを通過しない */
            sil_wrw_mem(PEGBADm(lp, 1) , INTC1_BASE_COREn(lp));
            sil_wrw_mem(PEGADVm(lp, 1) , ~INTC1_SIZE);
            sil_wrw_mem(PEGKCPROTm(lp), LOCKKEY_VAL);
        }
    }

    /*
     *  INTC2のガード
     */
    sil_wrw_mem(INTC2GKCKPROT, UNLOCKKEY_VAL);

    /* HVからは全レジスタアクセス可能 */
#if (TBIT_SUPPORT_CORE & 0x01) == 0x01
    sil_wrw_mem(INTC2GMPIDm(CORE0_HV_INTC2GMPID), HV_SPID_CORE0);
#endif /* (TBIT_SUPPORT_CORE & 0x01) == 0x01 */
#if (TBIT_SUPPORT_CORE & 0x02) == 0x02
    sil_wrw_mem(INTC2GMPIDm(CORE2_HV_INTC2GMPID), HV_SPID_CORE1);
#endif /* (TBIT_SUPPORT_CORE & 0x02) == 0x02 */    
#if (TBIT_SUPPORT_CORE & 0x04) == 0x04
    sil_wrw_mem(INTC2GMPIDm(CORE3_HV_INTC2GMPID), HV_SPID_CORE2);
#endif /* (TBIT_SUPPORT_CORE & 0x04) == 0x04 */
#if (TBIT_SUPPORT_CORE & 0x08) == 0x08
    sil_wrw_mem(INTC2GMPIDm(CORE4_HV_INTC2GMPID), HV_SPID_CORE3);
#endif /* (TBIT_SUPPORT_CORE & 0x08) == 0x08 */
    
    sil_wrw_mem(INTC2GPORT_GR, CPCR_GEN|CPCR_DBG|CPCR_RG|(HV_INTC2GMPID_BITMASK << INTC2GPORT_MPID));
    sil_wrw_mem(INTC2GPORT_IMR, CPCR_GEN|CPCR_DBG|CPCR_RG|(HV_INTC2GMPID_BITMASK << INTC2GPORT_MPID));
    for (lp = INTC2_INTNO_MIN; lp <= INTC2_INTNO_MAX; lp++) {
        sil_wrw_mem(INTC2GPORT_n(lp), CPCR_GEN|CPCR_DBG|CPCR_RG|(HV_INTC2GMPID_BITMASK << INTC2GPORT_MPID));
    }

#ifndef USE_DYNAMIC_INTC2GMPID
    /* VMの数が4個以下の場合 */
    /* 各VMに割り当てるINTC2GmPIDの初期化 */
    for (lp = 0; lp < TNUM_VM; lp++) {
        sil_wrw_mem(INTC2GMPIDm(lp), (p_vmcb_table[lp])->p_vminib->initspid);
    }
    /* 各INTC2に参照するINTC2GmPIDのIDを設定 */
    for (lp = 0; lp < TNUM_VM; lp++) {
        const VMINIB  *p_vminib = &(vminib_table[lp]);
        uint32  vmintlp;
        for (vmintlp = 0; vmintlp < p_vminib->num_vmint; vmintlp++) {
            uint32_t gmpid = lp;
            uint32_t intno = INTNO_MASK((p_vminib->p_vmintinb)[vmintlp].intno);
            /* INTC2割込みなら設定 */
            if (intno > INTC1_INTNO_MAX) {
                sil_wrw_mem(INTC2GPORT_n(intno),
                            (CPCR_GEN|CPCR_DBG|CPCR_RG|
                             (((HV_INTC2GMPID_BITMASK)|(1U << gmpid)) << INTC2GPORT_MPID)));
            }
        }
    }
#endif /* USE_DYNAMIC_INTC2GMPID */

    sil_wrw_mem(INTC2GKCKPROT, LOCKKEY_VAL);    

    /*
     *  周辺回路のガード
     */

    /*
     *  INTC2 Guard/I-Bus Guard へのアクセス権の設定
     */
    sil_wrw_mem(PBGERRSLV00_PBGKCPROT, UNLOCKKEY_VAL);
    sil_wrw_mem(PBG00_PBGPROT0_m(0)  , CPCR_GEN|CPCR_DBG|CPCR_RG);
    sil_wrw_mem(PBG00_PBGPROT1_m(0)  , TBIT_HV_SPID);    
    sil_wrw_mem(PBGERRSLV00_PBGKCPROT, LOCKKEY_VAL);

    /*
     *  ECMへのアクセス権の設定
     */
    sil_wrw_mem(PBGERRSLV00_PBGKCPROT, UNLOCKKEY_VAL);
    sil_wrw_mem(PBG00_PBGPROT0_m(0)  , CPCR_GEN|CPCR_DBG|CPCR_RG);
    sil_wrw_mem(PBG00_PBGPROT1_m(0)  , TBIT_HV_SPID);    
    sil_wrw_mem(PBGERRSLV00_PBGKCPROT, LOCKKEY_VAL);
    
    
    /*
     *  省略....
     */

    /*
     *  システム周期タイマの設定
     */
    sysctimer_sguard_init();

    /*
     *  タイムウィンドウタイマの設定
     */
    twdtimer_sguard_init();
}

/*
 *  p_runvmの更新
 *  p_runtwdを更新してから呼び出すこと 
 */
LOCAL_INLINE void
update_p_runvm(CCB *p_my_ccb)
{
    /*
     *  p_runvmの更新
     */
    if ((p_my_ccb->p_runtwd == NULL) || p_my_ccb->p_runtwd->vmid == VMID_HVTWD) {
        /* HVタイムウィンドウまたはアイドルタイムウィンドウの場合 */
        p_my_ccb->p_runvm = NULL;
        p_my_ccb->runvmid = 0;
    }
    else {
        /* 通常のタイムウィンドウの場合 */
        p_my_ccb->p_runvm = get_vmcb(p_my_ccb->p_runtwd->vmid);
        p_my_ccb->runvmid = p_my_ccb->p_runtwd->vmid;
    }
}

/*
 *  VMタイムウィンドウへの遷移
 */
static void
vmtwd_start(uint32_t my_coreid, CCB *p_my_ccb, boolean istwtimer)
{
#ifdef USE_DYNAMIC_INTC2GMPID    
    uint32_t vmintlp;
    const VMINIB  *p_vminib = p_my_ccb->p_runvm->p_vminib;
#endif /* USE_DYNAMIC_INTC2GMPID */
    
    /* 
     *  VMTWD実行中のホストモードのPSWの値
     *   Clear EBV(Enable RBASE) 
     */
    set_fepsw((PSW_EIMASK_ENALL << PSW_EIMASK_BIT)|PSW_CU0_MASK);

    /* Set FEPSWH to change to VM mode */
    set_fepswh(PSW_GM_MASK|(p_my_ccb->p_runvm->p_vminib->gpid << PSW_GPID_BIT));

    /* Set FEPC to VM return address */
    set_fepc(p_my_ccb->p_runvm->pc);

    /* MPUの設定 */
    set_mpu_vm((uint32*)(p_my_ccb->p_runvm->p_hempuinib));
#ifdef SUPPORT_SBUF
    set_mpu_sbuf((uint32*)(p_my_ccb->p_runvm->sbufmpu));
#endif /* SUPPORT_SBUF */
      
    if (istwtimer && (TBIT_SINGLEVM_CORE & (1 << my_coreid)) == 0) {
        /*
         *  タイムウィンドウタイマのスタート
         */
        twdtimer_start(my_coreid, p_my_ccb->p_runtwd->duration);
    }


#ifdef USE_DYNAMIC_INTC2GMPID
    /*
     *  INTC2のアクセス許可
     */
    acquire_giant_lock();
    sil_wrw_mem(INTC2GKCKPROT, UNLOCKKEY_VAL);
    /* コア毎のINTC2GMPIDmに実行するVMのspidを設定 */
    sil_wrw_mem(INTC2GMPIDm(core_vm_intc2gmpid_table[my_coreid]), p_my_ccb->p_runvm->p_vminib->initspid);   
    /* VMに割り当てられたINTC2へアクセス可能とする */
    for (vmintlp = 0; vmintlp < p_vminib->num_vmint; vmintlp++) {

        sil_wrw_mem(INTC2GPORT_n(INTNO_MASK((p_vminib->p_vmintinb)[vmintlp].intno)),
                                 (CPCR_GEN|CPCR_DBG|CPCR_RG
                                  |(((HV_INTC2GMPID_BITMASK)
                                     |(1U << core_vm_intc2gmpid_table[my_coreid])) << INTC2GPORT_MPID)));
    }    
    sil_wrw_mem(INTC2GKCKPROT, LOCKKEY_VAL);
    release_giant_lock();
#endif /* USE_DYNAMIC_INTC2GMPID */

    /*
     *  VMタイムウィンドウスタート
     */
    vmtwd_start_asm(&(p_my_ccb->p_runvm->reg[0]), &(p_my_ccb->p_runvm->s_sysreg[0]));
}

/*
 *  HVタイムウィンドウへの遷移
 */
LOCAL_INLINE void
hvtwd_start(uint32_t my_coreid, CCB *p_my_ccb)
{
    /*
     *  タイムウィンドウタイマのスタート
     */
    twdtimer_start(my_coreid, p_my_ccb->p_runtwd->duration);

    /*
     *  HVタイムウィンドウスタート
     */
    hvtwd_start_asm();
}

/*
 *  タイムウィンドウトリガ割込みの発生
 */
LOCAL_INLINE void
twtgint_raise(CCB* p_my_ccb)
{
    /*
     *  タイムウィンドウトリガ割込みが登録されていれば
     */
    if (p_my_ccb->p_runtwd->twtgintno != 0xFFFFFFFF) {
        (*p_my_ccb->p_runtwd->p_twtgcnt)++;
        if (*(p_my_ccb->p_runtwd->p_twtgcnt) == p_my_ccb->p_runtwd->twtgintcyc) {
            raise_exint(p_my_ccb->p_runtwd->twtgintno);
            *(p_my_ccb->p_runtwd->p_twtgcnt) = 0;
        }
    }
}

/*
 * システム動作モードの更新
 */
#if TNUM_SUPPORT_CORE > 1
static void 
update_cursom(CCB *p_my_ccb)
{
    /*
     * コア間ロックの取得
     */
    acquire_giant_lock();
    
    /*
     *  システム動作モードの変更
     */
    if (scycprc_bitmap == 0U) {
        /*
         *  最初にシステム周期を切り換えるプロセッサの場合
         */
        p_global_cursom = p_global_nxtsom;
    }
    scycprc_bitmap |= (1U << get_my_coreid());
    if (scycprc_bitmap == (TBIT_SUPPORT_CORE & ~TBIT_SINGLEVM_CORE)) {
        /*
         *  最後にシステム周期を切り換えるプロセッサの場合
         */
        scycprc_bitmap = 0U;
    }
    p_my_ccb->p_cursom = p_global_cursom;

    /*
     * コア間ロックの解放
     */
    release_giant_lock();
}
#else /* !TNUM_SUPPORT_CORE > 1 */
static void 
update_cursom(CCB *p_my_ccb)
{
    p_global_cursom = p_global_nxtsom;
    p_my_ccb->p_cursom = p_global_cursom;
}
#endif /* TNUM_SUPPORT_CORE > 1 */

/*
 *  システム周期切換え処理
 *
 *  この関数は，CPUロック状態で呼び出される．
 */
void
scyc_switch(void)
{
    CCB        *p_my_ccb = get_my_ccb();
    uint32    my_coreid = get_my_coreid();

#ifdef ENABLE_SYSC_SWITCH_HOOK
    /*
     *  フック関数の呼び出し
     */
    scyc_switch_hook();
#endif /* ENABLE_SYSC_SWITCH_HOOK */

    /*
     *   IDLE処理終了
     */
    p_my_ccb->runidle = false;

    /*
     *  システム周期タイマのクリア
     */
    sysctimer_clearint(my_coreid);

    /*
     *  システム周期の開始
     */
    DEBUGOUT("scyc_switch() : sytem cycle handler start.\n\r");

    /*
     * システム動作モードの更新
     */
    update_cursom(p_my_ccb);

    /*
     *  p_runtwdの更新
     */
    p_my_ccb->p_runtwd = &(p_my_ccb->p_cursom->p_twdinib[my_coreid])[0];

    /*
     *  タイムウィンドウトリガ割込みの発生
     */
    twtgint_raise(p_my_ccb);

    /*
     *  p_runvmの更新
     */
    update_p_runvm(p_my_ccb);

    /*
     *  遷移先に応じた関数を呼び出す
     */
    if (p_my_ccb->p_runvm != NULL) {
        /* VMTWへの遷移の場合 */
        vmtwd_start(my_coreid, p_my_ccb, true);
    } else if (p_my_ccb->p_runtwd != NULL) {
        /* HVTWへの遷移の場合 */
        hvtwd_start(my_coreid, p_my_ccb);
    }

    /* IDLEの場合はリターン */
    p_my_ccb->runidle = true;
}

/*
 *  タイムウィンドウ切換え処理
 */
void
twd_switch(void)
{
    CCB       *p_my_ccb = get_my_ccb();
    uint32    my_coreid = get_my_coreid();
    
    DEBUGOUT("twd_switch() : timer window handler start.\n\r");

#ifdef ENABLE_TWD_SWITCH_HOOK
    /*
     *  フック関数の呼び出し
     */
    twd_switch_hook();
#endif /* ENABLE_TWD_SWITCH_HOOK */

    /*
     *  タイムウィンドウタイマの停止
     */
    twdtimer_stop(my_coreid);


#ifdef USE_DYNAMIC_INTC2GMPID
    /*
     *  INTC2のアクセス許可の解除
     */
    if (p_my_ccb->p_runvm != NULL) {
        uint32_t vmintlp;
        const VMINIB  *p_vminib = p_my_ccb->p_runvm->p_vminib;
        acquire_giant_lock();
        sil_wrw_mem(INTC2GKCKPROT, UNLOCKKEY_VAL);
        /* VMTWの場合は許可していたINTC2へのアクセスを禁止する */
        for (vmintlp = 0; vmintlp < p_vminib->num_vmint; vmintlp++) {
            sil_wrw_mem(INTC2GPORT_n(INTNO_MASK((p_vminib->p_vmintinb)[vmintlp].intno)),
                        CPCR_GEN|CPCR_DBG|CPCR_RG|
                        (HV_INTC2GMPID_BITMASK << INTC2GPORT_MPID));
        }
        sil_wrw_mem(INTC2GKCKPROT, LOCKKEY_VAL);
        release_giant_lock();
    }
#endif /* USE_DYNAMIC_INTC2GMPID */
    
    /*
     *  p_runtwdの更新
     */
    p_my_ccb->p_runtwd++;

    /*
     *  タイムウィンドウトリガ割込みの発生
     */
    twtgint_raise(p_my_ccb);

    /* 最後のタイムウィンドウの場合*/
    if (p_my_ccb->p_runtwd->vmid == VMID_IDLE) {
        p_my_ccb->p_runtwd = NULL;
    }

    /*
     *  p_runvmの更新
     */
    update_p_runvm(p_my_ccb);

    if (p_my_ccb->p_runvm != NULL) {
        /* VMTWへの遷移の場合 */
        vmtwd_start(my_coreid, p_my_ccb, true);
    } else if (p_my_ccb->p_runtwd != NULL) {
        /* HVTWへの遷移の場合 */
        hvtwd_start(my_coreid, p_my_ccb);
    }

    /* IDLEの場合はリターン */
    p_my_ccb->runidle = true;
}

/*
 *  IDLE-VMへの切り替え
 */
void
switch_to_idle_vm(ID vmid)
{
    CCB       *p_my_ccb = get_my_ccb();
    uint32    my_coreid = get_my_coreid();

    DEBUGOUT("switch_to_idle_vm(void) : timer window handler start.\n\r");

    /*
     *  p_runvmの更新
     *  vmidはチェック済み
     */
    p_my_ccb->p_runvm = get_vmcb(vmid);
    p_my_ccb->runvmid = vmid;

    /* VMTWへの遷移の場合 */
    vmtwd_start(my_coreid, p_my_ccb, false);

    /* ここには来ない */
    while(1);
}

static void
call_hvint(void) {
    uint    my_coreid = get_my_coreid();
    uint    intno;
    FP      handler;
    CCB   *p_my_ccb = get_my_ccb();

    /* 割込み要因の取得 */
    intno = get_eiic();
    intno -= TMIN_EIIC_EIINT_NO; /* 0オリジンに変換 */
    
    if(intno <= INTC1_INTNO_MAX) {
        handler = (p_hvint_intc1_table[my_coreid])[intno];
    }
    else {
        handler = hvint_intc2_table[intno - INTC2_INTNO_MIN];
    }

    /* ハンドラ呼び出し */
    (handler)();
}

/*
 *  HV割込みの呼び出し処理
 */
void
call_hvint_in_vmtw(uint32 reg_base)
{
    uint    my_coreid = get_my_coreid();
    uint    time_left;
    CCB   *p_my_ccb = get_my_ccb();
    VMCB   *p_vmcb = p_my_ccb->p_runvm;
    boolean pausetwt;
    
    /* シングルVMなら状況によらずタイマ操作しない */
    if ((TBIT_SINGLEVM_CORE & (1 << my_coreid)) != 0) {
        pausetwt = false;
    }

    /*
     *  TWタイマの停止
     */
    if (pausetwt) {
        time_left = twdtimer_pause(my_coreid);
    }

    /*
     * ResetVM()/RasieVMFakeFE()/RaiseVMFakeEI() で設定する可能性のある
     * レジスタをVMCBに書き込む
     */  
    p_vmcb->reg[6] = *(uint32*)(reg_base + HVINT_REGBASE_R6);
    p_vmcb->reg[7] = *(uint32*)(reg_base + HVINT_REGBASE_R7);
    p_vmcb->reg[8] = *(uint32*)(reg_base + HVINT_REGBASE_R8);
    p_vmcb->reg[9] = *(uint32*)(reg_base + HVINT_REGBASE_R9);    
    p_vmcb->pc = get_eipc();
    p_vmcb->d_sysreg[GMPSW_NO] = get_gmpsw();
    p_vmcb->d_sysreg[GMFEIC_NO] = get_gmfeic();
    p_vmcb->d_sysreg[GMEIIC_NO] = get_gmeiic();
    p_vmcb->d_sysreg[GMFEPSW_NO] = get_gmfepsw();
    p_vmcb->d_sysreg[GMFEPC_NO] = get_gmfepc();
    p_vmcb->d_sysreg[GMEIPSW_NO] = get_gmeipsw();
    p_vmcb->d_sysreg[GMEIPC_NO] = get_gmeipc();

    call_hvint();

    /*
     * ResetVM()/RasieVMFakeFE()/RaiseVMFakeEI() で設定された可能性のある
     * レジスタを書き戻す
     */
    *(uint32*)(reg_base + HVINT_REGBASE_R6) =  p_vmcb->reg[6];
    *(uint32*)(reg_base + HVINT_REGBASE_R7) =  p_vmcb->reg[7];
    *(uint32*)(reg_base + HVINT_REGBASE_R8) =  p_vmcb->reg[8];
    *(uint32*)(reg_base + HVINT_REGBASE_R9) =  p_vmcb->reg[9];
    set_eipc(p_vmcb->pc);
    set_gmeipc(p_vmcb->d_sysreg[GMEIPC_NO]);
    set_gmeipsw(p_vmcb->d_sysreg[GMEIPSW_NO]);
    set_gmfepc(p_vmcb->d_sysreg[GMFEPC_NO]);
    set_gmfepsw(p_vmcb->d_sysreg[GMFEPSW_NO]);    
    set_gmpsw(p_vmcb->d_sysreg[GMPSW_NO]);
    set_gmeiic(p_vmcb->d_sysreg[GMEIIC_NO]);
    set_gmfeic(p_vmcb->d_sysreg[GMFEIC_NO]);    

    /*
     * TWタイマの再開
     */
    if (pausetwt) {
        twdtimer_continue(my_coreid, time_left);
    } 
}

/*
 *  HV割込みの呼び出し処理
 */
void
call_hvint_in_idle(void)
{
    call_hvint();
}

/*
 *  HV割込みの呼び出し処理
 */
void
call_hvint_in_hvtw(void)
{
    uint    my_coreid = get_my_coreid();
    uint    time_left;
    CCB   *p_my_ccb = get_my_ccb();
    
    /*
     *  TWタイマの停止
     */
    time_left = twdtimer_pause(my_coreid);

    call_hvint();

    /*
     * TWタイマの再開
     */
    twdtimer_continue(my_coreid, time_left);
}

/*
 *  HVアイドル処理中にタイムウィンドウ割込みが発生した場合
 */
void
error_call_twd_handler_idle(void)
{
    uart_puts(0, "error_call_twd_handler_idle() : TWD Handler is called at hv idle!\n\r");
    while(1);
}

/*
 *  VMタイムウィンドウ処理中にシステム周期割込みが発生した場合
 */
void
error_call_scyc_handler_vmtwd(void)
{
    uart_puts(0, "error_call_scyc_handler_vmtwd() : SCYCD Handler is called at vm time window!\n\r");
    while(1);
}

/*
 *  HVタイムウィンドウ処理中にシステム周期割込みが発生した場合
 */
void
error_call_scyc_handler_hvtwd(void)
{
    uart_puts(0, "error_call_scyc_handler_hvtwd() : SCYCD Handler is called at hv time window!\n\r");
    while(1);
}

/*
 *  VMで発生した例外のハンドラ
 *  FE例外を前提（MIP/MDP, SYSERR）
 */
void
call_vm_exc_handler(uint32 reg_base)
{
    CCB   *p_my_ccb = get_my_ccb();
    uint32_t cause_no;
    VMEXC_INFO vmexc_info;
    VMCB   *p_vmcb = p_my_ccb->p_runvm;
      
    /* VM発生の例外情報の設定 */
    vmexc_info.cause = get_feic();
    vmexc_info.vmid = p_my_ccb->runvmid;
    vmexc_info.pc = get_fepc();
    vmexc_info.psw = get_fepsw();
    vmexc_info.regbase = reg_base;

    /*
     * ResetVM()/RasieVMFakeFE()/RaiseVMFakeEI() で設定する可能性のある
     * レジスタをVMCBに書き込む
     */  
    p_vmcb->reg[6] = *(uint32*)(reg_base + VMEXC_REGBASE_R6);
    p_vmcb->reg[7] = *(uint32*)(reg_base + VMEXC_REGBASE_R7);
    p_vmcb->reg[8] = *(uint32*)(reg_base + VMEXC_REGBASE_R8);
    p_vmcb->reg[9] = *(uint32*)(reg_base + VMEXC_REGBASE_R9);    
    p_vmcb->pc = get_fepc();
    p_vmcb->d_sysreg[GMPSW_NO] = get_gmpsw();
    p_vmcb->d_sysreg[GMFEIC_NO] = get_gmfeic();
    p_vmcb->d_sysreg[GMEIIC_NO] = get_gmeiic();
    p_vmcb->d_sysreg[GMFEPSW_NO] = get_gmfepsw();
    p_vmcb->d_sysreg[GMFEPC_NO] = get_gmfepc();
    p_vmcb->d_sysreg[GMEIPSW_NO] = get_gmeipsw();
    p_vmcb->d_sysreg[GMEIPC_NO] = get_gmeipc();
      
    /* ユーザーハンドラ呼び出し */
    cause_no = vmexc_info.cause & EXCNO_MASK;
    if ((MIN_MIPMDP_EXCNO <= cause_no) && (MAX_MIPMDP_EXCNO >= cause_no)) {
        vm_mip_handler(&vmexc_info);
    }
    else if ((MIN_SYSERR_EXCNO <= cause_no) && (MAX_SYSERR_EXCNO >= cause_no)) {
        vm_syserr_handler(&vmexc_info);
    }
    else {
        while(1);
    }

    /*
     * ResetVM()/RasieVMFakeFE()/RaiseVMFakeEI() で設定された可能性のある
     * レジスタを書き戻す
     */
   *(uint32*)(reg_base + VMEXC_REGBASE_R6) =  p_vmcb->reg[6];
   *(uint32*)(reg_base + VMEXC_REGBASE_R7) =  p_vmcb->reg[7];
   *(uint32*)(reg_base + VMEXC_REGBASE_R8) =  p_vmcb->reg[8];
   *(uint32*)(reg_base + VMEXC_REGBASE_R9) =  p_vmcb->reg[9];

    set_fepc(p_vmcb->pc);
    set_gmeipc(p_vmcb->d_sysreg[GMEIPC_NO]);
    set_gmeipsw(p_vmcb->d_sysreg[GMEIPSW_NO]);
    set_gmfepc(p_vmcb->d_sysreg[GMFEPC_NO]);
    set_gmfepsw(p_vmcb->d_sysreg[GMFEPSW_NO]);    
    set_gmpsw(p_vmcb->d_sysreg[GMPSW_NO]);
    set_gmeiic(p_vmcb->d_sysreg[GMEIIC_NO]);
    set_gmfeic(p_vmcb->d_sysreg[GMFEIC_NO]);        
}


/*
 *  HVで発生したFE例外のハンドラ
 */
void
call_hv_fe_handler(uint32 reg_base)
{
    HVEXC_INFO hvexc_info;

    /* VM発生の例外情報の設定 */
    hvexc_info.cause = get_feic();
    hvexc_info.pc = get_fepc();
    hvexc_info.psw = get_fepsw();
    hvexc_info.regbase = reg_base;

    /* ユーザーハンドラ呼び出し */
    hv_fe_handler(&hvexc_info);
}

/*
 *  HVで発生したEI例外のハンドラ
 */
void
call_hv_ei_handler(uint32 reg_base)
{
    HVEXC_INFO hvexc_info;

    hvexc_info.cause = get_eiic();
    hvexc_info.pc = get_eipc();
    hvexc_info.psw = get_eipsw();
    hvexc_info.regbase = reg_base;

    /* ユーザーハンドラ呼び出し */
    hv_ei_handler(&hvexc_info);
}
