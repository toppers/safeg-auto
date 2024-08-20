#include "hv_impl.h"
#include "hv_cfg.h"
#include "vmcom.h"
#include <string.h>

#ifdef SUPPORT_VMCOM

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
 *  オブジェクト状態エラーのチェック（E_OBJ）
 */
#define CHECK_OBJ(exp) do {    \
    if (!(exp)) {              \
        ercd = E_OBJ;          \
        goto error_exit;       \
    }                          \
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

/*
 *  オブジェクトアクセス権のチェック（E_OACV）
 */
#define CHECK_ACCESS(p_my_ccb, id) do {             \
    if (!(!p_my_ccb->runhvc                         \
          || (VMID(p_my_ccb->p_runvm) == id))) {    \
        ercd = E_OACV;                              \
        goto error_exit;                            \
    }                                               \
} while (false)

#define CHECK_ACCESSPTN(p_my_ccb, idbit) do {                \
    if (!((1 << (VMID(p_my_ccb->p_runvm)-1)) & idbit)) {     \
        ercd = E_OACV;                                       \
        goto error_exit;                                     \
    }                                                        \
} while (false)

/*
 *  メモリアクセス権のチェック（E_MACV）
 */
#define CHECK_MACV_BUF_WRITE(p_my_ccb, base, size) do {         \
    if (!(probe_mem_write(p_my_ccb->p_runvm, base, size))) {    \
        ercd = E_MACV;                                          \
        goto error_exit;                                        \
    }                                                           \
} while (false)

#define CHECK_MACV_BUF_READ(p_myccb, base, size) do {          \
    if (!(probe_mem_read(p_my_ccb->p_runvm, base, size))) {    \
        ercd = E_MACV;                                         \
        goto error_exit;                                       \
    }                                                          \
} while (false)

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

#ifdef SUPPORT_SVAR
LOCAL_INLINE void
start_mpucheck(const void *base, uint size) {
    /* ベースアドレスのセット */
    set_mca((uint32)base);
    /* サイズのセット */
    set_mcs((uint32)size);
    /* チェックの開始 */
    set_mcc(0U);
    /* WG/RGを設定しているのでSPIDは使用しない */
}

LOCAL_INLINE boolean
probe_mem_read(VMCB *p_vmcb, const void *base, uint size)
{
    /* チェックのスタート */
    start_mpucheck(base, size);
    /*
     *  HSREはホスト管理エントリに対してホストモードのSVモードでのアクセス
     *  権をチェックするため，常に1となる．
     *  ホスト管理エントリでは，SWとUWを同じ設定としているため，ユーザーモ
     *  ードへのアクセス権（HURE）により判定する．
     */
    return((get_mcr() & MCR_HURE_MASK) == MCR_HURE_MASK);
}

LOCAL_INLINE boolean
probe_mem_write(VMCB *p_vmcb, const void *base, uint size)
{
    /* チェックのスタート */
    start_mpucheck(base, size);
    /*
     *  HUWEでチェックする理由はprobe_mem_read()と同じ
     */
    return((get_mcr() & MCR_HUWE_MASK) == MCR_HUWE_MASK);
}
#endif /* SUPPORT_SVAR */

/*
 *  前方参照
 */
extern void initialize_statevariable(void);
extern void initialize_messagequeue(void);
extern void initialize_sharedbuffer(void);

/*
 *  初期化関数
 */
void
vmcom_init(void)
{
#ifdef SUPPORT_SVAR
    initialize_statevariable();
#endif /* SUPPORT_SVAR */
#ifdef SUPPORT_MSGQ
    initialize_messagequeue();
#endif /* SUPPORT_MSGQ */    
#ifdef SUPPORT_SBUF
    initialize_sharedbuffer();
#endif /* SUPPORT_SBUF */
}

#endif /* SUPPORT_VMCOM */

#ifdef SUPPORT_SVAR
/*
 *  状態変数関連
 */

