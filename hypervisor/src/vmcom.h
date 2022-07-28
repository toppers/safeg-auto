#ifndef TOPPERS_VMCOM_H
#define TOPPERS_VMCOM_H

/*
 *  VM間通信機能HV用ヘッダファイル
 */

#ifndef TOPPERS_MACRO_ONLY

/*
 *  VM間通信機能の定義（VMと共通）
 */
#include "vmcom_def.h"

/*
 *  メモリ領域確保のためのマクロ
 */
#define VMCOM_COUNT_SZ(sz, unit) (((sz) + (unit) - 1) / (unit))
#define VMCOM_ROUND_SZ(sz, unit) (((sz) + (unit) - 1) & ~((unit) - 1))

/*
 *  状態変数IDの最小値
 */
#define TMIN_SVARID        1

/*
 *  状態変数初期化ブロック
 */
typedef struct statevariable_initialization_block {
    uint     size;           /* バッファサイズ */
    ID       writable_vm;    /* 書き込み可能VM */
    boolean  istate;         /* 初期状態 */
    void     *pbuf;          /* バッファの先頭アドレス */
} SVARINIB;

/*
 *  状態変数管理ブロック
 */
typedef struct statevariable_control_block {
    boolean  isactive;             /* 状態 */
    const SVARINIB *p_svarinib;    /* 初期化ブロックへのポインタ */
    uint32   lock;                 /* ロック変数 */
} SVARCB;

/*
 *  状態変数IDの最大値（hv_cfg.c）
 */
extern const ID  tmax_svarid;

/*
 *  状態変数初期化ブロックのエリア（hv_cfg.c）
 */
extern const SVARINIB  svarinib_table[];

/*
 *  状態変数管理ブロックへのポインタテーブル（hv_cfg.c）
 */
extern SVARCB *const  p_svarcb_table[];

/*
 *  状態変数IDの範囲の判定
 */
#define VALID_SVARID(svarid) (TMIN_SVARID <= (svarid) && (svarid) <= tmax_svarid)

/*
 *  メッセージキュー
 */

/*
 *  メッセージキューの最小値
 */
#define TMIN_MSGQID  1

/*
 *  メッセージキュー初期化ブロック
 */
typedef struct messagequeue_initialization_block {
    int     maxmsz;         /* 最大メッセージサイズ */
    uint    msgqsz;         /* メッセージキューバッファ管理領域のサイズ */
    ID      writable_vm;    /* 書き込み可能VM */
    ID      readable_vm;    /* 読み込み可能VM */
    boolean istate;         /* 初期状態 */
    void    *msgqmb;        /* メッセージキューバッファ管理領域の先頭アドレス */
} MSGQINIB;

/*
 *  メッセージキュー管理ブロック
 */
typedef struct messagequeue_control_block {
    boolean  isactive;             /* 状態 */
    const MSGQINIB *p_msgqinib;    /* 初期化ブロックへのポインタ */
    uint32   lock;                 /* ロック変数 */
    uint     fmsgqsz;              /* 空き領域のサイズ */
    uint     head;                 /* 最初のメッセージの格納場所 */
    uint     tail;                 /* 最後のメッセージの格納場所の次 */
    uint     smsgqcnt;             /* 管理領域に格納されているメッセージの数 */
} MSGQCB;

/*
 *  メッセージキューIDの最大値（hv_cfg.c）
 */
extern const ID  tmax_msgqid;

/*
 *  メッセージキュー初期化ブロックのエリア（hv_cfg.c）
 */
extern const MSGQINIB  msgqinib_table[];

/*
 *  メッセージキュー管理ブロックへのポインタテーブル（hv_cfg.c）
 */
extern MSGQCB *const  p_msgqcb_table[];

/*
 *  メッセージキューIDの範囲の判定
 */
#define VALID_MSGQID(msgqid) (TMIN_MSGQID <= (msgqid) && (msgqid) <= tmax_msgqid)

#ifdef SUPPORT_SBUF

/*
 *  共有バッファ
 */

/*
 *  共有バッファの最小値
 */
#define TMIN_SBUFID        1

/*
 *  共有バッファ初期化ブロック
 */
typedef struct sharedbuffer_initialization_block {
    uint8   *bufmpla;   /* バッファの開始アドレス(4byte境界) */    
    uint8   *bufmpua;   /* バッファのリミットアドレス(4byte境界) */    
    uint    accessvm;   /* アクセス可能なVMのビットマップ */
    uint32  impubit;    /* 空きMPU領域のビットマップの初期値 */
} SBUFINIB;

/*
 *  共有バッファ管理ブロック
 */
typedef struct sharedbuffer_control_block {    
    const SBUFINIB *p_sbufinib;    /* 初期化ブロックへのポインタ */
    ID       acquirevmid;          /* バッファ取得中のVMのID */
    uint32   lock;                 /* ロック変数 */
    uint32   mpidx;                /* 取得したVMの使用したMPUエントリ */
    uint32   mpubit;               /* 空きMPU領域のビットマップ */
} SBUFCB;

/*
 *  共有バッファIDの最大値（hv_cfg.c）
 */
extern const ID  tmax_sbufid;

/*
 *  共有バッファ初期化ブロックのエリア（hv_cfg.c）
 */
extern const SBUFINIB   sbufinib_table[];

/*
 *  共有バッファ管理ブロックへのポインタテーブル（hv_cfg.c）
 */
extern SBUFCB *const  p_sbufcb_table[];

/*
 *  共有バッファIDの範囲の判定
 */
#define VALID_SBUFID(sbufid) (TMIN_SBUFID <= (sbufid) && (sbufid) <= tmax_sbufid)

#endif /* SUPPORT_SBUF */

/*
 *  初期化関数
 */
extern void vmcom_init(void);

/*
 *  状態変数API
 */
extern void initialize_statevariable(void);
extern int ReadStateVariable(uint SverID, void *Variable);
extern int WriteStateVariable(uint SverID, const void *Variable);
extern int DeactivateStateVariable(uint SverID);

/*
 *  メッセージキューAPI
 */
extern int ReadMessageQueue(uint MsgqID, void *WritePtr);
extern int WriteMessageQueue(uint MsgqID, const void *ReadPtr, uint MsgSize);
extern int DeactivateMessageQueue(uint MsgqID);

#ifdef SUPPORT_SBUF
/*
 *  共有バッファAPI
 */
extern int AcquireSharedBuffer(uint SBufID, void **BufPtr);
extern int ReleaseSharedBuffer(uint SBufID);
#endif /* SUPPORT_SBUF */
#endif /* TOPPERS_MACRO_ONLY */

#endif /* TOPPERS_TARGET_H */

