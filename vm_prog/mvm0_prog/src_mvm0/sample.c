#include "u2a.h"
#include "hvc_cfg.h"
#include "libguest.h"
#include "syslog.h"

#define VMNAME              "VM0"
#define OSTMNO_CORE0          0
#define OSTMNO_CORE1          1
#define TWTGINTNO_CORE0     544
#define TWTGINTNO_CORE1     545

const uint rlin3_portno[] = {
    RLIN3_PORT_NO_CORE0,
    RLIN3_PORT_NO_CORE1
};

const uint ostmno[] = {
    OSTMNO_CORE0,
    OSTMNO_CORE1
};

const uint twtgintno[] = {
    TWTGINTNO_CORE0,
    TWTGINTNO_CORE1
};

/*
 *  起動時の同期用変数
 */
volatile int bootsync;

void
ostm0_handler(void)
{
    uint    coreidx = get_peid();
    syslog(VMNAME  " : OSTM0 Handler Start on Core%d.\n", coreidx);
}

void
ostm1_handler(void)
{
    uint    coreidx = get_peid();
    syslog(VMNAME  " : OSTM1 Handler Start on Core%d.\n", coreidx);
}

void
ostm2_handler(void)
{
    uint    coreidx = get_peid();
    syslog(VMNAME  " : OSTM2 Handler Start on Core%d.\n", coreidx);
}

void
ostm3_handler(void)
{
    uint    coreidx = get_peid();
    syslog(VMNAME  " : OSTM3 Handler Start on Core%d.\n", coreidx);
}

void
ostm4_handler(void)
{
    uint    coreidx = get_peid();
    syslog(VMNAME  " : OSTM4 Handler Start on Core%d.\n", coreidx);
}

void
twtgint_handler(void)
{
    uint    coreidx = get_peid();
    syslog("\n" VMNAME  " : TWTGINT Handler Start on Core%d.\n", coreidx);
}

volatile int loop[2];

void
rh850_main(void) {
    uint coreidx = get_peid();
    uint my_rlin3_portno = rlin3_portno[coreidx];
    uint my_ostmno = ostmno[coreidx];
    uint my_twtgintno = twtgintno[coreidx];
    int cnt = 0;
    int ret;
    uint32 timeleft;

    syslog_init(my_rlin3_portno);

    syslog("\n" VMNAME " : start on Core%d!\n", coreidx);

    /* 拡張割込みを有効に */
    set_intcfg(INTCFG_EPL);

    /* 割込みの登録 */    
    set_exint_vector(OSTM_ITNO(my_ostmno), INTVECTOR_PRI);
    set_exint_pri(OSTM_ITNO(my_ostmno), 7);
    ena_exint(OSTM_ITNO(my_ostmno));
    set_exint_vector(my_twtgintno, INTVECTOR_PRI);
    set_exint_pri(my_twtgintno, 7);
    ena_exint(my_twtgintno);

    syslog(VMNAME " : Enable interrupt.\n");
    set_plmr(PSW_EIMASK_ENALL);
    x_set_exipm(PSW_EIMASK_ENALL);
    enable_ei();

    ostm_start_interval(my_ostmno, OSTM_TO_CYC(1000000));

    while(1) {
        for(loop[coreidx] = 0; loop[coreidx] < 50000000U; loop[coreidx]++);
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
}
