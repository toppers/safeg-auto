#include "hv_impl.h"


/*
 *  RLIN30 Port Configration
 *   P10_4 : RLIN30TX (第2兼用Output)
 *   P10_5 : RLIN30RX (第4兼用Input)
 */
#define RLIN30_P10_MASK		((uint16)0x0030)

#define RLIN30_PMC10_INIT	((uint16)0x0030)
#define RLIN30_PFCAE10_INIT	((uint16)0x0000)
#define RLIN30_PFCE10_INIT	((uint16)0x0020)
#define RLIN30_PFC10_INIT	((uint16)0x0030)
#define RLIN30_PM10_INIT	((uint16)0x0020)

/*
 *  RLIN31 Port Configration
 *   P2_5 : RLIN31TX (第3兼用Output)
 *   P2_4 : RLIN31RX (第3兼用Input)
 */
#define RLIN31_P2_MASK		((uint16)0x0030)

#define RLIN31_PMC2_INIT	((uint16)0x0030)
#define RLIN31_PFCAE2_INIT	((uint16)0x0000)
#define RLIN31_PFCE2_INIT	((uint16)0x0030)
#define RLIN31_PFC2_INIT	((uint16)0x0000)
#define RLIN31_PM2_INIT		((uint16)0x0010)

/*
 *  RLIN32 Port Configration
 *   P2_0 : RLIN32TX (第4兼用Output)
 *   P2_1 : RLIN32RX (第4兼用Input)
 */
#define RLIN32_P2_MASK		((uint16)0x0003)

#define RLIN32_PMC2_INIT	((uint16)0x0003)
#define RLIN32_PFCAE2_INIT	((uint16)0x0000)
#define RLIN32_PFCE2_INIT	((uint16)0x0003)
#define RLIN32_PFC2_INIT	((uint16)0x0003)
#define RLIN32_PM2_INIT		((uint16)0x0002)

/*
 *  RLIN33 Port Configration
 *   P4_14 : RLIN33TX (第3兼用Output)
 *   P4_15 : RLIN33RX (第3兼用Input)
 */
#define RLIN33_P2_MASK		((uint16)0xC000)

#define RLIN33_PMC2_INIT	((uint16)0xC000)
#define RLIN33_PFCAE2_INIT	((uint16)0x0000)
#define RLIN33_PFCE2_INIT	((uint16)0xC000)
#define RLIN33_PFC2_INIT	((uint16)0x0000)
#define RLIN33_PM2_INIT		((uint16)0x8000)

#define FCLACTL0_RLIN3    ((uint8_t*)0xFFED4000)
#define FCLACTL1_RLIN3    ((uint8_t*)0xFFED4004)

