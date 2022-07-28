#include "u2a.h"
#include "hvc.h"
#include "hvc_cfg.h"
#include "libguest.h"
#include "ivccom_guest.h"
#include "ivccom_cfg.h"
#include "syslog.h"

#define VMNAME       "VM0_1"
#define OSTMNO       1U
#define TWTGINTNO    708

void
ostm0_handler(void)
{
    syslog(VMNAME  " : OSTM0 Handler Start.\n");
}

void
ostm1_handler(void)
{
    syslog(VMNAME  " : OSTM1 Handler Start.\n");
}

void
ostm2_handler(void)
{
    syslog(VMNAME  " : OSTM2 Handler Start.\n");
}

void
ostm3_handler(void)
{
    syslog(VMNAME  " : OSTM3 Handler Start.\n");
}

void
ostm4_handler(void)
{
    syslog(VMNAME  " : OSTM4 Handler Start.\n");
}

void
twtgint_handler(void)
{
    syslog("\n" VMNAME  " : TWTGINT Handler Start.\n");
}



#ifdef ENABLE_PERF
#define PERF_IVCSVARID        4U

#define PERF_IVCMSGQID        3U
#define PERF_IVCMSGQ_SIZE    64U
#endif /* ENABLE_PERF */

volatile int loop;

void
rh850_main(void)
{
#ifndef ENABLE_PERF
    int        tmp;
    uint32    data1[4];
    uint32    msgqdata[4];

    syslog_init(RLIN3_PORT_NO);

    syslog("\n" VMNAME " : start!\n");

    /* 拡張割込みを有効に */
    set_intcfg(INTCFG_EPL);

    /* 割込みの登録 */
    set_exint_vector(OSTM_ITNO(OSTMNO), INTVECTOR_PRI);
    set_exint_pri(OSTM_ITNO(OSTMNO), 7);
    ena_exint(OSTM_ITNO(OSTMNO));

    set_exint_vector(TWTGINTNO, INTVECTOR_PRI);
    set_exint_pri(TWTGINTNO, 7);
    ena_exint(TWTGINTNO);

    syslog(VMNAME " : Enable interrupt.\n");
    set_plmr(PSW_EIMASK_ENALL);
    x_set_exipm(PSW_EIMASK_ENALL);
    enable_ei();

    ostm_start_interval(OSTMNO, OSTM_TO_CYC(1000000));

#if 0
    /*
     *  IVCの低レベルAPIのテスト
     */
    tmp = *((volatile uint8 *)(0xFE071000));
    if (tmp == 0x11) {
        uart_puts(RLIN3_PORT_NO, VMNAME " : read collect data!\n\r");
    }
    else {
        uart_puts(RLIN3_PORT_NO, VMNAME " : read wrong data!\n\r");
    }
    
    while(1){
        for(loop = 0; loop < 10000000U; loop++);
        uart_puts(RLIN3_PORT_NO, VMNAME " : running!\n\r");
        tmp = TryLockIVC(0, 0);
        if (tmp == 1) {
            uart_puts(RLIN3_PORT_NO, VMNAME " : lock IVC sucess!\n\r");
        }
        else if (tmp == -1) {
            uart_puts(RLIN3_PORT_NO, VMNAME " : lock IVC fail -1 !\n\r");
        }
        else if (tmp == -2) {
            uart_puts(RLIN3_PORT_NO, VMNAME " : lock IVC fail -2 !\n\r");
        }
        else if (tmp == -3) {
            uart_puts(RLIN3_PORT_NO, VMNAME " : lock IVC fail -3 !\n\r");
        }
        tmp = *((volatile uint8 *)(0xFE071000));
        if (tmp == 0x11) {
            uart_puts(RLIN3_PORT_NO, VMNAME " : 1 read collect data!\n\r");
        }
        else {
            uart_puts(RLIN3_PORT_NO, VMNAME " : 1 read wrong data!\n\r");
        }
        UnLockIVC(0, 0);
    }
#endif
    while(1){
        for(loop = 0; loop < 10000000U; loop++);
        syslog(VMNAME " : running!\n");
        if ((tmp = ReadIVCStateVariable(IVCSVAR1, data1)) != E_OK) {
            syslog(VMNAME " : ReadIVCStateVariable() error! %d\n", tmp);
        };
        if ((data1[0] == 2) && (data1[1] == 3) && (data1[2] == 4) && (data1[3] == 5)) {
            syslog(VMNAME " : IVCSVAR1 : read collect data!\n");
        }
        else {
            syslog(VMNAME " : IVCSVAR1 : read wrong data! %d, %d, %d, %d\n", data1[0], data1[1], data1[2], data1[3]);
        }
        if((tmp = ReadIVCMessageQueue(IVCMSGQ1, &msgqdata)) < 0) {
            syslog(VMNAME " : ReadIVCMessageQueue() error! %d\n", tmp);
        }
        else {
            syslog(VMNAME " : IVCMSGQ1 : read data are %d, %d, %d, %d\n", msgqdata[0], msgqdata[1], msgqdata[2], msgqdata[3]);
        }
    }
#else /* ENABLE_PERF */
    unsigned char writedata[100];
    int tmp;

    syslog_init(RLIN3_PORT_NO);
    syslog("\n" VMNAME " : start!\n");

    /*
     *  IVC状態変数のテスト
     */
    hvc_test0(); /* Iキャッシュフラッシュ */
    perf_start_allvm_pcnt();
    tmp = WriteIVCStateVariable(PERF_IVCSVARID, writedata);
    cnt = perf_end_allvm_pcnt_val();
    if (tmp == E_OK) {
        syslog("WriteIVCStateVariable(64) : %d \n", cnt);
    }
    else {
        syslog("WriteIVCStateVariable(64) : error %d \n", tmp);
    }

    perf_start_allvm_pcnt();
    tmp = WriteIVCStateVariable(PERF_IVCSVARID, writedata);
    cnt = perf_end_allvm_pcnt_val();
    if (tmp == E_OK) {
        syslog("WriteIVCStateVariable(64) : %d \n", cnt);
    }
    else {
        syslog("WriteIVCStateVariable(64) : error %d \n", tmp);
    }

    while(1) {
        for(cnt = 0; cnt < 10000000U; cnt++);
        syslog(VMNAME " : running!\n");
    }
#endif /* !ENABLE_PERF */
}
