#ifndef _U2A_H_
#define _U2A_H_

#include "tool_stddef.h"
#include "rh850.h"

/*
 *  U2A Serise Max Core Number
 */
#define TMAX_PHYS_CORE_U2A    (4)

/*
 *  LRAM
 */
#define LRAM_BASE_CORE0     0xFDC00000
#define LRAM_BASE_CORE1     0xFDA00000
#define LRAM_BASE_CORE2     0xFD800000
#define LRAM_BASE_CORE3     0xFD600000
#define LRAM_BASE_COREn(n)  (0xFDC00000 - (0x200000 * n))
#define LRAM_SIZE           0x0000FFFF

/*
 *  CRAM
 */
#define CRAM_BASE_0         0xFE000000
#define CRAM_BASE_1         0xFE100000
#define CRAM_SIZE           0x0007FFFF

#define INTVECTOR_PRI     (0U)
#define INTVECTOR_TABLE   (1U)

/*
 *  Interrupt Number
 */
#define OSTM0_INTNO     (199U)
#define OSTM1_INTNO     (200U)
#define OSTM2_INTNO     (201U)
#define OSTM3_INTNO     (202U)
#define OSTM4_INTNO     (203U)
#define OSTM5_INTNO     (204U)
#define OSTM6_INTNO     (205U)
#define OSTM7_INTNO     (206U)
#define OSTM8_INTNO     (207U)
#define OSTM9_INTNO     (208U)

#define OSTM_ITNO(no)   (OSTM0_INTNO + no)

#define INTC1_INTNO_MIN    (0U)
#define INTC1_INTNO_MAX    (31U)
#define INTC2_INTNO_MIN    (32U)
#define INTC2_INTNO_MAX    (767U)

#define RLIN30_RX_INTNO    (418U)
#define RLIN31_RX_INTNO    (422U)
#define RLIN32_RX_INTNO    (426U)
#define RLIN33_RX_INTNO    (430U)
#define RLIN34_RX_INTNO    (434U)
#define RLIN35_RX_INTNO    (438U)
#define RLIN36_RX_INTNO    (442U)
#define RLIN37_RX_INTNO    (446U)

#define RLIN38_RX_INTNO    (450U)
#define RLIN39_RX_INTNO    (454U)
#define RLIN310_RX_INTNO   (458U)
#define RLIN311_RX_INTNO   (462U)

#define TAUJ0I0_INTNO    (360U)
#define TAUJ0I1_INTNO    (361U)
#define TAUJ0I2_INTNO    (362U)
#define TAUJ0I3_INTNO    (363U)

#define TAUJ1I0_INTNO    (364U)
#define TAUJ1I1_INTNO    (365U)
#define TAUJ1I2_INTNO    (366U)
#define TAUJ1I3_INTNO    (367U)

#define TAUJ2I0_INTNO    (368U)
#define TAUJ2I1_INTNO    (369U)
#define TAUJ2I2_INTNO    (370U)
#define TAUJ2I3_INTNO    (371U)

#define TAUJ3I0_INTNO    (372U)
#define TAUJ3I1_INTNO    (373U)
#define TAUJ3I2_INTNO    (374U)
#define TAUJ3I3_INTNO    (375U)

#define TAUD0_0_INTNO    (10U)
#define TAUD0_1_INTNO    (376U)
#define TAUD0_2_INTNO    (11U)
#define TAUD0_3_INTNO    (377U)
#define TAUD0_4_INTNO    (12U)
#define TAUD0_5_INTNO    (378U)
#define TAUD0_6_INTNO    (13U)
#define TAUD0_7_INTNO    (379U)
#define TAUD0_8_INTNO    (23U)
#define TAUD0_9_INTNO    (380U)
#define TAUD0_10_INTNO   (24U)
#define TAUD0_11_INTNO   (381U)
#define TAUD0_12_INTNO   (25U)
#define TAUD0_13_INTNO   (382U)
#define TAUD0_14_INTNO   (26U)

#ifndef TOPPERS_MACRO_ONLY

/*
 *  INTC register bit
 */
#define EIBD_GM          (1U<<15)
#define EIBD_GPID_BIT    (8U)
#define EIBD_GPID_MASK   (7U<<8)
#define EIBD_PEID_MASK   (7U)

#define EIC_EITB        (1U<<6)
#define EIC_EIMK        (1U<<7)
#define EIC_EIP_MASK    (0x0f)

#define EEIC_EITB       (1U<<22)
#define EEIC_EIMK       (1U<<23)
#define EEIC_EIRF       (1U<<28)
#define EEIC_EIP_MASK   (0xffU)

