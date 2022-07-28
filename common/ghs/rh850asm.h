#ifndef _RH850ASM_H_
#define _RH850ASM_H_

/*
 *  ラベルロード Lea (Load Effective Address)
 */
.macro Lea name, destination
    mov name, destination
.endm

/*
 *  PEIDの読み込み
 */
.macro rpeid destination
    stsr  0, destination, 2
.endm

#endif /* _RH850ASM_H_ */
