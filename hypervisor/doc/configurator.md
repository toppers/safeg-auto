# SafeG-Auto コンフィギュレータ仕様書


# このドキュメントについて

SafeG-Auto のコンフィギュレータとコンフィギュレーションファイルの仕様について説明したものである．

# 概要

コンフィギュレーションファイルはハイパーバイザー(HV)及びバーチャルマシン(VM)のコンフィギュレーションを記載するファイルであり，yaml形式で記載する．

コンフィギュレータは，コンフィギュレーションファイルを読み込み，ハイパーバーイザーの構成ファイルを出力する．

# コンフィギュレータ

実行方法は次の通りである．./cfgはコンフィギュレータの格納ディレクトリに読み替える．

	python3 ./cfg  -comp [ghs|ccrh] -cfg [inputfile] -target [targetfile]
		-comp [ghs|ccrh]	
			コンパイラの指定
				ghs  : GHSを指定
				ccrh : CCRHを指定
		-cfg  [inputfile]
			入力とするコンフィギュレーションファイル
		-target [targetfile]
			ターゲット依存のコンフィギュレーションファイル

入力とするコンフィギュレーションファイルの仕様については，コンフィギュレータ仕様で説明する．

出力としては，次のファイルが出力される．

- hv_cfg.h
  - HV構成情報ヘッダファイル
- hv_cfg.c
  - HV構成情報ソースファイル（C言語用）
- hv_cfg_asm.h/hv_cfg_asm.inc
  - HV構成情報ソースファイル（アセンブリ言語用）
  - -comp に GHS を指定した場合は，hv_cfg_asm.hが，CC-RHを指定した場合は hv_cfg_asm.inc が生成される．
- hvc_cfg.h
  - ハイパーバイザーコール構成情報ヘッダファイル

## ターゲット依存のコンフィギュレーションファイル

ターゲット依存のコンフィギュレーションファイルでは，ターゲットプロセッサごとに異なるコンフィギュレーションを記載する．

ユーザーではなく，ターゲット依存部開発者が用意することを想定する．

ターゲット依存のコンフィギュレーションファイルの例を以下に示す．

	MCU:
 		NumberOfCores: 2 #物理的なコア数
 		NumberOfINTC2Interrupt: 735  #INTC2割込みの数, INTC1は32固定
 		Memory : 
  		- { base : 0x00000000, size : 0x00400000, type : rom, id : 0}
  		- { base : 0x00800000, size : 0x00400000, type : rom, id : 1}
  		- { base : 0x00C00000, size : 0x00400000, type : rom, id : 2}
  		- { base : 0x08000000, size : 0x00010000, type : rom, id : 3}
  		- { base : 0xFDC00000, size : 0x00010000, type : lram, id : 0}
  		- { base : 0xFDA00000, size : 0x00010000, type : lram, id : 1}
  		- { base : 0xFD800000, size : 0x00010000, type : lram, id : 2}
  		- { base : 0xFD600000, size : 0x00010000, type : lram, id : 3}
  		- { base : 0xFE000000, size : 0x00080000, type : cram, id : 0}
  		- { base : 0xFE100000, size : 0x00080000, type : cram, id : 1}
  		- { base : 0xFE400000, size : 0x00200000, type : cram, id : 2}
  		- { base : 0xFF000000, size : 0x01000000, type : peri, id : 0}


### MCU
- ターゲット依存のコンフィギュレーションファイルのトップキー

### NumberOfCores
- 親キー : MCU
- 値 : 数値 
- 説明
  - ターゲットが備える物理的なコア数を指定する．

### NumberOfINTC2Interrupt
- 親キー : MCU
- 値 : 数値
- 説明
  - INTC2がサポートする割込みの数を指定する．

### Memory
- 親キー : MCU
- 値 : メモリ構成のリスト
- 説明
  - メモリ構成定義のリストを指定する．
  - base
    - 値 : 数値 
    - 説明
      - ベースアドレスを指定する．
  - size
    - 値 : 数値 
    - 説明
      - メモリサイズを指定する．
  - type 
    - 値 : rom|cram|lram|peri
    - 説明
       - メモリの種類を指定．
         - rom : ROMの場合に指定する．
         - cram : CRAMの場合に指定する．
         - lram : LRAMの場合に指定する．
         - peri : 周辺回路の場合に指定する．
  - id
    - 値 :  数値
    - 説明
      - ID番号を指定．メモリの種類ごとにユニークな番号を持つ．	
