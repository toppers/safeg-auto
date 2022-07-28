#ifndef _SAMPLE1_H_
#define _SAMPLE1_H_

#ifndef TOPPERS_MACRO_ONLY

/*
 *  HV割込みハンドラ
 */
extern void hvint0_handler(void);
extern void hvint1_handler(void);
extern void twdint_handler(void);

/*
 *  HVC関数
 */
extern int hvc_test0(void);
extern int hvc_test1(int arg0);
extern int hvc_test2(int arg0, int arg1);
extern int hvc_test3(int arg0, int arg1, int arg3);

#endif /* TOPPERS_MACRO_ONLY */

#endif /* _SAMPLE1_H_ */
