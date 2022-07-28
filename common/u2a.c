#include "u2a.h"

/*
 *  Interrupt Controller
 */

/*
 *  Bind interrupt to host machine
 */
void
bind_int_to_host(uint32_t intno, uint32_t coreidx)
{
    if(intno <= INTC1_INTNO_MAX) {
        sil_wrw_mem(INTC1_EIBD(intno),  coreidx);
    }
    else if(intno <= INTC2_INTNO_MAX) {
        sil_wrw_mem(INTC2_EIBD(intno), coreidx);
    }
}

/*
 *  Bind interrupt to vm
 */
void
bind_int_to_gm(uint32_t intno, uint32_t gpid, uint32_t coreidx)
{
    if(intno <= INTC1_INTNO_MAX) {
        sil_wrw_mem(INTC1_EIBD(intno), EIBD_GM|(gpid << EIBD_GPID_BIT)|coreidx);
    }
    else if(intno <= INTC2_INTNO_MAX) {
        sil_wrw_mem(INTC2_EIBD(intno), EIBD_GM|(gpid << EIBD_GPID_BIT)|coreidx);
    }
}

/*
 *  Set vector type
 */
void
set_exint_vector(uint32_t intno, uint32_t type)
{
    uint32_t *reg;

    reg = (intno <= INTC1_INTNO_MAX)? INTC1_EEIC(intno) : INTC2_EEIC(intno);

    if(type == INTVECTOR_PRI) {
        sil_wrw_mem(reg,
                    sil_rew_mem(reg) & ~EEIC_EITB);
    } else {
        sil_wrw_mem(reg,
                    sil_rew_mem(reg) | EEIC_EITB);
    }
}

/*
 * Set interrupt priority
 */
void
set_exint_pri(uint32_t intno, uint32_t pri)
{
    uint32_t tmp;
    uint32_t *reg;

    reg = (intno <= INTC1_INTNO_MAX)? INTC1_EEIC(intno) : INTC2_EEIC(intno);

    tmp = sil_rew_mem(reg);
    tmp = (tmp & ~EEIC_EIP_MASK) | (pri & EEIC_EIP_MASK);
    sil_wrw_mem(reg, tmp);
}

/*
 *  Enable interrupt
 */
void
ena_exint(uint32_t intno)
{
    uint32_t tmp;
    uint32_t *reg;

    reg = (intno <= INTC1_INTNO_MAX)? INTC1_EEIC(intno) : INTC2_EEIC(intno);

    tmp = sil_rew_mem(reg);
    tmp &= ~EEIC_EIMK;
    sil_wrw_mem(reg, tmp);
}

/*
 *  Disable interrupt
 */
void
dis_exint(uint32_t intno)
{
    uint32_t tmp;
    uint32_t *reg;

    reg = (intno <= INTC1_INTNO_MAX)? INTC1_EEIC(intno) : INTC2_EEIC(intno);

    tmp = sil_rew_mem(reg);
    tmp |= EEIC_EIMK;
    sil_wrw_mem(reg, tmp);
}

/*
 *  Raise interrupt
 */
void
raise_exint(uint32_t intno)
{
    uint32_t tmp;
    uint32_t *reg;

    reg = (intno <= INTC1_INTNO_MAX)? INTC1_EEIC(intno) : INTC2_EEIC(intno);

    tmp = sil_rew_mem(reg);
    tmp |= EEIC_EIRF;
    sil_wrw_mem(reg, tmp);
}


/*
 *  Check interrupt
 */  
uint32_t
check_exint(uint32_t intno) 
{
    uint32_t *reg;

    reg = (intno <= INTC1_INTNO_MAX)? INTC1_EEIC(intno) : INTC2_EEIC(intno);

    if (sil_rew_mem(reg) & EEIC_EIRF){
        return 1;
    }
    else {
        return 0;
    }
}

/*
 *  Clear interrupt
 */
