/*
 *  TOPPERS ATK2
 *      Toyohashi Open Platform for Embedded Real-Time Systems
 *      Automotive Kernel Version 2
 *
 *  Copyright (C) 2012-2014 by Center for Embedded Computing Systems
 *              Graduate School of Information Science, Nagoya Univ., JAPAN
 *
 *  上記著作権者は，以下の(1)～(4)の条件を満たす場合に限り，本ソフトウェ
 *  ア（本ソフトウェアを改変したものを含む．以下同じ）を使用・複製・改
 *  変・再配布（以下，利用と呼ぶ）することを無償で許諾する．
 *  (1) 本ソフトウェアをソースコードの形で利用する場合には，上記の著作
 *      権表示，この利用条件および下記の無保証規定が，そのままの形でソー
 *      スコード中に含まれていること．
 *  (2) 本ソフトウェアを，ライブラリ形式など，他のソフトウェア開発に使
 *      用できる形で再配布する場合には，再配布に伴うドキュメント（利用
 *      者マニュアルなど）に，上記の著作権表示，この利用条件および下記
 *      の無保証規定を掲載すること．
 *  (3) 本ソフトウェアを，機器に組み込むなど，他のソフトウェア開発に使
 *      用できない形で再配布する場合には，次のいずれかの条件を満たすこ
 *      と．
 *    (a) 再配布に伴うドキュメント（利用者マニュアルなど）に，上記の著
 *        作権表示，この利用条件および下記の無保証規定を掲載すること．
 *    (b) 再配布の形態を，別に定める方法によって，TOPPERSプロジェクトに
 *        報告すること．
 *  (4) 本ソフトウェアの利用により直接的または間接的に生じるいかなる損
 *      害からも，上記著作権者およびTOPPERSプロジェクトを免責すること．
 *      また，本ソフトウェアのユーザまたはエンドユーザからのいかなる理
 *      由に基づく請求からも，上記著作権者およびTOPPERSプロジェクトを
 *      免責すること．
 *
 *  本ソフトウェアは，AUTOSAR（AUTomotive Open System ARchitecture）仕
 *  様に基づいている．上記の許諾は，AUTOSARの知的財産権を許諾するもので
 *  はない．AUTOSARは，AUTOSAR仕様に基づいたソフトウェアを商用目的で利
 *  用する者に対して，AUTOSARパートナーになることを求めている．
 *
 *  本ソフトウェアは，無保証で提供されているものである．上記著作権者お
 *  よびTOPPERSプロジェクトは，本ソフトウェアに関して，特定の使用目的
 *  に対する適合性も含めて，いかなる保証も行わない．また，本ソフトウェ
 *  アの利用により直接的または間接的に生じたいかなる損害に関しても，そ
 *  の責任を負わない．
 *
 *  $Id: uart_rlin.c 540 2015-12-29 01:00:01Z ertl-honda $
 */

/*
 *		シリアルI/Oデバイス（SIO）ドライバ（RLIN用）
 */

#include "Os.h"
#include "target_serial.h"
#include "uart_rlin.h"
#include "Os_Lcfg.h"


/* 内部関数のプロトタイプ宣言 */
LOCAL_INLINE uint8 uart_getchar(void);

/*
 *  受信した文字の取り出し
 */
LOCAL_INLINE uint8
uart_getchar(void)
{
	return(sil_reb_mem((void *) (RLIN3x_BASE + RLIN3xLURDRL_B)));
}

/*
 *  初期化処理
 */
void
InitHwSerial(void)
{

	/* Uart Mode を有効(ノイズフィルタも有効) */
	sil_wrb_mem((void *) (RLIN3x_BASE + RLIN3xLMD_B), 0x31);

	/* ボーレート設定 */
	sil_wrb_mem((void *) (RLIN3x_BASE + RLIN3xLWBR_B), RLIN3xLWBR_VAL);
	sil_wrh_mem((void *) (RLIN3x_BASE + RLIN3xLBRP01_H), RLIN3xLBRP01_VAL);

	/* エラー検出許可 */
	sil_wrb_mem((void *) (RLIN3x_BASE + RLIN3xLEDE_B), 0x0d);

	/* データ フォーマット */
	sil_wrb_mem((void *) (RLIN3x_BASE + RLIN3xLBFC_B), 0x00);

	/* リセット解除 */
	sil_wrb_mem((void *) (RLIN3x_BASE + RLIN3xLCUC_B), 0x01);

	/* リセット解除待ち */
	while (sil_reb_mem((void *) (RLIN3x_BASE + RLIN3xLMST_B)) == 0x00) {
	}

	/* 送受信動作許可 */
	sil_wrb_mem((void *) (RLIN3x_BASE + RLIN3xLUOER_B), 0x03);

	/* 受信割込み許可 */
	sil_wrb_mem((void *) (RLIN3x_BASE + RLIN3xLIE_B), 0x02);
}

/*
 *  シリアルI/Oポートのクローズ
 */
void
TermHwSerial(void)
{
	/* 受信割込みの禁止 */
	sil_wrb_mem((void *) (RLIN3x_BASE + RLIN3xLIE_B), 0x00);
}

/*
 *  SIOの割込みハンドラ
 */
ISR(RxHwSerialInt)
{
	/*
	 *  受信通知コールバックルーチンを呼び出す
	 */
	RxSerialInt(uart_getchar());
}
