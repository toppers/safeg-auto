#ifndef _PERF_H_
#define _PERF_H_

#include "rh850.h"

#define PERF_NUM_MEASURE   (100U)

#define PERF_ALL_PCNTNO    (0U)
#define PERF_NINT_PCNTNO   (1U)
#define PERF_ALLVM_PCNTNO  (6U)
#define PERF_FLG_PCNTNO    (7U)

LOCAL_INLINE void
perf_init(void) {
    int i;

    /* Enable Access from ALL VM */
    set_pmgmctrl(PMGMCTRL_ENAALL);

    /* Stop and Clear Performance Counter */
    for (i = 0; i < TNUM_PMCOUNT; i++) {
        stop_pmcount(i);
        set_pmcount(i, 0);
    }
}

LOCAL_INLINE void
perf_start(void) {
    stop_pmcount(PERF_ALL_PCNTNO);
    stop_pmcount(PERF_NINT_PCNTNO);
    set_pmcount(PERF_ALL_PCNTNO, 0);
    set_pmcount(PERF_NINT_PCNTNO, 0);
    start_pmcount(PERF_ALL_PCNTNO,  PMCTRL_CND_ALL,   PMCTRL_CNT_HOST);
    start_pmcount(PERF_NINT_PCNTNO, PMCTRL_CND_NOINT, PMCTRL_CNT_HOST);
}

#define perf_twint_pass perf_sysint_pass

LOCAL_INLINE void
perf_sysint_pass(void) {
    set_pmcount(PERF_FLG_PCNTNO, 1);
    stop_pmcount(PERF_ALLVM_PCNTNO);
    set_pmcount(PERF_ALLVM_PCNTNO, 0xffffffff);
}

LOCAL_INLINE boolean
perf_hvint_delay(uint32_t *array, uint32_t *cnt) {
    uint32_t cnt1, cnt2;
    boolean ret = false;

    if (*cnt < PERF_NUM_MEASURE) {
        cnt1 = get_pmcount(PERF_ALL_PCNTNO);
        cnt2 = get_pmcount(PERF_NINT_PCNTNO);
        array[(*cnt)++] = cnt1 - cnt2;
        ret = true;
    }
    set_pmcount(PERF_FLG_PCNTNO, 1);

    return ret;
}


#define perf_sysint_oh  perf_twint_oh

LOCAL_INLINE boolean
perf_twint_oh(uint32_t *array, uint32_t *cnt) {
    uint32_t cnt1, cnt2, flag;
    boolean ret = false;

    do {
        flag = get_pmcount(PERF_FLG_PCNTNO);
    }while(flag == 0);

    if (*cnt < PERF_NUM_MEASURE) {
        cnt1 = get_pmcount(PERF_ALL_PCNTNO);
        cnt2 = get_pmcount(PERF_NINT_PCNTNO);
        array[(*cnt)++] = cnt1 - cnt2;
        ret = true;
    }

    /* Restart Counter */
    stop_pmcount(PERF_ALL_PCNTNO);
    stop_pmcount(PERF_NINT_PCNTNO);
    set_pmcount(PERF_ALL_PCNTNO, 0);
    set_pmcount(PERF_NINT_PCNTNO, 0);
    start_pmcount(PERF_ALL_PCNTNO,  PMCTRL_CND_ALL,   PMCTRL_CNT_HOST);
    start_pmcount(PERF_NINT_PCNTNO, PMCTRL_CND_NOINT, PMCTRL_CNT_HOST);

    /* Clear Flag */
    set_pmcount(PERF_FLG_PCNTNO, 0);

    return ret;
}

LOCAL_INLINE void
perf_start_allvm_pcnt(void) {
    stop_pmcount(PERF_ALLVM_PCNTNO);
    set_pmcount(PERF_ALLVM_PCNTNO, 0);
    start_pmcount(PERF_ALLVM_PCNTNO, PMCTRL_CND_ALL, PMCTRL_CNT_ALL);
}

LOCAL_INLINE boolean
perf_end_allvm_pcnt(uint32_t *array, uint32_t *cnt) {
    boolean ret = false;

    if (get_pmcount(PERF_ALLVM_PCNTNO) == 0xffffffff) {
        return false;
    }

    if (*cnt < PERF_NUM_MEASURE) {
        array[(*cnt)++] = get_pmcount(PERF_ALLVM_PCNTNO);
        ret = true;
    }

    return ret;
}

LOCAL_INLINE uint
perf_end_allvm_pcnt_val(void) {
    return get_pmcount(PERF_ALLVM_PCNTNO);
}

#endif /* _PERF_H_ */
