#ifndef _HVC_CFG_H_
#define _HVC_CFG_H_

/*
 * HVCコンフィギュレーションファイルヘッダー
 * VMがインクルードしてHVCを使用する． 
 * 将来的にはコンギュレータで生成する想定． 
 */

#include "hvc.h"

/*
 *  機能コード
 */
#define TFN_HVC_TEST0   0U
#define TFN_HVC_TEST1   1U
#define TFN_HVC_TEST2   2U
#define TFN_HVC_TEST3   3U

/*
 *  テスト用のサービスコール
 */ 
LOCAL_INLINE int
hvc_test0(void)
{
    return CAL_HVC_0(int, TFN_HVC_TEST0);
}

LOCAL_INLINE int
hvc_test1(int arg0)
{
    return CAL_HVC_1(int, TFN_HVC_TEST1, int, arg0);
}

LOCAL_INLINE int
hvc_test2(int arg0, int arg1)
{
    return CAL_HVC_2(int, TFN_HVC_TEST2, int, arg0, int, arg1);
}

LOCAL_INLINE int
hvc_test3(int arg0, int arg1, int arg2)
{
    return CAL_HVC_3(int, TFN_HVC_TEST3, int, arg0, int, arg1, int, arg2);
}

#endif /* _HVC_CFG_H_ */
