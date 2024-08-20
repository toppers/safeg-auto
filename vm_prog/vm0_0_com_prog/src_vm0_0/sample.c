#include "u2a.h"
#include "hvc_cfg.h"
#include "libguest.h"
#include "vmcom_guest.h"
#include "syslog.h"
#include <string.h>

#define VMNAME       "VM0_0"
#define OSTMNO       0U
#define TWTGINTNO  544  /* アセンブラファイルは手動で変更すること */

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

volatile int twtgint_cnt;

void
twtgint_handler(void)
{
    syslog("\n" VMNAME  " : TWTGINT Handler Start.\n");
    twtgint_cnt++;
}

#define CHECK_EXP(no, exp) do {                     \
    if (!(exp)) {                                   \
        syslog(VMNAME " : Test %d error!\n", no);   \
        error++;                                    \
    }                                               \
} while (false)

#define TEST_SVARID           1U
#define INVALID_SVARID        6U
#define NOWRITE_SVARID        3U

#define TEST_WRITE_MSGQID           1U
#define TEST_WRITE_MSGQID2          3U
#define TEST_READ_MSGQID            2U
#define TEST_WRITE_MSGQ_SIZE        8U
#define INVALID_WRITE_MSGQ_SIZE    16U
#define INVALID_MSGQID              6U
#define NOWRITE_MSGQID              2U

#define INVALID_WRITEPTR    0x00000000U
#define INVALID_READPTR     0x00000000U

#define PERF_SVARID        4U

#define PERF_MSGQID        3U
#define PERF_MSGQ_SIZE    64U

#define VALID_SBUFID     1U
#define VALID_SBUFID2    2U
#define VALID_SBUFID3    3U
#define INVALID_SBUFID   5U
#define NOACCESS_SBUFID  4U

#define SBUF1_SIZE        64U
#define SBUF2_SIZE        128U

volatile int loop;

