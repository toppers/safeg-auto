#ifndef _RH850_H_
#define _RH850_H_

#include <Compiler.h>

#define INTCFG_EPL          (1U<<1) 
#define INTCFG_ISPC         (1U<<0)

#define PSW_EIMASK_BIT      (20U)
#define PSW_EIMASK_MASK     (0xff < PSW_EIMASK_BIT)
#define PSW_EIMASK_ENALL    0xffU
#define PSW_EIMASK_DISALL   0x00U
#define PSW_EBV_BIT         (15U)
#define PSW_EBV_MASK        (1U << PSW_EBV_BIT)
#define PSW_CU0_BIT         (16U)
#define PSW_CU0_MASK        (1U << PSW_CU0_BIT)

#define PSW_GPID_BIT        (8U)
#define PSW_GM_MASK         (1U << 31)

#define PSW_NP_BIT          (7U)
#define PSW_NP_MASK         (1U << PSW_NP_BIT)

#define PSW_ID_BIT          (5U)
#define PSW_ID_MASK         (1U << PSW_ID_BIT)

#define TMIN_GPID           (0U)
#define TMAX_GPID           (7U)

#define MPU_WG_MASK        (1U<<15)
#define MPU_RG_MASK        (1U<<14)
#define MPU_E_MASK         (1U<<7)
#define MPU_SX_MASK        (1U<<5)
#define MPU_SW_MASK        (1U<<4)
#define MPU_SR_MASK        (1U<<3)
#define MPU_UX_MASK        (1U<<2)
#define MPU_UW_MASK        (1U<<1)
#define MPU_UR_MASK        (1U<<0)

#define MPU_ALL        (MPU_SX_MASK|MPU_SR_MASK|MPU_SW_MASK|MPU_UX_MASK|MPU_UR_MASK|MPU_UW_MASK)
#define MPU_ROM        (MPU_SX_MASK|MPU_SR_MASK|MPU_UX_MASK|MPU_UR_MASK)
#define MPU_RAM        (MPU_SW_MASK|MPU_SR_MASK|MPU_UW_MASK|MPU_UR_MASK)
#define MPU_PERI       (MPU_SW_MASK|MPU_SR_MASK|MPU_UW_MASK|MPU_UR_MASK)

#define MPM_GMPE_MASK  (1U<<2)
#define MPM_MPE_MASK   (1U<<0)

#define HVCFG_HVE_MASK (1U<<0)

#define MPCFG_HBE_BIT  (8U)
#define MPCFG_HBE_MASK (0x3fU)

#define MCR_HSWE_MASK  (1U<<28)
#define MCR_HSRE_MASK  (1U<<27)

#define MCR_HUWE_MASK  (1U<<25)
#define MCR_HURE_MASK  (1U<<24)

#define GMCFG_HMP_MASK    (1U<<1U)
#define GMCFG_GSYSE_MASK  (1U<<4U)
#define GMCFG_GCU0_MASK (1U<<16U)

#define TNUM_MPU        (32U)

#define TMIN_EIIC_EIINT_NO    (0x1000U)

#define NUMBER_OF_CACHELINE   (128U)

#define DECFG_EHE   (1U << 2U)
#define DECFG_EGE   (1U << 1U)
#define DECFG_ESE   (1U << 0U)

#define MIN_MIPMDP_EXCNO   0x90U
#define MAX_MIPMDP_EXCNO   0x9DU

#define MIN_SYSERR_EXCNO   0x10U
#define MAX_SYSERR_EXCNO   0x1FU

#define EXCNO_MASK         0xFFU

#ifndef TOPPERS_MACRO_ONLY

LOCAL_INLINE void
disable_ei(void)
{
    __DI();
}

LOCAL_INLINE void
enable_ei(void)
{
    __EI();
}

LOCAL_INLINE void
set_pmr(uint32_t pmr)
{
    __LDSR(11, 2, pmr);
}

LOCAL_INLINE uint32_t
get_pmr(void)
{
    return  __STSR(11, 2);
}

LOCAL_INLINE void
x_set_ipm(uint32_t pri)
{
    uint32_t pmr = (0xffffffff << pri) & 0xffff;
    set_pmr(pmr);
}

LOCAL_INLINE void
set_psw(uint32_t psw)
{
    __LDSR(5, 0, psw);
}

LOCAL_INLINE uint32_t
get_psw(void)
{
    return __STSR(5, 0);
}

