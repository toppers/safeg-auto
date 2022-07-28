/*
 * 将来的にはコンフィギュレーションファイルから自動生成する想定． 
 */
#include "hv.h"
#include "vmcom.h"
#include "hv_cfg.h"
#include "hv_impl.h"
#include "sample1.h"

/*
 *  SPIDテーブル(hv_cfg.c)
 */
const uint8 hvspid_table[TNUM_PHYS_CORE] ={
    HV_SPID_CORE0,
    0,
#if TNUM_PHYS_CORE == 4
    0,
    0,
#endif /* TNUM_PHYS_CORE == 4 */
};

/*
 *  CCB
 */
#pragma ghs section bss=".lbss0"
static CCB ccb_core0;
#pragma ghs section bss=default

CCB *const p_ccb_table[TNUM_PHYS_CORE] = {
    &ccb_core0,
    NULL,
#if TNUM_PHYS_CORE == 4
    NULL,
    NULL,
#endif /* TNUM_PHYS_CORE == 4 */
};

/*
 *  HVタイムウィンドウ処理コントロールブロック
 */
#pragma ghs section bss=".lbss0"
HVTWDCB    hvtwdcb_core0;
#pragma ghs section bss=default

HVTWDCB *const p_hvtwdcb_table[TNUM_PHYS_CORE] = {
    &hvtwdcb_core0,
    NULL,
#if TNUM_PHYS_CORE == 4    
    NULL,
    NULL
#endif /* TNUM_PHYS_CORE == 4 */      
};

/*
 *  VM初期化ブロックテーブル
 */
const VMINIB vminib_table[TNUM_VM] = {
    {
        0,
        VM0_0_ROM_BASE,
        VM0_0_GPID,
        VM0_0_SPIDLIST,
        VM0_0_SPID
    },
    {
        0,
        VM0_1_ROM_BASE,
        VM0_1_GPID,
        VM0_1_SPIDLIST,
        VM0_1_SPID
    }
};

/*
 *  VMコントロールブロック
 */
#pragma ghs section bss=".lbss0"
static VMCB vmcb_vm0_0;
static VMCB vmcb_vm0_1;
#pragma ghs section bss=default

VMCB *const p_vmcb_table[TNUM_VM] = {
    &vmcb_vm0_0,
    &vmcb_vm0_1
};

/*
 * VM割込み初期化ブロックテーブル
 */
const VMINTINIB vmintinib_table[TNUM_VMINT] = {
    {OSTM0_INTNO, 1, 0},
    {OSTM1_INTNO, 2, 0},
    {707, 1, 0},
    {708, 2, 0}
};

/*
 *  タイムウィンドウトリガ割込みのためのカウンタ
 */
uint32 twtgcnt_core0[TNUM_TW_CORE0];

/*
 *  タイムウィンドウ初期化ブロック
 */
const TWDINIB twdinib_core0[] = {
    {
        VMID_VM0_0,
        TWDTIMER_TO_CYC(4000000U),
        707,
        1,
        &twtgcnt_core0[0]
    },
    {
        VMID_VM0_1,
        TWDTIMER_TO_CYC(2000000U),
        708,
        2,
        &twtgcnt_core0[1]
    },
    {
        VMID_HVTWD,
        TWDTIMER_TO_CYC(1500000U),
        709,
        1,
        &twtgcnt_core0[2]
    },
    {
        VMID_IDLE,
        0xFFFFFFFF,
        0,
        0
    }
};

const TWDINIB* const p_twdinib_table[TNUM_PHYS_CORE] = {
    twdinib_core0,
    NULL,
#if TNUM_PHYS_CORE == 4
    NULL,
    NULL,
#endif /* TNUM_PHYS_CORE == 4 */
};

/*
 *  HEMPU初期化ブロックテーブル
 */
