/*
 *    コンパイラ依存定義（CCRH）
 */
#ifndef _COMPILER_H_
#define _COMPILER_H_

/*
 *  CCRHでは，インライン関数は「#pragma」を利用して、関数定義の前に
 *  インライン化するシンボルを予め指定しなければならない．
 */
#define INLINE          inline 
#define LOCAL_INLINE    static inline

/*
 *  組込み関数のGHSからの読み替え
 */
#define __LDSR   __ldsr_rh
#define __STSR   __stsr_rh
#define __HALT   __halt
#define __SYNCM  __syncm
#define __SYNCP  __syncp
#define __SYNCI  __synci
#define __SCH1R  __sch1r

#pragma inline_asm rh850_acquire_lock
static uint32_t
rh850_acquire_lock(uint32_t *p_lock){
   .local _fail,_end
   pushsp  r21,r21
   ldl.w   [r6], r21
   cmp     r0, r21
   bnz     _fail       
   mov     1, r21 
   stc.w   r21, [r6]
   cmp     r0, r21 
   be      _fail
   mov     1, r10
   br      _end
_fail:     
   mov     0, r10
_end:
   popsp   r21,r21
}

#pragma inline_asm rh850_snooze
static void rh850_snooze(void){
  snooze
}

/*
 *  メモリ再読込が必要なことをコンパイラに伝えるためのダミー関数（u2a.c）
 *  CCRH にはGHSの __memory_changed() 相当がないため空の関数を呼び出す．
 */
extern void _rh850_dummy_call(void);

/*
 *  メモリの再読み込みが必要なことをコンパイラに伝える
 */
#define MEMORY_CHANGED _rh850_dummy_call();

#endif /* _COMPILER_H_ */