void
clear_exint(uint32_t intno) 
{
    uint32_t *reg;

    reg = (intno <= INTC1_INTNO_MAX)? INTC1_EEIC(intno) : INTC2_EEIC(intno);
    sil_wrw_mem(reg,
                sil_rew_mem(reg) & ~EEIC_EIRF);
}


/*
 *  Clock Controller
 */
#define MOSCE      ((uint32_t*)(0xFF988000))
#define MOSCS      ((uint32_t*)(0xFF988004))
#define PLLE       ((uint32_t*)(0xFF980000))
#define PLLS       ((uint32_t*)(0xFF980004))
#define CLKD_PLLC  ((uint32_t*)(0xFF980120))
#define CLKD_PLLS  ((uint32_t*)(0xFF980128))
#define CKSC_CPUC  ((uint32_t*)(0xFF980100))
#define CKSC_CPUS  ((uint32_t*)(0xFF980108))

void
clk_init(uint32_t clk_cpu_div, uint32_t clk_pll0_div)
{
    uint32_t wk;

    /* Unlock */
    sil_wrw_mem(CLKKCPROT1, UNLOCKKEY_VAL);
    __SYNCP();

    /* Start Main OSC */
    sil_wrw_mem(MOSCE, 0x01);
    do {
        wk = sil_rew_mem(MOSCS);
    } while(!((wk & 0x02) == 0x02));
    __SYNCP();

    /* Start PLL */
    sil_wrw_mem(PLLE, 0x01);
    do {
        wk = sil_rew_mem(PLLS);
    } while(!((wk & 0x02) == 0x02));
    __SYNCP();

    /* Select CPU Clock */
    sil_wrw_mem(CLKD_PLLC, clk_cpu_div); 
    do {
        wk = sil_rew_mem(CLKD_PLLS);
    } while(!((wk & 0x02) == 0x02));
    __SYNCP();

    /* Select PLL0 */
    sil_wrw_mem(CKSC_CPUC, 0x00);
    do {
        wk = sil_rew_mem(CKSC_CPUS);
    } while(!((wk & 0x01) == 0x00));
    __SYNCP();

    sil_wrw_mem(CLKD_PLLC, clk_pll0_div); 
    do {
        wk = sil_rew_mem(CLKD_PLLS);
    } while(!((wk & 0x02) == 0x02));
    __SYNCP();
    
    sil_wrw_mem(CLKKCPROT1, LOCKKEY_VAL);
    __SYNCP();
}


/*
 *  RLIN30
 */
#define RLIN30_BASE  (0xFFD28000)
#define RLIN31_BASE  (0xFFC7C000)
#define RLIN32_BASE  (0xFFD28200)
#define RLIN33_BASE  (0xFFC7C080)
#define RLIN34_BASE  (0xFFD28400)
#define RLIN35_BASE  (0xFFC7C100)
#define RLIN36_BASE  (0xFFD28600)
#define RLIN37_BASE  (0xFFC7C180)
#define RLIN38_BASE  (0xFFD28800)
#define RLIN39_BASE  (0xFFC7C200)
#define RLIN310_BASE (0xFFD28C00)
#define RLIN311_BASE (0xFFC7C280)

