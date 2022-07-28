#ifndef _COMMON_DEF_H
#define _COMMON_DEF_H

/*
 *  ゲスト処理・ユーザーホスト処理・ハイパーバイザー共通の定義
 */

/*
 *  ツールの定義
 */
#ifndef NONUSE_HV_STDDEF
#include "tool_stddef.h"
#endif /* NONUSE_HV_STDDEF */

#ifndef TOPPERS_MACRO_ONLY

/*
 *  型定義
 */
typedef int ER;    /* エラーコード */
typedef int ID;    /* オブジェクトのID番号 */

#endif /* TOPPERS_MACRO_ONLY */

/*
 *  エラーコード
 */
#ifndef E_OK
#define E_OK    (0)     /* 正常終了 */
#endif /* E_OK */
#define E_ID    (-1)    /* ID番号不正 */
#define E_OBJ   (-2)    /* オブジェクト状態エラー */
#define E_BUF   (-3)    /* バッファ状態エラー */
#define E_MACV  (-4)    /* メモリアクセス違反 */
#define E_OACV  (-5)    /* オブジェクトアクセス違反 */
#define E_PAR   (-6)    /* パラメータエラー */
#define E_CTX   (-7)    /* コンテキストエラー */

#endif /* _COMMON_DEF_H */