void
rh850_main(void)
{
#ifndef ENABLE_PERF
    uint error;
    unsigned char writedata[100];
    unsigned char readdata[100];
    int cnt = 0;
    int ret;
    uint32 timeleft;
    uint8 *p_data;
    uint8 *p_data1;
    uint8 *p_data2;
    int pre_twtgint_cnt;
    
    twtgint_cnt = 0;
    error = 0;
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

    /*
     *  状態変数のテスト
     */
    
    /*
     *  状態変数シンプル異常系のテスト
     */
    /* 状態変数テスト1 : WriteStateVariable() : E_ID */
    CHECK_EXP(1,  WriteStateVariable(INVALID_SVARID, writedata) == E_ID);

    /* 状態変数テスト2 : WriteStateVariable() : E_MACV */
    CHECK_EXP(2,  WriteStateVariable(TEST_SVARID, (void*)INVALID_READPTR) == E_MACV);

    /* 状態変数テスト3 : WriteStateVariable() : E_OACV */
    CHECK_EXP(3,  WriteStateVariable(NOWRITE_SVARID, writedata) == E_OACV);

    /* 状態変数テスト4 : ReadStateVariable() : E_ID */
    CHECK_EXP(4,  ReadStateVariable(INVALID_SVARID, readdata) == E_ID);

    /* 状態変数テスト5 : ReadStateVariable() : E_MACV */
    CHECK_EXP(5,  ReadStateVariable(TEST_SVARID, (void*)INVALID_WRITEPTR) == E_MACV);

    /* 状態変数テスト6 : ReadStateVariable() : E_OBJ */
    CHECK_EXP(6,  ReadStateVariable(NOWRITE_SVARID, readdata) == E_OBJ);

    /* 状態変数テスト7 : DeactivateStateVariable() : E_ID */
    CHECK_EXP(7,  DeactivateStateVariable(INVALID_SVARID) == E_ID);

    /* 状態変数テスト8 : DeactivateStateVariable() : E_OACV */
    CHECK_EXP(8,  DeactivateStateVariable(NOWRITE_SVARID) == E_OACV);

    /*
     *  状態変数正常系のテスト
     */
    /* 状態変数テスト10 : WriteStateVariable() : E_OK */
    CHECK_EXP(10,  WriteStateVariable(TEST_SVARID, (void*)("poi")) == E_OK);

    /* 状態変数テスト11 : ReadStateVariable() : E_OK */
    CHECK_EXP(11,  ReadStateVariable(TEST_SVARID, (void*)readdata) == E_OK);
    
    /* 状態変数テスト12 : 書き込んだ値が読み込めたか : E_OK */
    CHECK_EXP(12,  strcmp((char*)readdata, "poi") == 0);

    /* 状態変数テスト13 : DeactivateStateVariable() : E_OK */
    CHECK_EXP(13,  DeactivateStateVariable(TEST_SVARID) == E_OK);

    /* 状態変数テスト14 : DeactivateStateVariable() : E_OK */
    CHECK_EXP(14,  DeactivateStateVariable(TEST_SVARID) == E_OK);

    /*
     *  状態変数異常系のテスト
     */
    /* 状態変数テスト20 : ReadStateVariable() : E_OK */
    CHECK_EXP(20,  ReadStateVariable(TEST_SVARID, (void*)readdata) == E_OBJ);

    /*
     *  VM0_1でのテスト用
     */
    WriteStateVariable(TEST_SVARID, (void*)("poi"));

    /*
     *  メッセージキューシンプル異常系のテスト
     */

    /* メッセージキューテスト30 : WriteMessageQueue() : E_ID */
    CHECK_EXP(30,  WriteMessageQueue(INVALID_MSGQID, writedata, TEST_WRITE_MSGQ_SIZE) == E_ID);

    /* メッセージキューテスト31 : WriteMessageQueue() : E_MACV */
    CHECK_EXP(31,  WriteMessageQueue(TEST_WRITE_MSGQID, (void*)INVALID_WRITEPTR, TEST_WRITE_MSGQ_SIZE) == E_MACV);

    /* メッセージキューテスト32 : WriteMessageQueue() : E_OACV */
    CHECK_EXP(32,  WriteMessageQueue(NOWRITE_MSGQID, writedata, TEST_WRITE_MSGQ_SIZE) == E_OACV);

    /* メッセージキューテスト33 : WriteMessageQueue() : E_MACV */
    CHECK_EXP(33,  WriteMessageQueue(TEST_WRITE_MSGQID, (void*)INVALID_WRITEPTR, TEST_WRITE_MSGQ_SIZE) == E_MACV);

    /* メッセージキューテスト34 : WriteMessageQueue() : E_PAR */
    CHECK_EXP(34,  WriteMessageQueue(TEST_WRITE_MSGQID, writedata, INVALID_WRITE_MSGQ_SIZE) == E_PAR);

    /* メッセージキューテスト35 : ReadMessageQueue() : E_ID */
    CHECK_EXP(35,  ReadMessageQueue(INVALID_MSGQID, readdata) == E_ID);

    /* メッセージキューテスト36 : DeactivateMessageQueue() : E_ID */
    CHECK_EXP(36,  DeactivateMessageQueue(INVALID_MSGQID) == E_ID);

    /* メッセージキューテスト37 : DeactivateMessageQueue() : E_OACV */
    CHECK_EXP(37,  DeactivateMessageQueue(NOWRITE_MSGQID) == E_OACV);

    /*
     *  メッセージキュー正常系のテスト
     */
    /* メッセージキューテスト40-44 : WriteMessageQueue() : E_OK */
    CHECK_EXP(40,  WriteMessageQueue(TEST_WRITE_MSGQID, (void*)("Test1"), 6) == E_OK);
    CHECK_EXP(41,  WriteMessageQueue(TEST_WRITE_MSGQID, (void*)("Test2"), 6) == E_OK);

    /*
     *  メッセージキュー異常系のテスト
     */
    /* メッセージキューテスト50 : WriteMessageQueue() : E_BUF */
    CHECK_EXP(50,  WriteMessageQueue(TEST_WRITE_MSGQID, (void*)("Test3"), 6) == E_BUF);

    /* メッセージキューテスト51 : WriteMessageQueue() : E_OACV */
    CHECK_EXP(51,  ReadMessageQueue(TEST_WRITE_MSGQID, readdata) == E_OACV);

    /*
     *  メッセージキュー正常系のテスト
     */
    /* メッセージキューテスト45 : DeactivateMessageQueue() : E_OK */
    CHECK_EXP(42,  DeactivateMessageQueue(TEST_WRITE_MSGQID) == E_OK);

    /*
     *  VM0_1でのテスト用
     */
    CHECK_EXP(40,  WriteMessageQueue(TEST_WRITE_MSGQID, (void*)("Test4"), 6) == E_OK);
    CHECK_EXP(41,  WriteMessageQueue(TEST_WRITE_MSGQID, (void*)("Test5"), 6) == E_OK);

    /*
     *  共有バッファのテスト
     */

    /*
     *  共有バッファ異常系のテスト
     */
    /* 共有バッファテスト1 : AcquireSharedBuffer() : E_ID */
    CHECK_EXP(60,  AcquireSharedBuffer(INVALID_SBUFID, (void*)&p_data) == E_ID);

    /* 共有バッファテスト2 : AcquireSharedBuffer() : E_OACV */
    CHECK_EXP(61,  AcquireSharedBuffer(NOACCESS_SBUFID, (void*)&p_data) == E_OACV);

    /* 共有バッファテスト3 : AcquireSharedBuffer() : E_OBJ */
    CHECK_EXP(62,  AcquireSharedBuffer(VALID_SBUFID, (void*)&p_data) == E_OK);
    CHECK_EXP(63,  AcquireSharedBuffer(VALID_SBUFID, (void*)&p_data) == E_OBJ);
    CHECK_EXP(64,  ReleaseSharedBuffer(VALID_SBUFID) == E_OK);

    /* 共有バッファテスト4 : ReleaseSharedBuffer() : E_ID */
    CHECK_EXP(65,  ReleaseSharedBuffer(INVALID_SBUFID) == E_ID);

    /* 共有バッファテスト5 : AcquireSharedBuffer() : E_OACV */
    CHECK_EXP(66,  ReleaseSharedBuffer(NOACCESS_SBUFID) == E_OACV);

    /* 共有バッファテスト6 : AcquireSharedBuffer() : E_OBJ */
    CHECK_EXP(67,  ReleaseSharedBuffer(VALID_SBUFID) == E_OBJ);

    /*
     *  共有バッファ正常系のテスト
     */
    /* 共有バッファテスト7 : バッファへアクセス可能か */
    CHECK_EXP(68,  AcquireSharedBuffer(VALID_SBUFID, (void*)&p_data) == E_OK);
    for (loop = 0; loop < SBUF1_SIZE; loop++) {
        *(p_data + loop) = loop;
    }
    CHECK_EXP(69,  ReleaseSharedBuffer(VALID_SBUFID) == E_OK);

    /* 共有バッファテスト8 : 1周期後もバッファへアクセス可能か */
    CHECK_EXP(70,  AcquireSharedBuffer(VALID_SBUFID2, (void*)&p_data) == E_OK);

    pre_twtgint_cnt = twtgint_cnt;
    while(twtgint_cnt == pre_twtgint_cnt);

    /* アクセス */
    for (loop = 0; loop < SBUF2_SIZE; loop++) {
        *(p_data + loop) = loop;
    }

    CHECK_EXP(71,  ReleaseSharedBuffer(VALID_SBUFID2) == E_OK);

    /* 共有バッファテスト9 : 複数のバッファへアクセス可能か */
    CHECK_EXP(72,  AcquireSharedBuffer(VALID_SBUFID, (void*)&p_data1) == E_OK);
    CHECK_EXP(73,  AcquireSharedBuffer(VALID_SBUFID2, (void*)&p_data2) == E_OK);
    for (loop = 0; loop < SBUF1_SIZE; loop++) {
        *(p_data1 + loop) = loop;
    }
    for (loop = 0; loop < SBUF2_SIZE; loop++) {
        *(p_data2 + loop) = loop;
    }
    CHECK_EXP(74,  ReleaseSharedBuffer(VALID_SBUFID) == E_OK);
    CHECK_EXP(75,  ReleaseSharedBuffer(VALID_SBUFID2) == E_OK);
    
    /* 共有バッファテスト9 : 複数のバッファへのアクセスが1周期後も可能か */
    CHECK_EXP(76,  AcquireSharedBuffer(VALID_SBUFID, (void*)&p_data1) == E_OK);
    CHECK_EXP(77,  AcquireSharedBuffer(VALID_SBUFID2, (void*)&p_data2) == E_OK);

    pre_twtgint_cnt = twtgint_cnt;
    while(twtgint_cnt == pre_twtgint_cnt);
    
    for (loop = 0; loop < SBUF1_SIZE; loop++) {
        *(p_data1 + loop) = loop;
    }
    for (loop = 0; loop < SBUF2_SIZE; loop++) {
        *(p_data2 + loop) = loop;
    }
    CHECK_EXP(78,  ReleaseSharedBuffer(VALID_SBUFID) == E_OK);
    CHECK_EXP(79,  ReleaseSharedBuffer(VALID_SBUFID2) == E_OK);
    
    
    if (error == 0) {
        syslog(VMNAME " : pass all test!\n\n");
    }
    else {
        syslog(VMNAME " : faile %d test!\n\n", error);
    };

    ostm_start_interval(OSTMNO, OSTM_TO_CYC(1000000));

    while(1) {
        for(loop = 0; loop < 50000000U; loop++);
        GetVMTWTimeLeft(&timeleft);
        syslog(VMNAME " : running %d : Time Left %d.\n", cnt++, timeleft);
        ret = hvc_test0();
        syslog(VMNAME " : hvc_test0() return %d.\n", ret);
        ret = hvc_test1(1);
        syslog(VMNAME " : hvc_test1() return %d.\n", ret);
        ret = hvc_test2(2,3);
        syslog(VMNAME " : hvc_test2() return %d.\n", ret);
        ret = hvc_test3(4,5,6);
        syslog(VMNAME " : hvc_test3() return %d.\n", ret);
    }
#else /* ENABLE_PERF */
    unsigned char writedata[100];
    unsigned char readdata[100];
    int tmp;
    uint8 *p_data;
    int cnt = 0;

    syslog_init(RLIN3_PORT_NO);

    syslog("\n" VMNAME " : Perf start!\n");

    hvc_test0(); /* Iキャッシュフラッシュ */
    perf_start_allvm_pcnt();
    tmp = WriteStateVariable(PERF_SVARID, writedata);
    cnt = perf_end_allvm_pcnt_val();
    if (tmp == E_OK) {
        syslog("WriteStateVariable(64) : %d \n", cnt);
    }
    else {
        syslog("WriteStateVariable(64) : error! : %d \n", tmp);
    }

    perf_start_allvm_pcnt();
    tmp = WriteStateVariable(PERF_SVARID, writedata);
    cnt = perf_end_allvm_pcnt_val();
    if (tmp == E_OK) {
        syslog("WriteStateVariable(64) : %d \n", cnt);
    }
    else {
        syslog("WriteStateVariable(64) : error! : %d \n", tmp);
    }
    
    hvc_test0(); /* Iキャッシュフラッシュ */
    perf_start_allvm_pcnt();
    tmp = ReadStateVariable(PERF_SVARID, readdata);
    cnt = perf_end_allvm_pcnt_val();
    if (tmp == E_OK) {
        syslog("ReadStateVariable(64) : %d \n", cnt);
    }
    else {
        syslog("ReadStateVariable(64) : error! : %d \n", tmp);
    }
    
    perf_start_allvm_pcnt();
    ReadStateVariable(PERF_SVARID, readdata);
    cnt = perf_end_allvm_pcnt_val();
    if (tmp == E_OK) {
        syslog("ReadStateVariable(64) : %d \n", cnt);
    }
    else {
        syslog("ReadStateVariable(64) : error! : %d \n", tmp);
    }

    hvc_test0(); /* Iキャッシュフラッシュ */
    perf_start_allvm_pcnt();
    DeactivateStateVariable(PERF_SVARID);
    cnt = perf_end_allvm_pcnt_val();
    syslog("DeactivateStateVariable(64) : %d \n", cnt);

    perf_start_allvm_pcnt();
    DeactivateStateVariable(PERF_SVARID);
    cnt = perf_end_allvm_pcnt_val();
    syslog("DeactivateStateVariable(64) : %d \n", cnt);

    hvc_test0(); /* Iキャッシュフラッシュ */
    perf_start_allvm_pcnt();
    WriteMessageQueue(PERF_MSGQID, writedata, PERF_MSGQ_SIZE);
    cnt = perf_end_allvm_pcnt_val();
    syslog("WriteMessageQueue(64) : %d \n", cnt);

    perf_start_allvm_pcnt();
    WriteMessageQueue(PERF_MSGQID, writedata, PERF_MSGQ_SIZE);
    cnt = perf_end_allvm_pcnt_val();
    syslog("WriteMessageQueue(64) : %d \n", cnt);

    hvc_test0(); /* Iキャッシュフラッシュ */
    perf_start_allvm_pcnt();
    DeactivateMessageQueue(PERF_MSGQID);
    cnt = perf_end_allvm_pcnt_val();
    syslog("DeactivateMessageQueue(64) : %d \n", cnt);

    perf_start_allvm_pcnt();
    DeactivateMessageQueue(PERF_MSGQID);
    cnt = perf_end_allvm_pcnt_val();
    syslog("DeactivateMessageQueue(64) : %d \n", cnt);

    WriteMessageQueue(PERF_MSGQID, writedata, PERF_MSGQ_SIZE);
    WriteMessageQueue(PERF_MSGQID, writedata, PERF_MSGQ_SIZE);
    WriteMessageQueue(PERF_MSGQID, writedata, PERF_MSGQ_SIZE);

    hvc_test0(); /* Iキャッシュフラッシュ */
    perf_start_allvm_pcnt();
    AcquireSharedBuffer(VALID_SBUFID, (void*)&p_data);
    cnt = perf_end_allvm_pcnt_val();
    syslog("AcquireSharedBuffer(1) : %d \n", cnt);

    hvc_test0(); /* Iキャッシュフラッシュ */
    perf_start_allvm_pcnt();
    AcquireSharedBuffer(VALID_SBUFID2, (void*)&p_data);
    cnt = perf_end_allvm_pcnt_val();
    syslog("AcquireSharedBuffer(2) : %d \n", cnt);

    hvc_test0(); /* Iキャッシュフラッシュ */
    perf_start_allvm_pcnt();
    AcquireSharedBuffer(VALID_SBUFID3, (void*)&p_data);
    cnt = perf_end_allvm_pcnt_val();
    syslog("AcquireSharedBuffer(3) : %d \n", cnt);    

    hvc_test0(); /* Iキャッシュフラッシュ */
    perf_start_allvm_pcnt();
    ReleaseSharedBuffer(VALID_SBUFID);
    cnt = perf_end_allvm_pcnt_val();
    syslog("ReleaseSharedBuffer(1) : %d \n", cnt);

    hvc_test0(); /* Iキャッシュフラッシュ */
    perf_start_allvm_pcnt();
    ReleaseSharedBuffer(VALID_SBUFID2);
    cnt = perf_end_allvm_pcnt_val();
    syslog("ReleaseSharedBuffer(2) : %d \n", cnt);

    hvc_test0(); /* Iキャッシュフラッシュ */
    perf_start_allvm_pcnt();
    ReleaseSharedBuffer(VALID_SBUFID3);
    cnt = perf_end_allvm_pcnt_val();
    syslog("ReleaseSharedBuffer(3) : %d \n", cnt);    
#endif /* !ENABLE_PERF */
}
