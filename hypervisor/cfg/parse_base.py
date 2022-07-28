#パーサーの基本クラス

from abc import *
from typing import *

from cfg import *

from def_cfg_kw import *
from yaml_node import *
from parse_util import *

from def_cfg import *

from cfg_mem import *

from error import *


#パーサーの基本クラス
class ParserBase_t(metaclass=ABCMeta):
	#ctor
	def __init__(self, cfg: Cfg_t) -> None:
		self.__cfg:	Cfg_t	= cfg


	#データ
	@property
	def Cfg(self) -> Cfg_t:
		return self.__cfg


	#パース
	@abstractmethod
	def Parse(self, node: Node_t) -> None:
		pass


	#Memoryノード共通処理
	#	target	Trueならターゲット設定ファイル
	def parseMemory(self, nodes: Node_t, target: bool) -> List[Mem_t]:
		#有効キーワード
		keys = [NodeDef_t(CFG_KW_BASE,	True),
				NodeDef_t(CFG_KW_SIZE,	True),
				NodeDef_t(CFG_KW_TYPE,	True)]

		#ターゲットならID追加
		if target:
			keys.append(NodeDef_t(CFG_KW_ID, True))

		mems: List[Mem_t] = list()

		for node in nodes.array():
			#キーワードチェック
			node.chkKeyword(keys)

			#base
			#err:baseが32bit範囲外
			base = node.get32bit(CFG_KW_BASE)

			#size
			#err:sizeが32bit範囲外
			size = node.get32bit(CFG_KW_SIZE)

			#領域が32bit範囲内か
			end = base + size - 1
			if end >= (1 << 32):
				#err:領域が32bitからオーバーフロー
				AppError(f'{node.LastName}: end address(0x{end:>08X}) overflow 0xFFFF_FFFF')

			#type
			#err:typeが指定外
			typ = None
			nodeType = node.get(CFG_KW_TYPE)

			#ターゲット設定かどうかで指定出来るtypeが異なる
			if target:
				typ = nodeType.asEnum(MemTypeTarget_t)
			else:
				typ = nodeType.asEnum(MemTypeCfg_t)

			if typ is not None:
				typ = nodeType.asEnum(MemType_t)

			id = -1
			if target:
				#ターゲットならID
				id = node.getInt(CFG_KW_ID)
				#同じ種類で同一のIDがあるか
				if typ is not None and id is not None and next((m for m in mems if m.type == typ and m.id == id), None) is not None:
					#@ER027:同じ種類で同一IDのメモリが既にある
					AppError(f'{node.LastName}:({id}) same ID of type({typ.name}) is already defined')
					continue
				#範囲が重なるメモリがあるか
				over = next((m for m in mems if m.isOverwrapped(base, size)), None)
				if over:
					#@ER026:メモリ範囲が重なる
					AppError(f'{node.FullName}: Memory(Type:{typ.name} ID:{id})  overwrapped with Type:{over.type.name} ID:{over.id}')
					continue

			else:
				#ターゲットでなければMCUのメモリにあるかチェック
				valid = False
				for memdef in (m for m in self.Cfg.MCU.Mems if typ in m.type):
					if memdef.isInclude(base, size):
						valid = True
						break
				if not valid:
					#@ER011:MCUのメモリにない範囲
					AppError(f'{node.FullName}: memory range is not defined in {CFG_KW_MCU}.{CFG_KW_MEMORY}')
					continue

			#Memオブジェクト生成			
			mem = Mem_t(base, size, typ, id)
			mems.append(mem)


		#ターゲットでない場合
		if not target:
			#NumberOfHostEntryMPUを越えていないか
			if len(mems) > self.Cfg.HV.HostEntryMPUs:
				#err:メモリ数がNumberOfHostEntryMPUの範囲外
				AppError(f'{nodes.FullName}: too much Memories({len(mems)}), must be <= {CFG_KW_HOST_ENTRY_MPU}({self.Cfg.HV.HostEntryMPUs})')

		return mems
