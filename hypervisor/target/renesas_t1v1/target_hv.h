#ifndef _TARGET_HV_H
#define _TARGET_HV_H

/*
 *  チップの定義のインクルード
 */
#include "u2a16.h"

/*
 *  OSTMの動作周波数
 */
#define OSTM_CLK_MHZ     (80U)

/*
 *  RLIN3のボーレートレジスタへの設定値 PCLK=80MHz, 115200bps
 */
#define RLIN3xLWBR_VAL      (0xf4U)
#define RLIN3xLBRP01_VAL    (10U)

/*
 *  TAUD0に関する設定
 * 
 *  U2Aでは，CLK_HSB(=80Mhz)が入力クロックとなっている
 *  各APIのカウント値は1us単位とする．
 *  カウンタは16bit(65535)   1M(1use)で 65m
 *  プリスケーラの値 : カウントクロック : 最大カウント時間(us)  : 最小カウント
 *                (2^2)  :  20Mhz           :        3,276.7  :  50ns(0.05)
 *                (2^3)  :  10Mhz           :        6,553.5  : 100ns
 *                (2^4)  :   5Mhz           :        13,107   : 200ns
 *                (2^5)  :   2.5Mz          :        26,214   : 400ns
 *                (2^6)  :   1.25Mhz        :        52,428   : 8000ns
 *                (2^7)  :   625Khz         :       104,856   :  1,600ns
 *                (2^8)  :   312.5Khz       :       209,712   :  3,200ns
 *                (2^9)  :   156.25Khz      :       419,424   :  6,400ns
 *                (2^10) :   78.125Khz      :       838,848   : 12,800ns
 *                (2^11) :   39.063Khz      :     1,677,696   : 25,600ns
 *                (2^12) :   19.531Khz      :     3,355,392   : 51,200ns
 *                (2^13) :    9.766Khz      :     6,710,784   : 102,400ns
 *                (2^14) :    4.833Khz      :    13,421,568   : 204,8800ns
 *                (2^15) :    2.441Khz      :    26,843,136   : 409,600ns
 */
#define TAUD_PCLK_MHZ       (80U)
#define TAUD_CLK_PRS        (15U)

#endif /* _TARGET_HV_H */
