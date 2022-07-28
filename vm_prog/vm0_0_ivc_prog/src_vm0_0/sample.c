#include "u2a.h"
#include "hvc_cfg.h"
#include "libguest.h"
#include "ivccom_guest.h"
#include "ivccom_cfg.h"
#include "syslog.h"
#include "perf.h"

#define VMNAME      "VM0_0"
#define OSTMNO      0U
#define TWTGINTNO   707

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

unsigned char writedata[2048];
unsigned char readdata[2048];

#endif /* ENABLE_PERF */

volatile int loop;

void
rh850_main(void)
{
#ifndef ENABLE_PERF
    int tmp;
    uint32    data[4];
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

    syslog(VMNAME " : Enable interrupt.\n");
    set_plmr(PSW_EIMASK_ENALL);
    x_set_exipm(PSW_EIMASK_ENALL);
    enable_ei();

    ostm_start_interval(OSTMNO, OSTM_TO_CYC(1000000));

    msgqdata[0] = 0;  msgqdata[1] = 1;

    while(1){
        for(loop = 0; loop < 10000000U; loop++);
        syslog(VMNAME " : running!\n");
        data[0] = 2; data[1] = 3; data[2] = 4; data[3] = 5;
        if ((tmp = WriteIVCStateVariable(IVCSVAR1, data)) != E_OK) {
            syslog(VMNAME " : WriteIVCStateVariable() error! %d\n", tmp);
        }
        if ((tmp = ReadIVCStateVariable(IVCSVAR1, data1)) != E_OK) {
            syslog(VMNAME " : ReadIVCStateVariable() error! %d\n", tmp);
        };
        if ((data1[0] == 2) && (data1[1] == 3) && (data1[2] == 4) && (data1[3] == 5)) {
            syslog(VMNAME " : IVCSVAR1 : read collect data!\n");
        }
        else {
            syslog(VMNAME " : IVCSVAR1 : read wrong data! %d, %d, %d, %d\n", data1[0], data1[1], data1[2], data1[3]);
        }

        if((tmp = WriteIVCMessageQueue(IVCMSGQ1, &msgqdata, 8)) != E_OK) {
            syslog(VMNAME " : WriteIVCMessageQueue() error! %d\n", tmp);
        }
        else {
            syslog(VMNAME " : IVCMSGQ1 : write data %d, %d.\n", msgqdata[0], msgqdata[1]);
            msgqdata[0]++;
            msgqdata[1]++;
        }
#if 0
        /*
         *  IVCの低レベルAPIのテスト
         */
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
#endif
    }
#else /* ENABLE_PERF */
    int tmp;
    int *pbuf;

    syslog_init(RLIN3_PORT_NO);

    syslog("\n" VMNAME " : Perf start!\n");

    hvc_test0(); /* Iキャッシュフラッシュ */
    perf_start_allvm_pcnt();
    *CRAMCRGn1_CRGIVCLOCKm(0, 0) = 0;
    *CRAMCRGn1_CRGIVCLOCKm(0, 0);
    *CRAMCRGn1_CRGIVCOWNRm(0, 0);
    Asm("synci");
    cnt = perf_end_allvm_pcnt_val();
    syslog("IVC Register Read : %d \n", cnt);

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

    hvc_test0(); /* Iキャッシュフラッシュ */
    perf_start_allvm_pcnt();
    tmp = ReadIVCStateVariable(PERF_IVCSVARID, readdata);
    cnt = perf_end_allvm_pcnt_val();
    if (tmp == E_OK) {
        syslog("ReadIVCStateVariable(64) : %d \n", cnt);
    }
    else {
        syslog("ReadIVCStateVariable(64) : error %d \n", tmp);
    }

    perf_start_allvm_pcnt();
    tmp = ReadIVCStateVariable(PERF_IVCSVARID, readdata);
    cnt = perf_end_allvm_pcnt_val();
    if (tmp == E_OK) {
        syslog("ReadIVCStateVariable(64) : %d \n", cnt);
    }
    else {
        syslog("ReadIVCStateVariable(64) : error %d \n", tmp);
    }

    hvc_test0(); /* Iキャッシュフラッシュ */
    perf_start_allvm_pcnt();
    tmp = DeactivateIVCStateVariable(PERF_IVCSVARID);
    cnt = perf_end_allvm_pcnt_val();
    if (tmp == E_OK) {
        syslog("DeactivateIVCStateVariable(64) : %d \n", cnt);
    }
    else {
        syslog("DeactivateIVCStateVariable(64) : error %d \n", tmp);
    }

    perf_start_allvm_pcnt();
    DeactivateIVCStateVariable(PERF_IVCSVARID);
    cnt = perf_end_allvm_pcnt_val();
    if (tmp == E_OK) {
        syslog("DeactivateIVCStateVariable(64) : %d \n", cnt);
    }
    else {
        syslog("DeactivateIVCStateVariable(64) : error %d \n", tmp);
    }

    /*
     *  IVC状態変数のテスト
     */
    hvc_test0(); /* Iキャッシュフラッシュ */
    perf_start_allvm_pcnt();
    tmp = WriteIVCMessageQueue(PERF_IVCMSGQID, writedata, PERF_IVCMSGQ_SIZE);
    cnt = perf_end_allvm_pcnt_val();
    if (tmp == E_OK) {
        syslog("WriteIVCMessageQueue(64) : %d \n", cnt);
    }
    else {
        syslog("WriteIVCMessageQueue(64) : error %d \n", tmp);
    }
    
    perf_start_allvm_pcnt();
    tmp = WriteIVCMessageQueue(PERF_IVCMSGQID, writedata, PERF_IVCMSGQ_SIZE);
    cnt = perf_end_allvm_pcnt_val();
    if (tmp == E_OK) {
        syslog("WriteIVCMessageQueue(64) : %d \n", cnt);
    }
    else {
        syslog("WriteIVCMessageQueue(64) : error %d \n", tmp);
    }

    hvc_test0(); /* Iキャッシュフラッシュ */
    perf_start_allvm_pcnt();
    tmp = DeactivateIVCMessageQueue(PERF_IVCMSGQID);
    cnt = perf_end_allvm_pcnt_val();
    if (tmp == E_OK) {
        syslog("DeactivateIVCMessageQueue(64) : %d \n", cnt);
    }
    else {
        syslog("DeactivateIVCMessageQueue(64) : error %d \n", tmp);
    }
    
    perf_start_allvm_pcnt();
    tmp = DeactivateIVCMessageQueue(PERF_IVCMSGQID);
    cnt = perf_end_allvm_pcnt_val();
    if (tmp == E_OK) {
        syslog("DeactivateIVCMessageQueue(64) : %d \n", cnt);
    }
    else {
        syslog("DeactivateIVCMessageQueue(64) : error %d \n", tmp);
    }

    tmp = WriteIVCMessageQueue(PERF_IVCMSGQID, writedata, PERF_IVCMSGQ_SIZE);
    if (tmp != E_OK) syslog("WriteIVCMessageQueue(64) : error %d \n", tmp);
    tmp = WriteIVCMessageQueue(PERF_IVCMSGQID, writedata, PERF_IVCMSGQ_SIZE);
    if (tmp != E_OK) syslog("WriteIVCMessageQueue(64) : error %d \n", tmp);
    tmp = WriteIVCMessageQueue(PERF_IVCMSGQID, writedata, PERF_IVCMSGQ_SIZE);
    if (tmp != E_OK) syslog("WriteIVCMessageQueue(64) : error %d \n", tmp);

    hvc_test0(); /* Iキャッシュフラッシュ */
    perf_start_allvm_pcnt();
    tmp = ReadIVCMessageQueue(PERF_IVCMSGQID, readdata);
    cnt = perf_end_allvm_pcnt_val();
    if (tmp > 0) {
        syslog("ReadIVCMessageQueue(64) : %d \n", cnt);
    }
    else {
        syslog("ReadIVCMessageQueue(64) : error %d \n", tmp);
    }
    
    perf_start_allvm_pcnt();
    tmp = ReadIVCMessageQueue(PERF_IVCMSGQID, readdata);
    cnt = perf_end_allvm_pcnt_val();
    if (tmp > 0) {
        syslog("ReadIVCMessageQueue(64) : %d \n", cnt);
    }
    else {
        syslog("ReadIVCMessageQueue(64) : error %d \n", tmp);
    }

    /*
     *  IVC状態変数の性能評価(サイズ変更による実行時間の変化)
     */
    hvc_test0(); /* Iキャッシュフラッシュ */
    perf_start_allvm_pcnt();
    tmp = WriteIVCStateVariable(5, writedata);
    cnt = perf_end_allvm_pcnt_val();
    if (tmp == E_OK) {
        syslog("WriteIVCStateVariable(256) : %d \n", cnt);
    }
    else {
        syslog("WriteIVCStateVariable(256) : error %d \n", tmp);
    }

    hvc_test0(); /* Iキャッシュフラッシュ */
    perf_start_allvm_pcnt();
    tmp = WriteIVCStateVariable(6, writedata);
    cnt = perf_end_allvm_pcnt_val();
    if (tmp == E_OK) {
        syslog("WriteIVCStateVariable(512) : %d \n", cnt);
    }
    else {
        syslog("WriteIVCStateVariable(512) : error %d \n", tmp);
    }

    hvc_test0(); /* Iキャッシュフラッシュ */
    perf_start_allvm_pcnt();
    tmp = WriteIVCStateVariable(7, writedata);
    cnt = perf_end_allvm_pcnt_val();
    if (tmp == E_OK) {
        syslog("WriteIVCStateVariable(1024) : %d \n", cnt);
    }
    else {
        syslog("WriteIVCStateVariable(1024) : error %d \n", tmp);
    }


    hvc_test0(); /* Iキャッシュフラッシュ */
    perf_start_allvm_pcnt();
    tmp = WriteIVCStateVariable(8, writedata);
    cnt = perf_end_allvm_pcnt_val();
    if (tmp == E_OK) {
        syslog("WriteIVCStateVariable(2048) : %d \n", cnt);
    }
    else {
        syslog("WriteIVCStateVariable(2048) : error %d \n", tmp);
    }



    /*
     *  IVC状態バッファの性能評価
     */
    hvc_test0(); /* Iキャッシュフラッシュ */
    perf_start_allvm_pcnt();
    tmp = AcquireIVCbuf(1, (void**)&pbuf);
    cnt = perf_end_allvm_pcnt_val();
    if (tmp == E_OK) {
        syslog("AcquireIVCbuf() : %d \n", cnt);
    }
    else {
        syslog("AcquireIVCbuf() : error %d \n", tmp);
    }

    hvc_test0(); /* Iキャッシュフラッシュ */
    perf_start_allvm_pcnt();
    tmp = ReleaseIVCbuf(1);
    cnt = perf_end_allvm_pcnt_val();
    if (tmp == E_OK) {
        syslog("ReleaseIVCbuf() : %d \n", cnt);
    }
    else {
        syslog("ReleaseIVCbuf() : error %d \n", tmp);
    }

#endif /* !ENABLE_PERF */
}