- エラーチェック
  - 各メモリ構成定義で指定したメモリ領域(base+size)が重複した領域を指定していないか．[ER026]
  - typeが同じメモリ構成に重複したID番号が割り振られていないか．[ER027]

# コンフィギュレーションファイル仕様

次のキー以下にコンフィギュレーションを記載する．

- HV
	- ハイパーバイザーに関するコンフィギュレーションを定義．
- VM
	- 仮想マシンに関するコンフィギュレーションを定義．
- TDMA
	- TDMAスケジューリングに関するコンフィギュレーションを定義．
- VMCOM
	- VM間通信に関するコンフィギュレーションを定義．

各キーの内容の詳細について説明する．

## HV 

HVの例を以下に示す．

	HV:
 		LeaderCoreID:          0 # リーダコアのコアID
 		NumberOfHostEntryMPU:  4 # 使用するホストエントリMPUの個数
		CRAMUsageSize:
			- { id : 0, size : 0x0007FFFF}
			- { id : 1, size : 0x0007FFFF}
 		Cores: # コア毎の定義
  			- ID:              0 # ID
    		  SPID:            0 # HV動作時のSPID
    		  StackSizeHV:     1024 # HVのスタックサイズ
    		  StackSizeHVTWD:  1024 # HVTWのスタックサイズ
    		  SingleVMCORE:    false  # 省略時はfalse
    		  MemorySection:  # メモリ領域のセクション名
     			CCB        : .lbss0
     			HVTWDCB    : .lbss0
     			VMCB       : .lbss0
     			StackHV    : .lbss0
     			StackHVTWD : .cbss0
    		  Interrupt : # HV割込みの定義
				- {IntNo : 380, IntHandler : hvint0_handler, Priority : 9, Enable : true}
				- {IntNo : 709, IntHandler : twdint_handler, Priority : 9, Enable : true}
 		HVCall: # HVCALLの定義
			  - {Name : hvc_test0, NumberOfArg : 0} 
			  - {Name : hvc_test1, NumberOfArg : 1} 
			  - {Name : hvc_test2, NumberOfArg : 2} 
			  - {Name : hvc_test3, NumberOfArg : 3}


### HV
- ハイパーバイザー定義のトップキー

### LeaderCoreID
- 値 : 数値
- 説明
  - リーダコアのコアID．
- エラーチェック
  - 指定したコアIDがHV:Coresで定義されているか．[ER001]

### NumberOfHostEntryMPU
- 値 : 数値
- 説明
  - 使用するホストエントリMPUの個数を指定．
- エラーチェック
  - 32個を超えていないか．[ER002]

### CRAMUsageSize
- 値 : CRAM使用定義のリスト
- 説明
  - CRAMの使用サイズを指定．指定したサイズ分他のコアのHVやVM等からアクセス可能なようにCRAMガードを設定する．
  - id
    - CRAMのID番号
  - size
    - 使用するサイズ．
- エラーチェック
  - idがMCU:Memoryで定義されたメモリ構成のCRAMに含まれるか．[ER029]
  - sizeがMCU:Memoryで定義されたメモリ構成のサイズ以内か．[ER030]
  - idに同一のcramのidを指定していないか．[ER031]

