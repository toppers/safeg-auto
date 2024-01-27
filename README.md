 # Safeg-Auto ユーザーズマニュアル

# このドキュメントについて

Safeg-Auto の使用方法・使用リソース・ファイル構成構成について説明する．

# 動作環境

- チップ
  - U2A
- 評価ボード
  - Renesas T1V1
    - CHIP             : U2A16
    - PiggyBack Board  : Y-RH850-U2A-516PIN-PB-T1-V1 
    - Main Board       : Y-RH850-X1X-MB-T1-V1
  - Sunny GPXU2A
    - S810-GPX-U2A 
      - CHIP             : U2A8
  - Renesas R-Car S4 Reference Board/Spider(R-Car S4 Spider)
- デバッガ
  - E2
  - CS+ for CC : V8.07.00
- コンパイラ
  - GHS : 201915
  - CCHR : V2.04.00
- ビルド環境
  - 以下のツールが使用できる環境
    - GNU Make 4.3 以降
    - Python 3.8 以降
  - Cygwin/Linuxで動作確認
     - Ubuntu 20.04.4 LTS

- ターミナルエミュレータ
  - TeraTerm 
- USB-UART x 4
  - https://www.switch-science.com/catalog/1196/

# セットアップ

## Renesas T1V1

- ボードのジャンパを適切に設定する．
- E2を接続
- USB-UARTを接続

| ヘッダ 1 | ヘッダ 2 |
| ------------- | ------------- |
| 内容セル  | 内容セル  |
| 内容セル  | 内容セル  |


| RLIN | ポート | ピン番号 | USB-UART |
| ------------- | ------------- | ------------- | ------------- |
| RLIN30TX | P10_4 | CN16 B26 | USB-UART0-RX |
| RLIN30RX | P10_5 | CN16 B27 | USB-UART0-TX |
| RLIN31TX | P2_5 | CN15 A30 | USB-UART1-RX |
| RLIN31RX | P2_4 | CN15 C27 | USB-UART1-TX |
| RLIN32TX | P2_0 | CN15 A27 | USB-UART2-RX |
| RLIN32RX | P2_1 | CN15 A23 | USB-UART2-TX |
| RLIN33TX | P4_14 | CN13 B17 | USB-UART3-RX |
| RLIN34RX | P4_15 | CN13 B19 | USB-UART3-TX |

## Sunny GPXU2A

- ボードのジャンパを適切に設定する．
- E2を接続
- USB-UARTを接続

| RLIN | ポート | ピン番号 | USB-UART |
| ------------- | ------------- | ------------- | ------------- |
| RLIN34TX | P6_12 | N19 | USB-UART0-RX |
| RLIN34RX | P6_15 | M20 | USB-UART0-TX |
| RLIN32RX | P21_4 | D5 | USB-UART0-TX |
| RLIN32TX | P21_5 | E4 | USB-UART0-RX |
| RLIN311RX | P3_7 | L16 | USB-UART0-TX |
| RLIN311TX | P3_8 | G19 | USB-UART0-RX |


## R-Car S4 Spider

- T.B.D

## 共通

- 各ポートにターミナルエミュレータを開いて  115200bps で接続．
- pip等でPyYAMLをインストール．
   - $ pip install pyyaml

## CC-RH on Linux
Linux上CC-RHを実行する際には次の手順で環境を整える

- wine のインストール
  - sudo apt install wine

- CCRHのインストール
  - Windowsでインストールして以下のファイルをLinux環境にコピー
    - C:\Program Files (x86)\Renesas Electronics\CS+\CC\CC-RH 
  - インストーラをLinux上で実行する方法も可能．

- ライブラリパスを環境変数で設定(バージョンは適宜読み替える)
  - export HLNK_DIR=/home/honda/.wine/ccrh/V2.04.00  

- パスの通った場所にシェルスクリプトを用意
  - 実行属性を付ける

rlink

	#!/bin/bash
	wine /home/honda/.wine/ccrh/V2.03.00/bin/rlink.exe "$@"


ccrh

	#!/bin/bash
	wine /home/honda/.wine/ccrh/V2.03.00/bin/ccrh.exe "$@"


# プロジェクト

サポートするコアやVMの構成の違いにより次の種類のプロジェクトがある．

- 1PE
  - 1コアで動作するプロジェクト．
  - VMを2個実行．
  - IDLE処理でIDLE用のVMを実行．
- 1PE_COM
  - 1コアで動作するプロジェクト．
  - VMを2個実行．
  - VM間でVM通信を用いた通信を行う．
- 1PE_IVC
  - 1コアで動作するプロジェクト．
  - VMを2個実行．
  - VM間でIVC通信を用いた通信を行う．
