#ifndef _HV_ASM_H_
#define _HV_ASM_H_

/*
 *  アセンブリファイルでも必要な定義
 *   CCRHはC言語のマクロを入力できないため，このファイルを元にCCRH用のヘッ
 *   ダファイルを生成する．
 */

/*
 *  書き込み可能な（コンテキストとして保存が必要な）汎用レジスタ数
 */
#define TNUM_REG    (31)

/*
 *  多重化されていないシステムレジスタ数
 */
#define TNUM_S_SYSREG    (17)

/*
 *  多重化されているシステムレジスタ数
 */
#define TNUM_D_SYSREG    (19)

/*
 *   HVTWDCBの各エントリのオフセット
 */
#define HVTWDCB_SP    0
#define HVTWDCB_PC    4
#define HVTWDCB_PSW   8

/*
 *   VMCBの各エントリのオフセット
 */
#define VMCB_REG        (4*0)
#define VMCB_REG_SP     (4 * (TNUM_REG - 3))
#define VMCB_PC         (4 * (TNUM_REG))
#define VMCB_S_SYSREG   (4 * (TNUM_REG + 1))
#define VMCB_SBUFMPU    (VMCB_S_SYSREG + (4 * (TNUM_S_SYSREG + TNUM_D_SYSREG + 2)))

/*
 *   CCBの各エントリのオフセット
 */
#define CCB_P_RUNVM     (4 * 0)
#define CCB_P_RUNTWD    (4 * 1)
#define CCB_SP          (4 * 2)
#define CCB_RUNHVC      (4 * 3)
#define CCB_RUNHVINT    (CCB_RUNHVC + 1)
#define CCB_RUNVMID     (CCB_RUNHVINT + 1)
#define CCB_RUNIDLE     (CCB_RUNVMID + 1)

/*
 *  HVでのFPSRの初期値
 */
#define HV_INIT_FPSR  0x00020000U

#endif /* _HV_ASM_H_ */
