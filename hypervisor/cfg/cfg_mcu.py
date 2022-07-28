#ターゲットMCUクラス

from typing import *

from cfg_mem import *

class MCU_t:
	def __init__(self) -> None:
		self.NumCore:	int			= 0
		self.NumINTC2:	int			= 0
		self.Mems:		List[Mem_t]	= list()





