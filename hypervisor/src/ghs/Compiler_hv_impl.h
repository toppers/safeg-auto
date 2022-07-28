/*
 *    HV実装のコンパイラ依存記述（CCRH）
 */
#ifndef _COMPILER_HV_IMPLE_H_
#define _COMPILER_HV_IMPLE_H_

#define _SET_MPU(From, To, pbase) Asm("ldm.mp [%0], e"#From"-e"#To""::"r"(pbase));
#define SET_MPU(From, To, pbase) _SET_MPU(From, To, pbase)

LOCAL_INLINE void
set_mpu_vm(uint32_t* pbase)
{
    SET_MPU(TMIN_HEMPU, TMAX_HEMPU, pbase);
}

LOCAL_INLINE void
set_mpu_sbuf(uint32_t* pbase)
{
    SET_MPU(TMIN_HEMPU_SBUF, TMAX_HEMPU_SBUF, pbase);
}

#endif /* _COMPILER_HV_IMPLE_H_ */
