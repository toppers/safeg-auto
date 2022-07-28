#ifndef _VM_CFG_H_
#define _VM_CFG_H_

/*
 * 将来的にはコンフィギュレーションファイルから自動生成する想定． 
 */

/*
 *  サポートするコア数
 */
#define TNUM_SUPPORT_CORE     0x01

/*
 *  サポートするコアのビットパターン
 */
#define TBIT_SUPPORT_CORE    0x01

/*
 *  リーダコアのコアID
 */
#define LEADER_COREID    0

/*
 *  シングルVMコアとするコアのビットパターン
 */
#define TBIT_SINGLEVM_CORE        0x00

/*
 *  各コアのSPID
 */
#define HV_SPID_CORE0        0

/*
 *  全コアのHVのSPIDのビットパターンのOR
 */
#define TBIT_HV_SPID        0x00000001

/*
 *  各コアの仮想マシンのSPIDのビットパターンのOR
 */
#define TBIT_VM_SPID_CORE0        0x00000030

/*
 *  全コアのHVと仮想マシンのSPIDのビットパターンのOR
 */
#define TBIT_SPID                0x00000031

/*
 *  CRAMの使用サイズ
 */
#define CRAM_USAGE_SIZE_0        0x0003FFFF

/*
 *  VMの数
 */
#define TNUM_VM                2
#define TNUM_VM_CORE0        2

/*
 *  VMID
 */
#define VMID_VM0_0        1
#define VMID_VM0_1        2

/*
 *  使用するホストエントリMPUの個数
 */
#define TNUM_HEMPU        5

/*
 *  VM切替時のホストエントリMPUの終了番号
 */
#define TMAX_HEMPU        4

/*
 *  HVCの個数
 */
#define TNUM_HVCID        11

/*
 *  各VMのROMのベースアドレス
 */
#define VM0_0_ROM_BASE        0x00100000
#define VM0_1_ROM_BASE        0x00200000

/*
 *  各VMのROMのサイズ
 */
#define VM0_0_ROM_SIZE        0x00100000
#define VM0_1_ROM_SIZE        0x00100000

/*
 *  各VMのLRAMのベースアドレス
 */
#define VM0_0_LRAM_BASE        0xfdc04000
#define VM0_1_LRAM_BASE        0xfdc08000

/*
 *  各VMのRAMのサイズ
 */
#define VM0_0_LRAM_SIZE        0x00004000
#define VM0_1_LRAM_SIZE        0x00004000

/*
 *  各VMのCRAMのベースアドレス
 */
#define VM0_0_CRAM_BASE        0xfe020000
#define VM0_1_CRAM_BASE        0xfe030000

/*
 *  各VMのRAMのサイズ
 */
#define VM0_0_CRAM_SIZE        0x00010000
#define VM0_1_CRAM_SIZE        0x00010000

/*
 *  各VMの周辺回路のベースアドレス
 */
#define VM0_0_PERI_BASE        0xff000000
#define VM0_1_PERI_BASE        0xff000000

/*
 *  各VMの周辺回路のサイズ
 */
#define VM0_0_PERI_SIZE        0x01000000
#define VM0_1_PERI_SIZE        0x01000000

/*
 * 各VMの共有メモリ領域のベースアドレス
 */
#define VM0_0_SMEM_BASE        0xfe070000
#define VM0_1_SMEM_BASE        0xfe070000

/*
 * 各VMの共有メモリ領域のサイズ
 */
#define VM0_0_SMEM_SIZE        0x00010000
#define VM0_1_SMEM_SIZE        0x00010000

/*
 *  各VMのGPID
 */
#define VM0_0_GPID        1
#define VM0_1_GPID        2

/*
 *  各VMのSPIDLIST(ビット定義)
 */
#define VM0_0_SPIDLIST        0x00000010
#define VM0_1_SPIDLIST        0x00000020

/*
 *  各VMのSPID
 */
#define VM0_0_SPID        4
#define VM0_1_SPID        5

/*
 *  VM割込みの数
 */
#define TNUM_VMINT        4

/*
 *  HV割込みの数
 */
#define TNUM_HVINT        5

/*
 *  タイムウインドウ数
 */
#define TNUM_TW_CORE0        3

/*
 *  システム周期(us)
 */
#define SYSTEM_INTERVAL_US        10000000U

/*
 *  VM間通信に関する定義
 */
#define TNUM_SVARID        4
#define TNUM_MSGQID        4

#endif /* _VM_CFG_H_ */
