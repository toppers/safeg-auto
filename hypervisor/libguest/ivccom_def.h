#ifndef TOPPERS_IVCCOM_DEF_H
#define TOPPERS_IVCCOM_DEF_H

/*
 *  ゲスト処理・ユーザーホスト処理・ハイパーバイザー共通の定義
 */
#include "../include/common_def.h"

/*
 *  IVC通信機能共通ヘッダファイル（定義）
 */

/*
 *  メモリ領域確保のためのマクロ
 */
#define IVCCOM_COUNT_SZ(sz, unit) (((sz) + (unit) - 1) / (unit))
#define IVCCOM_ROUND_SZ(sz, unit) (((sz) + (unit) - 1) & ~((unit) - 1))

#ifndef TOPPERS_MACRO_ONLY

/*
 *  IVC状態変数IDの最小値
 */
#define TMIN_IVCSVARID        1

/* 
 * IVC状態変数管理ブロック
 */
typedef struct statevariable_control_block {
    boolean  isactive;    /* 状態 */
    uint32   lock;        /* ロック変数 */
} IVCSVARCB;

/*
 *  IVC状態変数初期化ブロック
 */
typedef struct ivc_statevariable_initialization_block {
    IVCSVARCB  *p_ivcsvarcb;     /* 管理ブロックへのポインタ */
    uint       size;             /* バッファサイズ */
    boolean    istate;           /* 初期状態 */
    void       *pbuf;            /* バッファの先頭アドレス */
    uint8      cramno;           /* CRAMの番号 */
    uint8      ivcch;            /* ch番号 */
} IVCSVARINIB;

/*
 *  IVC状態変数IDの最大値（ivccom_cfg.c）
 */
extern const ID  tmax_ivcsvarid;

/*
 *  状態変数初期化ブロックのエリア（ivccom_cfg.c）
 */
extern const IVCSVARINIB  ivcsvarinib_table[];

/*
 *  状態変数IDの範囲の判定
 */
#define VALID_IVCSVARID(ivcsvarid)  (TMIN_IVCSVARID <= (ivcsvarid) && (ivcsvarid) <= tmax_ivcsvarid)

/*
 *  IVCメッセージキューの最小値
 */
#define TMIN_IVCMSGQID   1

/*
 *  IVCメッセージキュー管理ブロック
 */
typedef struct ivcmessagequeue_control_block {
    boolean  isactive;    /* 状態 */
    uint     fmsgqsz;     /* 空き領域のサイズ */
    uint     head;        /* 最初のメッセージの格納場所 */
    uint     tail;        /* 最後のメッセージの格納場所の次 */
    uint     smsgqcnt;    /* 管理領域に格納されているメッセージの数 */
    uint32   lock;        /* ロック変数 */
} IVCMSGQCB;

/*
 *  IVCメッセージキュー初期化ブロック
 */
typedef struct ivcmessagequeue_initialization_block {
    IVCMSGQCB  *p_ivcmsgqcb;    /* 管理ブロックへのポインタ */
    int        maxmsz;          /* 最大メッセージサイズ */
    uint       msgqsz;          /* メッセージキューバッファ管理領域のサイズ */
    boolean    istate;          /* 初期状態 */
    void       *msgqmb;         /* メッセージキューバッファ管理領域の先頭アドレス */
    uint8      cramno;          /* CRAMの番号 */
    uint8      ivcch;           /* ch番号 */
} IVCMSGQINIB;

/*
 *  IVCメッセージキューIDの最大値（ivccom_cfg.c）
 */
extern const ID  tmax_ivcmsgqid;

/*
 *  IVCメッセージキュー初期化ブロックのエリア（ivccom_cfg.c）
 */
extern const IVCMSGQINIB  ivcmsgqinib_table[];

/*
 *  IVCメッセージキューIDの範囲の判定
 */
#define VALID_IVCMSGQID(ivcmsgqid) (TMIN_IVCMSGQID <= (ivcmsgqid) && (ivcmsgqid) <= tmax_ivcmsgqid)

/*
 *  IVCバッファIDの最小値
 */
#define TMIN_IVCBUFID    1

/*
 *  IVCバッファ初期化ブロック
 */
typedef struct ivc_statebuffer_initialization_block {
    void     *pbuf;     /* バッファの先頭アドレス */
    uint8    cramno;    /* CRAMの番号 */
    uint8    ivcch;     /* ch番号 */
} IVCBUFINIB;

/*
 *  IVCバッファIDの最大値（ivccom_cfg.c）
 */
extern const ID  tmax_ivcbufid;

/*
 *  IVCバッファ初期化ブロックのエリア（ivccom_cfg.c）
 */
extern const IVCBUFINIB  ivcbufinib_table[];

/*
 *  IVCバッファIDの範囲の判定
 */
#define VALID_IVCBUFID(ivcbufid) (TMIN_IVCBUFID <= (ivcbufid) && (ivcbufid) <= tmax_ivcbufid)

#endif /* TOPPERS_MACRO_ONLY */

#endif /* TOPPERS_IVCCOM_DEF_H */

