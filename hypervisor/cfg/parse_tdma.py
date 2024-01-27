##コンフィグファイルパーサークラス

from typing import *

from parse_base import *

from def_cfg_kw import *
from yaml_node import *
from parse_util import *

from def_cfg import *

from cfg import *

from cfg_tdma import *
from cfg_schedule import *
from cfg_timewin import *
from cfg_som import *

from error import *


class Parser_TDMA_t(ParserBase_t):
	#ctor
	def __init__(self, cfg: Cfg_t) -> None:
		super().__init__(cfg)


	#TDMAノード
	def Parse(self, node: Node_t) -> None:
		#有効キーワード
		keys = [NodeDef_t(CFG_KW_SYS_INTERVAL,	True),
				NodeDef_t(CFG_KW_SOM,			True)]

		#キーワードチェック
		node.chkKeyword(keys)

		#TDMAオブジェクト
		tdma = self.Cfg.TDMA

		#SystemIntervalUS
		interval = node.getInt(CFG_KW_SYS_INTERVAL)
		tdma.Interval = interval
		
		#SystemOperationMode
		nodeSystemOperationMode = node.get(CFG_KW_SOM)
		if nodeSystemOperationMode:
			tdma.SystemOperationModes = self.parseSystemOperationMode(nodeSystemOperationMode, interval)

		#コア毎のタイムウィンドウ数を取得
		for som in tdma.SystemOperationModes.values():
			for sched in som.Schedules.values():
				if (sched.Core.ID in tdma.NumTW):
					tdma.NumTW[sched.Core.ID] += len(sched.TimeWins)
				else:
					tdma.NumTW[sched.Core.ID] = len(sched.TimeWins)

		return

	#SystemOperationModeノード
	def parseSystemOperationMode(self, nodes: Node_t, sysInterval: int) -> Dict[str, Som_t]:
		keys = [NodeDef_t(CFG_KW_NAME,			True),
				NodeDef_t(CFG_KW_SCHEDULE_TBL,	True)]

		soms: Dict[str, Som_t] = OrderedDict()
		
		for node in nodes.array():
			#有効キーワードチェック
			node.chkKeyword(keys)

			#ID
			#err:SOMIDが識別子として無効
			name = node.getIdentifier(CFG_KW_NAME)
			if name:
				#err:同名のオブジェクトが存在するか
				self.Cfg.existName(name, node.LastName)

			#SOMオブジェクト生成
			som = Som_t(name)
			
			#ScheduleTable
			nodeSchTbl = node.get(CFG_KW_SCHEDULE_TBL)
			if nodeSchTbl:
				som.Schedules = self.parseScheduleTbl(nodeSchTbl, sysInterval)

			#同じ名前がなければ追加
			if name and not name in soms:
				soms[name] = som

		return soms

	#ScheduleTableノード
	def parseScheduleTbl(self, nodes: Node_t, sysInterval: int) -> Dict[int, Schedule_t]:
		#有効キーワード
		keys = [NodeDef_t(CFG_KW_COREID,	True),
				NodeDef_t(CFG_KW_TIMEWIN,	True)]

		schedules: Dict[int, Schedule_t] = OrderedDict()

		for node in nodes.array():
			#キーワードチェック
			node.chkKeyword(keys)

			#スケジュールオブジェクト生成
			schedule = Schedule_t()

			#CoreID
			coreID = node.getInt(CFG_KW_COREID)
			#存在するか
			core = self.Cfg.HV.Cores.get(coreID)
			if not core:
				#err:コアが未定義
				AppErrorLastNode(node, coreID, 'CoreID is not defined')
			#既にあるか
			elif coreID in schedules:
				#err:コアIDが重複
				AppErrorLastNode(node, coreID, f'ScheduleTable of CoreID is already defined')
			else:
				schedules[coreID] = schedule
				schedule.Core = core

			#TimeWindow
			nodeTW = node.get(CFG_KW_TIMEWIN)
			if nodeTW:
				schedule.TimeWins = self.parseTimeWin(nodeTW, sysInterval, core)

		return schedules


	#TimeWindowノード
	def parseTimeWin(self, nodes: Node_t, sysInterval: int, core: Optional[Core_t]) -> List[TimeWindow_t]:
		#有効キーワード
		keys = [NodeDef_t(CFG_KW_TYPE,			True),
				NodeDef_t(CFG_KW_VMNAME,		True),
				NodeDef_t(CFG_KW_DURATION,		True),
				NodeDef_t(CFG_KW_TWTG_INT,		False),
				NodeDef_t(CFG_KW_TWTG_INTERVAL,	False)]

		timeWins: List[TimeWindow_t] = list()

		for node in nodes.array():
			#キーワードチェック
			node.chkKeyword(keys)

			#TimeWindowオブジェクト生成
			timeWin = TimeWindow_t()

			#Type
			#err:無効なType
			typ = node.getEnum(CFG_KW_TYPE, PEType_t)
			timeWin.type = typ

			#VMID
			pe: Union[Core_t, VM_t, None] = None	#対象のPE VM/Core
			if typ == PEType_t.VM:
				#Type=VMなら必要
				vmid = node.getStr(CFG_KW_VMNAME)
				if vmid:
					#存在するか
					vm = self.Cfg.VMs.get(vmid)
					if not vm:
						#err:TimeWindowで指定されたVMIDが未定義
						AppError(f'{node.LastName}:({vmid}) is not defined')
					else:
						timeWin.VM = vm
						pe = vm
			elif typ == PEType_t.HV:
				#Type=HVならあってはいけない
				nodeVMID = node.get(CFG_KW_VMNAME, True)
				if nodeVMID != None:
					#err:HVのTimeWindowにVMIDが指定されている
					AppError(f'{node.LastName}: must not be defined for {CFG_KW_TYPE}={typ.name}')

				pe = core

			#DurationUS
			duration = node.getInt(CFG_KW_DURATION)
			#SystemIntervalUS以下か
			if duration > sysInterval:
				#err:DurationUSがSystemIntervalUSを越えている
				AppError(f'{node.LastName}:({duration}) must be <= {CFG_KW_SYS_INTERVAL}({sysInterval})')
			timeWin.Duration = duration

			#TwtgIntNo 無ければ-1
			Int = node.getInt(CFG_KW_TWTG_INT, -1)
			if Int > 0:
				#対象PEの割込に登録されているか
				if pe and not Int in pe.Ints:
					#err:TwtgIntNoが対象Core/VMの割込に登録されていない
					AppError(f'{node.LastName}:({Int}) is not registered to {CFG_KW_INT} of {pe.DispName}')
			timeWin.TwtgInt = Int

			#TwtgIntInterval
			interval = node.getInt(CFG_KW_TWTG_INTERVAL, 1)
			timeWin.TwtgInterval= interval

			timeWins.append(timeWin)

		return timeWins

