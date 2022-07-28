#include "hv_impl.h"


/*
 *  RLIN37 Port Configration
 *   P2_14 : RLIN37TX (第5兼用Output)
 *   P2_13 : RLIN37RX (第5兼用Input)
 */
#define RLIN37_P2_MASK      ((uint16)0x6000)

#define RLIN37_PMC2_INIT    ((uint16)0x6000)
#define RLIN37_PFCAE2_INIT  ((uint16)0x6000)
#define RLIN37_PFCE2_INIT   ((uint16)0x0000)
#define RLIN37_PFC2_INIT    ((uint16)0x0000)
#define RLIN37_PM2_INIT     ((uint16)0x2000)

/*
 *  RLIN34 Port Configration
 *   P6_12 : RLIN34TX (第4兼用Output)
 *   P6_15 : RLIN34RX (第7兼用Input)
 */
#define RLIN34_P6_MASK      ((uint16)0x9000)
#define RLIN34_PMC6_INIT    ((uint16)0x9000)
#define RLIN34_PFCAE6_INIT  ((uint16)0x8000)
#define RLIN34_PFCE6_INIT   ((uint16)0x9000)
#define RLIN34_PFC6_INIT    ((uint16)0x1000)
#define RLIN34_PM6_INIT     ((uint16)0x8000)


/*
 *  RLIN32 Port Configration
 *   P21_5 : RLIN32TX (第4兼用Output)
 *   P21_4 : RLIN32RX (第4兼用Input)
 */
#define RLIN32_P21_MASK      ((uint16)0x0030)
#define RLIN32_PMC21_INIT    ((uint16)0x0030)
#define RLIN32_PFCAE21_INIT  ((uint16)0x0000)
#define RLIN32_PFCE21_INIT   ((uint16)0x0030)
#define RLIN32_PFC21_INIT    ((uint16)0x0030)
#define RLIN32_PM21_INIT     ((uint16)0x0010)

/*
 *  RLIN311 Port Configration
 *   P3_8 : RLIN311TX (第2兼用Output)  
 *   P3_7 : RLIN311RX (第2兼用Input)
 */
#define RLIN311_P3_MASK      ((uint16)0x0180)
#define RLIN311_PMC3_INIT    ((uint16)0x0180)
#define RLIN311_PFCAE3_INIT  ((uint16)0x0000)
#define RLIN311_PFCE3_INIT   ((uint16)0x0000)
#define RLIN311_PFC3_INIT    ((uint16)0x0180)
#define RLIN311_PM3_INIT     ((uint16)0x0080)


#define FCLACTL0_RLIN3    ((uint8_t*)0xFFED4000)
#define FCLACTL1_RLIN3    ((uint8_t*)0xFFED4004)