/*
 *  状態変数の数
 */
#define tnum_svar ((uint)(tmax_svarid - TMIN_SVARID + 1U))

/*
 *  状態変数IDから状態変数管理ブロックを取り出すためのマクロ
 */
#define INDEX_SVAR(svarid) ((uint)((svarid) - TMIN_SVARID))
#define get_svarcb(svarid) (p_svarcb_table[INDEX_SVAR(svarid)])

/*
 *  状態変数初期化処理
 */
void
initialize_statevariable(void)
{
    uint    i;
    SVARCB  *p_svarcb;

    for (i = 0; i < tnum_svar; i++) {
        p_svarcb = p_svarcb_table[i];
        p_svarcb->p_svarinib = &(svarinib_table[i]);
        p_svarcb->isactive = p_svarcb->p_svarinib->istate;
        p_svarcb->lock = 0U;
    }
}

/*
 *  状態変への書き込み
 */
int
WriteStateVariable(uint SvarID, const void *Variable)
{
    SVARCB  *p_svarcb;
    int ercd = E_OK;
    CCB *p_my_ccb = get_my_ccb();

    CHECK_ID(VALID_SVARID(SvarID));
    p_svarcb = get_svarcb(SvarID);
    CHECK_ACCESS(p_my_ccb, p_svarcb->p_svarinib->writable_vm);
    CHECK_MACV_BUF_READ(p_my_ccb, Variable, p_svarcb->p_svarinib->size);

    vmcom_acquire_lock(&(p_svarcb->lock));
    MEMORY_CHANGED

    memcpy(p_svarcb->p_svarinib->pbuf, Variable, p_svarcb->p_svarinib->size);
    p_svarcb->isactive = true;

    vmcom_release_lock(&(p_svarcb->lock));

  error_exit:
    return(ercd);
}

/*
 *  状態変数からの読込み
 */
int
ReadStateVariable(uint SvarID, void *Variable) {
    SVARCB  *p_svarcb;
    int ercd = E_OK;
    CCB *p_my_ccb = get_my_ccb();

    CHECK_ID(VALID_SVARID(SvarID));
    p_svarcb = get_svarcb(SvarID);
    CHECK_MACV_BUF_WRITE(p_my_ccb, Variable, p_svarcb->p_svarinib->size);

    vmcom_acquire_lock(&(p_svarcb->lock));
    MEMORY_CHANGED

    if (p_svarcb->isactive == false) {
        ercd = E_OBJ;
    }
    else {
        memcpy(Variable, p_svarcb->p_svarinib->pbuf, p_svarcb->p_svarinib->size);
    }

    vmcom_release_lock(&(p_svarcb->lock));

  error_exit:
    return(ercd);
}

/*
 *  状態変数の状態をディアクティブに変更
 */
int
DeactivateStateVariable(uint SvarID) {
    SVARCB  *p_svarcb;
    int ercd = E_OK;
    CCB *p_my_ccb = get_my_ccb();

    CHECK_ID(VALID_SVARID(SvarID));
    p_svarcb = get_svarcb(SvarID);
    CHECK_ACCESS(p_my_ccb, p_svarcb->p_svarinib->writable_vm);

    p_svarcb->isactive = false;

  error_exit:
    return(ercd);
}
#endif /* SUPPORT_SVAR */

#ifdef SUPPORT_MSGQ
/*
 *  メッセージキュー関連
 */

/*
 *  状態変数の数
 */
#define tnum_msgq ((uint)(tmax_msgqid - TMIN_MSGQID + 1))

/*
 *  メッセージキューIDからメッセージキュー管理ブロックを取り出すためのマクロ
 */
#define INDEX_MSGQ(msgqid) ((uint)((msgqid) - TMIN_MSGQID))
#define get_msgqcb(msgqid) (p_msgqcb_table[INDEX_MSGQ(msgqid)])