- 1PE_ATK2
  - 1コアで動作するプロジェクト．
  - ATK2-SC1を用いたVMを2個実行．
- 2PE
  - 2コアで動作するプロジェクト．
  - 各コアでVMを2個実行（合計4個）．
- 2PE_MVM
  - 2コアで動作するプロジェクト．
  - 各コアでVMを2個実行（合計4個）．
  - 各コアで同じタイミングで動作するVMはマルチコアVMとなっており，同一バイナリで動作する．
- 2PE_SVM
  - 2コアで動作するプロジェクト．
  - コア0でVMを2個実行．
  - コア1はシングルVMコアとしてVMを1個実行．
- 2PE_ATK2
  - 2コアで動作するプロジェクト．
  - ATK2-SC1を用いたVMを各コアで2個実行．
  - Sunny GPXU2A

各プロジェクトはハイパーバイザのバイナリとゲストのバイナリで構成されている．

それぞれのバイナリが生成されるディレクトリを以下に示す．このディレクトリ以下にソースコード等が置かれている．

ハイパーバイザディレクトリは，./hypervisor/obj 以下にあり，ゲストディレクトリは ./vm_prog 以下にある．

| 名称 | ハイパーバイザディレクトリ| ゲストディレクトリ
| ---  | ---------------------  | --------------
| 1PE      | obj_1pe            | vm0_0_prog/vm0_1_prog
| 1PE_COM  | obj_1pe_com        | vm0_0_com_prog/vm0_1_com_prog
| 1PE_IVC  | obj_1pe_ivc        | vm0_0_ivc_prog/vm0_1_ivc_prog
| 2PE      | obj_2pe            | vm0_0_prog/vm0_1_prog/vm1_0_prog/vm1_1_prog
| 2PE_MVM  | obj_2pe_mvm        | mvm0_prog/mvm1_prog
| 2PE_SVM  | obj_2pe_svm        | vm0_0_prog/vm0_1_prog/vm1_0_prog
| 1PE_ATK2 | obj_1pe_atk2       | vm1_atk2/vm2_atk2
| 2PE_ATK2 | obj_2pe_atk2       | vm1_atk2/vm2_atk2/vm3_atk2/vm4_atk2


# ビルド方法

トップディレクトリにある Makefile.config を編集して使用する評価ボードのコメントを外す．

	TARGET = renesas_t1v1
	#TARGET = sunny_gpxu2a
	#TARGET = renesas_s4spider

コンパイラを指定する．

	COMPILER = GHS
	#COMPILER = CCRH

トップディレクトリにある Makefile を編集してビルド対象のプロジェクトコメントアウトを外す．

	HV_OBJ = obj_1pe   #1コアで2個の仮想マシンを実行
	#HV_OBJ = obj_2pe  #2コアで各コアで2個の仮想マシンを実行

## ATK2を使用しないプロジェクトの場合
make 

を実行してビルドする．

## ATK2を使用するプロジェクトの場合

make depend

make 

を実行してビルドする．一度 make depend を実行した後は，以降ファイルを変更した後の再ビルドする場合は make を実行するだけで良い．

# 実行

ハイパーバイザディレクトリにあるCS+のプロジェクトをCS＋で開いてダウンロードして実行する．

# 使用リソース

## ハイパーバイザ

全てのプロジェクトにおいてハイパーバイザが使用するリソースは同じである．

- SPID
  - 0 : CORE0
  - 1 : CORE1
- TAUD0
  - チャネル1  : CORE0 システム周期タイマ 
  - チャネル3  : CORE1 システム周期タイマ
  - チャネル9  : CORE0 HV割込み用タイマ
  - チャネル11 : CORE1 HV割込み用タイマ
- TPTM
  - TPTU チャネル0 : 各コアのタイムウィンドウタイマ
- UART
  - Renesas T1V1
    - RLIN30 : RLIN3_PORT0として使用
    - RLIN31 : RLIN3_PORT1として使用
    - RLIN32 : RLIN3_PORT2として使用
    - RLIN33 : RLIN3_PORT3として使用
  - Sunny GPXU2A
    - RLIN37 : RLIN3_PORT0として使用
    - RLIN34 : RLIN3_PORT1として使用
    - RLIN32 : RLIN3_PORT2として使用
    - RLIN311 : RLIN3_PORT3として使用
- 割込み番号
  - 709 : CORE0 HVタイムウィンドウ用タイムウィンドウトリガ割込み
  - 719 : CORE1 HVタイムウィンドウ用タイムウィンドウトリガ割込み
- FLASH 
  - 1MB  : 0x00000000～0x000fffff
