#割込ハンドラクラス
from __future__ import annotations
from typing import *

if TYPE_CHECKING:
    from cfg_vm import VM_t
    from cfg_core import Core_t

from error import *

from def_global import *
from def_cfg import *


class IntHandler_t:
	def __init__(self, no: int):
		self.no:		int					= no
		self.PEType:	PEType_t			= None
		self.PE:		Union[Core_t, VM_t]	= None
		self.handler:	str					= None

	#HV割込を登録
	def RegisterHV(self, core: Core_t, name: str):
		from cfg_core import Core_t

		if not type(core) == Core_t:
			AppError(f'core({core}) is not Cfg_Core', True)

		self.PEType		= PEType_t.HV
		self.PE			= core
		self.handler	= name
	    
	#VM割込を登録
	def RegisterVM(self, vm: VM_t):
		from cfg_vm import VM_t

		if not type(vm) == VM_t:
			AppError(f'vm({vm}) is not Cfg_VM', True)

		self.PEType		= PEType_t.VM
		self.PE			= vm
		self.handler	= None

	#登録済みか
	def Used(self) -> bool:
		return True if self.PE else False

	#登録先を表す文字列(エラー用)
	def RegisteredStr(self):
		if not self.Used():
			return None

		s = ""
		if self.PEType == PEType_t.HV:
			s += f'Core{self.PE.ID}.{self.handler}'
		elif self.PEType == PEType_t.VM:
			s += f'VM-{self.PE.ID}'

		return s




class IntHandlerTbl_t:
	def __init__(self, start: int, size: int):
		self.tbl:	List[IntHandler_t]	= list()

		for no in range(start, start + size):
			handler = IntHandler_t(no)
			self.tbl.append(handler)

	@property
	def MaxIntNo(self) -> int:
		return self.tbl[-1].no

	def get(self, no: int) -> IntHandler_t:
		#先頭の番号を減算
		idx = no - self.tbl[0].no
		if idx >= 0 and idx < len(self.tbl):
			return self.tbl[idx]
		return None


