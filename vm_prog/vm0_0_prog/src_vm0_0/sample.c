#include "u2a.h"
#include "hvc_cfg.h"
#include "libguest.h"
#include "syslog.h"

#define VMNAME     "VM0_0"
#define OSTMNO     0U
#define TWTGINTNO  544  /* アセンブラファイルは手動で変更すること */

#define GEN_MDP_ERROR

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

/* MIP/MDP違反 */
void
mdp_violate(void) {
    /* MDP違反 */
    *(volatile unsigned int *)(0xfe030000) = 0x12;
    /* OSTM1 へのアクセス */
    *(volatile unsigned int *)(0xFFBF0104) = 0x00;
    /* OSTM1 の割込みの操作 */
    set_exint_pri(OSTM_ITNO(1), 7);
}

volatile int loop;

void
rh850_main(void)
{
    int cnt = 0;
    int ret;
    uint32 timeleft;

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
#ifdef GEN_MDP_ERROR
        mdp_violate();
#endif /* GEN_MDP_ERROR */        
    }
}
