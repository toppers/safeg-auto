#Memoryクラス

from typing import *
from enum import *


#メモリタイプ
class MemType_t(IntFlag):
	rom		= 1 << 0
	ram		= 1 << 1
	peri	= 1 << 2
	lram	= ram | (1 << 8)
	cram	= ram | (2 << 9)

	@classmethod
	def str(cls, v: 'MemType_t') -> str:
		if v == MemType_t.rom:
			return "ROM"
		if v == MemType_t.ram:
			return "RAM"
		if v == MemType_t.peri:
			return "PERI"
		return None

#ターゲット設定で指定出来るType
class MemTypeTarget_t(Enum):
	rom		= auto()
	lram	= auto()
	cram	= auto()
	peri	= auto()

#Cfgで指定出来るType
class MemTypeCfg_t(Enum):
	rom		= auto()
	ram		= auto()
	peri	= auto()


class Mem_t:
	def __init__(self, base: int, size: int, type: MemType_t, id: int = -1):
		self.base:	int			= base
		self.size:	int			= size
		self.type:	MemType_t	= type
		self.id:	int			= id

	#終了アドレス+1
	@property 
	def End(self) -> int:
		return self.base + self.size

	#範囲内か
	def isInclude(self, base: int, size: int = 1) -> bool:
		last  = base + size - 1
		return self.base <= base and last < self.End

	#重なっているか
	def isOverwrapped(self, base: int, size: int = 1) -> bool:
		#開始/末尾アドレスのいずれかが範囲内なら重なっている
		return self.isInclude(base) or self.isInclude(base + (size - 1 if size > 0 else 0))