### Cores
- 値 : 各コアのHVの構成のリスト．
- 説明
  - 各コアのHVの構成定義をリストで指定する．
  - ID
     - 値 : 数値
     - 説明
       - HVを実行するコアIDを指定する．
     - エラーチェック
       - MCU:NumberOfCoresで指定したコア数の範囲に含まれているか．[ER028]
	   - 他のHVの構成定義と重複していない．[ER032]
  - SPID
     - 値 : 数値
     - 説明
       - HV実行時のSPIDを指定する．
     - エラーチェック
       - 0から31の間か．[ER004]
  - StackSizeHV
     - 値 : 数値
     - 説明
       - HVのスタックサイズを指定する．
  - StackSizeHVTWD
     - 値 : 数値
     - 説明
       - HVTWDのスタックサイズを指定する．
  - SingleVMCORE
     - 値 : boolean
     - 説明
       - シングルVMとするかを指定する．省略時はfalseとなる．          
  - MemorySection  
     - 値 : ハッシュ
     - 説明
        - メモリ領域のセクション名を指定．省略時はセクション指定を行わない．
        - CCB       : コア毎のデータ構造のセクション指定．
        - HVTWDCB   : HVTWD用のデータ構造のセクション指定．
        - VMCB      : VM用のデータ構造のセクション指定．
        - StackHV   : HV用のスタックのセクション指定．
        - StackHVTWD : HVTWD用のスタックのセクション指定．
  - Interrupt
     - 値 : HV割込み定義のリスト
     - 説明
       - HV割込みを定義する．
       - IntNo
         - 値 : 数値
         - 説明
           - 割込み番号を指定する．
         - エラーチェック
           - NumberOfINTC2Interrupt未満であるか．[ER005]
		   - 重複した番号を指定していないか．[ER035]
       - IntHandler         
         - 値 : 文字列
         - 説明
            - 割り込みハンドラ名を指定する．
       - Priority
         - 値 : 数値
         - 説明
            - 割込み優先度を指定する．
         - エラーチェック
           - 64未満であるか．[ER006]
       - Enable
         - 値 : boolean
         - 説明
           - 起動時の割込み許可の状態を指定．
  - HVCall
     - 値 : HVCALL定義のリスト
     - 説明
       - HVCALLを定義する．
       - Name
         - 値 : 文字列
         - 説明
            - HVCALLとする関数名を指定する．
         - エラーチェック
            - 他の定義と重複していないか．-[ER033]
       - NumberOfArg 
         - 値 : 数値
         - 説明
            - HVCALLの引数．0から3を指定可能．
         - エラーチェック
           - 3以下であるか．[ER007]

## VM

	VM:
		 -  Name: VM0_0 # VM名
			CoreID: 0 # 割り付けるコアのコアID
			RestBase: 0x00100000 # リセットアドレス
			Memory: # メモリ割り当て
				- {Base: 0x00100000, Size: 0x00100000, Type: rom}
				- {Base: 0xfdc04000, Size: 0x00004000, Type: ram}
				- {Base: 0xfe020000, Size: 0x00010000, Type: ram}
				- {Base: 0xff000000, Size: 0x01000000, Type: peri}
			SPID: 4 # 初期SPID
			SPIDList: [4]  # 割り付けるSPIDのリスト
			Interrupt: # VM割込み
				- 199 
				- 707

### VM
- 値 : VMの構成のリスト．
- 説明
  - 各VMの構成をハッシュのリストで指定する．
  - Name
   - 値 : 文字列
   - 説明
      - VM名を指定する．
   - エラーチェック
      - 重複した名前を指定していないか．[ER008]
	  - VMの個数は32個以内か．[ER036]
  - CoreID
   - 値 : 数値
     - 説明
       - VMを割り付けるコアのコアIDを指定する．
     - エラーチェック
       - SingleVMCORE を trueとしたコアに複数のVMを割り付けていないか．[ER010]
  - ResetBase
     - 値 : 数値
     - 説明
       - リセットアドレスを指定する．
  - Memory
     - 値 : メモリ割り当て構成のリスト
       - Base
         - 値 : 数値
         - 説明
            - ベースアドレスを指定する．
         - エラーチェック
           - ターゲット依存のコンフィギュレーションファイルで指定したメモリの範囲に入っているか．[ER011]
       - Size
         - 値 : 数値
         - 説明
            - サイズを指定する．
         - エラーチェック
            - Base + Size がターゲット依存のコンフィギュレーションファイルで指定したメモリの範囲に入っているか．[ER012]
       - Type
         - 値 : [rom, ram, peri]
         - 説明
            - メモリの種類をを指定する．
  - SPID
     - 値 : 数値
     - 説明
       - 初期SPIDを指定する．
     - エラーチェック
       - SPIDListに含まれるか．[ER013]
  - SPIDList
     - 値 : 数値のリスト
     - 説明
       - VMに設定可能なSPIDのリストを指定する．
     - エラーチェック
       - 0から31の値か．[ER014]
  - Interrupt
     - 値 : 数値のリスト
     - 説明
       - VMに割り付ける割り込みの割込み番号のリスト
     - エラーチェック
         - NumberOfINTC2Interrupt未満であるか．[ER015]
         - HVや他のVMで指定されていないか．[ER016]
         - 重複した番号を指定していないか．[ER034]