#define INTC1_BASE      0xFFFC0000
#define INTC1_EIC(n)    ((uint32_t *)(INTC1_BASE + 0x000 + (2*n)))
#define INTC1_EEIC(n)   ((uint32_t *)(INTC1_BASE + 0x200 + (4*n)))
#define INTC1_IMR       ((uint32_t *)(INTC1_BASE + 0x00F0))
#define INTC1_EIBD(n)   ((uint32_t *)(INTC1_BASE + 0x100 + (4*n)))
#define INTC1_IHVCFG    ((uint32_t *)(INTC1_BASE + 0x02F0))
#define INTC1_IHVCFG_IHVE_MASK    (0x01U)

#define INTC1_BASE_COREn(n)    (0xFFFC0000 + (0x4000 * n))
#define INTC1_SIZE             0x00003FFF

#define INTC2_BASE       0xFFF80000
#define INTC2_EIC(n)     ((uint32_t *)(INTC2_BASE + 0x000 + (2*n)))
#define INTC2_EEIC(n)    ((uint32_t *)(INTC2_BASE + 0x4000 + (4*n)))
#define INTC2_IMR        ((uint32_t *)(INTC2_BASE + 0x1000))
#define INTC2_EIBD(n)    ((uint32_t *)(INTC2_BASE + 0x2000 + (4*n)))

/*
 *  Functions for INTC(INTC1/INTC2)
 */
extern void bind_int_to_host(uint32_t intno, uint32_t coreidx);
extern void bind_int_to_gm(uint32_t intno, uint32_t gpid, uint32_t coreidx);

extern void set_exint_vector(uint32_t intno, uint32_t type);
extern void set_exint_pri(uint32_t into, uint32_t pri);
extern void ena_exint(uint32_t intno);
extern void dis_exint(uint32_t intno);
extern void raise_exint(uint32_t intno);
extern uint32_t check_exint(uint32_t intno);
extern void clear_exint(uint32_t intno);

LOCAL_INLINE void
set_ihvcfg(uint32_t reg) {
    sil_wrw_mem(INTC1_IHVCFG, reg);
}

#endif /* TOPPERS_MACRO_ONLY */

/*
 *  Clock
 */
#define LOCKKEY_VAL    0xA5A5A500
#define UNLOCKKEY_VAL  0xA5A5A501

#define CLKKCPROT1     ((uint32_t*)0xFF980700)
#define CKSC_ATAUJC    ((uint32_t*)0xFF988210)
#define CKSC_ATAUJS    ((uint32_t*)0xFF988218)

#ifndef TOPPERS_MACRO_ONLY
/*
 *  Functions for Clock Controller
 */
extern void clk_init(uint32_t clk_cpu_div, uint32_t clk_pll0_div);
#endif /* TOPPERS_MACRO_ONLY */

     
#define PORT_BASE  (0xffd90000)

#define P(n)        ((uint16_t*)(PORT_BASE + 0x0000U + (n * 0x40U)))     /* ポート・モード・コントロール・レジスタ */
#define PMC(n)      ((uint16_t*)(PORT_BASE + 0x0014U + (n * 0x40U)))     /* ポート・モード・コントロール・レジスタ */
#define PMCSR(n)    ((uint16_t*)(PORT_BASE + 0x0024U + (n * 0x40U)))     /* ポート・モード・コントロール・セット／リセット・レジスタ */
#define PIPC(n)     ((uint16_t*)(PORT_BASE + 0x4008U + (n * 0x40U)))     /* ポートIP コントロール・レジスタ */
#define PM(n)       ((uint16_t*)(PORT_BASE + 0x0010U + (n * 0x40U)))     /* ポート・モード・レジスタ */
#define PMSR(n)     ((uint16_t*)(PORT_BASE + 0x0020U + (n * 0x40U)))     /* ポート・モード・セット／リセット・レジスタ */
#define PIBC(n)     ((uint16_t*)(PORT_BASE + 0x4000U + (n * 0x40U)))     /* ポート入力バッファ・コントロール・レジスタ */
#define PFC(n)      ((uint16_t*)(PORT_BASE + 0x0018U + (n * 0x40U)))     /* ポート機能コントロール・レジスタ */
#define PFCE(n)     ((uint16_t*)(PORT_BASE + 0x001CU + (n * 0x40U)))     /* ポート機能コントロール・レジスタ */
#define PFCAE(n)    ((uint16_t*)(PORT_BASE + 0x0028U + (n * 0x40U)))     /* ポート機能コントロール・拡張レジスタ */
#define PISA(n)     ((uint16_t*)(PORT_BASE + 0x4024U + (n * 0x40U)))

