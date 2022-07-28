#include "ivccom_guest.h"
#include "u2a.h"
#include <string.h>

/*
 *  IVC通信機能VM用ファイル
 */

/*
 *  チエックマクロ
 */

/*
 *  不正ID番号のチェック（E_ID）
 */
#define CHECK_ID(exp) do {    \
    if (!(exp)) {             \
        ercd = E_ID;          \
        goto error_exit;      \
    }                         \
} while (false)

/*
 *  パラメータエラーのチェック（E_PAR）
 */
#define CHECK_PAR(exp) do {    \
    if (!(exp)) {              \
        ercd = E_PAR;          \
        goto error_exit;       \
    }                          \
} while (false)


#ifdef USE_SPINLOCK
LOCAL_INLINE void
vmcom_acquire_lock(uint32 *p_lock)
{
    while (1) {
        if (rh850_acquire_lock(p_lock)) {
            return;
        }
    };
}

LOCAL_INLINE void
vmcom_release_lock(uint32 *p_lock)
{
    *p_lock = 0;
    /* スピンロック取得解放のタイトループで同期しないように一定時間待つ */
    rh850_snooze();
}
#endif /* USE_SPINLOCK */

/*
 *  排他制御関連
 */
LOCAL_INLINE uint
ivccom_disable_int(void)
{
    uint psw = get_psw();
    set_psw(psw | PSW_ID_MASK);

    return(psw);
}

LOCAL_INLINE void
ivccom_enable_int(uint psw)
{
    set_psw(psw);
}

/*
 *  IVC状態変数関連
 */

/*
 *  IVC状態変数IDからIVC状態変数管理ブロックを取り出すためのマクロ
 */
#define INDEX_IVCSVAR(ivcsvarid)    ((uint)((ivcsvarid) - TMIN_IVCSVARID))
#define get_ivcsvarinib(ivcsvarid)    (&ivcsvarinib_table[INDEX_IVCSVAR(ivcsvarid)])

/*
 *  IVC状態変への書き込み
 */
int
WriteIVCStateVariable(uint IVCSvarID, const void *Variable)
{
    const IVCSVARINIB  *p_ivcsvarinib;
    int ercd = E_OK;
    uint psw;

    CHECK_ID(VALID_IVCSVARID(IVCSvarID));
    p_ivcsvarinib = get_ivcsvarinib(IVCSvarID);

    psw = ivccom_disable_int();
#ifdef USE_SPINLOCK
    vmcom_acquire_lock(&(p_ivcsvarinib->p_ivcsvarcb->lock));
    MEMORY_CHANGED
#else /* USE_SPINLOCK */
    if (!TryLockIVC(p_ivcsvarinib->cramno, p_ivcsvarinib->ivcch)) {
        ercd = E_IVC;
        ivccom_enable_int(psw);
        goto error_exit;
    }
    MEMORY_CHANGED
#endif /* USE_SPINLOCK */

    memcpy(p_ivcsvarinib->pbuf, Variable, p_ivcsvarinib->size);
    p_ivcsvarinib->p_ivcsvarcb->isactive = true;

#ifdef USE_SPINLOCK
    vmcom_release_lock(&(p_ivcsvarinib->p_ivcsvarcb->lock));
#else /* USE_SPINLOCK */
    UnLockIVC(p_ivcsvarinib->cramno, p_ivcsvarinib->ivcch);
#endif /* USE_SPINLOCK */
    ivccom_enable_int(psw);
  error_exit:
    return(ercd);
}

/*
 *  状態変数からの読込み
 */
int
ReadIVCStateVariable(uint IVCSvarID, void *Variable) {
    const IVCSVARINIB  *p_ivcsvarinib;
    int ercd = E_OK;
    uint psw;

    CHECK_ID(VALID_IVCSVARID(IVCSvarID));
    p_ivcsvarinib = get_ivcsvarinib(IVCSvarID);

    psw = ivccom_disable_int();
    if (!TryLockIVC(p_ivcsvarinib->cramno, p_ivcsvarinib->ivcch)) {
        ercd = E_IVC;
        ivccom_enable_int(psw);
        goto error_exit;
    }

    if (p_ivcsvarinib->p_ivcsvarcb->isactive == false) {
        ercd = E_OBJ;
    }
    else {
        memcpy(Variable, p_ivcsvarinib->pbuf, p_ivcsvarinib->size);
    }

    UnLockIVC(p_ivcsvarinib->cramno, p_ivcsvarinib->ivcch);
    ivccom_enable_int(psw);

  error_exit:
    return(ercd);
}

