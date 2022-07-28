#VM間通信オブジェクトの基本クラス

from __future__ import annotations

from typing import *

from collections import OrderedDict

from def_global import *

from cfg_hvc import *

class VCOM_Base_t:
	#ctor
	def __init__(self) -> None:
		self.Name:	Optional[str]	= None
		self.Size:	int				= 0
		self.Init:	bool			= False

		self.ID:	int				= 0	#オブジェクト種類毎の通しID


	#出力の定義名
	def defIDName(self, val: bool = False) -> Union[str, int]:
		if val:
			return self.ID
		else:
			return self.Name.upper()	#大文字の名前


	#クラス変数 オブジェクト型毎の情報を管理 サブクラスでオーバーライドする
	TypeName:	str						= None	#オブジェクト型名 出力で使用
	HVCs:		List[HVC_t]				= []	#オブジェクト操作HVC
	Insts:		Dict[str, VCOM_Base_t]	= None	#インスタンス [Name, obj]


	#インスタンス追加しIDを付与
	#	同名があれば追加せずFalseを返す
	@classmethod
	def AddInstance(cls, inst: VCOM_Base_t) -> bool:
		#同名があるか
		if inst.Name in cls.Insts:
			return False
		#ID付与 1～
		inst.ID = cls.NumInsts() + 1
		#インスタンスリストに追加
		cls.Insts[inst.Name] = inst
		return True
	
	#インスタンス数
	@classmethod
	def NumInsts(cls) -> int:
		return len(cls.Insts)
	

	#出力のサポート状況
	@classmethod
	def defSupport(cls, val: bool = False) -> Optional[str]:
		if val:
			return None
		else:
			return f'SUPPORT_{cls.TypeName}'


	#出力のオブジェクト数
	@classmethod
	def defTNUM(cls, val: bool = False) -> Union[str, int]:
		if val:
			return len(cls.Insts)
		else:
			return f'TNUM_{cls.TypeName}ID'