const HEMPUINIB hempuinib_table[][TNUM_HEMPU] = {
    {
        /* VM0_0 */
        {
            VM0_0_ROM_BASE,
            VM0_0_ROM_BASE + VM0_0_ROM_SIZE -1,
            (MPU_E_MASK|MPU_ROM|MPU_WG_MASK|MPU_RG_MASK)
        },
        {
            VM0_0_LRAM_BASE,
            VM0_0_LRAM_BASE + VM0_0_LRAM_SIZE -1,
            (MPU_E_MASK|MPU_RAM|MPU_WG_MASK|MPU_RG_MASK)
        },
        {
            VM0_0_CRAM_BASE,
            VM0_0_CRAM_BASE + VM0_0_CRAM_SIZE -1,
            (MPU_E_MASK|MPU_RAM|MPU_WG_MASK|MPU_RG_MASK)
        },
        {
            VM0_0_PERI_BASE,
            VM0_0_PERI_BASE + VM0_0_PERI_SIZE -1,
            (MPU_E_MASK|MPU_PERI|MPU_WG_MASK|MPU_RG_MASK)
        },
        {
            VM0_0_SMEM_BASE,
            VM0_0_SMEM_BASE + VM0_0_SMEM_SIZE -1,
            (MPU_E_MASK|MPU_PERI|MPU_WG_MASK|MPU_RG_MASK)
        }
    },
    {
        /* VM0_1 */
        {
            VM0_1_ROM_BASE,
            VM0_1_ROM_BASE + VM0_1_ROM_SIZE -1,
            (MPU_E_MASK|MPU_ROM|MPU_WG_MASK|MPU_RG_MASK)
        },
        {
            VM0_1_LRAM_BASE,
            VM0_1_LRAM_BASE + VM0_1_LRAM_SIZE -1,
            (MPU_E_MASK|MPU_RAM|MPU_WG_MASK|MPU_RG_MASK)
        },
        {
            VM0_1_CRAM_BASE,
            VM0_1_CRAM_BASE + VM0_1_CRAM_SIZE -1,
            (MPU_E_MASK|MPU_RAM|MPU_WG_MASK|MPU_RG_MASK)
        },
        {
            VM0_1_PERI_BASE,
            VM0_1_PERI_BASE + VM0_1_PERI_SIZE -1,
            (MPU_E_MASK|MPU_PERI|MPU_WG_MASK|MPU_RG_MASK)
        },
        {
            VM0_1_SMEM_BASE,
            VM0_1_SMEM_BASE + VM0_1_SMEM_SIZE -1,
            (MPU_E_MASK|MPU_PERI|MPU_WG_MASK|MPU_RG_MASK)
        }
    }
};


/*
 *  スタック関連
 */

/*
 *  HV処理/アイドル処理用のスタックサイズ
 */
#define HV_STACKSIZE_CORE0        1024

/*
 *  HV+IDLEタイムウィンドウスタック
 */
#pragma ghs section bss=".lbss0"
uint8    hv_stack_core0[HV_STACKSIZE_CORE0];
#pragma ghs section bss=default

uint8 *const hv_stkpt_table[TNUM_PHYS_CORE] = {
    (hv_stack_core0 + HV_STACKSIZE_CORE0),
    NULL,
#if TNUM_PHYS_CORE == 4
    NULL,
    NULL
#endif /* TNUM_PHYS_CORE == 4 */
};

/*
 *  HVタイムウィンドウ処理のスタックサイズ
 */
#define HVTWD_STACKSIZE_CORE0        1024

/*
 *  HVタイムウィンドウ処理のスタック
 */
#pragma ghs section bss=".cbss0"
uint8    hvtwd_stack_core0[HVTWD_STACKSIZE_CORE0];
#pragma ghs section bss=default

uint8 *const hvtwd_stkpt_table[TNUM_PHYS_CORE] = {
    (hvtwd_stack_core0 + HVTWD_STACKSIZE_CORE0),
    NULL,
#if TNUM_PHYS_CORE == 4
    NULL,
    NULL
#endif /* TNUM_PHYS_CORE == 4 */
};

/*
 *  HV割込み初期化ブロック
 */
HVINTINIB hvinib_table[TNUM_HVINT] = {
    {380, 0, 9, true}, /* TAUD0_9 */
    {381, 1, 9,    true}, /* TAUD0_11 */
    {382, 2, 9, true}, /* TAUD0_12 */
    {383, 3, 9, true}, /* TAUD0_13 */
    {709, 0, 9, true}, /* TWTGINT_CORE0 */
};

/*
 *  HV割込みハンドラのINTC1用の登録テーブル
 */