void
port_init(void)
{
    uint16 wk;

    sil_wrw_mem(PKCPROT, UNLOCKKEY_VAL);

    /*
     * RLIN30
     */    
    /* Bypass filter*/
    sil_wrb_mem(FCLACTL0_RLIN3, 0x80);
    /* TTL level */
    sil_wrh_mem(PISA(10), 0x20);
    
    /*
     *  RLIN37
     */
    /*
     *  Enabel Write
     */
    sil_wrw_mem(PWE, 1U << 3U);
    
    /*
     * Select Alternative mode
     */
    wk = sil_reh_mem(PMC(2));
    wk &= ~RLIN37_P2_MASK;
    wk |= (RLIN37_PMC2_INIT & RLIN37_P2_MASK);
    sil_wrh_mem(PMC(2), wk);

    /*
     * Select Function
     */
    /* Set PFCAE2 */
    wk = sil_reh_mem(PFCAE(2));
    wk &= ~RLIN37_P2_MASK;
    wk |= (RLIN37_PFCAE2_INIT & RLIN37_P2_MASK);
    sil_wrh_mem(PFCAE(2), wk);

    /* Set PFCE2 */
    wk = sil_reh_mem(PFCE(2));
    wk &= ~RLIN37_P2_MASK;
    wk |= (RLIN37_PFCE2_INIT & RLIN37_P2_MASK);
    sil_wrh_mem(PFCE(2), wk);

    /* Set PFC2 */
    wk = sil_reh_mem(PFC(2));
    wk &= ~RLIN37_P2_MASK;
    wk |= (RLIN37_PFC2_INIT & RLIN37_P2_MASK);
    sil_wrh_mem(PFC(2), wk);

    /* Set PM2 */
    wk = sil_reh_mem(PM(2));
    wk &= ~RLIN37_P2_MASK;
    wk |= (RLIN37_PM2_INIT & RLIN37_P2_MASK);
    sil_wrh_mem(PM(2), wk);

    /* Set P2 */
    wk = sil_reh_mem(P(2));
    wk &= ~RLIN37_P2_MASK;
    wk |= (0x0000 & RLIN37_P2_MASK);
    sil_wrh_mem(P(2), wk);

    /*
     *  RLIN34
     */
    /*
     *  Enabel Write
     */
    sil_wrw_mem(PWE, 1U << 7U);
    
    /*
     * Select Alternative mode
     */
    wk = sil_reh_mem(PMC(6));
    wk &= ~RLIN34_P6_MASK;
    wk |= (RLIN34_PMC6_INIT & RLIN34_P6_MASK);
    sil_wrh_mem(PMC(6), wk);

    /*
     * Select Function
     */
    /* Set PFCAE6 */
    wk = sil_reh_mem(PFCAE(6));
    wk &= ~RLIN34_P6_MASK;
    wk |= (RLIN34_PFCAE6_INIT & RLIN34_P6_MASK);
    sil_wrh_mem(PFCAE(6), wk);

    /* Set PFCE6 */
    wk = sil_reh_mem(PFCE(6));
    wk &= ~RLIN34_P6_MASK;
    wk |= (RLIN34_PFCE6_INIT & RLIN34_P6_MASK);
    sil_wrh_mem(PFCE(6), wk);

    /* Set PFC6 */
    wk = sil_reh_mem(PFC(6));
    wk &= ~RLIN34_P6_MASK;
    wk |= (RLIN34_PFC6_INIT & RLIN34_P6_MASK);
    sil_wrh_mem(PFC(6), wk);

    /* Set PM6 */
    wk = sil_reh_mem(PM(6));
    wk &= ~RLIN34_P6_MASK;
    wk |= (RLIN34_PM6_INIT & RLIN34_P6_MASK);
    sil_wrh_mem(PM(6), wk);

    /* Set P6 */
    wk = sil_reh_mem(P(6));
    wk &= ~RLIN34_P6_MASK;
    wk |= (0x0000 & RLIN34_P6_MASK);
    sil_wrh_mem(P(6), wk);


    /*
     *  RLIN32
     */
    /*
     *  Enabel Write
     */
    sil_wrw_mem(PWE, 1U << 17U);
    
    /*
     * Select Alternative mode
     */
    wk = sil_reh_mem(PMC(21));
    wk &= ~RLIN32_P21_MASK;
    wk |= (RLIN32_PMC21_INIT & RLIN32_P21_MASK);
    sil_wrh_mem(PMC(21), wk);

    /*
     * Select Function
     */
    /* Set PFCAE21 */
    wk = sil_reh_mem(PFCAE(21));
    wk &= ~RLIN32_P21_MASK;
    wk |= (RLIN32_PFCAE21_INIT & RLIN32_P21_MASK);
    sil_wrh_mem(PFCAE(21), wk);

    /* Set PFCE21 */
    wk = sil_reh_mem(PFCE(21));
    wk &= ~RLIN32_P21_MASK;
    wk |= (RLIN32_PFCE21_INIT & RLIN32_P21_MASK);
    sil_wrh_mem(PFCE(21), wk);

    /* Set PFC21 */
    wk = sil_reh_mem(PFC(21));
    wk &= ~RLIN32_P21_MASK;
    wk |= (RLIN32_PFC21_INIT & RLIN32_P21_MASK);
    sil_wrh_mem(PFC(21), wk);

    /* Set PM21 */
    wk = sil_reh_mem(PM(21));
    wk &= ~RLIN32_P21_MASK;
    wk |= (RLIN32_PM21_INIT & RLIN32_P21_MASK);
    sil_wrh_mem(PM(21), wk);

    /* Set P21 */
    wk = sil_reh_mem(P(21));
    wk &= ~RLIN32_P21_MASK;
    wk |= (0x0000 & RLIN32_P21_MASK);
    sil_wrh_mem(P(21), wk);


    /*
     *  RLIN311
     */
    /*
     *  Enabel Write
     */
    sil_wrw_mem(PWE, 1U << 4U);
    
    /*
     * Select Alternative mode
     */
    wk = sil_reh_mem(PMC(3));
    wk &= ~RLIN311_P3_MASK;
    wk |= (RLIN311_PMC3_INIT & RLIN311_P3_MASK);
    sil_wrh_mem(PMC(3), wk);

    /*
     * Select Function
     */
    /* Set PFCAE3 */
    wk = sil_reh_mem(PFCAE(3));
    wk &= ~RLIN311_P3_MASK;
    wk |= (RLIN311_PFCAE3_INIT & RLIN311_P3_MASK);
    sil_wrh_mem(PFCAE(3), wk);

    /* Set PFCE3 */
    wk = sil_reh_mem(PFCE(3));
    wk &= ~RLIN311_P3_MASK;
    wk |= (RLIN311_PFCE3_INIT & RLIN311_P3_MASK);
    sil_wrh_mem(PFCE(3), wk);

    /* Set PFC3 */
    wk = sil_reh_mem(PFC(3));
    wk &= ~RLIN311_P3_MASK;
    wk |= (RLIN311_PFC3_INIT & RLIN311_P3_MASK);
    sil_wrh_mem(PFC(3), wk);

    /* Set PM3 */
    wk = sil_reh_mem(PM(3));
    wk &= ~RLIN311_P3_MASK;
    wk |= (RLIN311_PM3_INIT & RLIN311_P3_MASK);
    sil_wrh_mem(PM(3), wk);

    /* Set P3 */
    wk = sil_reh_mem(P(3));
    wk &= ~RLIN311_P3_MASK;
    wk |= (0x0000 & RLIN311_P3_MASK);
    sil_wrh_mem(P(3), wk);    

    /*
     *  Disable Write
     */
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