/*
 *  状態変数の状態をディアクティブに変更
 */
int
DeactivateIVCStateVariable(uint IVCSvarID) {
    const IVCSVARINIB  *p_ivcsvarinib;
    int ercd = E_OK;
    uint psw;

    CHECK_ID(VALID_IVCSVARID(IVCSvarID));
    p_ivcsvarinib = get_ivcsvarinib(IVCSvarID);

    psw = ivccom_disable_int();
    if (!TryLockIVC(p_ivcsvarinib->cramno, p_ivcsvarinib->ivcch)) {
        ercd = E_IVC;
        ivccom_enable_int(psw);
        goto error_exit;
    }
    p_ivcsvarinib->p_ivcsvarcb->isactive = false;
    UnLockIVC(p_ivcsvarinib->cramno, p_ivcsvarinib->ivcch);
    ivccom_enable_int(psw);

  error_exit:
    return(ercd);
}

/*
 *  IVCメッセージキュー関連
 */

/*
 *  IVCメッセージキューの数
 */
#define tnum_ivcmsgq ((uint)(tmax_ivcmsgqid - TMIN_IVCMSGQID + 1))

/*
 *  メッセージキューIDからメッセージキュー管理ブロックを取り出すためのマクロ
 */
#define INDEX_IVCMSGQ(ivcmsgqid) ((uint)((ivcmsgqid) - TMIN_IVCMSGQID))
#define get_ivcmsgqinib(ivcmsgqid) (&ivcmsgqinib_table[INDEX_IVCMSGQ(ivcmsgqid)])

/*
 *  メッセージキュー管理領域へのメッセージの格納
 */
static boolean
enqueue_message(const IVCMSGQINIB *p_ivcmsgqinib, const void *msg, uint msgsz)
{
    IVCMSGQCB *p_ivcmsgqcb = p_ivcmsgqinib->p_ivcmsgqcb;
    char    *mbuffer = (char *)(p_ivcmsgqinib->msgqmb);
    uint    allocsz;
    size_t  remsz;

    /* バッファの確保サイズを決める */
    allocsz = sizeof(uint) + IVCCOM_ROUND_SZ(msgsz, sizeof(uint));
    /* バッファに余裕がある場合は格納 */
    if (allocsz <= p_ivcmsgqcb->fmsgqsz) {
        /* 先頭にサイズを格納 */
        *((uint *) &(mbuffer[p_ivcmsgqcb->tail])) = msgsz;
        /* 格納先を1個進める */
        p_ivcmsgqcb->tail += sizeof(uint);
        /* tailの循環処理 */
        if (p_ivcmsgqcb->tail >= p_ivcmsgqinib->msgqsz) {
            p_ivcmsgqcb->tail = 0U;
        }
        /* バッファの残りサイズを計算 */
        remsz = p_ivcmsgqinib->msgqsz - p_ivcmsgqcb->tail;
        /* メッセージサイズより小さい場合 */
        if (remsz < msgsz) {
            /* 末端までコピー */
            memcpy(&(mbuffer[p_ivcmsgqcb->tail]), msg, remsz);
            msg = ((char *) msg) + remsz;
            msgsz -= remsz;
            p_ivcmsgqcb->tail = 0U;
        }
        /* tailの位置からバッファに挿入してtailを更新 */
        memcpy(&(mbuffer[p_ivcmsgqcb->tail]), msg, msgsz);
        p_ivcmsgqcb->tail += IVCCOM_ROUND_SZ(msgsz, sizeof(uint));
        /* tailの循環処理 */
        if (p_ivcmsgqcb->tail >= p_ivcmsgqinib->msgqsz) {
            p_ivcmsgqcb->tail = 0U;
        }
        /* バッファの残りサイズ更新 */
        p_ivcmsgqcb->fmsgqsz -= allocsz;
        /* 格納データ数更新 */
        p_ivcmsgqcb->smsgqcnt++;
        return(true);
    }
    return(false);
}

