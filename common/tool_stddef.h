#ifndef _TOOL_STDDEF_H_
#define _TOOL_STDDEF_H_

#ifndef TOPPERS_MACRO_ONLY

#include <stdint.h>

/*
 *  コンパイラ依存定義
 */
#include <Compiler.h>

#define NULL        (0U)     /* 無効ポインタ */
#define true        (1U)     /* 真 */
#define false       (0U)    /* 偽 */

typedef unsigned char      boolean;

typedef char                 char8;
typedef unsigned char        uint8;
typedef signed char          sint8;
typedef unsigned short       uint16;
typedef signed short         sint16;
typedef unsigned int         uint32;
typedef signed int           sint32;
typedef unsigned long long   uint64;
typedef signed long long     sint64;
typedef unsigned int         uint;
typedef signed int           sint;

typedef unsigned long    uint8_least;
typedef unsigned long    uint16_least;
typedef unsigned long    uint32_least;
typedef signed long      sint8_least;
typedef signed long      sint16_least;
typedef signed long      sint32_least;

typedef float    float32;
typedef double   float64;

typedef uint32   uintptr;    /* ポインタを格納できる符号無し整数 */
typedef sint32   sintptr;    /* ポインタを格納できる符号付き整数 */

typedef void     (*FP)(void);

/*
 *  8ビット単位の読出し／書込み
 */
LOCAL_INLINE uint8_t
sil_reb_mem(const uint8_t *mem)
{
    uint8_t data;

    data = *((const volatile uint8_t *) mem);
    return(data);
}

LOCAL_INLINE void
sil_wrb_mem(uint8_t *mem, uint8_t data)
{
    *((volatile uint8_t *) mem) = data;
}

/*
 *  16ビット単位の読出し／書込み
 */
LOCAL_INLINE uint16_t
sil_reh_mem(const uint16_t *mem)
{
    uint16_t data;

    data = *((const volatile uint16_t *) mem);
    return(data);
}

LOCAL_INLINE void
sil_wrh_mem(uint16_t *mem, uint16_t data)
{
    *((volatile uint16_t *) mem) = data;
}

/*
 *  32ビット単位の読出し／書込み
 */
LOCAL_INLINE uint32_t
sil_rew_mem(uint32_t *mem)
{
    uint32 data;

    data = *((const volatile uint32_t *) mem);
    return(data);
}

LOCAL_INLINE void
sil_wrw_mem(const uint32_t *mem, uint32_t data)
{
    *((volatile uint32_t *) mem) = data;
}

#endif /* TOPPERS_MACRO_ONLY */

#endif /* _TOOL_STDDEF_H_ */
