#StateVariableクラス
from typing import *

from def_global import *

from cfg_vcom_base import *

from cfg_vm import *
from cfg_hvc import *

class SVar_t(VCOM_Base_t):
	#ctor
	def __init__(self) -> None:
		super().__init__()
		self.wrVM:	VM_t	= None	#書込可VM


	#クラス変数初期化
	TypeName	= "SVAR"
	HVCs		= [HVC_t(name, code=code) for (name, code) in HVC_VCOM_STATE_VAR]
	Insts: Dict[str, VCOM_Base_t] = OrderedDict()	#MyPyで型ヒント必要

