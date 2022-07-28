##ターゲット設定パーサークラス

from typing import *

from parse_base import *

from def_cfg_kw import *
from yaml_node import *
from parse_util import *

from def_cfg import *

from cfg import *

from cfg_mcu import *
from cfg_mem import *

from error import *


class Parser_Target_t(ParserBase_t):
	#ctor
	def __init__(self, cfg: Cfg_t) -> None:
		super().__init__(cfg)


	#パース
	def Parse(self, node: Node_t) -> None:
		#キーワードテーブル
		keys = [NodeDef_t(CFG_KW_MCU,	True)]
		#有効キーワードチェック
		node.chkKeyword(keys)

		#MCU
		nodeMCU = node.get(CFG_KW_MCU)
		if nodeMCU:
			self.parseMCU(nodeMCU)

		return


	#MCUノード
	def parseMCU(self, node: Node_t) -> None:
		#キーワードテーブル
		keys = [NodeDef_t(CFG_KW_NUM_CORES,	True),
				NodeDef_t(CFG_KW_NUM_INTC2, True),
				NodeDef_t(CFG_KW_MEMORY,	True)]
		#有効キーワードチェック
		node.chkKeyword(keys)

		mcu = self.Cfg.MCU

		#物理コア数
		cores = node.getInt(CFG_KW_NUM_CORES)
		if cores == 0:
			AppError("{node.LastName}:(core) must be > 0")
		mcu.NumCore		= cores

		#INTC2割込数
		intc2 = node.getInt(CFG_KW_NUM_INTC2)
		mcu.NumINTC2	= intc2

		#Memory
		nodeMem = node.get(CFG_KW_MEMORY)
		if nodeMem:
			mcu.Mems = self.parseMemory(nodeMem, True)

		return


	