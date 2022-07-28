# ATK2向けのプロジェクトの詳細

## 使用方法

Makefile.hv に

## 使用リソース

### vm1_atk2
- コア
 - 0
- VMID
 - 1
- メモリ
 - ROM   : 0x00100000 : 1M
 - LRAM  : 0xFDC04000 : 16KB
 - CRAM  : 0xFE020000 : 64KB
- UART
 - RLIN30
- タイマ
 - TAUJ1

### vm2_atk2
- コア
 - 0
- VMID
 - 1
- メモリ
 - ROM   : 0x00200000 : 1M
 - LRAM  : 0xFDC08000 : 16KB
 - CRAM  : 0xFE030000 : 64KB
- UART
 - RLIN31
- タイマ
 - TAUJ3

### vm3_atk2
- コア
 - 1
- VMID
 - 3
- メモリ
 - ROM   : 0x00500000 : 1M
 - LRAM  : 0xFDA04000 : 16KB
 - CRAM  : 0xFE040000 : 64KB
- UART
 - RLIN32
- タイマ
 - TAUJ0

### vm4_atk2
- コア
 - 1
- VMID
 - 4
- メモリ
 - ROM   : 0x00600000 : 1M
 - LRAM  : 0xFDA08000 : 16KB
 - CRAM  : 0xFE050000 : 64KB
- UART
 - RLIN33
- タイマ
 - TAUJ2

以上．