#define PKCPROT     ((uint32_t*)(PORT_BASE + 0x2F40))
#define PWE         ((uint32_t*)(PORT_BASE + 0x2F44))
     
#ifndef TOPPERS_MACRO_ONLY
/*
 *  Functions for RLIN30
 */
extern void uart_init(uint32_t no, uint32_t lwbr_val, uint32_t lbrp01_val);
extern void uart_putc(uint32_t no, char8 c);
extern void uart_puts(uint32_t no, char *str);
extern char uart_getc(uint32_t no);
extern char uart_pgetc(uint32_t no);

/*
 * Functions for OSTM
 */
extern void ostm_pol_wait(uint32_t no, uint32_t cnt);
extern void ostm_start_interval(uint32_t no, uint32_t cnt);
extern void ostm_start_oneshot(uint32_t no, uint32_t cnt);
extern void ostm_init_oneshot(uint32_t no);
extern void ostm_raise_int(uint32_t no);
extern uint32_t ostm_cnt_read(uint32_t no);
extern void ostm_clear_int(uint32_t no);
extern void ostm_clear_stop_int(uint32_t no);

extern const uint32_t ostm_base[];
extern const uint32_t ostm_intno[];

/*
 *  アクセスマクロ
 */
#define OSTM_BASE(no)  ostm_base[no]
#define OSTM_INTNO(no) ostm_intno[no]

#define OSTM_TO_CYC(us)  ((uint32_t)(us*OSTM_CLK_MHZ))

#define OSTM_CMP(base)    ((uint32_t*)(base + 0x00U))
#define OSTM_CNT(base)    ((uint32_t*)(base + 0x04U))
#define OSTM_TE(base)     ((uint8_t*)(base + 0x10U))
#define OSTM_TS(base)     ((uint8_t*)(base + 0x14U))
#define OSTM_TT(base)     ((uint8_t*)(base + 0x18U))
#define OSTM_CTL(base)    ((uint8_t*)(base + 0x20U))

LOCAL_INLINE void
ostm_pause(uint32_t no)
{
    sil_wrb_mem(OSTM_TT(ostm_base[no]), 0x01U);
}

LOCAL_INLINE void
ostm_continue(uint32_t no)
{
    sil_wrb_mem(OSTM_TS(ostm_base[no]), 0x01U);
}

/*
 *  Functions for TPTM
 */
#define TPTM_BASE      0xFFFBB000
#define TPTMSIRUN      ((uint32_t*)(TPTM_BASE + 0x00U))
#define TPTMSIRRUN     ((uint32_t*)(TPTM_BASE + 0x04U))
#define TPTMSISTP      ((uint32_t*)(TPTM_BASE + 0x08U))
#define TPTMSISTR      ((uint32_t*)(TPTM_BASE + 0x0CU))
#define TPTMSIIEN      ((uint32_t*)(TPTM_BASE + 0x10U))
#define TPTMSIUSTR     ((uint32_t*)(TPTM_BASE + 0x14U))
#define TPTMSIDIV      ((uint32_t*)(TPTM_BASE + 0x18U))
#define TPTMSICNT0(no) ((uint32_t*)(TPTM_BASE + 0x80U + (no * 0x10U)))
#define TPTMSILD0(no)  ((uint32_t*)(TPTM_BASE + 0x84U + (no * 8U)))

#define TPTMSURUN         ((uint32_t*)(TPTM_BASE + 0x40U))
#define TPTMSURRUN        ((uint32_t*)(TPTM_BASE + 0x44U))
#define TPTMSUSTP         ((uint32_t*)(TPTM_BASE + 0x48U))
#define TPTMSUSTR         ((uint32_t*)(TPTM_BASE + 0x4CU))
#define TPTMSUIEN         ((uint32_t*)(TPTM_BASE + 0x50U))
#define TPTMSUDIV         ((uint32_t*)(TPTM_BASE + 0x58U))
#define TPTMSUTRG         ((uint32_t*)(TPTM_BASE + 0x5CU))
#define TPTMSUCNT0(no)    ((uint32_t*)(TPTM_BASE + 0xC0U + (no * 0x10U)))
#define TPTMSUCCMP00(cmp) ((uint32_t*)(TPTM_BASE + 0xC4U + (cmp * 4U)))

#define TPTMI_INTNO  (31U)

