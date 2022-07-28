#VCOMクラス

from typing import *

from cfg_hvc import *

from cfg_vcom_base import *

from cfg_vcom_svar import *
from cfg_vcom_msgq import *
from cfg_vcom_sbuf import *

#VCOMクラス
class VCOM_t:
	#ctor
	def __init__(self):
		#種類別クラス インスタンスではなくクラスを保持
		self.SVars = SVar_t
		self.MsgQs = MsgQ_t
		self.SBufs = SBuf_t

		#通信オブジェクトクラスのリスト
		self.comTypes:	List[Type[VCOM_Base_t]]	= [	self.SVars,
													self.MsgQs,
													self.SBufs]


	#VCOMあるか
	def hasVCOM(self) -> bool:
		for comType in self.comTypes:
			if comType.NumInsts():
				return True
		return False


	#出力のサポート状況
	def defSupport(self, val: bool = False) -> str:
		if val:
			return None
		else:
			return 'SUPPORT_VMCOM'