const FP hvint_intc1_core0[INTC1_INTNO_MAX+1] = {
    NULL, /* 0 */
    NULL, /* 1 */
    NULL, /* 2 */
    NULL, /* 3 */
    NULL, /* 4 */
    NULL, /* 5 */
    NULL, /* 6 */
    NULL, /* 7 */
    NULL, /* 8 */
    NULL, /* 9 */
    NULL, /* 10 */
    NULL, /* 11 */
    NULL, /* 12 */
    NULL, /* 13 */
    NULL, /* 14 */
    NULL, /* 15 */
    NULL, /* 16 */
    NULL, /* 17 */
    NULL, /* 18 */
    NULL, /* 19 */
    NULL, /* 20 */
    NULL, /* 21 */
    NULL, /* 22 */
    NULL, /* 23 */
    NULL, /* 24 */
    NULL, /* 25 */
    NULL, /* 26 */
    NULL, /* 27 */
    NULL, /* 28 */
    NULL, /* 29 */
    NULL, /* 30 */
    NULL, /* 31 */
};

const FP *const p_hvint_intc1_table[TNUM_PHYS_CORE] = {
    hvint_intc1_core0,
    NULL,
#if TNUM_PHYS_CORE == 4    
    NULL,
    NULL
#endif /* TNUM_PHYS_CORE == 4 */      
};

/*
 *  HV割込みハンドラのINTC2用の登録テーブル
 */