#define RLIN3xLWBR(base)      ((uint8_t*)(base + 0x01U))
#define RLIN3xLBRP01(base)    ((uint16_t*)(base + 0x02U))
#define RLIN3xLBRP0(base)     ((uint8_t*)(base + 0x02U))
#define RLIN3xLBRP1(base)     ((uint8_t*)(base + 0x03U))
#define RLIN3xLSTC(base)      ((uint8_t*)(base + 0x04U))
#define RLIN3xLMD(base)       ((uint8_t*)(base + 0x08U))
#define RLIN3xLBFC(base)      ((uint8_t*)(base + 0x09U))
#define RLIN3xLSC(base)       ((uint8_t*)(base + 0x0aU))
#define RLIN3xLWUP(base)      ((uint8_t*)(base + 0x0bU))
#define RLIN3xLIE(base)       ((uint8_t*)(base + 0x0cU))
#define RLIN3xLEDE(base)      ((uint8_t*)(base + 0x0dU))
#define RLIN3xLCUC(base)      ((uint8_t*)(base + 0x0eU))
#define RLIN3xLTRC(base)      ((uint8_t*)(base + 0x10U))
#define RLIN3xLMST(base)      ((uint8_t*)(base + 0x11U))
#define RLIN3xLST(base)       ((uint8_t*)(base + 0x12U))
#define RLIN3xLEST(base)      ((uint8_t*)(base + 0x13U))
#define RLIN3xLDFC(base)      ((uint8_t*)(base + 0x14U))
#define RLIN3xLIDB(base)      ((uint8_t*)(base + 0x15U))
#define RLIN3xLCBR(base)      ((uint8_t*)(base + 0x16U))
#define RLIN3xLUDB0(base)     ((uint8_t*)(base + 0x17U))
#define RLIN3xLDBR1(base)     ((uint8_t*)(base + 0x18U))
#define RLIN3xLDBR2(base)     ((uint8_t*)(base + 0x19U))
#define RLIN3xLDBR3(base)     ((uint8_t*)(base + 0x1aU))
#define RLIN3xLDBR4(base)     ((uint8_t*)(base + 0x1bU))
#define RLIN3xLDBR5(base)     ((uint8_t*)(base + 0x1cU))
#define RLIN3xLDBR6(base)     ((uint8_t*)(base + 0x1dU))
#define RLIN3xLDBR7(base)     ((uint8_t*)(base + 0x1eU))
#define RLIN3xLDBR8(base)     ((uint8_t*)(base + 0x1fU))
#define RLIN3xLUOER(base)     ((uint8_t*)(base + 0x20U))
#define RLIN3xLUOR1(base)     ((uint8_t*)(base + 0x21U))
#define RLIN3xLUTDR(base)     ((uint16_t*)(base + 0x24U))
#define RLIN3xLUTDRL(base)    ((uint8_t*)(base + 0x24U))
#define RLIN3xLUTDRH(base)    ((uint8_t*)(base + 0x25U))
#define RLIN3xLURDR(base)     ((uint16_t*)(base + 0x26U))
#define RLIN3xLURDRL(base)    ((uint8_t*)(base + 0x26U))
#define RLIN3xLURDRH(base)    ((uint8_t*)(base + 0x27U))
#define RLIN3xLUWTDR(base)    ((uint16_t*)(base + 0x28U))
#define RLIN3xLUWTDRL(base)   ((uint8_t*)(base + 0x28U))
#define RLIN3xLUWTDRH(base)   ((uint8_t*)(base + 0x29U))

const uint32_t rlin30_base[] = {
    RLIN30_BASE, RLIN31_BASE, RLIN32_BASE, RLIN33_BASE,
    RLIN34_BASE, RLIN35_BASE, RLIN36_BASE, RLIN37_BASE,
    RLIN38_BASE, RLIN39_BASE, RLIN310_BASE, RLIN311_BASE
};

const uint32_t rlin30_rx_int[] = {
    RLIN30_RX_INTNO, RLIN31_RX_INTNO, RLIN32_RX_INTNO, RLIN33_RX_INTNO,
    RLIN34_RX_INTNO, RLIN35_RX_INTNO, RLIN36_RX_INTNO, RLIN37_RX_INTNO,
    RLIN38_RX_INTNO, RLIN39_RX_INTNO, RLIN310_RX_INTNO, RLIN311_RX_INTNO
};

