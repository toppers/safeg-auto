#VMクラス

from typing import *

if TYPE_CHECKING:
    from cfg_core import Core_t

from cfg_mem import *

class VM_t:
	def __init__(self, name: str):
		self.Name:		str			= name
		self.Core:		Core_t		= None
		self.ResetBase:	int			= 0
		self.Mems:		List[Mem_t]	= list()
		self.SPIDList:	List[int]	= list()
		self.SPID:		int			= 0
		self.Ints:		List[int]	= list()

		self.ID:	int	= 0
		self.GPID:	int	= 0

		
	#表示名
	@property
	def DispName(self) -> str:
		return f'VM({self.Name})'

	#VMID
	def defVMID(self, val: bool = False) -> Union[str, int]:
		if val:
			return self.ID
		else:
			return f'VMID_{self.Name.upper()}'

	#GPID
	def defGPID(self, val: bool = False) -> Union[str, int]:
		if val:
			return self.GPID
		else:
			return f'{self.Name.upper()}_GPID'

	#SPID
	def defSPID(self, val: bool = False) -> Union[str, int]:
		if val:
			return self.SPID
		else:
			return f'{self.Name.upper()}_SPID'

	#SPID-bit
	@property
	def bitSPID(self) -> int:
		return 1 << self.SPID


	#SPIDLIST
	def defSPIDLIST(self, val: bool = False) -> Union[str, int]:
		if val:
			return self.bitSPIDLIST
		else:
			return f'{self.Name.upper()}_SPIDLIST'

	#bitSPIDLIST
	@property
	def bitSPIDLIST(self) -> int:
		spidList = 0
		for spid in self.SPIDList:
			spidList |= (1 << spid)

		return spidList
