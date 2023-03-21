#ifndef _RH850ASM_H_
#define _RH850ASM_H_

;/*
; * ラベルロード Lea (Load Effective Address)
; */
Lea .macro name, destination
	mov32	#name, destination
.endm

;/*
; *  PEIDの読み込み
; */
rpeid .macro destination
    stsr  0, destination, 2
.endm

;/*
; * システムレジスタ番号の定義
; */
    eipc    .set    0
    eipsw   .set    1
    fepc    .set    2
    fepsw   .set    3
    ecr     .set    4
    psw     .set    5
    fpsr    .set    6
    fpepc   .set    7
    fpst    .set    8
    fpcc    .set    9
    fpcfg   .set    10
    sccfg   .set    11
    scbp    .set    12
    eiic    .set    13
    feic    .set    14
    pswh    .set    15
    ctpc    .set    16
    eipswh  .set    18
    fepswh  .set    19
    eiwr    .set    28
    fewr    .set    29
#endif /* _RH850ASM_H_ */
