#SharedBufferクラス

from typing import *

from def_global import *

from cfg_vcom_base import *

from cfg_vm import *
from cfg_hvc import *

class SBuf_t(VCOM_Base_t):
	#ctor
	def __init__(self) -> None:
		super().__init__()
		self.AccesVMs:	List[VM_t]	= []	#アクセス可能VMリスト


	#クラス変数初期化
	TypeName	= "SBUF"
	HVCs		= [HVC_t(name, code=code) for (name, code) in HVC_VCOM_SHARED_BUF]
	Insts: Dict[str, VCOM_Base_t] = OrderedDict()	#MyPyで型ヒント必要