const FP hvint_intc2_table[INTC2_INTNO_MAX-INTC2_INTNO_MIN+1] = {
    NULL, /* 32 */
    NULL, /* 33 */
    NULL, /* 34 */
    NULL, /* 35 */
    NULL, /* 36 */
    NULL, /* 37 */
    NULL, /* 38 */
    NULL, /* 39 */
    NULL, /* 40 */
    NULL, /* 41 */
    NULL, /* 42 */
    NULL, /* 43 */
    NULL, /* 44 */
    NULL, /* 45 */
    NULL, /* 46 */
    NULL, /* 47 */
    NULL, /* 48 */
    NULL, /* 49 */
    NULL, /* 50 */
    NULL, /* 51 */
    NULL, /* 52 */
    NULL, /* 53 */
    NULL, /* 54 */
    NULL, /* 55 */
    NULL, /* 56 */
    NULL, /* 57 */
    NULL, /* 58 */
    NULL, /* 59 */
    NULL, /* 60 */
    NULL, /* 61 */
    NULL, /* 62 */
    NULL, /* 63 */
    NULL, /* 64 */
    NULL, /* 65 */
    NULL, /* 66 */
    NULL, /* 67 */
    NULL, /* 68 */
    NULL, /* 69 */
    NULL, /* 70 */
    NULL, /* 71 */
    NULL, /* 72 */
    NULL, /* 73 */
    NULL, /* 74 */
    NULL, /* 75 */
    NULL, /* 76 */
    NULL, /* 77 */
    NULL, /* 78 */
    NULL, /* 79 */
    NULL, /* 80 */
    NULL, /* 81 */
    NULL, /* 82 */
    NULL, /* 83 */
    NULL, /* 84 */
    NULL, /* 85 */
    NULL, /* 86 */
    NULL, /* 87 */
    NULL, /* 88 */
    NULL, /* 89 */
    NULL, /* 90 */
    NULL, /* 91 */
    NULL, /* 92 */
    NULL, /* 93 */
    NULL, /* 94 */
    NULL, /* 95 */
    NULL, /* 96 */
    NULL, /* 97 */
    NULL, /* 98 */
    NULL, /* 99 */
    NULL, /* 100 */
    NULL, /* 101 */
    NULL, /* 102 */
    NULL, /* 103 */
    NULL, /* 104 */
    NULL, /* 105 */
    NULL, /* 106 */
    NULL, /* 107 */
    NULL, /* 108 */
    NULL, /* 109 */
    NULL, /* 110 */
    NULL, /* 111 */
    NULL, /* 112 */
    NULL, /* 113 */
    NULL, /* 114 */
    NULL, /* 115 */
    NULL, /* 116 */
    NULL, /* 117 */
    NULL, /* 118 */
    NULL, /* 119 */
    NULL, /* 120 */
    NULL, /* 121 */
    NULL, /* 122 */
    NULL, /* 123 */
    NULL, /* 124 */
    NULL, /* 125 */
    NULL, /* 126 */
    NULL, /* 127 */
    NULL, /* 128 */
    NULL, /* 129 */
    NULL, /* 130 */
    NULL, /* 131 */
    NULL, /* 132 */
    NULL, /* 133 */
    NULL, /* 134 */
    NULL, /* 135 */
    NULL, /* 136 */
    NULL, /* 137 */
    NULL, /* 138 */
    NULL, /* 139 */
    NULL, /* 140 */
    NULL, /* 141 */
    NULL, /* 142 */
    NULL, /* 143 */
    NULL, /* 144 */
    NULL, /* 145 */
    NULL, /* 146 */
    NULL, /* 147 */
    NULL, /* 148 */
    NULL, /* 149 */
    NULL, /* 150 */
    NULL, /* 151 */
    NULL, /* 152 */
    NULL, /* 153 */
    NULL, /* 154 */
    NULL, /* 155 */
    NULL, /* 156 */
    NULL, /* 157 */
    NULL, /* 158 */
    NULL, /* 159 */
    NULL, /* 160 */
    NULL, /* 161 */
    NULL, /* 162 */
    NULL, /* 163 */
    NULL, /* 164 */
    NULL, /* 165 */
    NULL, /* 166 */
    NULL, /* 167 */
    NULL, /* 168 */
    NULL, /* 169 */
    NULL, /* 170 */
    NULL, /* 171 */
    NULL, /* 172 */
    NULL, /* 173 */
    NULL, /* 174 */
    NULL, /* 175 */
    NULL, /* 176 */
    NULL, /* 177 */
    NULL, /* 178 */
    NULL, /* 179 */
    NULL, /* 180 */
    NULL, /* 181 */
    NULL, /* 182 */
    NULL, /* 183 */
    NULL, /* 184 */
    NULL, /* 185 */
    NULL, /* 186 */
    NULL, /* 187 */
    NULL, /* 188 */
    NULL, /* 189 */
    NULL, /* 190 */
    NULL, /* 191 */
    NULL, /* 192 */
    NULL, /* 193 */
    NULL, /* 194 */
    NULL, /* 195 */
    NULL, /* 196 */
    NULL, /* 197 */
    NULL, /* 198 */
    NULL, /* 199 */
    NULL, /* 200 */
    NULL, /* 201 */
    NULL, /* 202 */
    NULL, /* 203 */
    NULL, /* 204 */
    NULL, /* 205 */
    NULL, /* 206 */
    NULL, /* 207 */
    NULL, /* 208 */
    NULL, /* 209 */
    NULL, /* 210 */
    NULL, /* 211 */
    NULL, /* 212 */
    NULL, /* 213 */
    NULL, /* 214 */
    NULL, /* 215 */
    NULL, /* 216 */
    NULL, /* 217 */
    NULL, /* 218 */
    NULL, /* 219 */
    NULL, /* 220 */
    NULL, /* 221 */
    NULL, /* 222 */
    NULL, /* 223 */
    NULL, /* 224 */
    NULL, /* 225 */
    NULL, /* 226 */
    NULL, /* 227 */
    NULL, /* 228 */
    NULL, /* 229 */
    NULL, /* 230 */
    NULL, /* 231 */
    NULL, /* 232 */
    NULL, /* 233 */
    NULL, /* 234 */
    NULL, /* 235 */
    NULL, /* 236 */
    NULL, /* 237 */
    NULL, /* 238 */
    NULL, /* 239 */
    NULL, /* 240 */
    NULL, /* 241 */
    NULL, /* 242 */
    NULL, /* 243 */
    NULL, /* 244 */
    NULL, /* 245 */
    NULL, /* 246 */
    NULL, /* 247 */
    NULL, /* 248 */
    NULL, /* 249 */
    NULL, /* 250 */
    NULL, /* 251 */
    NULL, /* 252 */
    NULL, /* 253 */
    NULL, /* 254 */
    NULL, /* 255 */
    NULL, /* 256 */
    NULL, /* 257 */
    NULL, /* 258 */
    NULL, /* 259 */
    NULL, /* 260 */
    NULL, /* 261 */
    NULL, /* 262 */
    NULL, /* 263 */
    NULL, /* 264 */
    NULL, /* 265 */
    NULL, /* 266 */
    NULL, /* 267 */
    NULL, /* 268 */
    NULL, /* 269 */
    NULL, /* 270 */
    NULL, /* 271 */
    NULL, /* 272 */
    NULL, /* 273 */
    NULL, /* 274 */
    NULL, /* 275 */
    NULL, /* 276 */
    NULL, /* 277 */
    NULL, /* 278 */
    NULL, /* 279 */
    NULL, /* 280 */
    NULL, /* 281 */
    NULL, /* 282 */
    NULL, /* 283 */
    NULL, /* 284 */
    NULL, /* 285 */
    NULL, /* 286 */
    NULL, /* 287 */
    NULL, /* 288 */
    NULL, /* 289 */
    NULL, /* 290 */
    NULL, /* 291 */
    NULL, /* 292 */
    NULL, /* 293 */
    NULL, /* 294 */
    NULL, /* 295 */
    NULL, /* 296 */
    NULL, /* 297 */
    NULL, /* 298 */
    NULL, /* 299 */
    NULL, /* 300 */
    NULL, /* 301 */
    NULL, /* 302 */
    NULL, /* 303 */
    NULL, /* 304 */
    NULL, /* 305 */
    NULL, /* 306 */
    NULL, /* 307 */
    NULL, /* 308 */
    NULL, /* 309 */
    NULL, /* 310 */
    NULL, /* 311 */
    NULL, /* 312 */
    NULL, /* 313 */
    NULL, /* 314 */
    NULL, /* 315 */
    NULL, /* 316 */
    NULL, /* 317 */
    NULL, /* 318 */
    NULL, /* 319 */
    NULL, /* 320 */
    NULL, /* 321 */
    NULL, /* 322 */
    NULL, /* 323 */
    NULL, /* 324 */
    NULL, /* 325 */
    NULL, /* 326 */
    NULL, /* 327 */
    NULL, /* 328 */
    NULL, /* 329 */
    NULL, /* 330 */
    NULL, /* 331 */
    NULL, /* 332 */
    NULL, /* 333 */
    NULL, /* 334 */
    NULL, /* 335 */
    NULL, /* 336 */
    NULL, /* 337 */
    NULL, /* 338 */
    NULL, /* 339 */
    NULL, /* 340 */
    NULL, /* 341 */
    NULL, /* 342 */
    NULL, /* 343 */
    NULL, /* 344 */
    NULL, /* 345 */
    NULL, /* 346 */
    NULL, /* 347 */
    NULL, /* 348 */
    NULL, /* 349 */
    NULL, /* 350 */
    NULL, /* 351 */
    NULL, /* 352 */
    NULL, /* 353 */
    NULL, /* 354 */
    NULL, /* 355 */
    NULL, /* 356 */
    NULL, /* 357 */
    NULL, /* 358 */
    NULL, /* 359 */
    NULL, /* 360 */
    NULL, /* 361 */
    NULL, /* 362 */
    NULL, /* 363 */
    NULL, /* 364 */
    NULL, /* 365 */
    NULL, /* 366 */
    NULL, /* 367 */
    NULL, /* 368 */
    NULL, /* 369 */
    NULL, /* 370 */
    NULL, /* 371 */
    NULL, /* 372 */
    NULL, /* 373 */
    NULL, /* 374 */
    NULL, /* 375 */
    NULL, /* 376 */
    NULL, /* 377 */
    NULL, /* 378 */
    NULL, /* 379 */
    hvint0_handler, /* 380 TAUD0_9  */
    hvint0_handler, /* 381 TAUD0_11 */
    hvint0_handler, /* 382 TAUD0_12 */
    hvint0_handler, /* 383 TAUD0_13 */
    NULL, /* 384 */
    NULL, /* 385 */
    NULL, /* 386 */
    NULL, /* 387 */
    NULL, /* 388 */
    NULL, /* 389 */
    NULL, /* 390 */
    NULL, /* 391 */
    NULL, /* 392 */
    NULL, /* 393 */
    NULL, /* 394 */
    NULL, /* 395 */
    NULL, /* 396 */
    NULL, /* 397 */
    NULL, /* 398 */
    NULL, /* 399 */
    NULL, /* 400 */
    NULL, /* 401 */
    NULL, /* 402 */
    NULL, /* 403 */
    NULL, /* 404 */
    NULL, /* 405 */
    NULL, /* 406 */
    NULL, /* 407 */
    NULL, /* 408 */
    NULL, /* 409 */
    NULL, /* 410 */
    NULL, /* 411 */
    NULL, /* 412 */
    NULL, /* 413 */
    NULL, /* 414 */
    NULL, /* 415 */
    NULL, /* 416 */
    NULL, /* 417 */
    NULL, /* 418 */
    NULL, /* 419 */
    NULL, /* 420 */
    NULL, /* 421 */
    NULL, /* 422 */
    NULL, /* 423 */
    NULL, /* 424 */
    NULL, /* 425 */
    NULL, /* 426 */
    NULL, /* 427 */
    NULL, /* 428 */
    NULL, /* 429 */
    NULL, /* 430 */
    NULL, /* 431 */
    NULL, /* 432 */
    NULL, /* 433 */
    NULL, /* 434 */
    NULL, /* 435 */
    NULL, /* 436 */
    NULL, /* 437 */
    NULL, /* 438 */
    NULL, /* 439 */
    NULL, /* 440 */
    NULL, /* 441 */
    NULL, /* 442 */
    NULL, /* 443 */
    NULL, /* 444 */
    NULL, /* 445 */
    NULL, /* 446 */
    NULL, /* 447 */
    NULL, /* 448 */
    NULL, /* 449 */
    NULL, /* 450 */
    NULL, /* 451 */
    NULL, /* 452 */
    NULL, /* 453 */
    NULL, /* 454 */
    NULL, /* 455 */
    NULL, /* 456 */
    NULL, /* 457 */
    NULL, /* 458 */
    NULL, /* 459 */
    NULL, /* 460 */
    NULL, /* 461 */
    NULL, /* 462 */
    NULL, /* 463 */
    NULL, /* 464 */
    NULL, /* 465 */
    NULL, /* 466 */
    NULL, /* 467 */
    NULL, /* 468 */
    NULL, /* 469 */
    NULL, /* 470 */
    NULL, /* 471 */
    NULL, /* 472 */
    NULL, /* 473 */
    NULL, /* 474 */
    NULL, /* 475 */
    NULL, /* 476 */
    NULL, /* 477 */
    NULL, /* 478 */
    NULL, /* 479 */
    NULL, /* 480 */
    NULL, /* 481 */
    NULL, /* 482 */
    NULL, /* 483 */
    NULL, /* 484 */
    NULL, /* 485 */
    NULL, /* 486 */
    NULL, /* 487 */
    NULL, /* 488 */
    NULL, /* 489 */
    NULL, /* 490 */
    NULL, /* 491 */
    NULL, /* 492 */
    NULL, /* 493 */
    NULL, /* 494 */
    NULL, /* 495 */
    NULL, /* 496 */
    NULL, /* 497 */
    NULL, /* 498 */
    NULL, /* 499 */
    NULL, /* 500 */
    NULL, /* 501 */
    NULL, /* 502 */
    NULL, /* 503 */
    NULL, /* 504 */
    NULL, /* 505 */
    NULL, /* 506 */
    NULL, /* 507 */
    NULL, /* 508 */
    NULL, /* 509 */
    NULL, /* 510 */
    NULL, /* 511 */
    NULL, /* 512 */
    NULL, /* 513 */
    NULL, /* 514 */
    NULL, /* 515 */
    NULL, /* 516 */
    NULL, /* 517 */
    NULL, /* 518 */
    NULL, /* 519 */
    NULL, /* 520 */
    NULL, /* 521 */
    NULL, /* 522 */
    NULL, /* 523 */
    NULL, /* 524 */
    NULL, /* 525 */
    NULL, /* 526 */
    NULL, /* 527 */
    NULL, /* 528 */
    NULL, /* 529 */
    NULL, /* 530 */
    NULL, /* 531 */
    NULL, /* 532 */
    NULL, /* 533 */
    NULL, /* 534 */
    NULL, /* 535 */
    NULL, /* 536 */
    NULL, /* 537 */
    NULL, /* 538 */
    NULL, /* 539 */
    NULL, /* 540 */
    NULL, /* 541 */
    NULL, /* 542 */
    NULL, /* 543 */
    NULL, /* 544 */
    NULL, /* 545 */
    NULL, /* 546 */
    NULL, /* 547 */
    NULL, /* 548 */
    NULL, /* 549 */
    NULL, /* 550 */
    NULL, /* 551 */
    NULL, /* 552 */
    NULL, /* 553 */
    NULL, /* 554 */
    NULL, /* 555 */
    NULL, /* 556 */
    NULL, /* 557 */
    NULL, /* 558 */
    NULL, /* 559 */
    NULL, /* 560 */
    NULL, /* 561 */
    NULL, /* 562 */
    NULL, /* 563 */
    NULL, /* 564 */
    NULL, /* 565 */
    NULL, /* 566 */
    NULL, /* 567 */
    NULL, /* 568 */
    NULL, /* 569 */
    NULL, /* 570 */
    NULL, /* 571 */
    NULL, /* 572 */
    NULL, /* 573 */
    NULL, /* 574 */
    NULL, /* 575 */
    NULL, /* 576 */
    NULL, /* 577 */
    NULL, /* 578 */
    NULL, /* 579 */
    NULL, /* 580 */
    NULL, /* 581 */
    NULL, /* 582 */
    NULL, /* 583 */
    NULL, /* 584 */
    NULL, /* 585 */
    NULL, /* 586 */
    NULL, /* 587 */
    NULL, /* 588 */
    NULL, /* 589 */
    NULL, /* 590 */
    NULL, /* 591 */
    NULL, /* 592 */
    NULL, /* 593 */
    NULL, /* 594 */
    NULL, /* 595 */
    NULL, /* 596 */
    NULL, /* 597 */
    NULL, /* 598 */
    NULL, /* 599 */
    NULL, /* 600 */
    NULL, /* 601 */
    NULL, /* 602 */
    NULL, /* 603 */
    NULL, /* 604 */
    NULL, /* 605 */
    NULL, /* 606 */
    NULL, /* 607 */
    NULL, /* 608 */
    NULL, /* 609 */
    NULL, /* 610 */
    NULL, /* 611 */
    NULL, /* 612 */
    NULL, /* 613 */
    NULL, /* 614 */
    NULL, /* 615 */
    NULL, /* 616 */
    NULL, /* 617 */
    NULL, /* 618 */
    NULL, /* 619 */
    NULL, /* 620 */
    NULL, /* 621 */
    NULL, /* 622 */
    NULL, /* 623 */
    NULL, /* 624 */
    NULL, /* 625 */
    NULL, /* 626 */
    NULL, /* 627 */
    NULL, /* 628 */
    NULL, /* 629 */
    NULL, /* 630 */
    NULL, /* 631 */
    NULL, /* 632 */
    NULL, /* 633 */
    NULL, /* 634 */
    NULL, /* 635 */
    NULL, /* 636 */
    NULL, /* 637 */
    NULL, /* 638 */
    NULL, /* 639 */
    NULL, /* 640 */
    NULL, /* 641 */
    NULL, /* 642 */
    NULL, /* 643 */
    NULL, /* 644 */
    NULL, /* 645 */
    NULL, /* 646 */
    NULL, /* 647 */
    NULL, /* 648 */
    NULL, /* 649 */
    NULL, /* 650 */
    NULL, /* 651 */
    NULL, /* 652 */
    NULL, /* 653 */
    NULL, /* 654 */
    NULL, /* 655 */
    NULL, /* 656 */
    NULL, /* 657 */
    NULL, /* 658 */
    NULL, /* 659 */
    NULL, /* 660 */
    NULL, /* 661 */
    NULL, /* 662 */
    NULL, /* 663 */
    NULL, /* 664 */
    NULL, /* 665 */
    NULL, /* 666 */
    NULL, /* 667 */
    NULL, /* 668 */
    NULL, /* 669 */
    NULL, /* 670 */
    NULL, /* 671 */
    NULL, /* 672 */
    NULL, /* 673 */
    NULL, /* 674 */
    NULL, /* 675 */
    NULL, /* 676 */
    NULL, /* 677 */
    NULL, /* 678 */
    NULL, /* 679 */
    NULL, /* 680 */
    NULL, /* 681 */
    NULL, /* 682 */
    NULL, /* 683 */
    NULL, /* 684 */
    NULL, /* 685 */
    NULL, /* 686 */
    NULL, /* 687 */
    NULL, /* 688 */
    NULL, /* 689 */
    NULL, /* 690 */
    NULL, /* 691 */
    NULL, /* 692 */
    NULL, /* 693 */
    NULL, /* 694 */
    NULL, /* 695 */
    NULL, /* 696 */
    NULL, /* 697 */
    NULL, /* 698 */
    NULL, /* 699 */
    NULL, /* 700 */
    NULL, /* 701 */
    NULL, /* 702 */
    NULL, /* 703 */
    NULL, /* 704 */
    NULL, /* 705 */
    NULL, /* 706 */
    NULL, /* 707 */
    NULL, /* 708 */
    twdint_handler, /* 709 */
    NULL, /* 710 */
    NULL, /* 711 */
    NULL, /* 712 */
    NULL, /* 713 */
    NULL, /* 714 */
    NULL, /* 715 */
    NULL, /* 716 */
    NULL, /* 717 */
    NULL, /* 718 */
    NULL, /* 719 */
    NULL, /* 720 */
    NULL, /* 721 */
    NULL, /* 722 */
    NULL, /* 723 */
    NULL, /* 724 */
    NULL, /* 725 */
    NULL, /* 726 */
    NULL, /* 727 */
    NULL, /* 728 */
    NULL, /* 729 */
    NULL, /* 730 */
    NULL, /* 731 */
    NULL, /* 732 */
    NULL, /* 733 */
    NULL, /* 734 */
    NULL, /* 735 */
    NULL, /* 736 */
    NULL, /* 737 */
    NULL, /* 738 */
    NULL, /* 739 */
    NULL, /* 740 */
    NULL, /* 741 */
    NULL, /* 742 */
    NULL, /* 743 */
    NULL, /* 744 */
    NULL, /* 745 */
    NULL, /* 746 */
    NULL, /* 747 */
    NULL, /* 748 */
    NULL, /* 749 */
    NULL, /* 750 */
    NULL, /* 751 */
    NULL, /* 752 */
    NULL, /* 753 */
    NULL, /* 754 */
    NULL, /* 755 */
    NULL, /* 756 */
    NULL, /* 757 */
    NULL, /* 758 */
    NULL, /* 759 */
    NULL, /* 760 */
    NULL, /* 761 */
    NULL, /* 762 */
    NULL, /* 763 */
    NULL, /* 764 */
    NULL, /* 765 */
    NULL, /* 766 */
    NULL, /* 767 */
};

