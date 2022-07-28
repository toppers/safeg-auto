##コンフィグファイルパーサークラス

from typing import *

from parse_base import *

from def_cfg_kw import *
from yaml_node import *
from parse_util import *

from def_cfg import *

from cfg import *

from cfg_vm import *

from error import *


class Parser_VM_t(ParserBase_t):
	#ctor
	def __init__(self, cfg: Cfg_t) -> None:
		super().__init__(cfg)


	#VMノード
	def Parse(self, nodes: Node_t) -> None:
		#有効キーワード
		keys = [NodeDef_t(CFG_KW_NAME,			True),
				NodeDef_t(CFG_KW_COREID,		True),
				NodeDef_t(CFG_KW_RESET_BASE,	True),
				NodeDef_t(CFG_KW_MEMORY,		True),
				NodeDef_t(CFG_KW_SPID_LIST,		True),
				NodeDef_t(CFG_KW_SPID,			True),
				NodeDef_t(CFG_KW_INT,			True)]

		for node in nodes.array():
			#有効キーワードチェック
			node.chkKeyword(keys)

			#ID
			#err:VMIDが識別子として無効
			name = node.getIdentifier(CFG_KW_NAME)
			if name:
				#err:同名のオブジェクトが存在するか
				self.Cfg.existName(name, node.LastName)
				##同じIDがあるか
				#if name in self.cfgTop.VMs:
				#	#@ER008IDが既に存在する
				#	AppError(f'{node.FullName}: {CFG_KW_NAME}({name}) is already defined')

			#VMオブジェクト生成
			vm = VM_t(name)

			#CoreID
			coreid = node.getInt(CFG_KW_COREID)
			core = self.Cfg.HV.Cores.get(coreid)
			#存在するか
			if core:
				#コアがSingleVMCOREなら複数関連付けのチェック
				if core.SingleVM and len(core.VMs):
					#err:SingleVMCOREのコアに複数割付
					AppErrorLastNode(node, coreid, f'{core.DispName} is SingleVMCORE, and already used by {core.VMs[0].Name}')
				else:
					vm.Core = core
					core.ConnectVM(vm)	#コアに接続
			else:
				#err:CoreIDのコアが存在しない
				AppErrorLastNode(node, coreid, 'CoreID is not defined')

			#ResetBase
			#err:ResetBaseが32bit範囲外
			base = node.get32bit(CFG_KW_RESET_BASE)
			vm.ResetBase = base

			#Memory
			nodeMems = node.get(CFG_KW_MEMORY)
			if nodeMems:
				vm.Mems = self.parseMemory(nodeMems, False)

			#SPIDList
			nodeSPIDList = node.get(CFG_KW_SPID_LIST)
			if nodeSPIDList:
				spidList: List[int] = list()
				#順に処理
				for nodeVal in nodeSPIDList.array():
					val = nodeVal.asInt()
					#最大値チェック
					if val > MAX_SPID:
						#err:SPIDListのSPIDが範囲外
						AppError(f'{nodeVal.FullName}:({val}) must be <= {MAX_SPID}')
					#既にあるか
					elif val in spidList:
						#err:SPIDList内でSPIDが重複
						AppError(f'{nodeVal.FullName}:({val}) is already included')
					else:
						spidList.append(val)

				vm.SPIDList = spidList

			#SPID
			spid = node.getInt(CFG_KW_SPID)
			#最大値チェック
			if spid > MAX_SPID:
				#err:SPIDが範囲外
				AppError(f'{node.LastName}:({spid}) must be <= {MAX_SPID}')
			#リストにあるか
			elif not spid in spidList:
				#err:SPIDListにないSPID
				AppError(f'{node.LastName}:({spid}) in not included in {CFG_KW_SPID_LIST} {spidList}')
			else:
				vm.SPID = spid

			#Interrupt
			nodeInt = node.get(CFG_KW_INT)
			if nodeInt:
				vm.Ints = self.parseVMInt(nodeInt, vm)

			#最大数か
			if len(self.Cfg.VMs) >= MAX_VM:
				#@ER036:VM数超過
				AppError(f'{node.FullName}:(name:{name}) Max number of VMs is {MAX_VM}')
				continue

			#同じ名前がなければ追加
			if name and not name in self.Cfg.VMs:
				self.Cfg.VMs[name] = vm

		return

	
	#VM:Interruptノード
	def parseVMInt(self, nodes: Node_t, vm: VM_t) -> List[int]:
		ints: List[int] = list()

		for node in nodes.array():
			no = node.asInt()
			#すでにあるか
			if no in ints:
				#err:VMの割込番号が重複
				AppError(f'{node.FullName}:({no}) is already used')

			#ハンドラ取得
			handler =None
			if vm.Core:
				handler = vm.Core.IntTbl.get(no)
			if not handler:
				handler = self.Cfg.IntTbl.get(no)
				if not handler:
					#err:割込番号が範囲外
					AppError(f'{node.FullName}:({no}) must be < {self.Cfg.IntTbl.MaxIntNo}')

			if handler:
				#既に使われているか
				if handler.Used():
					#err:割込番号が他のコアで既に使われている
					AppError(f'{node.FullName}: {CFG_KW_INT_NO}({no}) is already used by {handler.RegisteredStr()}')
				else:
					#ハンドラに登録
					handler.RegisterVM(vm)

			ints.append(no)

		return ints


