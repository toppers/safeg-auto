#include "hv.h"
#include "hv_cfg.h"
#include "ivccom.h"
#include "ivccom_def.h"
#include <string.h>
#include <v800_ghs.h>
#include <ghs_barrier.h>

/*
 *  IVC状態変数の数
 */
#define tnum_ivcsvar ((uint)(tmax_ivcsvarid - TMIN_IVCSVARID + 1))

/*
 *  IVC状態変数初期化処理
 */
void
initialize_ivcstatevariable(void)
{
    uint i;
    const IVCSVARINIB *p_ivcsvarinib;

    for (i = 0; i < tmax_ivcsvarid; i++) {
        p_ivcsvarinib = &ivcsvarinib_table[i];
        p_ivcsvarinib->p_ivcsvarcb->isactive = ivcsvarinib_table[i].istate;
        p_ivcsvarinib->p_ivcsvarcb->lock = 0;
    }
}

/*
 *  IVCメッセージキューの数
 */
#define tnum_ivcmsgq ((uint)(tmax_ivcmsgqid - TMIN_IVCMSGQID + 1))

/*
 *  IVCメッセージキュー初期化
 */
void
initialize_ivcmessagequeue(void)
{
    uint    i;
    const IVCMSGQINIB    *p_ivcmsgqinib;

    for (i = 0; i < tnum_ivcmsgq; i++) {
        p_ivcmsgqinib = &ivcmsgqinib_table[i];
        p_ivcmsgqinib->p_ivcmsgqcb->isactive = p_ivcmsgqinib->istate;
        p_ivcmsgqinib->p_ivcmsgqcb->fmsgqsz = p_ivcmsgqinib->msgqsz;
        p_ivcmsgqinib->p_ivcmsgqcb->head = 0U;
        p_ivcmsgqinib->p_ivcmsgqcb->tail = 0U;
        p_ivcmsgqinib->p_ivcmsgqcb->smsgqcnt = 0U;
        p_ivcmsgqinib->p_ivcmsgqcb->lock = 0U;
    }
}