#define TPTMU_INTNO(coreidx, no)    (209 + (coreidx * 4) + no)

#define TPTMSEL    ((uint32_t*)(0xFF090000 + 0x200))

LOCAL_INLINE void
tptmi_seteiint(void)
{
    sil_wrw_mem(TPTMSEL, 0x0fU);
}

LOCAL_INLINE void
tptmi_start(uint8 no, uint32_t count)
{
    /* Set Load value */
    sil_wrw_mem(TPTMSILD0(no), count);
    /* Start */
    sil_wrw_mem(TPTMSIRUN, (1U << no));
}

LOCAL_INLINE void
tptmi_stop(uint8 no)
{
    sil_wrw_mem(TPTMSISTP, (1U << no));
}

LOCAL_INLINE void
tptmi_restart(uint8 no)
{
    sil_wrw_mem(TPTMSIRRUN, (1U << no));
}

LOCAL_INLINE void
tptmi_clearint(uint8 no)
{
    sil_wrw_mem(TPTMSIUSTR,
                sil_rew_mem(TPTMSIUSTR) & ~(1U << no));
}

LOCAL_INLINE void
tptmi_enaint(uint8 no)
{
    sil_wrw_mem(TPTMSIIEN,
                sil_rew_mem(TPTMSIIEN) | (1U << no));    
}

LOCAL_INLINE void
tptmi_setcnt(uint8 no, uint32 cnt)
{
    sil_wrw_mem(TPTMSICNT0(no), cnt);
}

LOCAL_INLINE void
tptmi_setdiv(uint32_t div)
{
    sil_wrw_mem(TPTMSIDIV, div);
}

LOCAL_INLINE boolean
tptmi_isrunning(uint8 no)
{
    return (sil_rew_mem(TPTMSISTR) & (1U << no));
}


LOCAL_INLINE boolean
tptmi_isuderflow(uint8 no)
{
    return (sil_rew_mem(TPTMSIUSTR) & (1U << no));
}

LOCAL_INLINE void
tptmu_set_cmp(uint32_t cmp, uint32_t count)
{
    /* Set cmpare value */
    sil_wrw_mem(TPTMSUCCMP00(cmp),  count);
}

LOCAL_INLINE uint32
tptmu_get_cmp(uint32_t cmp)
{
    /* Get cmpare value */
    return sil_rew_mem(TPTMSUCCMP00(cmp));
}

LOCAL_INLINE void
tptmu_start(uint8 no)
{
    /* Start */
    sil_wrw_mem(TPTMSURUN, (1U << no));
}

LOCAL_INLINE void
tptmu_stop(uint8 no)
{
    /* Stop */
    sil_wrw_mem(TPTMSUSTP, (1U << no));
}

LOCAL_INLINE void
tptmu_restart(uint8 no)
{
    /* Stop */
    sil_wrw_mem(TPTMSURRUN, (1U << no));
}

LOCAL_INLINE void
tptmu_enaint(uint8 no, uint8 cmp)
{
    sil_wrw_mem(TPTMSUIEN,
                sil_rew_mem(TPTMSUIEN) | (1U << ((no * 8U) + cmp)));
}

LOCAL_INLINE void
tptmu_setdiv(uint32_t div)
{
    sil_wrw_mem(TPTMSUDIV, div);
}

LOCAL_INLINE uint32
tptmu_getcnt(uint8 no)
{
    return sil_rew_mem(TPTMSUCNT0(no));
}


/*
 *  Module Standby Register(MSR)
 */
#define MSR_TAUD        ((uint32_t*)(0xFF981130))
#define MSR_TAUJ_ISO    ((uint32_t*)(0xFF981140))
#define MSR_TAUJ_AWO    ((uint32_t*)(0xFF988E20))
#define MSR_RLIN3       ((uint32_t*)(0xFF981060))
#define MSR_OSTM        ((uint32_t*)(0xFF981180))
#define MSRKCPROT       ((uint32_t*)(0xFF981710))
#define MSR_RSCFD       ((uint32_t*)(0xFF981000))

LOCAL_INLINE void
msr_enable_taud0(void)
{
    sil_wrw_mem(MSRKCPROT, UNLOCKKEY_VAL);
    sil_wrw_mem(MSR_TAUD,
                sil_rew_mem(MSR_TAUD) & ~0x01U);
    sil_wrw_mem(MSRKCPROT, LOCKKEY_VAL);
    __SYNCM();
}