/*
 *  HVC登録テーブル(hv_cfg.c)
 */
void * const hvc_table[TNUM_HVCID] = {
    (void *) &hvc_test0,
    (void *) &hvc_test1,
    (void *) &hvc_test2,
    (void *) &hvc_test3,
    (void *) &ReadStateVariable,
    (void *) &WriteStateVariable,
    (void *) &DeactivateStateVariable,
    (void *) &ReadMessageQueue,
    (void *) &WriteMessageQueue,
    (void *) &DeactivateMessageQueue,
};

/*
 *  状態変数
 */

const ID tmax_svarid = (TMIN_SVARID + TNUM_SVARID - 1);

static uintptr svarbuf_SVAR1[VMCOM_COUNT_SZ(4, sizeof(uintptr))];
static uintptr svarbuf_SVAR2[VMCOM_COUNT_SZ(8, sizeof(uintptr))];
static uintptr svarbuf_SVAR3[VMCOM_COUNT_SZ(32, sizeof(uintptr))];
static uintptr svarbuf_SVAR4[VMCOM_COUNT_SZ(64, sizeof(uintptr))];

const SVARINIB    svarinib_table[TNUM_SVARID] = {
    {4,  1, false, svarbuf_SVAR1},
    {32, 1, false, svarbuf_SVAR2},
    {32, 2, false, svarbuf_SVAR3},
    {64, 2, false, svarbuf_SVAR4},
};