void
uart_init(uint32_t no, uint32_t lwbr_val, uint32_t lbrp01_val)
{
    uint32_t base =rlin30_base[no];
      
    /* Uart Mode を有効(ノイズフィルタも有効) */
    sil_wrb_mem(RLIN3xLMD(base), 0x31U);

    /* ボーレート設定 */
    sil_wrb_mem(RLIN3xLWBR(base), lwbr_val);
    sil_wrh_mem(RLIN3xLBRP01(base), lbrp01_val);

    /* エラー検出許可 */
    sil_wrb_mem(RLIN3xLEDE(base), 0x0DU);

    /* データ フォーマット */
    sil_wrb_mem(RLIN3xLBFC(base), 0x00U);

    /* リセット解除 */
    sil_wrb_mem(RLIN3xLCUC(base), 0x01U);

    /* リセット解除待ち */
    while (sil_reb_mem(RLIN3xLMST(base)) == 0U) {
    }

    /* 送受信動作許可 */
    sil_wrb_mem(RLIN3xLUOER(base), 0x03U);
}

void
uart_putc(uint32_t no, char c)
{
    uint32_t base =rlin30_base[no];
      
    while ((sil_reb_mem(RLIN3xLST(base)) & 0x10) == 0x10) ;
    sil_wrb_mem(RLIN3xLUTDRL(base), c);
}

void
uart_puts(uint32_t no, char *str) 
{
    int i;
    for (i = 0; str[i] != '\0'; i++){
        uart_putc(no, str[i]);
    }
}

char
uart_getc(uint32_t no) 
{
    uint32_t base = rlin30_base[no];
    
    return(sil_reb_mem(RLIN3xLURDRL(base)));
}

char
uart_pgetc(uint32_t no) 
{
    uint32_t base = rlin30_base[no];
    
    while(check_exint(rlin30_rx_int[no]) == 0);
    clear_exint(rlin30_rx_int[no]);
    return(sil_reb_mem(RLIN3xLURDRL(base)));
}


/*
 *  OSTM
 */
#define OSTM0_BASE    (0xFFBF0000)
#define OSTM1_BASE    (0xFFBF0100)
#define OSTM2_BASE    (0xFFBF0200)
#define OSTM3_BASE    (0xFFBF0300)
#define OSTM4_BASE    (0xFFBF0400)
#define OSTM5_BASE    (0xFFBF0500)
#define OSTM6_BASE    (0xFFBF0600)
#define OSTM7_BASE    (0xFFBF0700)
#define OSTM8_BASE    (0xFFBF0800)
#define OSTM9_BASE    (0xFFBF0900)

const uint32_t ostm_base[] = {
    OSTM0_BASE, OSTM1_BASE, OSTM2_BASE, OSTM3_BASE, OSTM4_BASE,
    OSTM5_BASE, OSTM6_BASE, OSTM7_BASE, OSTM8_BASE, OSTM9_BASE
};

const uint32_t ostm_intno[] = {
    OSTM0_INTNO, OSTM1_INTNO, OSTM2_INTNO, OSTM3_INTNO, OSTM4_INTNO,
    OSTM5_INTNO, OSTM6_INTNO, OSTM7_INTNO, OSTM8_INTNO, OSTM9_INTNO
};

void
ostm_pol_wait(uint32_t no, uint32_t cnt)
{
    uint32_t c_cnt;
    uint32_t base = ostm_base[no];

    /* stop timer */
    sil_wrb_mem(OSTM_TT(base), 0x01);

    /* フリーラーニングモード */
    sil_wrb_mem(OSTM_CTL(base), 0x02);

    /* コンペアレジスタの設定 */
    sil_wrw_mem(OSTM_CMP(base), 0xffffffff);

    sil_wrw_mem(OSTM_CNT(base), 0x00000000);

    /* スタート */
    sil_wrb_mem(OSTM_TS(base), 0x01);

    /* wait untile underflow */
    do {
        c_cnt = sil_rew_mem(OSTM_CNT(base));
    } while (c_cnt < cnt);
    
    /* ストップ */
    sil_wrb_mem(OSTM_TT(base), 0x01);
}