/*
 *  メッセージキュー管理領域からのメッセージの取出し
 */
static uint
dequeue_message(const IVCMSGQINIB *p_ivcmsgqinib, void *msg)
{
    IVCMSGQCB *p_ivcmsgqcb = p_ivcmsgqinib->p_ivcmsgqcb;
    char    *mbuffer = (char *)(p_ivcmsgqinib->msgqmb);
    uint    msgsz, copysz;
    size_t  remsz;

    msgsz = *((uint *) &(mbuffer[p_ivcmsgqcb->head]));
    /* headの循環処理 */
    p_ivcmsgqcb->head += sizeof(uint);
    if (p_ivcmsgqcb->head >= p_ivcmsgqinib->msgqsz) {
        p_ivcmsgqcb->head = 0U;
    }
    /* ヘッドからバッファの末尾までのサイズを取得 */
    remsz = p_ivcmsgqinib->msgqsz - p_ivcmsgqcb->head;
    copysz = msgsz;
    /* データがバッファで循環している場合 */
    if (remsz < copysz) {
        /*  バッファの末端まで読み込み */
        memcpy(msg, &(mbuffer[p_ivcmsgqcb->head]), remsz);
        msg = ((char *) msg) + remsz;
        copysz -= remsz;
        p_ivcmsgqcb->head = 0U;
    }
    /* バッファの残りサイズ分読込み */
    memcpy(msg, &(mbuffer[p_ivcmsgqcb->head]), copysz);
    p_ivcmsgqcb->head += IVCCOM_ROUND_SZ(copysz, sizeof(uint));
    /* headの循環処理 */
    if (p_ivcmsgqcb->head >= p_ivcmsgqinib->msgqsz) {
        p_ivcmsgqcb->head = 0U;
    }
    p_ivcmsgqcb->fmsgqsz += sizeof(uint) + IVCCOM_ROUND_SZ(msgsz, sizeof(uint));
    p_ivcmsgqcb->smsgqcnt--;
    return(msgsz);
}

int
WriteIVCMessageQueue(uint IVCMsgqID, const void *Message, uint MsgSize)
{
    const IVCMSGQINIB    *p_ivcmsgqinib;
    int ercd;
    uint psw;

    CHECK_ID(VALID_IVCMSGQID(IVCMsgqID));
    p_ivcmsgqinib = get_ivcmsgqinib(IVCMsgqID);
    CHECK_PAR(MsgSize <= p_ivcmsgqinib->maxmsz);

    psw = ivccom_disable_int();
    if (!TryLockIVC(p_ivcmsgqinib->cramno, p_ivcmsgqinib->ivcch)) {
        ercd = E_IVC;
        ivccom_enable_int(psw);
        goto error_exit;
    }

    if (enqueue_message(p_ivcmsgqinib, Message, MsgSize)) {
        ercd = E_OK;
    }
    else {
        ercd = E_BUF;
    }

    p_ivcmsgqinib->p_ivcmsgqcb->isactive = true;
    UnLockIVC(p_ivcmsgqinib->cramno, p_ivcmsgqinib->ivcch);
    ivccom_enable_int(psw);

  error_exit:
    return(ercd);
}