static SVARCB svarcb_SVAR1;
static SVARCB svarcb_SVAR2;
static SVARCB svarcb_SVAR3;
static SVARCB svarcb_SVAR4;

SVARCB *const    p_svarcb_table[TNUM_SVARID] = {
    &svarcb_SVAR1,
    &svarcb_SVAR2,
    &svarcb_SVAR3,
    &svarcb_SVAR4
};

/*
 *  メッセージキュー
 */

const ID tmax_msgqid = (TMIN_MSGQID + TNUM_MSGQID - 1);

static uintptr msgqbuf_MSGQ1[VMCOM_COUNT_SZ(32, sizeof(uintptr))];
static uintptr msgqbuf_MSGQ2[VMCOM_COUNT_SZ(64, sizeof(uintptr))];
static uintptr msgqbuf_MSGQ3[VMCOM_COUNT_SZ(128, sizeof(uintptr))];
static uintptr msgqbuf_MSGQ4[VMCOM_COUNT_SZ(256, sizeof(uintptr))];

const MSGQINIB    msgqinib_table[TNUM_MSGQID] = {
    {8,   32, 1, 2, false, msgqbuf_MSGQ1},
    {16,  64, 2, 1, false, msgqbuf_MSGQ2},
    {128, 512, 1, 2, false, msgqbuf_MSGQ3},
    {64,  256, 2, 1, false, msgqbuf_MSGQ4},
};

static MSGQCB msgqcb_MSGQ1;
static MSGQCB msgqcb_MSGQ2;
static MSGQCB msgqcb_MSGQ3;
static MSGQCB msgqcb_MSGQ4;

MSGQCB *const    p_msgqcb_table[TNUM_MSGQID] = {
    &msgqcb_MSGQ1,
    &msgqcb_MSGQ2,
    &msgqcb_MSGQ3,
    &msgqcb_MSGQ4
};
