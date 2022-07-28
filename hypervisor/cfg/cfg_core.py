#Coreクラス

from typing import *


from enum import *
from collections import OrderedDict

from def_global import *

from cfg_vm import *
from cfg_hvint import *
from cfg_inthandler import *

class Section_t(Enum):
	CCB			= auto()
	HVTWDCB		= auto()
	VMCB		= auto()
	StackHV		= auto()
	StackHVTW	= auto()


class Core_t:
	def __init__(self, id: int) -> None:
		self.ID:		int						= id
		self.SPID:		int						= 0
		self.StackHV:	int						= 0
		self.StackHVTW:	int						= 0
		self.SingleVM:	bool					= False
		self.Sections:	Dict[Section_t, str]	= OrderedDict()
		self.Ints:		Dict[int, HVInt_t]		= OrderedDict()
		self.VMs:		List[VM_t]				= []
		self.IntTbl:	IntHandlerTbl_t			= IntHandlerTbl_t(0, NUM_INTC1_INT)

	#VMを接続
	def ConnectVM(self, vm: VM_t):
		#コア毎の最大VM数
		if len(self.VMs) >= MAX_VM_CORE:
			#err:コアのVM数超過
			AppError(f'{vm.DispName}: Core{self.ID} is already assigned MAX VMs({MAX_VM_CORE})')

		self.VMs.append(vm)

	#表示名
	@property
	def DispName(self) -> str:
		return f'Core{self.ID}'

	#ID名
	def defIDName(self, val: bool = False) -> str:
		if val:
			return f'{self.ID}'
		else:
			return f'COREID_{self.ID}'

	#ID-bit
	def bitID(self) -> int:
		return 1 << self.ID


	#出力コードのSPID名
	def defSPID(self, val: bool = False) -> Union[str, int]:
		if val:
			return self.SPID
		else:
			return f'HV_SPID_CORE{self.ID}'

	#SPID-bit
	@property
	def bitSPID(self) -> int:
		return 1 << self.SPID


	#出力コードのVM_SPID名
	def defVMSPID(self, val: bool = False) -> str:
		if val:
			return f'0x{self.bitVMSPID:>08X}'
		else:
			return f'TBIT_VM_SPID_CORE{self.ID}'

	#全VMのSPID-bit
	@property
	def bitVMSPID(self) -> int:
		spid = 0
		for vm in self.VMs:
			spid |= vm.bitSPID
		return spid