LOCAL_INLINE void
msr_enable_tauj0(void)
{
    sil_wrw_mem(MSRKCPROT, UNLOCKKEY_VAL);
    sil_wrw_mem(MSR_TAUJ_ISO,
                sil_rew_mem(MSR_TAUJ_ISO) & ~0x01U);
    sil_wrw_mem(MSRKCPROT, LOCKKEY_VAL);
    __SYNCM();
}

LOCAL_INLINE void
msr_enable_tauj1(void)
{
    sil_wrw_mem(MSRKCPROT, UNLOCKKEY_VAL);
    sil_wrw_mem(MSR_TAUJ_ISO,
                sil_rew_mem(MSR_TAUJ_ISO) & ~0x02U);    
    sil_wrw_mem(MSRKCPROT, LOCKKEY_VAL);
    __SYNCM();
}

LOCAL_INLINE void
msr_enable_tauj2(void)
{
    sil_wrw_mem(MSRKCPROT, UNLOCKKEY_VAL);
    sil_wrw_mem(MSR_TAUJ_AWO,
                sil_rew_mem(MSR_TAUJ_AWO) & ~0x01U);    
    sil_wrw_mem(MSRKCPROT, LOCKKEY_VAL);
    __SYNCM();
}

LOCAL_INLINE void
msr_enable_tauj3(void)
{
    sil_wrw_mem(MSRKCPROT, UNLOCKKEY_VAL);
    sil_wrw_mem(MSR_TAUJ_AWO,
                sil_rew_mem(MSR_TAUJ_AWO) & ~0x02U);
    sil_wrw_mem(MSRKCPROT, LOCKKEY_VAL);
    __SYNCM();
}

LOCAL_INLINE void
msr_enable_uart(uint32_t enable) 
{
    uint32_t wk;
    
    sil_wrw_mem(MSRKCPROT, UNLOCKKEY_VAL);
    wk = sil_rew_mem(MSR_RLIN3);
    wk &= ~enable;
    wk |= 0x10000000U;
    sil_wrw_mem(MSR_RLIN3, wk);
    sil_wrw_mem(MSRKCPROT, LOCKKEY_VAL);
    __SYNCM();
}

LOCAL_INLINE void
msr_enable_ostm(uint32_t enable) 
{
    uint32_t wk;
    sil_wrw_mem(MSRKCPROT, 0xA5A5A501);
    wk = sil_rew_mem(MSR_OSTM);
    wk &= ~enable;
    sil_wrw_mem(MSR_OSTM, wk);
    sil_wrw_mem(MSRKCPROT, 0xA5A5A500);
    __SYNCM();
}

LOCAL_INLINE void
msr_enable_rscan(void) {
    sil_wrw_mem((void *) MSRKCPROT, 0xA5A5A501);
    sil_wrw_mem((void *) MSR_RSCFD, 0x00000000);
    sil_wrw_mem((void *) MSRKCPROT, 0xA5A5A500);
    __SYNCM();
}

LOCAL_INLINE void
set_tauj23_clk(uint32_t val)
{
    sil_wrw_mem(CLKKCPROT1, UNLOCKKEY_VAL);
    sil_wrw_mem(CKSC_ATAUJC, val);    
    while(sil_rew_mem(CKSC_ATAUJS) != val);
    __SYNCP();
    sil_wrw_mem(CLKKCPROT1, LOCKKEY_VAL);
}

#endif /* TOPPERS_MACRO_ONLY */


/*
 *  TAUD0
 */
#define TAUD0_BASE    0xFFBF4000

#define TAUD_TPS_PRS0_MASK  (0x0f)

#define TAUD0_TPS    ((uint16_t*)(TAUD0_BASE + 0x240U))
#define TAUD0_BRS    ((uint16_t*)(TAUD0_BASE + 0x244U))

#define TAUD0_CDR(m)    ((uint16_t*)(TAUD0_BASE + (m * 4U)))
#define TAUD0_CNT(m)    ((uint16_t*)(TAUD0_BASE + 0x080U + (m * 4U)))
#define TAUD0_CMOR(m)   ((uint16_t*)(TAUD0_BASE + 0x200U + (m * 4U)))
#define TAUD0_CMUR(m)   ((uint16_t*)(TAUD0_BASE + 0x0C0U + (m * 4U)))
#define TAUD0_CSR(m)    ((uint16_t*)(TAUD0_BASE + 0x140U + (m * 4U)))
#define TAUD0_CSC(m)    ((uint16_t*)(TAUD0_BASE + 0x180U + (m * 4U)))

