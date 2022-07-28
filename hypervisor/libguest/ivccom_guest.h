#ifndef TOPPERS_IVCCOM_GUEST_H
#define TOPPERS_IVCCOM_GUEST_H

#include "ivccom_def.h"

/*
 *  IVC通信機能VM用ヘッダファイル
 */

#ifndef TOPPERS_MACRO_ONLY

/*
 *  エラーコード
 */
#define E_OK    (0)     /* 正常終了 */
#define E_ID    (-1)    /* ID番号不正 */
#define E_OBJ   (-2)    /* オブジェクト状態エラー */
#define E_BUF   (-3)    /* バッファ状態エラー */
#define E_MACV  (-4)    /* メモリアクセス違反 */
#define E_OACV  (-5)    /* オブジェクトアクセス違反 */
#define E_PAR   (-6)    /* パラメータエラー */
#define E_IVC   (-7)    /* IVCLOCKEDエラー */

/*
 *  IVC状態変数API
 */
extern int ReadIVCStateVariable(uint IVCSvarID, void *Variable);
extern int WriteIVCStateVariable(uint IVCSvarID, const void *Variable);
extern int DeactivateIVCStateVariable(uint IVCSvarID);

/*
 *  IVCメッセージキューAPI
 */
extern int ReadIVCMessageQueue(uint IVCMsgqID, void *WritePtr);
extern int WriteIVCMessageQueue(uint IVCMsgqID, const void *ReadPtr, uint MsgSize);
extern int DeactivateIVCMessageQueue(uint IVCMsgqID);

/*
 *  IVC状態バッファ
 */
extern int AcquireIVCbuf(uint IVCbufID, void **BufPtr);
extern int ReleaseIVCbuf(uint IVCbufID);
extern int DeactivateIVCbuf(uint IVCbufID);

#endif /* TOPPERS_MACRO_ONLY */

#endif /* TOPPERS_IVCCOM_GUEST_H */

