/*
 *    コンパイラ依存定義（GHS）
 */
#ifndef _COMPILER_H_
#define _COMPILER_H_

#include <v800_ghs.h>
#include <ghs_barrier.h>

/*
 *  CCRHでは，インライン関数は「#pragma」を利用して、関数定義の前に
 *  インライン化するシンボルを予め指定しなければならない．
 */
#define INLINE          __inline__
#define LOCAL_INLINE    static __inline__

#define Asm             __asm__ volatile    /* インラインアセンブラ（最適化抑止）*/

LOCAL_INLINE uint32_t
rh850_acquire_lock(uint32_t *p_lock)
{
    uint32_t locked = 0;
    Asm("1: ldl.w [%1], r21 \n"
        "   cmp   r0, r21   \n"
        "   bnz   2f        \n"
        "   mov   1, r21    \n"
        "   stc.w r21, [%1] \n"
        "   cmp   r0, r21   \n"
        "   be    2f        \n"
        "   mov   1, %0     \n"
        "   br    3f        \n"
        "2:                 \n"
        "   mov   0, %0     \n"
        "3:                 \n"
        : "=r" (locked)
        : "r" (p_lock)
        : "cc", "r21");

    return locked;
}

LOCAL_INLINE void
rh850_snooze(void){
    Asm("snooze");
}

/*
 *  メモリの再読み込みが必要なことをコンパイラに伝える
 */
#define MEMORY_CHANGED __memory_changed();

#endif /* _COMPILER_H_ */