/*
 *  メッセージキュー初期化処理
 */
void
initialize_messagequeue(void)
{
    uint    i;
    MSGQCB  *p_msgqcb;

    for (i = 0; i < tnum_msgq; i++) {
        p_msgqcb = p_msgqcb_table[i];
        p_msgqcb->p_msgqinib = &(msgqinib_table[i]);
        p_msgqcb->isactive = p_msgqcb->p_msgqinib->istate;
        p_msgqcb->lock = 0U;
        p_msgqcb->fmsgqsz = p_msgqcb->p_msgqinib->msgqsz;
        p_msgqcb->head = 0U;
        p_msgqcb->tail = 0U;
        p_msgqcb->smsgqcnt = 0U;
    }
}

/*
 *  メッセージキュー管理領域へのメッセージの格納
 */
static boolean
enqueue_message(MSGQCB *p_msgqcb, const void *msg, uint msgsz)
{
    const MSGQINIB  *p_msgqinib = p_msgqcb->p_msgqinib;
    char    *mbuffer = (char *)(p_msgqinib->msgqmb);
    uint    allocsz;
    size_t  remsz;

    /* バッファの確保サイズを決める */
    allocsz = sizeof(uint) + VMCOM_ROUND_SZ(msgsz, sizeof(uint));
    /* バッファに余裕がある場合は格納 */
    if (allocsz <= p_msgqcb->fmsgqsz) {
        /* 先頭にサイズを格納 */
        *((uint *) &(mbuffer[p_msgqcb->tail])) = msgsz;
        /* 格納先を1個進める */
        p_msgqcb->tail += sizeof(uint);
        /* tailの循環処理 */
        if (p_msgqcb->tail >= p_msgqinib->msgqsz) {
            p_msgqcb->tail = 0U;
        }
        /* バッファの残りサイズを計算 */
        remsz = p_msgqinib->msgqsz - p_msgqcb->tail;
        /* メッセージサイズより小さい場合 */
        if (remsz < msgsz) {
            /* 末端までコピー */
            memcpy(&(mbuffer[p_msgqcb->tail]), msg, remsz);
            msg = ((char *) msg) + remsz;
            msgsz -= remsz;
            p_msgqcb->tail = 0U;
        }
        /* tailの位置からバッファに挿入してtailを更新 */
        memcpy(&(mbuffer[p_msgqcb->tail]), msg, msgsz);
        p_msgqcb->tail += VMCOM_ROUND_SZ(msgsz, sizeof(uint));
        /* tailの循環処理 */
        if (p_msgqcb->tail >= p_msgqinib->msgqsz) {
            p_msgqcb->tail = 0U;
        }
        /* バッファの残りサイズ更新 */
        p_msgqcb->fmsgqsz -= allocsz;
        /* 格納データ数更新 */
        p_msgqcb->smsgqcnt++;
        return(true);
    }
    return(false);
}

/*
 *  メッセージキュー管理領域からのメッセージの取出し
 */
static uint
dequeue_message(MSGQCB *p_msgqcb, void *msg)
{
    const MSGQINIB  *p_msgqinib = p_msgqcb->p_msgqinib;
    char    *mbuffer = (char *)(p_msgqinib->msgqmb);
    uint    msgsz, copysz;
    size_t  remsz;

    msgsz = *((uint *) &(mbuffer[p_msgqcb->head]));
    /* headの循環処理 */
    p_msgqcb->head += sizeof(uint);
    if (p_msgqcb->head >= p_msgqinib->msgqsz) {
        p_msgqcb->head = 0U;
    }
    /* ヘッドからバッファの末尾までのサイズを取得 */
    remsz = p_msgqinib->msgqsz - p_msgqcb->head;
    copysz = msgsz;
    /* データがバッファで循環している場合 */
    if (remsz < copysz) {
        /*  バッファの末端まで読み込み */
        memcpy(msg, &(mbuffer[p_msgqcb->head]), remsz);
        msg = ((char *) msg) + remsz;
        copysz -= remsz;
        p_msgqcb->head = 0U;
    }
    /* バッファの残りサイズ分読込み */
    memcpy(msg, &(mbuffer[p_msgqcb->head]), copysz);
    p_msgqcb->head += VMCOM_ROUND_SZ(copysz, sizeof(uint));
    /* headの循環処理 */
    if (p_msgqcb->head >= p_msgqinib->msgqsz) {
        p_msgqcb->head = 0U;
    }
    p_msgqcb->fmsgqsz += sizeof(uint) + VMCOM_ROUND_SZ(msgsz, sizeof(uint));
    p_msgqcb->smsgqcnt--;
    return(msgsz);
}