## TDMA

    TDMA:
    SystemIntervalUS: 10000000
    SystemOperationMode:
      - Name : SOM1
        ScheduleTable:
        - CoreID: 0
          TimeWindow:
          - {Type: VM, VMName: VM0_0, DurationUS: 4000000, TwtgIntNo: 707, TwtgIntInterval: 1}
          - {Type: VM, VMName: VM0_1, DurationUS: 2000000, TwtgIntNo: 708, TwtgIntInterval: 2}
          - {Type: HV,                DurationUS: 1500000, TwtgIntNo: 709, TwtgIntInterval: 1}
        - CoreID: 1
          TimeWindow:
          - {Type: VM, VMName: VM1_0, DurationUS: 4000001, TwtgIntNo: 717, TwtgIntInterval: 1}
          - {Type: VM, VMName: VM1_1, DurationUS: 2000001, TwtgIntNo: 718, TwtgIntInterval: 2}
          - {Type: HV,                DurationUS: 1500001, TwtgIntNo: 719, TwtgIntInterval: 1}
      - Name : SOM2
        ScheduleTable:
        - CoreID: 0
          TimeWindow:
          - {Type: VM, VMName: VM0_1, DurationUS: 2000000, TwtgIntNo: 708, TwtgIntInterval: 2}
          - {Type: VM, VMName: VM0_0, DurationUS: 4000000, TwtgIntNo: 707, TwtgIntInterval: 1}
          - {Type: HV,                DurationUS: 1500000, TwtgIntNo: 709, TwtgIntInterval: 1}
        - CoreID: 1
          TimeWindow:
          - {Type: VM, VMName: VM1_1, DurationUS: 2000001, TwtgIntNo: 718, TwtgIntInterval: 2}
          - {Type: VM, VMName: VM1_0, DurationUS: 4000001, TwtgIntNo: 717, TwtgIntInterval: 1}
          - {Type: HV,                DurationUS: 1500001, TwtgIntNo: 719, TwtgIntInterval: 1}

### TDMA
- TDMAスケジューリング定義のトップキー

### SystemIntervalU
- 値 : 数値
- 説明
  - システム周期マイクロ秒で指定．
  
### SystemOperationMode
- 値 : システム動作モードのリスト

#### Name
- 値 : 文字列
- 説明
   - システム動作モードの名前を指定する．
   - エラーチェック
       - 他の定義と重複していないか．[ER008]
#### ScheduleTable
- 値 : スケジュールテーブル定義のリスト
- 説明
  - スケジュールテーブル定義のリストを記載する．
- エラーチェック
  - 内部で定義されたタイムウィンドウのDurationUSの総和がSystemIntervalUSで定義した値を超えていないか．[ER020]
- CoreID
  - 値 : 数値
  - 説明
    - スケジュールテーブルを実行するコアのコアIDを指定．
  - エラーチェック
    - 指定したコアIDがHV:Coresで定義されているか．[ER017]
    - SingleVMCOREがtrueのコアでないか．[ER018]
    - 複数のScheduleTableで同じコアIDを指定していないか．[ER019]
- TimeWindow
  - 値 : タイムウィンドウ定義のリスト
  - 説明
    - タイムウィンドウ定義のリストを指定する．タイムウィンドウは定義した順序で実行される．
    - Type
      - 値 : VM|HV
        -  VM  : VMタイムウィンドウの場合に指定
        -  HV  : HVタイムウィンドウの場合に指定
      - 説明
        - タイムウィンドウの種類を指定する．
    - VMName
      - 値 : 文字列
         - 説明
           - タイムウィンドウで実行するVMを指定する．
           - TypeがVMの場合のみ定義可能．
      - エラーチェック
         - VM:IDで定義されているVMのVMNameであるか．[ER021]
         - VMの割り付けコアとスケジュールテーブルの割り付けコアのコアIDは同じか．[ER022]
         - TypeがVMであるか．[ER023]
    - DurationUS
       - 値 : 数値
       - 説明
          - タイムウィンドウの時間を指定する．
       - エラーチェック
          - SystemIntervalUSで定義した値を超えていないか．[ER042]
    - TwtgIntNo
       - 値 : 数値
       - 説明
          - タイムウィンドウトリガ割込みとする割込みの番号．
          - 省略可能
       - エラーチェック
          - VM:Interrupt:で対象のVMに定義されたVM割込みの割込み番号であるか．[ER025]	
    - TwtgIntInterval
       - 値 : 数値
       - 説明
         - タイムウィンドウトリガ割込みの実行周期．
       - エラーチェック
         - TwtgIntNoは定義されているか．[ER026]	