LOCAL_INLINE void
set_pswh(uint32_t pswh)
{
    __LDSR(15, 0, pswh);
}

LOCAL_INLINE uint32_t
get_pswh(void)
{
    return __STSR(15, 0);
}

LOCAL_INLINE uint32_t
get_fepc(void)
{
    return __STSR(2, 0);
}

LOCAL_INLINE uint32_t
get_fepsw(void)
{
    return __STSR(3, 0);
}

LOCAL_INLINE uint32_t
get_feic(void)
{
    return __STSR(14, 0);
}

LOCAL_INLINE void
set_fepswh(uint32_t fepswh)
{
    __LDSR(19, 0, fepswh);
}

LOCAL_INLINE void
set_fepc(uint32_t pc)
{
    __LDSR(2, 0, pc);
}

LOCAL_INLINE void
set_fepsw(uint32_t fepsw)
{
    __LDSR(3, 0, fepsw);
}

LOCAL_INLINE uint32_t
get_eipc(void)
{
    return __STSR(0, 0);
}

LOCAL_INLINE uint32_t
get_eipsw(void)
{
    return __STSR(1, 0);
}

LOCAL_INLINE void
set_eipc(uint32_t pc)
{
    __LDSR(0, 0, pc);
}

LOCAL_INLINE void
set_eipsw(uint32_t eipsw)
{
    __LDSR(1, 0, eipsw);
}

LOCAL_INLINE void
set_plmr(uint32_t plmr)
{
    __LDSR(14, 2, plmr);
}

LOCAL_INLINE void
x_set_exipm(uint32_t pri)
{
    set_psw((get_psw() & ~PSW_EIMASK_MASK) | (pri << PSW_EIMASK_BIT));
}

LOCAL_INLINE void
set_intcfg(uint32_t intcfg)
{
    __LDSR(13, 2, intcfg);
}

LOCAL_INLINE uint32_t
get_intcfg(void)
{
    return __STSR(13, 2);
}

LOCAL_INLINE void
set_mpm(uint32_t mpm)
{
    __LDSR(0, 5, mpm);
}

LOCAL_INLINE void
set_mpcfg(uint32_t mpcfg)
{
    __LDSR(2, 5, mpcfg);
}

LOCAL_INLINE uint32_t
get_mpcfg(void)
{
    return __STSR(2, 5);
}

LOCAL_INLINE void
set_mpidx(uint32_t mpidx)
{
    __LDSR(16, 5, mpidx);
}

LOCAL_INLINE void
set_mpla(uint32_t mpla)
{
    __LDSR(20, 5, mpla);
}

LOCAL_INLINE void
set_mpua(uint32_t mpua)
{
    __LDSR(21, 5, mpua);
}

LOCAL_INLINE void
set_mpat(uint32_t mpat)
{
    __LDSR(22, 5, mpat);
}

LOCAL_INLINE uint32_t
get_mpat(void)
{
    return __STSR(22, 5);
}

LOCAL_INLINE void
set_mca(uint32_t mca)
{
    __LDSR(8, 5, mca);
}

LOCAL_INLINE void
set_mcs(uint32_t mcs)
{
    __LDSR(9, 5, mcs);
}

LOCAL_INLINE void
set_mcc(uint32_t mcc)
{
    __LDSR(10, 5, mcc);
}

LOCAL_INLINE uint32_t
get_mcr(void)
{
    return __STSR(11, 5);
}

LOCAL_INLINE void
set_mci(uint32_t mci)
{
    __LDSR(12, 5, mci);
}

LOCAL_INLINE void
set_gmcfg(uint32_t gmcfg)
{
    __LDSR(17, 1, gmcfg);
}

LOCAL_INLINE uint32_t
get_gmcfg(void)
{
    return __STSR(17, 1);
}

LOCAL_INLINE void
set_ebase(uint32_t ebase)
{
    __LDSR(3, 1, ebase);
}

LOCAL_INLINE uint32_t
get_peid(void)
{
    return __STSR(0, 2);
}

LOCAL_INLINE void
set_spid(uint32_t spid)
{
    __LDSR(0, 1, spid);
}

LOCAL_INLINE uint32_t
get_spid(void)
{
    return __STSR(0, 1);
}

LOCAL_INLINE uint32_t
get_eiic(void)
{
    return __STSR(13, 0);
}

LOCAL_INLINE void
halt(void)
{
    __HALT();
}