int
WriteMessageQueue(uint MsgqID, const void *Message, uint MsgSize)
{
    MSGQCB  *p_msgqcb;
    int  ercd;
    CCB  *p_my_ccb = get_my_ccb();

    CHECK_ID(VALID_MSGQID(MsgqID));
    p_msgqcb = get_msgqcb(MsgqID);
    CHECK_ACCESS(p_my_ccb, p_msgqcb->p_msgqinib->writable_vm);
    CHECK_MACV_BUF_READ(p_my_ccb, Message, MsgSize);
    CHECK_PAR(MsgSize <= p_msgqcb->p_msgqinib->maxmsz);

    vmcom_acquire_lock(&(p_msgqcb->lock));
    MEMORY_CHANGED

    if (enqueue_message(p_msgqcb, Message, MsgSize)) {
        ercd = E_OK;
    }
    else {
        ercd = E_BUF;
    }
    p_msgqcb->isactive = true;

    vmcom_release_lock(&(p_msgqcb->lock));

  error_exit:
    return(ercd);
}

int
ReadMessageQueue(uint MsgqID, void *Message)
{
    MSGQCB  *p_msgqcb;
    int  ercd = E_OK;
    CCB  *p_my_ccb = get_my_ccb();

    CHECK_ID(VALID_MSGQID(MsgqID));
    p_msgqcb = get_msgqcb(MsgqID);
    CHECK_ACCESS(p_my_ccb, p_msgqcb->p_msgqinib->readable_vm);
    CHECK_MACV_BUF_WRITE(p_my_ccb, Message, p_msgqcb->p_msgqinib->maxmsz);

    vmcom_acquire_lock(&(p_msgqcb->lock));
    MEMORY_CHANGED

    if (p_msgqcb->isactive == false) {
        ercd = E_OBJ;
    }
    else if (p_msgqcb->smsgqcnt > 0) {
        ercd = dequeue_message(p_msgqcb, Message);
    }
    else {
        ercd = E_BUF;
    }

    vmcom_release_lock(&(p_msgqcb->lock));

  error_exit:
    return(ercd);
}

/*
 *  メッセージキューの状態をディアクティブに変更
 */
int
DeactivateMessageQueue(uint MsgqID)
{
    MSGQCB  *p_msgqcb;
    int  ercd = E_OK;
    CCB  *p_my_ccb = get_my_ccb();

    CHECK_ID(VALID_MSGQID(MsgqID));
    p_msgqcb = get_msgqcb(MsgqID);
    CHECK_ACCESS(p_my_ccb, p_msgqcb->p_msgqinib->writable_vm);

    vmcom_acquire_lock(&(p_msgqcb->lock));
    MEMORY_CHANGED

    p_msgqcb->isactive = false;
    p_msgqcb->fmsgqsz = p_msgqcb->p_msgqinib->msgqsz;
    p_msgqcb->head = 0U;
    p_msgqcb->tail = 0U;
    p_msgqcb->smsgqcnt = 0U;

    vmcom_release_lock(&(p_msgqcb->lock));

  error_exit:
    return(ercd);
}
#endif /* SUPPORT_MSGQ */