int
ReadIVCMessageQueue(uint IVCMsgqID, void *Message)
{
    const IVCMSGQINIB    *p_ivcmsgqinib;
    int ercd = E_OK;
    uint psw;

    CHECK_ID(VALID_IVCMSGQID(IVCMsgqID));
    p_ivcmsgqinib = get_ivcmsgqinib(IVCMsgqID);

    psw = ivccom_disable_int();
    if (!TryLockIVC(p_ivcmsgqinib->cramno, p_ivcmsgqinib->ivcch)) {
        ercd = E_IVC;
        ivccom_enable_int(psw);
        goto error_exit;
    }

    if (p_ivcmsgqinib->p_ivcmsgqcb->isactive == false) {
        ercd = E_OBJ;
    }
    else if (p_ivcmsgqinib->p_ivcmsgqcb->smsgqcnt > 0) {
        ercd = dequeue_message(p_ivcmsgqinib, Message);
    }
    else {
        ercd = E_BUF;
    }

    UnLockIVC(p_ivcmsgqinib->cramno, p_ivcmsgqinib->ivcch);
    ivccom_enable_int(psw);

  error_exit:
    return(ercd);
}

/*
 *  メッセージキューの状態をディアクティブに変更
 */
int
DeactivateIVCMessageQueue(uint IVCMsgqID)
{
    const IVCMSGQINIB    *p_ivcmsgqinib;
    int ercd = E_OK;
    uint psw;

    CHECK_ID(VALID_IVCMSGQID(IVCMsgqID));
    p_ivcmsgqinib = get_ivcmsgqinib(IVCMsgqID);

    psw = ivccom_disable_int();
    if (!TryLockIVC(p_ivcmsgqinib->cramno, p_ivcmsgqinib->ivcch)) {
        ercd = E_IVC;
        ivccom_enable_int(psw);
        goto error_exit;
    }

    p_ivcmsgqinib->p_ivcmsgqcb->isactive = false;
    p_ivcmsgqinib->p_ivcmsgqcb->fmsgqsz = p_ivcmsgqinib->msgqsz;
    p_ivcmsgqinib->p_ivcmsgqcb->head = 0U;
    p_ivcmsgqinib->p_ivcmsgqcb->tail = 0U;
    p_ivcmsgqinib->p_ivcmsgqcb->smsgqcnt = 0U;

    UnLockIVC(p_ivcmsgqinib->cramno, p_ivcmsgqinib->ivcch);
    ivccom_enable_int(psw);

  error_exit:
    return(ercd);
}

/*
 *  IVCバッファ関連
 */

/*
 *  IVCバッファIDからIVCバッファ管理ブロックを取り出すためのマクロ
 */
#define INDEX_IVCBUF(ivcbufid)    ((uint)((ivcbufid) - TMIN_IVCBUFID))
#define get_ivcbufinib(ivcbufid)    (&ivcbufinib_table[INDEX_IVCBUF(ivcbufid)])

/*
 *  IVCバッファの取得
 */
int
AcquireIVCbuf(uint IVCbufID, void **BufPtr)
{
    const IVCBUFINIB    *p_ivcbufinib;
    int ercd = E_OK;
    uint psw;

    CHECK_ID(VALID_IVCBUFID(IVCbufID));
    p_ivcbufinib = get_ivcbufinib(IVCbufID);

    psw = ivccom_disable_int();
    if (!TryLockIVC(p_ivcbufinib->cramno, p_ivcbufinib->ivcch)) {
        ercd = E_IVC;
        ivccom_enable_int(psw);
        goto error_exit;
    }
    MEMORY_CHANGED

    *BufPtr = p_ivcbufinib->pbuf;

    ivccom_enable_int(psw);
  error_exit:
    return(ercd);
}

/*
 *  IVCバッファの開放
 */
int
ReleaseIVCbuf(uint IVCbufID)
{
    const IVCBUFINIB    *p_ivcbufinib;
    int ercd = E_OK;
    uint psw;

    CHECK_ID(VALID_IVCBUFID(IVCbufID));
    p_ivcbufinib = get_ivcbufinib(IVCbufID);

    psw = ivccom_disable_int();

    /* ロックされかつオーナーであればロックを解放 */
    if (IsLockedIVC(p_ivcbufinib->cramno, p_ivcbufinib->ivcch, get_spid()) ) {
        UnLockIVC(p_ivcbufinib->cramno, p_ivcbufinib->ivcch);
    }
    else {
        ercd = E_IVC;
    }

    ivccom_enable_int(psw);

  error_exit:
    return(ercd);
}
