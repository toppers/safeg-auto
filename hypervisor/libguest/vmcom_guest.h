#ifndef TOPPERS_VMCOM_H
#define TOPPERS_VMCOM_H

/*
 *  VM間通信機能VM用ヘッダファイル
 */

#ifndef TOPPERS_MACRO_ONLY

/*
 *  HVTRAPのコンフィギュレーションヘッダファイル
 */
#include "hvc_cfg.h"

#ifdef SUPPORT_VMCOM

/*
 *  VM間通信機能の定義（HVと共通）
 */
#include "vmcom_def.h"

#ifdef SUPPORT_SVAR

/*
 *  状態変数API
 */
LOCAL_INLINE int
ReadStateVariable(unsigned int SvarID, void *Variable)
{
    return CAL_HVC_2(int, TFN_HVC_RSVAR, int, SvarID, void*, Variable);
}

LOCAL_INLINE int
WriteStateVariable(unsigned int SvarID, const void *Variable)
{
    return CAL_HVC_2(int, TFN_HVC_WSVAR, int, SvarID, const void*, Variable);
}

LOCAL_INLINE int
DeactivateStateVariable(unsigned int SvarID)
{
    return CAL_HVC_1(int, TFN_HVC_DSVAR, int, SvarID);
}
#endif /* SUPPORT_SVAR */


#ifdef SUPPORT_MSGQ

/*
 *  メッセージキューAPI
 */
LOCAL_INLINE int
ReadMessageQueue(unsigned int MsgqID, void* WritePtr)
{
    return CAL_HVC_2(int, TFN_HVC_RMSGQ, unsigned int, MsgqID, void*, WritePtr);
}

LOCAL_INLINE int
WriteMessageQueue(unsigned int MsgqID, const void *ReadPtr, unsigned int MessageSize)
{
    return CAL_HVC_3(int, TFN_HVC_WMSGQ, unsigned int, MsgqID, const void*, ReadPtr, unsigned int, MessageSize);
}

LOCAL_INLINE int
DeactivateMessageQueue(unsigned int MsgqID)
{
    return CAL_HVC_1(int, TFN_HVC_DMSGQ, unsigned int, MsgqID);
}
#endif /* SUPPORT_MSGQ */

#ifdef SUPPORT_SBUF
/*
 *  共有バッファAPI
 */
LOCAL_INLINE int
AcquireSharedBuffer(unsigned int SBufID, void **BufPtr)
{
    return CAL_HVC_2(int, TFN_HVC_AQSB, unsigned int, SBufID, void**, BufPtr);
}

LOCAL_INLINE int
ReleaseSharedBuffer(unsigned int SBufID)
{
    return CAL_HVC_1(int, TFN_HVC_RSSB, unsigned int, SBufID);
}
#endif /* SUPPORT_SBUF */

#endif /* SUPPORT_VMCOM */

#endif /* TOPPERS_MACRO_ONLY */

#endif /* TOPPERS_TARGET_H */
