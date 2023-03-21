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

/*
 *  VMのリセット（hv_service.c）
 */
extern ER ResetVM(ID VMID, uint32 arg0, uint32 arg1, uint32 arg2, uint32 arg3);

/*
 *  VMへのフェイクFE例外の発生（hv_service.c）
 */
extern ER RaiseVMFakeFE(ID vmid, uint32 voffset, uint32 cause);

/*
 *  VMへのフェイクEI例外の発生（hv_service.c）
 */
extern ER RaiseVMFakeEI(ID vmid, uint32 voffset, uint32 cause);

/*
 *  IDLE-VMの呼び出し
 */
extern ER CallIdleVM(ID vmid);

/*
 *  VM発生の例外情報
 */
typedef struct {
    uint32 cause;   /* 例外要因 */
    ID     vmid;    /* VMID */
    uint32 pc;      /* pc */
    uint32 psw;     /* psw */
    uint32 regbase; /* レジスタの保存先のベースアドレス */
} VMEXC_INFO;

/*
 *  例外を発生したVMのコンテキスのregbaseからのオフセット
 */
#define VMEXC_REGBASE_SP   (0 * 4)
#define VMEXC_REGBASE_FPSR (1 * 4)
#define VMEXC_REGBASE_R31  (2 * 4)
#define VMEXC_REGBASE_R30  (3 * 4)
#define VMEXC_REGBASE_R29  (4 * 4)
#define VMEXC_REGBASE_R28  (5 * 4)
#define VMEXC_REGBASE_R27  (6 * 4)
#define VMEXC_REGBASE_R26  (7 * 4)
#define VMEXC_REGBASE_R25  (8 * 4)
#define VMEXC_REGBASE_R24  (9 * 4)
#define VMEXC_REGBASE_R23  (10 * 4)
#define VMEXC_REGBASE_R22  (11 * 4)
#define VMEXC_REGBASE_R21  (12 * 4)
#define VMEXC_REGBASE_R20  (13 * 4)
#define VMEXC_REGBASE_R19  (14 * 4)
#define VMEXC_REGBASE_R18  (15 * 4)
#define VMEXC_REGBASE_R17  (16 * 4)
#define VMEXC_REGBASE_R16  (17 * 4)
#define VMEXC_REGBASE_R15  (18 * 4)
#define VMEXC_REGBASE_R14  (19 * 4)
#define VMEXC_REGBASE_R13  (20 * 4)
#define VMEXC_REGBASE_R12  (21 * 4)
#define VMEXC_REGBASE_R11  (22 * 4)
#define VMEXC_REGBASE_R10  (23 * 4)
#define VMEXC_REGBASE_R9   (24 * 4)
#define VMEXC_REGBASE_R8   (25 * 4)
#define VMEXC_REGBASE_R7   (26 * 4)
#define VMEXC_REGBASE_R6   (27 * 4)
#define VMEXC_REGBASE_R5   (28 * 4)
#define VMEXC_REGBASE_R4   (29 * 4)
#define VMEXC_REGBASE_R3   (0 * 4)
#define VMEXC_REGBASE_R2   (31 * 4)
#define VMEXC_REGBASE_R1   (32 * 4)

/*
 *  HV発生の例外情報
 */
typedef struct {
    uint32 cause;   /* 例外要因 */
    uint32 pc;      /* pc */
    uint32 psw;     /* psw */
    uint32 regbase; /* レジスタの保存先のベースアドレス */
} HVEXC_INFO;

/*
 *  例外を発生したVMのコンテキスのregbaseからのオフセット
 */
#define HVEXC_REGBASE_SP   (0 * 4)
#define HVEXC_REGBASE_FPSR (1 * 4)
#define HVEXC_REGBASE_R31  (2 * 4)
#define HVEXC_REGBASE_R30  (3 * 4)
#define HVEXC_REGBASE_R29  (4 * 4)
#define HVEXC_REGBASE_R28  (5 * 4)
#define HVEXC_REGBASE_R27  (6 * 4)
#define HVEXC_REGBASE_R26  (7 * 4)
#define HVEXC_REGBASE_R25  (8 * 4)
#define HVEXC_REGBASE_R24  (9 * 4)
#define HVEXC_REGBASE_R23  (10 * 4)
#define HVEXC_REGBASE_R22  (11 * 4)
#define HVEXC_REGBASE_R21  (12 * 4)
#define HVEXC_REGBASE_R20  (13 * 4)
#define HVEXC_REGBASE_R19  (14 * 4)
#define HVEXC_REGBASE_R18  (15 * 4)
#define HVEXC_REGBASE_R17  (16 * 4)
#define HVEXC_REGBASE_R16  (17 * 4)
#define HVEXC_REGBASE_R15  (18 * 4)
#define HVEXC_REGBASE_R14  (19 * 4)
#define HVEXC_REGBASE_R13  (20 * 4)
#define HVEXC_REGBASE_R12  (21 * 4)
#define HVEXC_REGBASE_R11  (22 * 4)
#define HVEXC_REGBASE_R10  (23 * 4)
#define HVEXC_REGBASE_R9   (24 * 4)
#define HVEXC_REGBASE_R8   (25 * 4)
#define HVEXC_REGBASE_R7   (26 * 4)
#define HVEXC_REGBASE_R6   (27 * 4)
#define HVEXC_REGBASE_R5   (28 * 4)
#define HVEXC_REGBASE_R4   (29 * 4)
#define HVEXC_REGBASE_R2   (31 * 4)
#define HVEXC_REGBASE_R1   (32 * 4)

#endif /* TOPPERS_MACRO_ONLY */

#endif /* _HV_H_ */
