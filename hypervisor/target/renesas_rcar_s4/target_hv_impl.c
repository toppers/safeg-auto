#include "hv_impl.h"


/*
 *  RLIN31 Port Configration
 *   AA6 : GP6_12 : RLIN31TX (Function 0：Output)
 *   AA5 : GP6_13 : RLIN31RX (Function 0：Input)
 */
#define RLIN31_GP6_MASK         ((uint32)0x00003000)
#define RLIN31_PMMER_INIT       ((uint32)0x00003000)
#define RLIN31_PMMR_INIT        ((uint32)0x00003000)
#define RLIN31_IPSR_MASK        ((uint32)0x00FF0000)
#define RLIN31_IPSR_INIT        ((uint32)0x00000000)
#define RLIN31_GPSR_INIT        ((uint32)0x00003000)
#define RLIN31_GP6_13_MASK      ((uint32)0x00002000)
#define RLIN31_POSNEG_INIT      ((uint32)0x00002000)
#define RLIN31_EDGLEVEL_INIT    ((uint32)0x00002000)
#define RLIN31_BOTHEDGE_INIT    ((uint32)0x00000000)
#define RLIN31_IOINTSEL_INIT    ((uint32)0x00002000)
#define RLIN31_INTCLR_INIT      ((uint32)0x00002000)
#define RLIN31_MSKCLR_INIT      ((uint32)0x00002000)

/*
 *  RLIN33 Port Configration
 *   AB6 : GP6_08 : RLIN33TX (Function 0：Output)
 *   AB4 : GP6_09 : RLIN33RX (Function 0：Input)
 */
#define RLIN33_GP6_MASK         ((uint32)0x00000300)
#define RLIN33_PMMER_INIT       ((uint32)0x00000300)
#define RLIN33_PMMR_INIT        ((uint32)0x00000300)
#define RLIN33_IPSR_MASK        ((uint32)0x000000FF)
#define RLIN33_IPSR_INIT        ((uint32)0x00000000)
#define RLIN33_GPSR_INIT        ((uint32)0x00000300)
#define RLIN33_GP6_09_MASK      ((uint32)0x00000200)
#define RLIN33_POSNEG_INIT      ((uint32)0x00000200)
#define RLIN33_EDGLEVEL_INIT    ((uint32)0x00000200)
#define RLIN33_BOTHEDGE_INIT    ((uint32)0x00000000)
#define RLIN33_IOINTSEL_INIT    ((uint32)0x00000200)
#define RLIN33_INTCLR_INIT      ((uint32)0x00000200)
#define RLIN33_MSKCLR_INIT      ((uint32)0x00000200)

/*
 *  RLIN35 Port Configration
 *   AC4 : GP6_04 : RLIN35TX (Function 0：Output)
 *   AC3 : GP6_05 : RLIN35RX (Function 0：Input)
 */
#define RLIN35_GP6_MASK         ((uint32)0x00000030)
#define RLIN35_PMMER_INIT       ((uint32)0x00000003)
#define RLIN35_PMMR_INIT        ((uint32)0x00000030)
#define RLIN35_IPSR_MASK        ((uint32)0x00FF0000)
#define RLIN35_IPSR_INIT        ((uint32)0x00000000)
#define RLIN35_GPSR_INIT        ((uint32)0x00000030)
#define RLIN35_GP6_05_MASK      ((uint32)0x00000020)
#define RLIN35_POSNEG_INIT      ((uint32)0x00000020)
#define RLIN35_EDGLEVEL_INIT    ((uint32)0x00000020)
#define RLIN35_BOTHEDGE_INIT    ((uint32)0x00000000)
#define RLIN35_IOINTSEL_INIT    ((uint32)0x00000020)
#define RLIN35_INTCLR_INIT      ((uint32)0x00000020)
#define RLIN35_MSKCLR_INIT      ((uint32)0x00000020)

/*
 *  RLIN37 Port Configration
 *   AD2 : GP6_00 : RLIN37TX (Function 0：Output)
 *   AD1 : GP6_01 : RLIN37RX (Function 0：Input)
 */
#define RLIN37_GP6_MASK         ((uint32)0x00000003)
#define RLIN37_PMMER_INIT       ((uint32)0x00000003)
#define RLIN37_PMMR_INIT        ((uint32)0x00000003)
#define RLIN37_IPSR_MASK        ((uint32)0x000000FF)
#define RLIN37_IPSR_INIT        ((uint32)0x00000000)
#define RLIN37_GPSR_INIT        ((uint32)0x00000003)
#define RLIN37_GP6_01_MASK      ((uint32)0x00000002)
#define RLIN37_POSNEG_INIT      ((uint32)0x00000002)
#define RLIN37_EDGLEVEL_INIT    ((uint32)0x00000002)
#define RLIN37_BOTHEDGE_INIT    ((uint32)0x00000000)
#define RLIN37_IOINTSEL_INIT    ((uint32)0x00000002)
#define RLIN37_INTCLR_INIT      ((uint32)0x00000002)
#define RLIN37_MSKCLR_INIT      ((uint32)0x00000002)