LOCAL_INLINE void
set_hvcfg(uint32_t hvcfg)
{
    __LDSR(16, 1, hvcfg);
}

/*
 *  Cache Operation
 */
LOCAL_INLINE void
set_icctrl(uint32_t icctrl)
{
    __LDSR(24, 4, icctrl);
}

LOCAL_INLINE uint32_t
get_icctrl(void)
{
    return __STSR(24, 4);
}


LOCAL_INLINE void
set_decfg(uint32_t decfg)
{
    __LDSR(16, 13, decfg);
}

LOCAL_INLINE uint32_t
get_decfg(void)
{
    return __STSR(16, 13);
}

LOCAL_INLINE void
set_dectrl(uint32_t dectrl)
{
    __LDSR(17, 13, dectrl);
}

LOCAL_INLINE uint32_t
get_dectrl(void)
{
    return __STSR(17, 13);
}

LOCAL_INLINE void
set_devds(uint32_t devds)
{
    __LDSR(18, 13, devds);
}

LOCAL_INLINE uint32_t
get_devds(void)
{
    return __STSR(18, 13);
}

/*
 *  Gust Context
 */
LOCAL_INLINE void
set_gmeipc(uint32_t gmeipc)
{
    __LDSR(0, 9, gmeipc);
}

LOCAL_INLINE uint32_t
get_gmeipc(void)
{
    return __STSR(0, 9);
}

LOCAL_INLINE void
set_gmeipsw(uint32_t gmeipsw)
{
    __LDSR(1, 9, gmeipsw);
}

LOCAL_INLINE uint32_t
get_gmeipsw(void)
{
    return __STSR(1, 9);
}

LOCAL_INLINE void
set_gmfepc(uint32_t fepc)
{
    __LDSR(2, 9, fepc);
}

LOCAL_INLINE uint32_t
get_gmfepc(void)
{
    return __STSR(2, 9);
}

LOCAL_INLINE void
set_gmfepsw(uint32_t fepsw)
{
    __LDSR(3, 9, fepsw);
}

LOCAL_INLINE uint32_t
get_gmfepsw(void)
{
    return __STSR(3, 9);
}

LOCAL_INLINE void
set_gmpsw(uint32_t gmpsw)
{
    __LDSR(5, 9, gmpsw);
}

LOCAL_INLINE uint32_t
get_gmpsw(void)
{
    return __STSR(5, 9);
}

LOCAL_INLINE void
set_gmeiic(uint32_t gmeiic)
{
    __LDSR(13, 9, gmeiic);
}

LOCAL_INLINE uint32_t
get_gmeiic(void)
{
    return __STSR(13, 9);
}

LOCAL_INLINE void
set_gmfeic(uint32_t gmfeic)
{
    __LDSR(14, 9, gmfeic);
}

LOCAL_INLINE uint32_t
get_gmfeic(void)
{
    return __STSR(14, 9);
}

LOCAL_INLINE uint32_t
get_gmebase(void)
{
    return __STSR(19, 9);
}

/*
 *  Timestamp count
 */
LOCAL_INLINE void
start_tscount(void)
{
    __LDSR(2, 11, 0x01);
}

LOCAL_INLINE void
stop_tscount(void)
{
    __LDSR(2, 11, 0x00);
}

LOCAL_INLINE uint64_t
get_tscount(void)
{
    uint32_t tmpl, tmph1, tmph2;
    uint64_t tmp;

    tmph1 = __STSR(1, 11);
    tmpl = __STSR(0, 11);
    tmph2 = __STSR(1, 11);
    
    if (tmph1 != tmph2) {
        tmpl = __STSR(0, 11);
    }
    tmp = ((uint64_t)tmph2 << 32);
    tmp |= tmpl;
    return tmp;
}

/*
 *  Performace counter
 */
#define TNUM_PMCOUNT         (8U)

#define PMCTRL_CND_ALL       (0x00U)
#define PMCTRL_CND_INST      (0x10U)
#define PMCTRL_CND_BRA       (0x18U)
#define PMCTRL_CND_CBRA      (0x19U)
#define PMCTRL_CND_CBRAMISS  (0x1AU)
#define PMCTRL_CND_EIINT     (0x20U)
#define PMCTRL_CND_FEINT     (0x21U)
#define PMCTRL_CND_PEXC      (0x22U)
#define PMCTRL_CND_RCEXCT    (0x23U)
#define PMCTRL_CND_BGEIINT   (0x24U)
#define PMCTRL_CND_BGREINT   (0x25U)
#define PMCTRL_CND_NOINT     (0x28U)
#define PMCTRL_CND_NOINTDIS  (0x29U)
#define PMCTRL_CND_IFETCH    (0x30U)
#define PMCTRL_CND_ICACHE    (0x31U)
#define PMCTRL_CND_ISTALL    (0x40U)
#define PMCTRL_CND_FLASHI    (0x50U)
#define PMCTRL_CND_FLASHD    (0x51U)

