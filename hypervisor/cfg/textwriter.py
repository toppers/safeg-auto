#TextWriterクラス

from typing import *

from io import StringIO

from def_global import *


class TextWriter:
	def __init__(self, file: StringIO):
		self.f:		StringIO	= file
		self.Tab:	int			= 0

	#文字列出力
	def PutS(self, s: str):
		self.f.write(s)

	#改行出力
	def PutNL(self):
		self.f.write("\n")

	#タブ出力
	def PutTab(self, tab: int = -1):
		if tab < 0:
			tab = self.Tab

		for i in range(tab):
			self.f.write(STR_TAB)

	#文字列と改行
	def PutSNL(self, s: str):
		self.PutS(s)
		self.PutNL()

	#タブと文字列
	def PutTabS(self, s: str, tab: int = -1):
		self.PutTab(tab)
		self.PutS(s)

	#タブと文字列と改行
	def PutTabSNL(self, s: str, tab: int = -1):
		self.PutTab(tab)
		self.PutSNL(s)

	#タブ追加
	def TabUp(self, tab: int = 1):
		self.Tab += tab

	#タブ解除
	def TabDown(self, tab: int = 1):
		if tab == 0 or self.Tab <= tab:
			self.Tab = 0
		else:
			self.Tab -= tab