void
port_init(void)
{
    uint16 wk;

    sil_wrw_mem(PKCPROT, UNLOCKKEY_VAL);

    /*
     * RLIN3x
     */
    /* Bypass filter*/
    sil_wrb_mem(FCLACTL0_RLIN3, 0x80);
    /* TTL level */
    sil_wrh_mem(PISA(10), 0x20);

    /*
     * RLIN30
     */
    /*
     * Enable write
     */
    sil_wrw_mem(PWE, (1U << 10));
    
    /*
     * Select Alternative mode
     */
    wk = sil_reh_mem(PMC(10));
    wk &= ~RLIN30_P10_MASK;
    wk |= (RLIN30_PMC10_INIT & RLIN30_P10_MASK);
    sil_wrh_mem(PMC(10), wk);

    /*
     * Select Function
     */
    /* Set PFCAE10 */
    wk = sil_reh_mem(PFCAE(10));
    wk &= ~RLIN30_P10_MASK;
    wk |= (RLIN30_PFCAE10_INIT & RLIN30_P10_MASK);
    sil_wrh_mem(PFCAE(10), wk);

    /* Set PFCE10 */
    wk = sil_reh_mem(PFCE(10));
    wk &= ~RLIN30_P10_MASK;
    wk |= (RLIN30_PFCE10_INIT & RLIN30_P10_MASK);
    sil_wrh_mem(PFCE(10), wk);

    /* Set PFC10 */
    wk = sil_reh_mem(PFC(10));
    wk &= ~RLIN30_P10_MASK;
    wk |= (RLIN30_PFC10_INIT & RLIN30_P10_MASK);
    sil_wrh_mem(PFC(10), wk);

    /* Set PM10 */
    wk = sil_reh_mem(PM(10));
    wk &= ~RLIN30_P10_MASK;
    wk |= (RLIN30_PM10_INIT & RLIN30_P10_MASK);
    sil_wrh_mem(PM(10), wk);

    sil_wrw_mem(PWE, 1U << 5);

    /*
     * RLIN31
     */
    /*
     * Enable write
     */
    sil_wrw_mem(PWE, (1U << 3));
    
    /*
     * Select Alternative mode
     */
    wk = sil_reh_mem(PMC(2));
    wk &= ~RLIN31_P2_MASK;
    wk |= (RLIN31_PMC2_INIT & RLIN31_P2_MASK);
    sil_wrh_mem(PMC(2), wk);

    /*
     * Select Function
     */
    /* Set PFCAE2 */
    wk = sil_reh_mem(PFCAE(2));
    wk &= ~RLIN31_P2_MASK;
    wk |= (RLIN31_PFCAE2_INIT & RLIN31_P2_MASK);
    sil_wrh_mem(PFCAE(2), wk);

    /* Set PFCE2 */
    wk = sil_reh_mem(PFCE(2));
    wk &= ~RLIN31_P2_MASK;
    wk |= (RLIN31_PFCE2_INIT & RLIN31_P2_MASK);
    sil_wrh_mem(PFCE(2), wk);

    /* Set PFC2 */
    wk = sil_reh_mem(PFC(2));
    wk &= ~RLIN31_P2_MASK;
    wk |= (RLIN31_PFC2_INIT & RLIN31_P2_MASK);
    sil_wrh_mem(PFC(2), wk);

    /* Set PM2 */
    wk = sil_reh_mem(PM(2));
    wk &= ~RLIN31_P2_MASK;
    wk |= (RLIN31_PM2_INIT & RLIN31_P2_MASK);
    sil_wrh_mem(PM(2), wk);

    /*
     * RLIN32
     */

    /*
     * Enable write
     */
    sil_wrw_mem(PWE, (1U << 3));
    
    /*
     * Select Alternative mode
     */
    wk = sil_reh_mem(PMC(2));
    wk &= ~RLIN32_P2_MASK;
    wk |= (RLIN32_PMC2_INIT & RLIN32_P2_MASK);
    sil_wrh_mem(PMC(2), wk);

    /*
     * Select Function
     */
    /* Set PFCAE2 */
    wk = sil_reh_mem(PFCAE(2));
    wk &= ~RLIN32_P2_MASK;
    wk |= (RLIN32_PFCAE2_INIT & RLIN32_P2_MASK);
    sil_wrh_mem(PFCAE(2), wk);

    /* Set PFCE2 */
    wk = sil_reh_mem(PFCE(2));
    wk &= ~RLIN32_P2_MASK;
    wk |= (RLIN32_PFCE2_INIT & RLIN32_P2_MASK);
    sil_wrh_mem(PFCE(2), wk);

    /* Set PFC2 */
    wk = sil_reh_mem(PFC(2));
    wk &= ~RLIN32_P2_MASK;
    wk |= (RLIN32_PFC2_INIT & RLIN32_P2_MASK);
    sil_wrh_mem(PFC(2), wk);

    /* Set PM2 */
    wk = sil_reh_mem(PM(2));
    wk &= ~RLIN32_P2_MASK;
    wk |= (RLIN32_PM2_INIT & RLIN32_P2_MASK);
    sil_wrh_mem(PM(2), wk);

    /*
     * RLIN33
     */

    /*
     * Enable write
     */
    sil_wrw_mem(PWE, (1U << 5));
    
    /*
     * Select Alternative mode
     */
    wk = sil_reh_mem(PMC(4));
    wk &= ~RLIN33_P2_MASK;
    wk |= (RLIN33_PMC2_INIT & RLIN33_P2_MASK);
    sil_wrh_mem(PMC(4), wk);

    /*
     * Select Function
     */
    /* Set PFCAE2 */
    wk = sil_reh_mem(PFCAE(4));
    wk &= ~RLIN33_P2_MASK;
    wk |= (RLIN33_PFCAE2_INIT & RLIN33_P2_MASK);
    sil_wrh_mem(PFCAE(4), wk);

    /* Set PFCE2 */
    wk = sil_reh_mem(PFCE(4));
    wk &= ~RLIN33_P2_MASK;
    wk |= (RLIN33_PFCE2_INIT & RLIN33_P2_MASK);
    sil_wrh_mem(PFCE(4), wk);

    /* Set PFC2 */
    wk = sil_reh_mem(PFC(4));
    wk &= ~RLIN33_P2_MASK;
    wk |= (RLIN33_PFC2_INIT & RLIN33_P2_MASK);
    sil_wrh_mem(PFC(4), wk);

    /* Set PM2 */
    wk = sil_reh_mem(PM(4));
    wk &= ~RLIN33_P2_MASK;
    wk |= (RLIN33_PM2_INIT & RLIN33_P2_MASK);
    sil_wrh_mem(PM(4), wk);

    sil_wrw_mem(PWE, 0x00000000);
    sil_wrw_mem(PKCPROT, LOCKKEY_VAL);
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
        clk_init(2,1);
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
