#ifndef _HV_H_
#define _HV_H_

/*
 *  RH850ハイパーバイザ 標準ヘッダファイル
 *
 */
#include "common_def.h"
#include "target_hv.h"
#include "hv_cfg.h"

#ifndef TOPPERS_MACRO_ONLY

/*
 *  起動処理（startup.c）
 */
extern void StartHV(uint32 mode);

/*
 *  コアIDの取得（hv_service.c）
 */
extern ER GetCoreID(ID* pCoreID);

/*
 *  HVタイムウィンドウの残り時間の取得（hv_service.c）
 */
extern ER GetHVTWTimeLeft(uint32* pTimeLeft);

#endif /* TOPPERS_MACRO_ONLY */

#endif /* _HV_H_ */
