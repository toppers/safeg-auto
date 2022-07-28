$ifndef _HV_ASM_H_
    _HV_ASM_H_ .set 1

;/*
 ;*  アセンブリファイルでも必要な定義
 ;*   CCRHはC言語のマクロを入力できないため，このファイルを元にCCRH用のヘッ
 ;*   ダファイルを生成する．
 ;*/

;/*
 ;*  書き込み可能な（コンテキストとして保存が必要な）汎用レジスタ数
 ;*/
    TNUM_REG .set (31)

;/*
 ;*  多重化されていないシステムレジスタ数
 ;*/
    TNUM_S_SYSREG .set (17)

;/*
 ;*  多重化されているシステムレジスタ数
 ;*/
    TNUM_D_SYSREG .set (19)

;/*
 ;*   HVTWDCBの各エントリのオフセット
 ;*/
    HVTWDCB_SP .set 0
    HVTWDCB_PC .set 4
    HVTWDCB_PSW .set 8

;/*
 ;*   VMCBの各エントリのオフセット
 ;*/
    VMCB_REG .set (4*0)
    VMCB_REG_SP .set (4 * (TNUM_REG - 3))
    VMCB_PC .set (4 * (TNUM_REG))
    VMCB_S_SYSREG .set (4 * (TNUM_REG + 1))
    VMCB_SBUFMPU .set (VMCB_S_SYSREG + (4 * (TNUM_S_SYSREG + TNUM_D_SYSREG + 2)))

;/*
 ;*   CCBの各エントリのオフセット
 ;*/
    CCB_P_RUNVM .set (4 * 0)
    CCB_P_RUNTWD .set (4 * 1)
    CCB_SP .set (4 * 2)
    CCB_RUNHVC .set (4 * 3)
    CCB_RUNHVINT .set (CCB_RUNHVC + 1)

$endif ;/* _HV_ASM_H_ */