void
ostm_start_interval(uint32_t no, uint32_t cnt)
{
    uint32_t base = ostm_base[no];
    
    /* stop timer */
    sil_wrb_mem(OSTM_TT(base), 0x01U);

    /* インターバル・タイマ・モード・割込み許可  */
    sil_wrb_mem(OSTM_CTL(base), (0x00U | 0x80U));

    /* コンペアレジスタの設定 */
    sil_wrw_mem(OSTM_CMP(base), cnt);

    /* スタート */
    sil_wrb_mem(OSTM_TS(base), 0x01U);
}

void
ostm_init_oneshot(uint32_t no) 
{
    uint32_t base = ostm_base[no];

    /* stop timer */
    sil_wrb_mem(OSTM_TT(base), 0x01);

    /* フリーランイン・タイマ・モード・割込み許可・自動ロードなし */
    sil_wrb_mem(OSTM_CTL(base), (0x02U | 0x80U | 0x04U));
}

void
ostm_raise_int(uint32_t no)
{
    uint32_t base = ostm_base[no];

    /* stop timer */
    sil_wrb_mem(OSTM_TT(base), 0x01U);

    /* フリーラーニングモード・割込み許可 */
    sil_wrb_mem(OSTM_CTL(base), (0x02U | 0x80U));

    /* コンペアレジスタの設定 */
    sil_wrw_mem(OSTM_CMP(base), 1U);

    /* スタート */
    sil_wrb_mem(OSTM_TS(base), 0x01U);
}

/*
 *  単発割込みの発生
 */
void
ostm_start_oneshot(uint32_t no, uint32_t cnt) 
{
    uint32_t base = ostm_base[no];
    
    /* stop timer */
    sil_wrb_mem(OSTM_TT(base), 0x01U);

    /* フリーランイン・タイマ・モード・割込み許可・自動ロードなし */
    sil_wrb_mem(OSTM_CTL(base), (0x02U | 0x80U | 0x04U));
    
    /* カウンタの設定 */
    sil_wrw_mem(OSTM_CNT(base), 0);

    /* コンペアレジスタの設定 */
    sil_wrw_mem(OSTM_CMP(base), cnt);

    /* スタート */
    sil_wrb_mem(OSTM_TS(base), 0x01);
}

uint32
ostm_cnt_read(uint32_t no)
{
    uint32_t base = ostm_base[no];
    
    return sil_rew_mem(OSTM_CNT(base));
}

void
ostm_clear_int(uint32_t no)
{

}

void
ostm_clear_stop_int(uint32_t no)
{
    uint32_t base = ostm_base[no];
    
    /* stop timer */
    sil_wrb_mem(OSTM_TT(base), 0x01);
}

/*
 *  TAUD0
 */
void
taud0_init(uint32_t prs)
{
    /* モジュールスタンバイの解除 */
    msr_enable_taud0();

    /* プリスケーラの設定 */
    sil_wrh_mem(TAUD0_TPS,
                (sil_reh_mem(TAUD0_TPS) & ~TAUD_TPS_PRS0_MASK)|prs);
}

void
taud0_start_interval(uint32_t ch, uint32_t cnt)
{
    /* CK0によるカウント，ソフトウェアトリガ，インターバルタイマ */
    sil_wrh_mem(TAUD0_CMOR(ch), ((0x00U << 14U)| (0x00U << 12U)) | (0x00U << 8U) | (0x00U));
    
    /* タイマ停止 */
    sil_wrh_mem(TAUD0_TT, (1U << ch));
    /* 周期のカウント値の設定 */
    sil_wrh_mem(TAUD0_CDR(ch), cnt);
    /* タイマスタート */
    sil_wrh_mem(TAUD0_TS, (1U << ch));
}

/*
 *  CCRH用のダミー関数
 */
void
_rh850_dummy_call(void) {
    
}
