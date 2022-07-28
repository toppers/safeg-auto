/*
 *    HV実装のコンパイラ依存記述（CCRH）
 */
#ifndef _COMPILER_HV_IMPLE_H_
#define _COMPILER_HV_IMPLE_H_

#define _SET_MPU(From, To, basereg)   ldm.mp [basereg], e##From-e##To
#define SET_MPU(From, To, basereg)   _SET_MPU(From, To, basereg)

#pragma inline_asm set_mpu_vm
static void
set_mpu_vm(uint32_t* pbase)
{
    SET_MPU(TMIN_HEMPU, TMAX_HEMPU, r6)
}


#pragma inline_asm set_mpu_sbuf
static void
set_mpu_sbuf(uint32_t* pbase)
{
    SET_MPU(TMIN_HEMPU_SBUF, TMAX_HEMPU_SBUF, r6)  
}

#endif /* _COMPILER_HV_IMPLE_H_ */