#define TAUD0_TS    ((uint16_t*)(TAUD0_BASE + 0x1C4U))
#define TAUD0_TE    ((uint16_t*)(TAUD0_BASE + 0x1C0U))
#define TAUD0_TT    ((uint16_t*)(TAUD0_BASE + 0x1C8U))

#ifndef TOPPERS_MACRO_ONLY
extern void taud0_init(uint32_t prs);
extern void taud0_start_interval(uint32_t ch, uint32_t cnt);
#endif /* TOPPERS_MACRO_ONLY */


/*
 *  Slave Guard
 */
#define CPCR_GEN    (1U << 8)
#define CPCR_DBG    (1U << 6)
#define CPCR_WG     (1U << 1)
#define CPCR_RG     (1U << 0)

/* PEG(LRAM/INTC1) */
#define PEG_BASE(pe)    (0xFFC6C000 + (0x100U * pe))

#define PEGKCPROTm(pe)  ((uint32_t*)(PEG_BASE(pe) + 0x00U))
#define PEGPROTm(pe, m) ((uint32_t*)(PEG_BASE(pe) + 0x40U + (m * 0x10U)))
#define PEGSPIDm(pe, m) ((uint32_t*)(PEG_BASE(pe) + 0x44U + (m * 0x10U)))
#define PEGBADm(pe, m)  ((uint32_t*)(PEG_BASE(pe) + 0x48U + (m * 0x10U)))
#define PEGADVm(pe, m)  ((uint32_t*)(PEG_BASE(pe) + 0x4CU + (m * 0x10U)))

/* CRG */
#define CRAMCRGn0_BASE(n)            (0xFFC6D000 + (0x200U * n))
#define CRAMCRGn0_CRGKCPROT(n)       ((uint32_t*)(CRAMCRGn0_BASE(n) + 0x00U))
#define CRAMCRGn0_CRGPROTm(n, m)     ((uint32_t*)(CRAMCRGn0_BASE(n) + 0x10U + (m * 0x20U)))
#define CRAMCRGn0_CRGSPIDm(n, m)     ((uint32_t*)(CRAMCRGn0_BASE(n) + 0x14U + (m * 0x20U)))
#define CRAMCRGn0_CRGBADm(n, m)      ((uint32_t*)(CRAMCRGn0_BASE(n) + 0x18U + (m * 0x20U)))
#define CRAMCRGn0_CRGADVm(n, m)      ((uint32_t*)(CRAMCRGn0_BASE(n) + 0x1CU + (m * 0x20U)))
#define CRAMCRGn0_CRGIVCSPIDm(n, m)  ((uint32_t*)(CRAMCRGn0_BASE(n) + 0x20U + (m * 0x20U)))

#define CRAMCSGn0_BASE(n)            (0xFFC6E000 + (0x200U * n)
#define CRAMCSGn0_CSGPROTm(n, m)     ((uint32_t*)(CRAMCSGn0_BASE(n) + 0x00U + (m * 0x20U)))
#define CRAMCSGn0_CSGSPIDm(n, m)     ((uint32_t*)(CRAMCSGn0_BASE(n) + 0x04U + (m * 0x20U)))
#define CRAMCSGn0_CSGBADm(n, m)      ((uint32_t*)(CRAMCSGn0_BASE(n) + 0x08U + (m * 0x20U)))
#define CRAMCSGn0_CSGADVm(n, m)      ((uint32_t*)(CRAMCSGn0_BASE(n) + 0x0CU + (m * 0x20U)))

/* INTC2 Guard */
#define GUARD_INTC2_BASE    (0xFFC64000)
#define INTC2GKCKPROT       ((uint32_t*)(GUARD_INTC2_BASE + 0x018U))
#define INTC2GMPIDm(m)      ((uint32_t*)(GUARD_INTC2_BASE + 0x040U + m * 4U))
#define INTC2GPORT_GR       ((uint32_t*)(GUARD_INTC2_BASE + 0x0F0U))
#define INTC2GPORT_IMR      ((uint32_t*)(GUARD_INTC2_BASE + 0x0F4U))
#define INTC2GPORT_n(n)     ((uint32_t*)(GUARD_INTC2_BASE + 0x100U + n * 4U))

#define INTC2GPORT_MPID    (16U)
#define TNUM_INTC2GMPID    (8U)

