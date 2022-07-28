#HVクラス

from typing import *

from collections import OrderedDict

from cfg_hvc import *
from cfg_core import *

from out_util import *

class HV_t:

	def __init__(self):
		self.LeaderCore:	Core_t				= None
		self.HostEntryMPUs:	int					= 0
		self.CRAMs:			List[Cfg_CRAM]		= []
		self.HVCs:			List[HVC_t]			= []
		self.Cores:			Dict[int, Core_t]	= OrderedDict()

		self.AllHVCs:		List[HVC_t]	= []	#ID付与のための全HVC


	#コア数
	@property
	def NumCores(self) -> int:
		return len(self.Cores)

	#出力コードのサポートコア数
	#	val	Trueなら値を出力
	def defNumSupportCore(self, val: bool = False) -> str:
		if val:
			return f'0x{self.NumCores:>02X}'
		else:
			return 'TNUM_SUPPORT_CORE'


	#出力コードのサポートコアビット
	#	val	Trueなら値を出力
	def defNumSupportCoreBit(self, val: bool = False) -> str:
		if val:
			return f'0x{self.bitNumSupportCore:>02X}'
		else:
			return 'TBIT_SUPPORT_CORE'

	#サポートコアビット
	@property
	def bitNumSupportCore(self) -> int:
		ret = 0
		for core in self.Cores.values():
			ret |= (1 << core.ID)
		return ret


	#出力コードのリーダーコア
	#	val	Trueなら値を出力
	def defLeaderCoreID(self, val: bool = False) -> str:
		if val:
			return self.LeaderCore.defIDName(True)	#ここではID名は使えない
		else:
			return 'LEADER_COREID'

	#出力コードのSingleVMコアビット
	#	val	Trueなら値を出力
	def defSingleVMCoreBit(self, val: bool = False) -> str:
		if val:
			return f'0x{self.bitSingleVMCore:>02X}'
		else:
			return f'TBIT_SINGLEVM_CORE'
	
	#SingleVMコアビット
	@property
	def bitSingleVMCore(self) -> int:
		ret = 0
		for core in self.Cores.values():
			if core.SingleVM:
				ret |= (1 << core.ID)
		return ret

	#出力コードのHV_SPID
	def defHV_SPID(self, val: bool = False) -> str:
		if val:
			return f'0x{self.bitHV_SPID:>08X}'
		else:
			return 'TBIT_HV_SPID'

	#HV_SPID
	@property
	def bitHV_SPID(self) -> int:
		ret = 0
		for core in self.Cores.values():
			ret |= core.bitSPID
		return ret


	#出力コードの全SPID
	def defAllSPID(self, val: bool = False) -> str:
		if val:
			return f'0x{self.bitAllSPID:>08X}'
		else:
			return 'TBIT_SPID'

	#全SPID
	@property
	def bitAllSPID(self) -> int:
		spid = self.bitHV_SPID
		for core in self.Cores.values():
			spid |= core.bitVMSPID
		return spid

	#出力コードのHVC数
	def defNUM_HVC(self, val: bool = False) -> Union[str, int]:
		if val:
			return len(self.AllHVCs)
		else:
			return 'TNUM_HVCID'
