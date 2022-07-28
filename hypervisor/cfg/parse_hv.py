##HVノードパーサークラス

from typing import *

from parse_base import *

from def_cfg_kw import *
from yaml_node import *
from parse_util import *

from def_cfg import *

from cfg import *

from cfg_hv import *
from cfg_hvc import *
from cfg_core import *
from cfg_hvint import *

from error import *


class Parser_HV_t(ParserBase_t):
	#ctor
	def __init__(self, cfg: Cfg_t) -> None:
		super().__init__(cfg)


	#HVノード
	def Parse(self, node: Node_t) -> None:
		#有効キーワード
		keys = [NodeDef_t(CFG_KW_LEADER_CORE,		True),
				NodeDef_t(CFG_KW_HOST_ENTRY_MPU,	True),
				NodeDef_t(CFG_KW_CRAM_USAGE_SIZE,	True),
				NodeDef_t(CFG_KW_HVCALL,			False),
				NodeDef_t(CFG_KW_CORES,			True)]
		#有効キーワードチェック
		node.chkKeyword(keys)

		#HVオブジェクト
		hv = self.Cfg.HV

		#ホストエントリMPU数
		hv.HostEntryMPUs = node.getInt(CFG_KW_HOST_ENTRY_MPU)
		#32より大ならエラー
		if hv.HostEntryMPUs > MAX_HOST_ENTRY_MPU:
			#@ER002:ホストエントリMPUの個数が多すぎる
			AppError(f'{CFG_KW_HOST_ENTRY_MPU}:({hv.HostEntryMPUs}) must be <= {MAX_HOST_ENTRY_MPU}')

		#CRAMUsageSize
		nodeCRAM = node.get(CFG_KW_CRAM_USAGE_SIZE)
		if nodeCRAM:
			hv.CRAMs = self.parseCRAMUsageSize(nodeCRAM)

		#HVCall
		nodeHVC = node.get(CFG_KW_HVCALL)
		if nodeHVC:
			hv.HVCs = self.parseHVCall(nodeHVC)
			hv.AllHVCs.extend(hv.HVCs)	#全HVCに追加

		#Cores
		nodeCores = node.get(CFG_KW_CORES)
		if nodeCores:
			hv.Cores = self.parseCores(nodeCores, self.Cfg.MCU.NumCore)

		#リーダーコアID
		LeaderCoreID = node.getInt(CFG_KW_LEADER_CORE)
		#指定されたコアがあるか
		hv.LeaderCore = hv.Cores.get(LeaderCoreID)
		if not hv.LeaderCore:
			#@ER001:コアに含まれない
			AppErrorLastNode(node, LeaderCoreID, f'CoreID is not defined')

		return


	#CRAMUsageSizeノード
	def parseCRAMUsageSize(self, nodes: Node_t) -> List[Mem_t]:
		#有効キーワード
		keys = [NodeDef_t(CFG_KW_ID,	True),
				NodeDef_t(CFG_KW_SIZE,	True)]

		crams: List[Mem_t] = list()
		
		for node in nodes.array():
			#有効キーワードチェック
			node.chkKeyword(keys)

			#ID
			id = node.getInt(CFG_KW_ID)
			#MCUのメモリにあるか
			if not next((m for m in self.Cfg.MCU.Mems if m.id == id and m.type == MemType_t.cram), None):
				#@ER029
				AppErrorLastNode(node, id, f'No CRAM-ID is defined in {CFG_KW_MCU}.{CFG_KW_MEMORY}')
				continue
			cramdef = self.Cfg.MCU.Mems[id]

			#Size
			size = node.getInt(CFG_KW_SIZE)
			#CRAMのサイズ未満か
			if size >= cramdef.size:
				#@ER030
				AppErrorLastNode(node, f'{size:>08X}', f'Size overflowed CRAM size ID:{id} Size:{cramdef.size:>08X}')
				continue

			#下位ビットから連続した1でなければならない
			#+1してandすると0になればOK
			if (size + 1) & size:
				#@ER:CRAMの使用Sizeの値がLSBから連続した1でない
				AppErrorLastNode(node, f'{size:>08X}',  f'must be (2 pow x) - 1')
				continue

			crams.append(Mem_t(0, size, MemType_t.cram, id))

		return crams


	#HVCallノード
	def parseHVCall(self, nodes: Node_t) -> List[HVC_t]:
		#有効キーワード
		keys = [NodeDef_t(CFG_KW_NAME,		True),
				NodeDef_t(CFG_KW_NUM_ARGS,	False)]

		hvcs: List[HVC_t] = list()

		for node in nodes.array():
			#有効キーワードチェック
			node.chkKeyword(keys)

			#Name
			#err:HVC名が識別子として無効
			name = node.getIdentifier(CFG_KW_NAME)
			if name:
				#err:同名のオブジェクトが存在するか
				self.Cfg.existName(name, node.LastName)
				##同名のHVCが既に存在するか
				#if next((i for i in hvcs if i.Name == name), None):
				#	#err:HVC名重複
				#	AppError(f'{node.FullName}: "{name}" is already defined')

			#引数の数はデフォルト0個
			args = node.getInt(CFG_KW_NUM_ARGS, 0)
			#引数の数チェック
			if args > MAX_HVC_ARGS:
				#err:HVCの引数の数が範囲外
				AppErrorLastNode(node, args,  f'must be <= {MAX_HVC_ARGS}')

			#名前が無効なら追加しない
			if name:
				hvcs.append(HVC_t(name, args))

		return hvcs


	#Coresノード
	def parseCores(self, nodes: Node_t, numCore: int) -> Dict[int, Core_t]:
		#有効キーワード
		keys = [NodeDef_t(CFG_KW_ID,				True),
				NodeDef_t(CFG_KW_SPID,				True),
				NodeDef_t(CFG_KW_STACK_SIZE_HV,		True),
				NodeDef_t(CFG_KW_STACK_SIZE_HVTW,	True),
				NodeDef_t(CFG_KW_SINGLE_VM_CORE,	False),
				NodeDef_t(CFG_KW_MEM_SECTION,		False),
				NodeDef_t(CFG_KW_INT,				True)]

		cores: Dict[int, Core_t] = OrderedDict()

		for node in nodes.array():
			#有効キーワードチェック
			node.chkKeyword(keys)

			#ID
			id = node.getInt(CFG_KW_ID)
			#コア数未満
			if id >= numCore:
				#@ER003:IDが範囲外
				AppErrorLastNode(node, id, f'must be < Number of Cores({numCore})')
			#既に同じIDが存在するか
			if id in cores:
				#@ER032:IDが重複
				AppErrorLastNode(node, id, f'is already defined')

			#Coreオブジェクト作成
			core = Core_t(id)

			#SPID
			spid = node.getInt(CFG_KW_SPID)
			#最大値チェック
			if spid > MAX_SPID:
				#err:SPIDが範囲外
				AppError(f'{node.LastName}:({spid}) must be <= {MAX_SPID}')
			core.SPID = spid

			#StackSizeHV
			stackHV = node.getInt(CFG_KW_STACK_SIZE_HV)
			core.StackHV = stackHV

			#StackSizeHVTW
			stackHVTW = node.getInt(CFG_KW_STACK_SIZE_HVTW)
			core.StackHVTW = stackHVTW

			#SingleVMCore
			singleVM = node.getBool(CFG_KW_SINGLE_VM_CORE, False)
			core.SingleVM = singleVM

			#MemorySection
			nodeMemSec = node.get(CFG_KW_MEM_SECTION)
			if nodeMemSec:
				core.Sections = self.parseMemorySection(nodeMemSec)

			#Interrupt
			nodeInt = node.get(CFG_KW_INT)
			if nodeInt:
				core.Ints = self.parseHVInt(nodeInt, core)


			cores[id] = core	#追加
		return cores


	#MemorySectionノード
	def parseMemorySection(self, node: Node_t) -> Dict[Section_t, str]:
		#有効なキーワード Sectoin_tの名前
		keys: List[NodeDef_t] = list()
		for e in Section_t:
			keys.append(NodeDef_t(e.name, False))	#全て省略可

		#キーワードチェック
		node.chkKeyword(keys)

		sections: Dict[Section_t,str] = OrderedDict()

		#有効なキーワードで取得する
		for nodeDef in keys:
			sec = nodeDef.name
			name = node.getStr(sec)
			if name != None:
				secType = EnumVal(Section_t, sec)	#keysでチェック済みなのでここでエラーは無い
				sections[secType] = name

		return sections


	#Interruptノード
	def parseHVInt(self, nodes: Node_t, core: Core_t) -> Dict[int, HVInt_t]:
		#有効なキーワード
		keys = [NodeDef_t(CFG_KW_INT_NO,		True),
				NodeDef_t(CFG_KW_INT_HANDLER,	True),
				NodeDef_t(CFG_KW_INT_PRI,		True),
				NodeDef_t(CFG_KW_INT_EN,		False)]

		ints: Dict[int, HVInt_t] = OrderedDict()

		for node in nodes.array():
			#キーワードチェック
			node.chkKeyword(keys)

			#IntNo
			no = node.getInt(CFG_KW_INT_NO)
			#同じ番号があるか
			if no in ints:
				#err:割込番号が既に使われている
				AppError(f'{node.LastName}:({no}) is already used')

			#ハンドラ取得
			handler = core.IntTbl.get(no)	#コア専用割込
			if not handler:
				handler = self.Cfg.IntTbl.get(no)	#共有割込
				if not handler:
					#err:割込番号が範囲外
					AppError(f'{node.LastName}:({no}) must be < {self.Cfg.IntTbl.MaxIntNo}')

			#既に使われているか
			if handler and handler.Used():
				#err:割込番号が他のコアで既に使われている
				AppError(f'{node.LastName}:({no}) is already used by {handler.RegisteredStr()}')

			#IntHandler
			#err:ハンドラ名が識別子として無効
			name = node.getIdentifier(CFG_KW_INT_HANDLER)

			#使われていなければハンドラに登録
			if handler and not handler.Used():
				handler.RegisterHV(core, name)

			#Priority
			pri = node.getInt(CFG_KW_INT_PRI)
			if pri > MAX_INT_PRIORITY:
				#err:Priorityが範囲外
				AppError(f'{node.LastName}:({pri}) must be <= {MAX_INT_PRIORITY}')

			#Enable
			en = node.getBool(CFG_KW_INT_EN, False)

			#割込オブジェクト作成
			Int = HVInt_t()
			Int.no		= no
			Int.handler	= name
			Int.pri		= pri
			Int.enable	= en

			ints[no] = Int

		return ints


