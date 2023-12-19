##コンフィグレーションデータクラス

from typing import *

import os

from error import *

from params import *

from cfg_mcu import *
from cfg_hv import *
from cfg_vm import *
from cfg_tdma import *
from cfg_inthandler import *
from cfg_vcom import *



class Cfg_t:
	#ctor
	def __init__(self, param: Params) -> None:
		self.Param:	Params	= param	#コマンドラインパラメータ
		#コンフィグレーションデータ
		self.MCU:		MCU_t			= MCU_t()
		self.HV:		HV_t			= HV_t()
		self.VMs:		Dict[str, VM_t]	= OrderedDict()
		self.TDMA:		TDMA_t			= TDMA_t()
		self.VCOM:		VCOM_t			= VCOM_t()
		self.IntTbl:	IntHandlerTbl_t	= None		#この時点では生成出来ない ターゲット設定読込後

		self.ObjNames:	Dict[str, Tuple[str, str]]	= {}	#全オブジェクトの名前 {大文字名: (元名, 定義位置)}


	#ターゲット設定読込後
	def PostTargetLoad(self) -> None:
		#ターゲット設定のINTC2数で割込ハンドラテーブルを初期化
		self.IntTbl = IntHandlerTbl_t(NUM_INTC1_INT, self.MCU.NumINTC2)


	#名前付きオブジェクトチェック
	#	loc	YAMLノード名
	#	既にある場合はエラーでTrueを返す
	def existName(self, name: str, loc: str) -> bool:
		uname = name.upper();
		obj = self.ObjNames.get(uname)
		if obj:
			(objName, objLoc) = obj
			#err:同名のオブジェクトが存在する                                     大文字小文字が違う場合は補足を表示 
			AppError(f"{loc}: '{name}' is already defined at {objLoc}:({objName}){' (case ignored)' if name != objName else ''}")
			return True
		else:
			self.ObjNames[uname] = (name, loc)	#対象オブジェクト名を追加
			return False


	#読込後処理
	def PostLoad(self):
		#VMのGPID割り当て コア毎に0～
		for core in self.HV.Cores.values():
			for (i, vm) in enumerate(core.VMs):
				vm.GPID = i

		#VMのID割り当て 1～
		for (i, vm) in enumerate(self.VMs.values()):
			vm.ID = i + 1	#1～

		#HVC-ID割り当て
		for (i, hvc) in enumerate(self.HV.AllHVCs):
			hvc.ID = i

		#SOMのID割り当て 1～
		for (i, som) in enumerate(self.TDMA.SystemOperationModes.values()):
			som.ID = i + 1	#1～

		return


	#ファイル生成
	def Generate(self):
		import gen_hv_cfg_h
		import gen_hv_cfg_c
		import gen_hvc_cfg_h
		import gen_hv_cfg_asm

		odir = self.Param.Dir

		#出力dir作成
		if odir:
			os.makedirs(odir, exist_ok=True)

		#HVC_Hファイル
		hvc_h = open(os.path.join(odir, FILE_HVC_H), "w", encoding="UTF8", newline='\r\n')
		gen_hvc_cfg_h.Generate(hvc_h, self)

		#HV_Hファイル
		hv_h = open(os.path.join(odir, FILE_HV_H), "w", encoding="UTF8", newline='\r\n')
		gen_hv_cfg_h.Generate(hv_h, self)

		#HV_ASMファイル
		gen_hv_cfg_asm.Generate(odir, self)

		#HV_Cファイル
		hv_c = open(os.path.join(odir, FILE_HV_C), "w", encoding="UTF8", newline='\r\n')
		gen_hv_cfg_c.Generate(hv_c, self)

		return