#define PMCTRL_CNT_HOST     (1U << 24)
#define PMCTRL_CNT_GPID7    (1U << 23)
#define PMCTRL_CNT_GPID6    (1U << 22)
#define PMCTRL_CNT_GPID5    (1U << 21)
#define PMCTRL_CNT_GPID4    (1U << 20)
#define PMCTRL_CNT_GPID3    (1U << 19)
#define PMCTRL_CNT_GPID2    (1U << 18)
#define PMCTRL_CNT_GPID1    (1U << 17)
#define PMCTRL_CNT_GPID0    (1U << 16)
#define PMCTRL_CNT_ALL      (0x1ffU << 16)

#define PMCTRL_CND_SHIFT    (8U)

#define PMGMCTRL_ENAALL     (0xffU)

#define ICCTRL_ICHCLR       (1U << 8)

LOCAL_INLINE void
set_pmgmctrl(uint32_t val) 
{
    __LDSR(9, 11, val);
}

LOCAL_INLINE uint32_t
get_pmcount(uint8_t no)
{
    uint32_t tmp;
    switch (no)
    {
        case 0:
            tmp = __STSR(16, 14);
            break;
        case 1:
            tmp = __STSR(17, 14);
            break;
        case 2:
            tmp = __STSR(18, 14);
            break;
        case 3:
            tmp = __STSR(19, 14);        
            break;
        case 4:
            tmp = __STSR(20, 14);
            break;
        case 5:
            tmp = __STSR(21, 14);
            break;
        case 6:
            tmp = __STSR(22, 14);
            break;
        case 7:
            tmp = __STSR(23, 14);
            break;
        default:
            break;        
    }
    return tmp;
}

LOCAL_INLINE void
set_pmcount(uint8_t no, uint32_t val)
{
    switch (no)
    {
        case 0:
            __LDSR(16, 14, val);
            break;
        case 1:
            __LDSR(17, 14, val);        
            break;
        case 2:
            __LDSR(18, 14, val);
            break;
        case 3:
            __LDSR(19, 14, val);        
            break;
        case 4:
            __LDSR(20, 14, val);        
            break;
        case 5:
            __LDSR(21, 14, val);        
            break;
        case 6:
            __LDSR(22, 14, val);        
            break;
        case 7:
            __LDSR(23, 14, val);        
            break;
        default:
            break;
    }
}

LOCAL_INLINE void
start_pmcount(uint8_t no, uint32_t cnd, uint32_t mode)
{
    uint32_t tmp = 0x01 | (cnd << PMCTRL_CND_SHIFT) | mode;
    switch (no)
    {
        case 0:
            __LDSR(0, 14, tmp);
            break;
        case 1:
            __LDSR(1, 14, tmp);        
            break;
        case 2:
            __LDSR(2, 14, tmp);        
            break;
        case 3:
            __LDSR(3, 14, tmp);        
            break;
        case 4:
            __LDSR(4, 14, tmp);        
            break;
        case 5:
            __LDSR(5, 14, tmp);        
            break;
        case 6:
            __LDSR(6, 14, tmp);        
            break;
        case 7:
            __LDSR(7, 14, tmp);        
            break;
        default:
            break;
    }
}

LOCAL_INLINE void
stop_pmcount(uint8_t no)
{
    switch (no)
    {
        case 0:
            __LDSR(0, 14, 0);
            break;
        case 1:
            __LDSR(1, 14, 0);        
            break;
        case 2:
            __LDSR(2, 14, 0);        
            break;
        case 3:
            __LDSR(3, 14, 0);
            break;
        case 4:
            __LDSR(4, 14, 0);       
            break;
        case 5:
            __LDSR(5, 14, 0);
            break;
        case 6:
            __LDSR(6, 14, 0);        
            break;
        case 7:
            __LDSR(7, 14, 0);        
            break;
        default:
            break;
    }
}

#endif /* TOPPERS_MACRO_ONLY */

#endif /* _RH850_H_ */
