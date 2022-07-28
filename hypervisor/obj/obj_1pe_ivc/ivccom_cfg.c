#include "ivccom_def.h"
#include "ivccom_cfg.h"

/*
 *  IVC通信のコンフィギュレーションデータ
 *  HV・VM両方共に使用する 
 */

/*
 *  CBSS00(CRAM0, Channel 0)
 */
/* コンパイルオプションに依存せずSDAは使用しない */
#pragma ghs startdata
#pragma ghs section bss=".cbss00"
IVCSVARCB ivcsvarcb_IVCSVAR1;
IVCSVARCB ivcsvarcb_IVCSVAR2;
IVCSVARCB ivcsvarcb_IVCSVAR3;
IVCSVARCB ivcsvarcb_IVCSVAR4;
IVCSVARCB ivcsvarcb_IVCSVAR5;
IVCSVARCB ivcsvarcb_IVCSVAR6;
IVCSVARCB ivcsvarcb_IVCSVAR7;
IVCSVARCB ivcsvarcb_IVCSVAR8;

static uintptr ivcsvarbuf_IVCSVAR1[IVCCOM_COUNT_SZ(16, sizeof(uintptr))];
static uintptr ivcsvarbuf_IVCSVAR2[IVCCOM_COUNT_SZ(32, sizeof(uintptr))];
static uintptr ivcsvarbuf_IVCSVAR3[IVCCOM_COUNT_SZ(64, sizeof(uintptr))];
static uintptr ivcsvarbuf_IVCSVAR4[IVCCOM_COUNT_SZ(128, sizeof(uintptr))];

static uintptr ivcsvarbuf_IVCSVAR5[IVCCOM_COUNT_SZ(256, sizeof(uintptr))];
static uintptr ivcsvarbuf_IVCSVAR6[IVCCOM_COUNT_SZ(512, sizeof(uintptr))];
static uintptr ivcsvarbuf_IVCSVAR7[IVCCOM_COUNT_SZ(1024, sizeof(uintptr))];
static uintptr ivcsvarbuf_IVCSVAR8[IVCCOM_COUNT_SZ(2048, sizeof(uintptr))];
#pragma ghs section bss=default
#pragma ghs enddata

const ID tmax_ivcsvarid = (TMIN_IVCSVARID + TNUM_IVCSVARID - 1);

const IVCSVARINIB    ivcsvarinib_table[TNUM_IVCSVARID] = {
    {&ivcsvarcb_IVCSVAR1, 16, false, ivcsvarbuf_IVCSVAR1, 0, 1},
    {&ivcsvarcb_IVCSVAR2, 32, false, ivcsvarbuf_IVCSVAR2, 0, 1},
    {&ivcsvarcb_IVCSVAR3, 64, false, ivcsvarbuf_IVCSVAR3, 0, 1},
    {&ivcsvarcb_IVCSVAR4, 128, false, ivcsvarbuf_IVCSVAR4, 0, 1},
    {&ivcsvarcb_IVCSVAR5,  256, false, ivcsvarbuf_IVCSVAR5, 0, 1},
    {&ivcsvarcb_IVCSVAR6,  512, false, ivcsvarbuf_IVCSVAR6, 0, 1},
    {&ivcsvarcb_IVCSVAR7, 1024, false, ivcsvarbuf_IVCSVAR7, 0, 1},
    {&ivcsvarcb_IVCSVAR8, 2048, false, ivcsvarbuf_IVCSVAR8, 0, 1},
};

/*
 *  メッセージキュー
 */
/* コンパイルオプションに依存せずSDAは使用しない */
#pragma ghs startdata
#pragma ghs section bss=".cbss00"
static IVCMSGQCB ivcmsgqcb_IVCMSGQ1;
static IVCMSGQCB ivcmsgqcb_IVCMSGQ2;
static IVCMSGQCB ivcmsgqcb_IVCMSGQ3;
static IVCMSGQCB ivcmsgqcb_IVCMSGQ4;

static uintptr ivcmsgqbuf_IVCMSGQ1[IVCCOM_COUNT_SZ(32, sizeof(uintptr))];
static uintptr ivcmsgqbuf_IVCMSGQ2[IVCCOM_COUNT_SZ(64, sizeof(uintptr))];
static uintptr ivcmsgqbuf_IVCMSGQ3[IVCCOM_COUNT_SZ(128, sizeof(uintptr))];
static uintptr ivcmsgqbuf_IVCMSGQ4[IVCCOM_COUNT_SZ(256, sizeof(uintptr))];
#pragma ghs section bss=default
#pragma ghs enddata

const ID tmax_ivcmsgqid = (TMIN_IVCMSGQID + TNUM_IVCMSGQID - 1);

const IVCMSGQINIB ivcmsgqinib_table[TNUM_IVCMSGQID] = {
    {&ivcmsgqcb_IVCMSGQ1,   8,  32, false, ivcmsgqbuf_IVCMSGQ1, 0, 1},
    {&ivcmsgqcb_IVCMSGQ2,  16,  64, false, ivcmsgqbuf_IVCMSGQ2, 0, 1},
    {&ivcmsgqcb_IVCMSGQ3, 128, 512, false, ivcmsgqbuf_IVCMSGQ3, 0, 1},
    {&ivcmsgqcb_IVCMSGQ4,  64, 256, false, ivcmsgqbuf_IVCMSGQ4, 0, 1},
};


/* コンパイルオプションに依存せずSDAは使用しない */
#pragma ghs startdata
#pragma ghs section bss=".cbss01"

static uintptr ivcbuf_IVCBUF1[IVCCOM_COUNT_SZ(256, sizeof(uintptr))];
#pragma ghs section bss=default
#pragma ghs enddata

const ID tmax_ivcbufid = (TMIN_IVCBUFID + TNUM_IVCBUFID - 1);

const IVCBUFINIB    ivcbufinib_table[TNUM_IVCBUFID] = {
    {ivcbuf_IVCBUF1, 0, 2},
};
