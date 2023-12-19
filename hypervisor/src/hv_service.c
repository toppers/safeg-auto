/*
 *  ハイパーバイザーサービス
 */
#include "hv_impl.h"

/*
 *  コアIDの取得
 */
ER
GetCoreID(ID* pCoreID)
{
    *pCoreID = get_my_coreid();

    return E_OK;
}

/*
 *  HVタイムウィンドウの残り時間の取得
 */
ER
GetHVTWTimeLeft(uint32* pTimeLeft)
{
    ER  ercd;
    CCB *p_my_ccb = get_my_ccb();

    if (!(p_my_ccb->runhvc) && !(p_my_ccb->runhvint)) {
        *pTimeLeft = (tptmu_get_cmp(0) -  tptmu_getcnt(TWDTIM_TPTMU_NO))/TPTM_CLK_MHZ;
        ercd = E_OK;
    }
    else {
        ercd = E_CTX;
    }

    return ercd;
}

/*
 *  VMのリセット
 */
ER
ResetVM(ID vmid, uint32 arg0, uint32 arg1, uint32 arg2, uint32 arg3)
{
    VMCB *p_vmcb;
    CCB  *p_my_ccb = get_my_ccb();
    
    /* vmidのエラーチェック */
    if (!VALID_VMID(vmid)) {
        return E_ID;
    }
    /* コンテキストのチェック */
    if (p_my_ccb->runhvc) {
        return E_CTX;
    }
    
    p_vmcb = get_vmcb(vmid);

    p_vmcb->d_sysreg[GMPSW_NO] = GMPSW_INIT;
    p_vmcb->pc = p_vmcb->p_vminib->rbase;
    p_vmcb->reg[6] = arg0;
    p_vmcb->reg[7] = arg1;
    p_vmcb->reg[8] = arg2;
    p_vmcb->reg[9] = arg2;

    return E_OK;
}

/*
 *  VMへのフェイクFE例外の発生
 */
ER
RaiseVMFakeFE(ID vmid, uint32 voffset, uint32 cause)
{
    VMCB *p_vmcb;
    CCB  *p_my_ccb = get_my_ccb();
    
    /* vmidのエラーチェック */
    if (!VALID_VMID(vmid)) {
        return E_ID;
    }
    /* コンテキストのチェック */
    if (p_my_ccb->runhvc) {
        return E_CTX;
    }
    
    p_vmcb = get_vmcb(vmid);

    p_vmcb->d_sysreg[GMFEPSW_NO] = p_vmcb->d_sysreg[GMPSW_NO];
    p_vmcb->d_sysreg[GMFEPC_NO] = p_vmcb->pc;
    p_vmcb->pc = p_vmcb->p_vminib->rbase + voffset;
    p_vmcb->d_sysreg[GMPSW_NO] = p_vmcb->d_sysreg[GMPSW_NO] | PSW_NP_MASK;
    p_vmcb->d_sysreg[GMFEIC_NO] = cause;

    return E_OK;
}

/*
 *  VMへのフェイクFE例外の発生
 */
ER
RaiseVMFakeEI(ID vmid, uint32 voffset, uint32 cause)
{
    VMCB *p_vmcb;
    CCB  *p_my_ccb = get_my_ccb();
    
    /* vmidのエラーチェック */
    if (!VALID_VMID(vmid)) {
        return E_ID;
    }
    /* コンテキストのチェック */
    if (p_my_ccb->runhvc) {
        return E_CTX;
    }

    p_vmcb = get_vmcb(vmid);

    p_vmcb->d_sysreg[GMEIPSW_NO] = p_vmcb->d_sysreg[GMPSW_NO];
    p_vmcb->d_sysreg[GMEIPC_NO] = p_vmcb->pc;
    p_vmcb->pc = p_vmcb->p_vminib->rbase + voffset;
    p_vmcb->d_sysreg[GMPSW_NO] = p_vmcb->d_sysreg[GMPSW_NO] | PSW_ID_MASK;
    p_vmcb->d_sysreg[GMEIIC_NO] = cause;

    return E_OK;
}

/*
 *  IDLE-VMの呼び出し
 */
ER
CallIdleVM(ID vmid) {
    CCB  *p_my_ccb = get_my_ccb();
    VMCB *p_vmcb;

    /* vmidのエラーチェック */
    if (!VALID_VMID(vmid)) {
        return E_ID;
    }

    p_vmcb = get_vmcb(vmid);

    /* 対象のVMが同じコアに割り当てられているかチェック */
    if (p_vmcb->p_vminib->coreid != get_my_coreid()) {
        return E_OBJ;
    }

    /* コンテキストのチェック */
    if (!(p_my_ccb->runidle) && p_my_ccb->runhvint) {
        return E_CTX;
    }

    /* 切り替え処理の呼び出し */
    cal_trap0_1(vmid);

    return E_OK;
}

/*
 *  システム動作モードの設定
 */
ER
ChangeSystemOperationMode(ID somid) {
    CCB  *p_my_ccb = get_my_ccb();
    const SOMINIB	*p_sominib;

    /* somidのエラーチェック */
    if (!VALID_SOMID(somid)) {
        return E_ID;
    }
    p_sominib = get_sominib(somid);

    /* 1変数への書き込みのみなのでコア間の排他制御は行わない */
    p_global_nxtsom = p_sominib;

    return E_OK;
}

/*
 *  システム動作モードの参照
 */
ER
GetSystemOperationMode(ID *p_somid) {
    /* 1変数からの読み込みなのでコア間の排他制御は行わない */
    *p_somid = SOMID(p_global_cursom);
    return E_OK;
}