## VMCOM

	VMCOM:
		StateVariable:
			- Name : SVAR1
			  Size : 4
			  WritableVM : VM0_0
			  InitState : true
		MessageQueue:
			- Name : MSGQ1
			  MaximumSize: 8
   			  MessageQueueSize : 16
			  WritableVM : VM0_0
			  ReadableVM : VM0_1
			  InitState : true
		SharedBuffer: 
  			- Name : SBUF1
			  Size : 64
			  AccessibleVM :
				- VM0_0
				- VM0_1
  
### VMCOM
- VM間通信の定義のトップキー

### StateVariable	
- 値 : 状態変数定義のリスト
- 説明
  - 状態変数を定義したリストを記載する．
  - Name 
    - 値 : 文字列
    - 説明
      - 状態変数の名前を定義する．
    - エラーチェック
      - 他のVM間通信及びVM名と重複して定義されていないか．[ER037]	
  - Size
    - 値 : 数値
    - 説明
      - 状態変数のサイズをバイトで指定
  - WritableVM
    - 値 : 文字列
    - 説明
      - 状態変数へ書き込み可能なVMの名前を指定する．
    - エラーチェック
      - VM:で定義されている名前か．[ER038]
  - InitState
    - 値 : boolean
    - 説明
      - 状態変数の初期状態を指定．

### MessageQueue
- 値 : メッセージキュー定義のリスト
- 説明
  - メッセージキューを定義したリストを記載する．
  - Name 
    - 値 : 文字列
    - 説明
      - メッセージキューの名前を定義する．
    - エラーチェック
      - 他のVM間通信及びVM名と重複して定義されていないか．[ER037]		
  - MaximumSize
    - 値 : 数値
    - 説明
      - メッセージキューに送信可能なデータの最大サイズをバイトで指定	  
    - エラーチェック
      - MessageQueueSize以下か．[ER041]
  - MessageQueueSize
    - 値 : 数値
    - 説明
      - メッセージキューのバッファサイズをバイトで指定	  
  - WritableVM
    - 値 : 文字列
    - 説明
      - メッセージキューへ書き込み可能なVMの名前を指定する．
    - エラーチェック
      - VM:で定義されている名前か．[ER038]
  - ReadableVM
    - 値 : 文字列
    - 説明
      - メッセージキューから読み込み可能なVMの名前を指定する．
    - エラーチェック
      - VM:で定義されている名前か．[ER040]
  - InitState
    - 値 : boolean
    - 説明
      - 状態変数の初期状態を指定．

### SharedBuffer
- 値 : 共有バッファ定義のリスト
- 説明
  - 共有バッファを定義したリストを記載する．
  - エラーチェック
    - VM:Memoryで定義したメモリ割り当てのリスト数が最大のVMのメモリ構成割り当ての数 + 共有バッファの数が32を超えないこと．[ER039]	  
  - Name 
    - 値 : 文字列
    - 説明
      - 共有バッファの名前を定義する．
    - エラーチェック
      - 他のVM間通信及びVM名と重複して定義されていないか．[ER037]
  - Size
    - 値 : 数値
    - 説明
      - 共有バッファサイズをバイトで指定	  
  - AccessibleVM
    - 値 : VM名のリスト
    - 説明
      - メッセージキューにアクセス可能なVMの名前を指定する．
    - エラーチェック
      - VM:で定義されている名前か．[ER038]
  

# 更新履歴
- 2023/12/19
	- システム動作モードの導入．
