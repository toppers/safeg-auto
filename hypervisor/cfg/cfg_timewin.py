#タイムウインドウクラス

from typing import *

from def_global import *
from def_cfg import *

from cfg_vm import *

class TimeWindow_t:
	def __init__(self):
		self.type:			PEType_t	= None	#PEタイプ
		self.VM:			VM_t		= None	#type=VMの場合のVM
		self.Duration:		int			= 0
		self.TwtgInt:		int			= -1	#-1なら割込使用しない
		self.TwtgInterval:	int			= 1
