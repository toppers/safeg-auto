##コンフィグファイルパーサークラス

from typing import *

from parse_base import *

from parse_target import *

from parse_hv import *
from parse_vm import *
from parse_tdma import *
from parse_vcom import *



#最上位のパーサークラス
class CfgParser_t(ParserBase_t):
	#ctor
	def __init__(self, cfg: Cfg_t, filename: str, target: str) -> None:
		super().__init__(cfg)
		self.__filename:	str	= filename
		self.__target:		str	= target


	#ファイル名
	@property
	def filename(self) -> str:
		return self.__filename

	#ターゲット設定ファイル名
	@property
	def target(self) -> str:
		return self.__target


	#ターゲット設定パース
	def ParseTarget(self) -> None:
		#ターゲット設定読込
		node = Node_t.FromFile(self.target)	#ターゲット設定

		#パース
		Parser_Target_t(self.Cfg).Parse(node)


	#コンフィグパース
	def ParseCfg(self) -> None:
		#設定読込
		node = Node_t.FromFile(self.filename)	#設定

		#パース
		self.Parse(node)


	#パース
	def Parse(self, node: Node_t) -> None:
		#キーワードテーブル
		keys = [NodeDef_t(CFG_KW_HV,	True),
				NodeDef_t(CFG_KW_VM,	True),
				NodeDef_t(CFG_KW_TDMA,	True),
				NodeDef_t(CFG_KW_VCOM,	False)]
		#有効キーワードチェック
		node.chkKeyword(keys)

		#HV
		nodeHV = node.get(CFG_KW_HV)
		if nodeHV:
			Parser_HV_t(self.Cfg).Parse(nodeHV)

		#VM
		nodeVM = node.get(CFG_KW_VM)
		if nodeVM:
			Parser_VM_t(self.Cfg).Parse(nodeVM)

		#TDMA
		nodeTDMA = node.get(CFG_KW_TDMA)
		if nodeTDMA:
			Parser_TDMA_t(self.Cfg).Parse(nodeTDMA)

		#VCOM
		nodeVCOM = node.get(CFG_KW_VCOM)
		if nodeVCOM:
			Parser_VCOM_t(self.Cfg).Parse(nodeVCOM)
