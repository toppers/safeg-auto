#ifndef _HVC_H
#define _HVC_H

#ifndef NONUSE_HV_STDDEF
#include "tool_stddef.h"
#endif /* NONUSE_HV_STDDEF */

#ifndef TOPPERS_MACRO_ONLY

typedef sint32 FunctionCodeType;

/*
 *  サービスコール呼び出し用の関数
 */
extern int cal_hvc_0(int fncd);
extern int cal_hvc_1(int par1, int fncd);
extern int cal_hvc_2(int par1, int par2, int fncd);
extern int cal_hvc_3(int par1, int par2, int par3, int fncd);

#define CAL_HVC_0(TYPE, FNCD) cal_hvc_0((int)((FunctionCodeType)FNCD))

#define CAL_HVC_1(TYPE, FNCD, TYPE1, PAR1) \
cal_hvc_1((int)((TYPE1)PAR1), (int)((FunctionCodeType)FNCD))

#define CAL_HVC_2(TYPE, FNCD, TYPE1, PAR1, TYPE2, PAR2) \
cal_hvc_2((int)((TYPE1)PAR1), (int)((TYPE2)PAR2), (int)((FunctionCodeType)FNCD))

#define CAL_HVC_3(TYPE, FNCD, TYPE1, PAR1, TYPE2, PAR2, TYPE3, PAR3) \
cal_hvc_3((int)((TYPE1)PAR1), (int)((TYPE2)PAR2), (int)((TYPE3)PAR3), (int)((FunctionCodeType)FNCD))

#endif /* TOPPERS_MACRO_ONLY */

#endif /* _HVC_H */

