#MessageQueueクラス

from typing import *

from def_global import *

from cfg_vcom_base import *

from cfg_vm import *
from cfg_hvc import *

class MsgQ_t(VCOM_Base_t):
	#ctor
	def __init__(self) -> None:
		super().__init__()
		self.MaxSize:	int		= 0		#データの最大サイズ
		#Size							#キューのサイズ
		self.wrVM:		VM_t	= None	#書込VM
		self.rdVM:		VM_t	= None	#読込VM


	#クラス変数初期化
	TypeName	= "MSGQ"
	HVCs		= [HVC_t(name, code=code) for (name, code) in HVC_VCOM_MSG_QUEUE]
	Insts: Dict[str, VCOM_Base_t] = OrderedDict()	#MyPyで型ヒント必要


