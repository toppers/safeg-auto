#include "u2a.h"
#include "hvc_cfg.h"
#include "libguest.h"
#include "vmcom_guest.h"
#include "syslog.h"
#include <string.h>

#define VMNAME       "VM0_1"
#define OSTMNO       1U
#define TWTGINTNO			545  /* アセンブラファイルは手動で変更すること */

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

#define CHECK_EXP(no, exp) do {                        \
    if (!(exp)) {                                      \
        syslog(VMNAME " : Test %d error!\n", no);      \
        error++;                                       \
    }                                                  \
} while (false)

#define TEST_SVARID            1U
#define TEST_MSGQID            1U

#define INVALID_WRITEPTR    0x00000000U
#define INVALID_READPTR     0x00000000U

#define PERF_MSGQID      3U

#define VALID_SBUFID     1U
#define VALID_SBUFID2    2U

#define SBUF1_SIZE      64U
#define SBUF2_SIZE     128U

volatile int loop;

void
rh850_main(void)
{
#ifndef ENABLE_PERF
    uint error;
    unsigned char readdata[100];
    int cnt = 0;
    uint32 timeleft;
    uint8 *p_data;

    error = 0;
    syslog_init(RLIN3_PORT_NO);

    syslog("\n" VMNAME " : start!\n");

    /*
     *  状態変数正常系のテスト
     */
    /* 状態変数テスト1 : ReadStateVariable() : E_OK */
    CHECK_EXP(1,  ReadStateVariable(TEST_SVARID, readdata) == E_OK);

    /* 状態変数テスト2 : 書き込んだ値が読み込めたか */
    CHECK_EXP(2,  strcmp((char*)readdata, "poi") == 0);


    /*
     *  メッセージキューの異常系のテスト
     */
    CHECK_EXP(10,  ReadMessageQueue(TEST_MSGQID, INVALID_READPTR) == E_MACV);

    /*
     *  メッセージキューの正常系のテスト
     */
    CHECK_EXP(20,  ReadMessageQueue(TEST_MSGQID, readdata) == 6);
    CHECK_EXP(21,  strcmp((char*)readdata, "Test4") == 0);
    CHECK_EXP(22,  ReadMessageQueue(TEST_MSGQID, readdata) == 6);
    CHECK_EXP(23,  strcmp((char*)readdata, "Test5") == 0);

    /*
     *  メッセージキューの異常系のテスト
     */
    CHECK_EXP(30,  ReadMessageQueue(TEST_MSGQID, readdata) == E_BUF);

    /*
     *  共有バッファの異常系のテスト
     */
    CHECK_EXP(40,  AcquireSharedBuffer(VALID_SBUFID2, (void*)&p_data) == E_OBJ);

    /*
     *  共有バッファの正常系のテスト
     */
    CHECK_EXP(50,  AcquireSharedBuffer(VALID_SBUFID, (void*)&p_data) == E_OK);
    for (loop = 0; loop < SBUF1_SIZE; loop++) {
        if (*(p_data + loop) != loop) {
            error++;
        }
    }
    CHECK_EXP(51,  ReleaseSharedBuffer(VALID_SBUFID) == E_OK);
    

    if (error == 0) {
        syslog(VMNAME " : pass all test!\n\n");
    }
    else {
        syslog(VMNAME " : faile %d test!\n\n", error);
    };


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

    while(1) {
        for(loop = 0; loop < 50000000U; loop++);
        GetVMTWTimeLeft(&timeleft);
        syslog(VMNAME " : running %d : Time Left %d.\n", cnt++, timeleft);
    }
#else /* ENABLE_PERF */
    unsigned char readdata[100];
    int cnt = 0;

    syslog_init(RLIN3_PORT_NO);

    syslog("\n" VMNAME " : Perf start!\n");

    hvc_test0(); /* Iキャッシュフラッシュ */
    perf_start_allvm_pcnt();
    ReadMessageQueue(PERF_MSGQID, readdata);
    cnt = perf_end_allvm_pcnt_val();
    syslog("ReadMessageQueue(64) : %d \n", cnt);

    perf_start_allvm_pcnt();
    ReadMessageQueue(PERF_MSGQID, readdata);
    cnt = perf_end_allvm_pcnt_val();
    syslog("ReadMessageQueue(64) : %d \n", cnt);
#endif /* !ENABLE_PERF */
}