#ifdef SUPPORT_SBUF

/*
 *  共有バッファ関連
 */

/*
 *  共有バッファの数
 */
#define tnum_sbuf ((uint)(tmax_sbufid - TMIN_SBUFID + 1))

/*
 *  共有バッファIDから共有バッファ管理ブロックを取り出すためのマクロ
 */
#define INDEX_SBUF(sbufid) ((uint)((sbufid) - TMIN_SBUFID))
#define get_sbufcb(sbufid) (p_sbufcb_table[INDEX_SBUF(sbufid)])

/*
 *  共有バッファ初期化処理
 */
void
initialize_sharedbuffer(void)
{
    uint  i;
    SBUFCB  *p_sbufcb;

    for (i = 0; i < tnum_sbuf; i++) {
        p_sbufcb = p_sbufcb_table[i];
        p_sbufcb->p_sbufinib = &(sbufinib_table[i]);
        p_sbufcb->acquirevmid = 0U;
        p_sbufcb->lock = 0U;
        p_sbufcb->mpidx = 0U;
    }
}

/*
 *  バッファの取得
 */
int
AcquireSharedBuffer(uint SBufID, void **BufPtr)
{
    SBUFCB  *p_sbufcb;
    int  ercd = E_OK;
    CCB  *p_my_ccb = get_my_ccb();
    int  mpidx;

    CHECK_ID(VALID_SBUFID(SBufID));
    p_sbufcb = get_sbufcb(SBufID);
    CHECK_ACCESSPTN(p_my_ccb, p_sbufcb->p_sbufinib->accessvm);

    vmcom_acquire_lock(&(p_sbufcb->lock));
    MEMORY_CHANGED

    if (p_sbufcb->acquirevmid != 0) {
        ercd = E_OBJ;
    }
    else {
        *BufPtr = (void*)p_sbufcb->p_sbufinib->bufmpla;
        p_sbufcb->acquirevmid = VMID(p_my_ccb->p_runvm);
        /* MPUの空きエントリを探す */
        mpidx = __SCH1R(p_my_ccb->p_runvm->sbuf_fmpubit) - 1;
        set_mpidx(mpidx);
        p_my_ccb->p_runvm->sbuf_fmpubit &= ~(1U << mpidx);
        set_mpla((uint32)p_sbufcb->p_sbufinib->bufmpla);
        set_mpua((uint32)p_sbufcb->p_sbufinib->bufmpua);
        set_mpat(MPU_E_MASK|MPU_RAM|MPU_WG_MASK|MPU_RG_MASK);
        p_sbufcb->mpidx = mpidx;
    }
    vmcom_release_lock(&(p_sbufcb->lock));

  error_exit:
    return(ercd);
}

/*
 *  バッファの解放
 */
int
ReleaseSharedBuffer(uint SBufID)
{
    SBUFCB  *p_sbufcb;
    int  ercd = E_OK;
    CCB  *p_my_ccb = get_my_ccb();

    CHECK_ID(VALID_SBUFID(SBufID));
    p_sbufcb = get_sbufcb(SBufID);
    CHECK_ACCESSPTN(p_my_ccb, p_sbufcb->p_sbufinib->accessvm);

    vmcom_acquire_lock(&(p_sbufcb->lock));
    MEMORY_CHANGED
    if (p_sbufcb->acquirevmid != VMID(p_my_ccb->p_runvm)) {
        ercd = E_OBJ;
    }
    else {
        p_sbufcb->acquirevmid = 0;
        set_mpidx(p_sbufcb->mpidx);
        set_mpat(0);  /* Disable */
        p_my_ccb->p_runvm->sbuf_fmpubit |= 1U << p_sbufcb->mpidx;
    }
    vmcom_release_lock(&(p_sbufcb->lock));

  error_exit:
    return(ercd);
}

#endif /* SUPPORT_SBUF */
