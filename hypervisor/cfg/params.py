#コマンドラインパラメータ処理クラス
#	パラメータ以外にもアプリ全体の設定も保持する
#	・データdir


from typing import *
from enum import *

import sys
import os
import argparse

from def_global import *
from enum_util import *


#コンパイラの種類
class Comp_t(Enum):
	ghs		= auto()
	ccrh	= auto()


class Params:
	#ctor	
	def __init__(self) -> None:
		self.__home = sys.argv[0]	#data-dirに使用

		parser = argparse.ArgumentParser()

		parser.add_argument("-comp",	help="Compiler [ghs*|ccrh]", default="ghs", choices=["ghs", "ccrh"])
		parser.add_argument("-cfg",		help="Input cfg-YAML filename", default=FILE_CFG_DEFAULT)
		parser.add_argument("-target",	help="Input target cfg filename", required=True);
		parser.add_argument("-dir",		help="Output directory", default="")
		parser.add_argument("-errstop",	help="Stop on error", action="store_true")

		#解析 エラーならここで終了
		self.__arg = parser.parse_args()

	#cfgファイル名
	@property
	def Cfg(self) -> str:
		return self.__arg.cfg

	#コンパイラ
	@property
	def Comp(self) -> Comp_t:
		return EnumVal(Comp_t, self.__arg.comp)

	#ターゲット設定ファイル
	@property
	def TargetCfg(self) -> str:
		return self.__arg.target

	#出力dir
	@property
	def Dir(self) -> str:
		return self.__arg.dir

	#エラーで停止するか
	@property
	def isErrStop(self) -> bool:
		return self.__arg.errstop

	#プロジェクトのホームdir
	@property
	def HomeDir(self) -> str:
		return self.__home

	#プロジェクトのdata-dir
	@property
	def DataDir(self) -> str:
		return os.path.join(self.HomeDir, DIR_DATA)