void
port_init(void)
{
    uint16 wk;

    /*
     *  RLIN31
     */
    wk = sil_rew_mem(PORT_PMMER6(0));
    wk &= ~RLIN31_PMMER_INIT;
    sil_wrw_mem(PORT_PMMER6(0), wk);

    /*
     * Select Function
     */
    sil_wrw_mem(PORT_PMMR6(0), RLIN31_PMMR_INIT);
    wk = sil_rew_mem(PORT_IP1SR6(0));
    wk &= ~RLIN31_IPSR_MASK;
    wk |= (RLIN31_IPSR_INIT & RLIN31_IPSR_MASK);
    sil_wrw_mem(PORT_IP1SR6(0), wk);

    sil_wrw_mem(PORT_PMMR6(0), RLIN31_PMMR_INIT);
    wk = sil_rew_mem(PORT_GPSR6(0));
    wk &= ~RLIN31_GP6_MASK;
    wk |= (RLIN31_GPSR_INIT & RLIN31_GP6_MASK);
    sil_wrw_mem(PORT_GPSR6(0), wk);

    wk = sil_rew_mem(PORT_PMMER6(0));
    wk |= RLIN31_PMMER_INIT;
    sil_wrw_mem(PORT_PMMER6(0), wk);

    /*
     *  RLIN33
     */
    wk = sil_rew_mem(PORT_PMMER6(0));
    wk &= ~RLIN33_PMMER_INIT;
    sil_wrw_mem(PORT_PMMER6(0), wk);

    /*
     * Select Function
     */
    sil_wrw_mem(PORT_PMMR6(0), RLIN33_PMMR_INIT);
    wk = sil_rew_mem(PORT_IP1SR6(0));
    wk &= ~RLIN33_IPSR_MASK;
    wk |= (RLIN33_IPSR_INIT & RLIN33_IPSR_MASK);
    sil_wrw_mem(PORT_IP1SR6(0), wk);

    sil_wrw_mem(PORT_PMMR6(0), RLIN33_PMMR_INIT);
    wk = sil_rew_mem(PORT_GPSR6(0));
    wk &= ~RLIN33_GP6_MASK;
    wk |= (RLIN33_GPSR_INIT & RLIN33_GP6_MASK);
    sil_wrw_mem(PORT_GPSR6(0), wk);

    wk = sil_rew_mem(PORT_PMMER6(0));
    wk |= RLIN33_PMMER_INIT;
    sil_wrw_mem(PORT_PMMER6(0), wk);

    /*
     *  RLIN35
     */
    wk = sil_rew_mem(PORT_PMMER6(0));
    wk &= ~RLIN35_PMMER_INIT;
    sil_wrw_mem(PORT_PMMER6(0), wk);

    /*
     * Select Function
     */
    sil_wrw_mem(PORT_PMMR6(0), RLIN35_PMMR_INIT);
    wk = sil_rew_mem(PORT_IP0SR6(0));
    wk &= ~RLIN35_IPSR_MASK;
    wk |= (RLIN35_IPSR_INIT & RLIN35_IPSR_MASK);
    sil_wrw_mem(PORT_IP0SR6(0), wk);

    sil_wrw_mem(PORT_PMMR6(0), RLIN35_PMMR_INIT);
    wk = sil_rew_mem(PORT_GPSR6(0));
    wk &= ~RLIN35_GP6_MASK;
    wk |= (RLIN35_GPSR_INIT & RLIN35_GP6_MASK);
    sil_wrw_mem(PORT_GPSR6(0), wk);

    wk = sil_rew_mem(PORT_PMMER6(0));
    wk |= RLIN35_PMMER_INIT;
    sil_wrw_mem(PORT_PMMER6(0), wk);

    /*
     *  RLIN37
     */
    wk = sil_rew_mem(PORT_PMMER6(0));
    wk &= ~RLIN37_PMMER_INIT;
    sil_wrw_mem(PORT_PMMER6(0), wk);

    /*
     * Select Function
     */
    sil_wrw_mem(PORT_PMMR6(0), RLIN37_PMMR_INIT);
    wk = sil_rew_mem(PORT_IP0SR6(0));
    wk &= ~RLIN37_IPSR_MASK;
    wk |= (RLIN37_IPSR_INIT & RLIN37_IPSR_MASK);
    sil_wrw_mem(PORT_IP0SR6(0), wk);

    sil_wrw_mem(PORT_PMMR6(0), RLIN37_PMMR_INIT);
    wk = sil_rew_mem(PORT_GPSR6(0));
    wk &= ~RLIN37_GP6_MASK;
    wk |= (RLIN37_GPSR_INIT & RLIN37_GP6_MASK);
    sil_wrw_mem(PORT_GPSR6(0), wk);

    wk = sil_rew_mem(PORT_PMMER6(0));
    wk |= RLIN37_PMMER_INIT;
    sil_wrw_mem(PORT_PMMER6(0), wk);

}


static void
target_100uswait(void) {
    volatile uint32 i;
    for(i = 0; i < 0xFFFF; i++);
}

/*
 *  HV起動時の初期化
 */
void
hardware_init_hook(void)
{
    uint8 i;

    if (is_leader()) {
        /* 起動通知用変数の0初期化 */
        bootsync = 0;
        /* クロックの初期化 */
        clk_init(1, 1);
        /* 他のコアを起動する */
        for (i = 0; i < TNUM_PHYS_CORE; i++) {
            if (TBIT_SUPPORT_CORE & (1 << i)) {
                boot_core(i);
                /* 100usの待ち */
                target_100uswait();
            }
        } 
    }
}

/*
 *  ターゲット依存の初期化
 */
void target_initialize(void)
{
    if (is_leader()) {
        msr_enable_uart(0x00000fffU);
        port_init();
        msr_enable_ostm(0x3ff);
    }
}