/* IBG */
#define GUARD_IPIR_BASE    (0xFFC6A200)
#define IPIGGKCPROT        ((uint32_t*)(GUARD_IPIR_BASE + 0x18U))
#define IPIGPROT0_Rn(n)    ((uint32_t*)(GUARD_IPIR_BASE + 0x80U + n * 0x10U))
#define IPIGPROT1_Rn(n)    ((uint32_t*)(GUARD_IPIR_BASE + 0x84U + n * 0x10U))
#define IPIGPROT0_Tn(n)    ((uint32_t*)(GUARD_IPIR_BASE + 0x88U + n * 0x10U))
#define IPIGPROT1_Tn(n)    ((uint32_t*)(GUARD_IPIR_BASE + 0x8cU + n * 0x10U))
#define IPIGPROT0_4        ((uint32_t*)(GUARD_IPIR_BASE + 0xC0U))
#define IPIGPROT1_4        ((uint32_t*)(GUARD_IPIR_BASE + 0xC4U))

#define GUARD_BARR_BASE  (0xFFC6A000)
#define BRGKCPROT        ((uint32_t*)(GUARD_BARR_BASE + 0x018U))
#define BRGPROT0_n(n)    ((uint32_t*)(GUARD_BARR_BASE + 0x080U + n * 0x08U))
#define BRGPROT1_n(n)    ((uint32_t*)(GUARD_BARR_BASE + 0x084U + n * 0x08U))
#define BRGPROT0_16      ((uint32_t*)(GUARD_BARR_BASE + 0x100U))
#define BRGPROT1_16      ((uint32_t*)(GUARD_BARR_BASE + 0x104U))

#define GUARD_TPTM_BASE  (0xFFC6A600)
#define TPTGKCPROT       ((uint32_t*)(GUARD_TPTM_BASE + 0x18U))
#define TPTGPROT0_n(n)   ((uint32_t*)(GUARD_TPTM_BASE + 0x80U + n * 8U))
#define TPTGPROT1_n(n)   ((uint32_t*)(GUARD_TPTM_BASE + 0x84U + n * 8U))

/* PBG */
#define PBGERRSLV00_BASE        (0xFFC6B000)
#define PBGERRSLV00_PBGKCPROT   ((uint32_t*)(PBGERRSLV00_BASE + 0x18U))

#define PBG00_BASE              (0xFFC6B080)
#define PBG00_PBGPROT0_m(m)     ((uint32_t*)(PBG00_BASE + 0U + m * 8U))
#define PBG00_PBGPROT1_m(m)     ((uint32_t*)(PBG00_BASE + 4U + m * 8U))

#define PBGERRSLV20_BASE        (0xFFDE1000)
#define PBGERRSLV20_PBGKCPROT   ((uint32_t*)(PBGERRSLV20_BASE + 0x18))

#define PBG20_BASE              (0xFFDE0B00)
#define PBG20_PBGPROT1_m(m)     ((uint32_t*)(PBG20_BASE + 4U + m * 8U))

#define PBG21_BASE              (0xFFDE0C00)
#define PBG21_PBGPROT1_m(m)     ((uint32_t*)(PBG21_BASE + 4U + m * 8U))

#define PBGERRSLV30_BASE        (0xFFC73200)
#define PBGERRSLV30_PBGKCPROT   ((uint32_t*)(PBGERRSLV30_BASE + 0x18U))

#define PBG32_BASE              (0xFFC72D00)
#define PBG32_PBGPROT1_m(m)     ((uint32_t*)(PBG32_BASE + 4U + m * 8U))

#define PBG33_BASE              (0xFFC72E00)
#define PBG33_PBGPROT1_m(m)     ((uint32_t*)(PBG33_BASE + 4U + m * 8U))

#define PBGERRSLV50_BASE        (0xFFC7B000)
#define PBGERRSLV50_PBGKCPROT   ((uint32_t*)(PBGERRSLV50_BASE + 0x18U))

#define PBG50_BASE              (0xFFC7A300)
#define PBG50_PBGPROT0_m(m)     ((uint32_t*)(PBG50_BASE + 0U + m * 8U))
#define PBG50_PBGPROT1_m(m)     ((uint32_t*)(PBG50_BASE + 4U + m * 8U))

#define PBG51_BASE              (0xFFC7A400)
#define PBG51_PBGPROT0_m(m)     ((uint32_t*)(PBG51_BASE + 0U + m * 8U))
#define PBG51_PBGPROT1_m(m)     ((uint32_t*)(PBG51_BASE + 4U + m * 8U))

#define PBG52_BASE              (0xFFC7A500)
#define PBG52_PBGPROT0_m(m)     ((uint32_t*)(PBG52_BASE + 0U + m * 8U))
#define PBG52_PBGPROT1_m(m)     ((uint32_t*)(PBG52_BASE + 4U + m * 8U))

