#include "u2a.h"
#include "hvc_cfg.h"
#include "libguest.h"
#include "syslog.h"

#define VMNAME				"VM0_1"
#define OSTMNO				1U
#define TWTGINTNO			708

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

volatile int loop;

void
rh850_main(void) 
{
	int cnt = 0;
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
	}
}