- LRAM  
  - 16KB  : 0xfdc00000～0xfdc03fff
  - 16KB  : 0xfda00000～0xfda03fff
- CRAM
  - 128KB : 0xfe000000～0xfe01ffff

### ゲスト

#### vm0_0_prog/vm0_0_com_prog/vm0_0_ivc_prog
- OSTM
 - チャネル0
- UART
  - RLIN3_PORT0 : コンソール出力 
- 割込み番号
  - 707 : VMタイムウィンドウ用タイムウィンドウトリガ割込み  
- FLASH 
  - 1MB : 0x00100000～0x001fffff
- LRAM  
  - 16KB  : 0xfdc04000～0xfdc07fff
- CRAM
  - 64KB : 0xfe020000～0xfe02ffff 

#### vm0_1_prog/vm0_1_com_prog/vm0_1_ivc_prog
- OSTM
 - チャネル1
- UART
  - RLIN3_PORT0 : コンソール出力 
- 割込み番号
  - 708 : VMタイムウィンドウ用タイムウィンドウトリガ割込み  
- FLASH 
  - 1MB  : 0x00200000～0x002fffff
- LRAM  
  - 16KB : 0xfdc08000～0xfdc0bfff
- CRAM
  - 64KB : 0xfe030000～0xfe03ffff 

### vm0_idle
- UART
  - RLIN3_PORT0 : CORE0 コンソール出力 
- FLASH 
  - 1MB  : 0x00300000～0x003fffff
- LRAM  
  - 16KB : 0xfdc0c000～0xfdc0ffff
- CRAM
  - 64KB : 0xfe040000～0xfe05ffff   

#### vm1_0_prog
- OSTM
 - チャネル2
- UART
  - RLIN3_PORT1 : コンソール出力 
- 割込み番号
  - 717 : VMタイムウィンドウ用タイムウィンドウトリガ割込み  
- FLASH 
  - 1MB : 0x00500000～0x005fffff
- LRAM  
  - 16KB : 0xfda04000～0xfda07fff
- CRAM
  - 64KB : 0xfe040000～0xfe04ffff

#### vm1_1_prog
- OSTM
 - チャネル3
- UART
  - RLIN3_PORT1 : コンソール出力 
- 割込み番号
  - 718 : VMタイムウィンドウ用タイムウィンドウトリガ割込み  
- FLASH 
  - 1MB : 0x00600000～0x006fffff
- LRAM  
  - 16KB : 0xfda08000～0xfda0bfff
- CRAM
  - 64KB : 0xfe050000～0xfe05ffff

#### mvm0_prog
- OSTM
 - チャネル0 CORE0 VM割込み
 - チャネル2 CORE1 VM割込み
- UART
  - RLIN3_PORT0 : CORE0 コンソール出力 
  - RLIN3_PORT1 : CORE1 コンソール出力 
- 割込み番号
  - 707 : CORE0 VMタイムウィンドウ用タイムウィンドウトリガ割込み  
  - 708 : CORE1 VMタイムウィンドウ用タイムウィンドウトリガ割込み    
- FLASH 
  - 1MB : 0x00100000～0x001fffff
- LRAM  
  - 16KB  : 0xfdc04000～0xfdc07fff
- CRAM
  - 64KB : 0xfe020000～0xfe03ffff 

#### mvm1_prog
- OSTM
 - チャネル1 CORE0 VM割込み
 - チャネル3 CORE1 VM割込み
- UART
  - RLIN3_PORT0 : CORE0 コンソール出力 
  - RLIN3_PORT1 : CORE1 コンソール出力 
- 割込み番号
  - 717 : CORE0 VMタイムウィンドウ用タイムウィンドウトリガ割込み  
  - 718 : CORE1 VMタイムウィンドウ用タイムウィンドウトリガ割込み    
- FLASH 
  - 1MB  : 0x00200000～0x002fffff
- LRAM  
  - 16KB : 0xfda04000～0xfda07fff
- CRAM
  - 64KB : 0xfe040000～0xfe05ffff 


# ファイル構成