#define PBG53_BASE              (0xFFC7A600)
#define PBG53_PBGPROT0_m(m)     ((uint32_t*)(PBG53_BASE + 0U + m * 8U))
#define PBG53_PBGPROT1_m(m)     ((uint32_t*)(PBG53_BASE + 4U + m * 8U))

#define PBGERRSLV80_BASE        (0xFFF2A000)
#define PBGERRSLV80_PBGKCPROT   ((uint32*)(PBGERRSLV80_BASE + 0x18))

#define PBG80_BASE              (0xFFF29300)
#define PBG80_PBGPROT1_m(m)     ((uint32*)(PBG80_BASE + 4 + m * 8))

/*
 *  IVC
 */

/*
 *  PBG register
 */
#define PGB00_BASE    (0xFFC6B080)

#define PGB00_PBGPROT0n(n)    ((uint32_t*)(PGB00_BASE + 0x00U + (n * 0x08U)))
#define PGB00_PBGPROT1n(n)    ((uint32_t*)(PGB00_BASE + 0x04U + (n * 0x08U)))

#define PGB00_C0RAMG_NO    (3U)
#define PGB00_C1RAMG_NO    (4U)
#define PGB00_C2RAMG_NO    (5U)
#define PGB00_C3RAMG_NO    (6U)

/*
 *  CRAMCRGn1
 */
#define CRAMCRGn1_BASE(n)    (0xFFC6E800 + (0x200U * n))
#define CRAMCRGn1_CRGIVCREQm(n, m)    ((uint32_t*)(CRAMCRGn1_BASE(n) + 0x00U + (m * 0x20U)))
#define CRAMCRGn1_CRGIVCLOCKm(n, m)   ((uint32_t*)(CRAMCRGn1_BASE(n) + 0x04U + (m * 0x20U)))
#define CRAMCRGn1_CRGIVCOWNRm(n, m)   ((uint32_t*)(CRAMCRGn1_BASE(n) + 0x08U + (m * 0x20U)))

#ifndef TOPPERS_MACRO_ONLY

/*
 *  ロックの取得の試行
 */
LOCAL_INLINE int32_t
TryLockIVC(uint8 cramno, uint8 ch) 
{
    /* オーナーが自VMか確認 */
    if ((sil_rew_mem(CRAMCRGn1_CRGIVCOWNRm(cramno, ch)) & (1U << get_spid())) == 0U) {
        /* オーナーで無ければオーナー権を要求 */
        sil_wrw_mem(CRAMCRGn1_CRGIVCREQm(cramno, ch), 0);
        /* オーナー権を取得できない(=ロックされている)ためfalseをリターン */
        if ((sil_rew_mem(CRAMCRGn1_CRGIVCOWNRm(cramno, ch)) & (1U << get_spid())) == 0U) {
            return -1;
        }
    }

    /* ロック要求 */
    sil_wrw_mem(CRAMCRGn1_CRGIVCLOCKm(cramno, ch), 1);

    if ((sil_rew_mem(CRAMCRGn1_CRGIVCLOCKm(cramno, ch)) == 1U)
        && ((sil_rew_mem(CRAMCRGn1_CRGIVCOWNRm(cramno, ch)) & (1U << get_spid())) != 0U)) {
        /* ロックされかつオーナーが自VMの場合 */
        __SYNCI();
        return 1;
    }

    return -2;
}

/*
 *  ロックの解放
 */
LOCAL_INLINE void
UnLockIVC(uint8 cramno, uint8 ch) 
{
    sil_wrw_mem(CRAMCRGn1_CRGIVCLOCKm(cramno, ch), 0);
}

/*
 *  指定されたSPIDがロックを取得しているか
 */
LOCAL_INLINE boolean
IsLockedIVC(uint8 cramno, uint8 ch, uint8 spid) 
{
    if ((sil_rew_mem(CRAMCRGn1_CRGIVCLOCKm(cramno, ch)) == 1U) &&
        ((sil_rew_mem(CRAMCRGn1_CRGIVCOWNRm(cramno, ch)) & (1U << spid)) != 0U)) {
        return true;
    }
    return false;
}


/*
 *  Boot
 */
#define BOOTCTRL    ((uint32_t*)(0xFFFB2000))

/*
 *  指定したIDのコアを起動
 */
LOCAL_INLINE void
boot_core(uint8 coreid)
{
    sil_wrw_mem(BOOTCTRL,
                sil_rew_mem(BOOTCTRL) | (1U << coreid));
}

#endif /* TOPPERS_MACRO_ONLY */

#endif /* _U2A_H_ */