- hyperviosr : ハイパーバイザのソースコード
  - doc      : ドキュメント
  - include  : ユーザーホストコード用のインクルードファイル
    - common_def.h : ハイパーバイザー・ゲスト用共通定義のヘッダファイル
    - hv.h : ハイパーバイザー定義のヘッダファイル
  - libguest : ゲスト用のファイル
    - hvc.h : HVコール用ヘッダファイル
    - hvc_asm.S : HVコール用アセンブリファイル
    - ivccom_def.h : IVC通信のゲスト用ヘッダファイル
    - ivccom_guest.c : IVC通信のゲスト用ソースファイル
    - ivccom_guest.h : IVC通信のゲスト用ヘッダファイル
    - libguest.h : ゲスト用共通ヘッダファイル
    - vmcom_def.h : VM間通信の定義ファイル
    - vmcom_guest.h : VM間通信のゲスト用ヘッダファイル
  - src      : ソースコード本体
    - hv_impl.c : ハイパーバイザー本体のソースファイル
    - hv_impl.h : ハイパーバイザー本体のヘッダファイル
    - hv_service.c : サービスのソースコード
    - hv_support.S : ハイパーバイザーのアセンブリコード
    - hvtimer_taud.h : ハイパーバイザー用タイマTAUD用ドライバ
    - hvtimer_tptm.h : ハイパーバイザー用タイマTPTM用ドライバ
    - ivccom.c : IVC通信のソースファイル
    - ivccom.h : IVC通信のヘッダーファイル
    - Makefile.hv : ハイパーバイザー用のMakefile
    - rvector_pe01.S : PE0/1用のリセットベクタ
    - rvector_pe23.S : PE2/3用のリセットベクタ    
    - start.S : スタートアップコード
    - startup.c : 起動処理のソースコード
    - u2a8_ghs.ld : U2A8用リンカスクリプト
    - u2a16_ghs.ld : U2A16用リンカスクリプト    
    - vector.S : ベクタテーブル
    - vmcom.c : VM間通信のソースファイル
    - vmcom.h : VM間通信のヘッダーファイル
  - target   
    - renesas_t1v1 : Renesas-T1V1依存部
	  - Makefile.target  : Makefileのターゲット依存部
	  - rvector_pe01.S   : PE01用リセットベクタ
	  - rvector_pe23.S   : PE23用リセットベクタ
	  - start.S          : スタートアップコード
	  - target_hv.h      : ターゲット依存のユーザーホスト処理用定義のヘッダファイル
	  - target_hv_impl.c : ターゲット依存のハイパーバイザー用定義のソースファイル
	  - target_hv_impl.h : ターゲット依存のハイパーバイザー用定義のヘッダファイル
	  - u2aeva_ghs.ld : ハイパーバイザー用リンカスクリプト
  - obj      : ハイパーバイザのプロジェクト
    - obj_1pe
	  - cs_plus.mtpj : CS+プロジェクトファイル
	  - hv_cfg.c  : HVコンフィギュレーションソースファイル
	  - hv_cfg.h  : HVコンフィギュレーションヘッダーファイル
	  - hvc_cfg.h : HVコールコンフィギュレーションヘッダーファイル
	  - Makefile  : ハイパーバイザービルド用Makefile
	  - sample1.c : ユーザーホスト処理ソースファイル
	  - sample1.h : ユーザーホスト処理ヘッダーファイル
    - obj_1pe_com
    - obj_1pe_ivc
	- obj_2pe
	- obj_2pe_mvm
	- obj_2pe_svm
- common     : ハイパーバイザ・ゲスト共通コード
	- asmmacro.h : アセンブラ用マクロファイル
	- rh850.h : RH850定義ファイル
	- syslog.c : システムログ出力ソースファイル
	- syslog.h : システムログ出力ヘッダファイル
	- tool_stddef.h : 標準定義ヘッダファイル
	- u2aeva.c : U2A関連のソースファイル
	- u2aeva.h : U2A関連のヘッダファイル
- vm_prog    : ゲストのプロジェクト
    - common : 共通ファイル
	  - Makefile.common : 共通Makefile
	- vm0_0_prog
	  - Makefile : ゲスト用Makefile
	  - rh850v2_ghs.ld : ゲスト用リンカスクリプト
	  - src_vm0_0 : ゲスト用ソースコード
	    - int.S  : 割込み出入り口処理
		- sample.c : ゲスト用メインソースコード
		- start.S : ゲスト用スタートアップコード
	- mvm0_prog
	- mvm1_prog
	- vm0_0_com_prog
	- vm0_0_ivc_prog
	- vm0_1_com_prog
	- vm0_1_ivc_prog
	- vm0_1_prgo
	- vm1_0_prgo
	- vm1_1_prgo
  - vm0_idle
- vm_atk2   : ATK2ゲストのプロジェクト

# 変更履歴

- 2024/xx/21 1.2
  - システム動作モードの追加．
  - TPTMへのカウント設定時にカウント値を-1するよう修正．

- 2023/03/21 1.1 
  - twtgint_raise() で割込みが無効なタイムウィンドウの判定方法が誤っていた問題を修正．
    - ./hypervisor/src/hv_impl.c
  - HVアイドル処理からのVM呼び出し機能を追加．
  - 例外処理機能を追加．

- 2022/07/28 1.0 最初のリリース

以上．
