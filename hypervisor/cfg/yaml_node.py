#CfgParserのサブ処理

from __future__ import annotations
from typing import *

import yaml

from error import *

from enum_util import *
from parse_util import *

##ノード定義
class NodeDef_t:
	def __init__(self, name: str, need: bool=False):
		self.name:	str		= name	#ノード名
		self.need:	bool	= need	#必須か

##ノード
class Node_t:
	def __init__(self, yaml, name: Union[str, int] = None, parent: 'Node_t' = None):
		self.yaml							= yaml		#ノード
		self.__parentName:	str				= None		#親ノード名
		self.__name:		Union[str, int]	= name		#自ノード名
		self.nodeTbl:		Dict[str, bool]	= dict()	#テーブル[ノード名, 必須]
		self.__lastName:	str				= ""		#最後にアクセスした子ノード名 エラー時に使用

		if parent != None and parent.__name != None:
			self.__parentName = parent.FullName


	#YAML読込んで生成
	#	ファイルは存在している事
	@classmethod
	def FromFile(cls, name: str) -> Node_t:
		#デフォルトのエンコーディングで開く
		fin = open(name, "r")

		try:
			try:
				top = yaml.safe_load(fin)
			except UnicodeDecodeError as e:
				#デフォルトで駄目ならUTF8を指定して開く
				fin.close()
				fin = open(name, "r", encoding="UTF8")
				try:
					top = yaml.safe_load(fin)
				except UnicodeDecodeError as e:
					#err:エンコーディングで読めない
					mes = str(e) + '\n'
					AppError(mes, True)
		except yaml.YAMLError as e:
			#err:YAML文法エラー
			mes = str(e) + '\n'
			AppError(mes, True)

		return Node_t(top)


	#自ノード名
	@property
	def Name(self) -> Union[str, int]:
		return self.__name


	#親ノード名
	@property
	def ParentName(self) -> str:
		return self.__parentName


	#エラー時に表示する自ノード名
	@property
	def FullName(self) -> str:
		if self.__parentName == None and self.__name == None:
			return '(top)'

		ret: str = ""
		if self.__parentName != None:
			ret += f'{self.__parentName}'

		if self.__name != None:
			if type(self.__name) is int:
				#名前がintなら配列
				ret += f'[{self.__name}]'
			else:
				if ret:
					ret += '.'
				ret += str(self.__name)

		return ret


	#最後に取得した子ノード名
	#	getInt() 等で取得した子ノードでのエラー時のノード名表示に使用する
	@property
	def LastName(self) -> str:
		return self.__lastName


	#必須ノードか
	def isNeed(self, name: str) -> bool:
		#テーブルに無ければFalse
		if not name in self.nodeTbl:
			return False

		return self.nodeTbl[name]	#テーブルの必須を返す


	#不要キーワードチェック
	#	tbl	ノード定義テーブル
	def chkKeyword(self, tbl: List[NodeDef_t]) -> bool:
		#マップか
		self.chkMap()

		#内部テーブル作成
		for nodeCheck in tbl:
			self.nodeTbl[nodeCheck.name] = nodeCheck.need

		keys = self.names()
		if not keys:
			return False

		for key in keys:
			#リストに無いキーワードか
			if not key in self.nodeTbl:
				kwList = [kw for kw in self.nodeTbl]
				#@err:無効なキーワード
				AppError(f'{self.FullName}: Invaid keyword - {key}, select {kwList}')
				return False

		return True


	#空ノードか
	def empty(self) -> bool:
		#空ノード
		if self.yaml == None:
			return True
		#空文字列も空ノードとみなす
		if self.yaml == "":
			return True
		
		return False


	#マップか
	def chkMap(self, noerr: bool = False) -> bool:
		#Noneも許容する
		if self.yaml != None and type(self.yaml) is not dict:
			if not noerr:
				#@err:ノードがマップ形式でない
				AppError(f'{self.FullName}: is not map(key: value) type')
			return False
		return True


	#配列か
	def chkArray(self, noerr: bool = False) -> bool:
		if not type(self.yaml) is list:
			if not noerr:
				#@err:ノードが配列形式でない
				AppError(f'{self.FullName}: is not sequence(array) type')
			return False
		return True


	#スカラか
	def chkScalar(self, noerr: bool = False) -> bool:
		#空か
		if self.empty():
			if not noerr:
				#err:スカラノードが空
				AppError(f'{self.FullName}: is empty node')
			return False

		if self.chkMap(True):
			if not noerr:
				#@err:ノードがスカラではなくマップ
				AppError(f'{self.FullName}: [type = map(key: value)] must be scalar type')
			return False

		if self.chkArray(True):
			if not noerr:
				#@err:ノードがスカラではなく配列
				AppError(f'{self.FullName}: [type = sequence(array)] must be scalar type')
			return False

		return True


	#配列のインデックスで取得
	def idx(self, idx: int) -> 'Node_t':
		#配列でなければエラー
		if not self.chkArray():
			return None

		return Node_t(self.yaml[idx], idx, self)

	
	#配列で取得
	def array(self) -> List['Node_t']:
		#空ノードか エラーにしない
		if self.empty():
			return []	#空のリスト

		#配列でなければエラー
		if not self.chkArray():
			return []	#空のリスト
	
		array: List['Node_t'] = list()
		for i in range(len(self.yaml)):
			array.append(self.idx(i))

		return array


	#マップの名前で取得
	def get(self, name: str, noNeed: bool = False) -> 'Node_t':
		#マップか
		if not self.chkMap():
			return None

		#存在しないか
		if self.yaml == None or not name in self.yaml:
			#必須ならエラー
			if noNeed == False and self.isNeed(name):
				#err:必須ノードがない
				AppError(f'{self.FullName}: no keyword - {name}')
			return None

		yaml = self.yaml[name]
		node = Node_t(yaml, name, self)
		self.__lastName = node.FullName

		return node


	#マップのノード名リスト取得
	def names(self) -> Iterable[str]:
		#空ノードか エラーにしない
		if self.empty():
			return []	#空のリスト

		#マップでなければエラーになる
		if not self.chkMap():
			return []	#空のリスト

		return self.yaml.keys()


	#マップの値リスト取得
	def values(self) -> List['Node_t']:
		#空ノードか エラーにしない
		if self.empty():
			return []	#空のリスト

		#マップでなければエラーになる
		if not self.chkMap():
			return []	#空のリスト

		lastName = self.__lastName	#LastNameは変えたくないので待避
		ret = [self.get(name) for name in self.names()]
		self.__lastName = lastName	#LastName復帰

		return ret


	#マップのint取得
	def getInt(self, name: str, default: int = None) -> int:
		#ノード取得
		node = self.get(name, default != None)
		if node is None:
			#デフォルト値があれば返す
			if default != None:
				return default

			#以降の処理が進まないので0とする
			return 0

		return node.asInt()


	#マップの32bit取得
	def get32bit(self, name: str, default: int = None) -> int:
		#ノード取得
		node = self.get(name, default != None)
		if node is None:
			#デフォルト値があれば返す
			if default != None:
				return default

			#以降の処理が進まないので0とする
			return 0

		return node.as32bit()


	#マップのstring取得
	def getStr(self, name: str, default: str = None) -> str:
		#ノード取得
		node = self.get(name, default != None)
		if node is None:
			#デフォルト値を返す
			if default != None:
				return default

			#以降の処理が進まないのでNoneとする
			return None

		return node.asStr()


	#マップの識別子取得
	def getIdentifier(self, name: str, default: str = None) -> str:
		#ノード取得
		node = self.get(name, default != None)
		if node is None:
			#デフォルト値を返す
			if default != None:
				return default

			#以降の処理が進まないので必須ならNoneとする
			return None

		return node.asIdentifier()


	#マップのbool取得
	def getBool(self, name: str, default: bool = None) -> bool:
		#ノード取得
		node = self.get(name, default != None)
		if node is None:
			#デフォルト値を返す
			if default != None:
				return default

			#以降の処理が進まないので必須ならFalseとする
			return False

		return node.asBool()


	#マップのenum取得
	def getEnum(self, name: str, enumCls: Type[Enum], default: Enum = None):
		#ノード取得
		node = self.get(name, default != None)
		if node is None:
			#デフォルト値を返す
			if default != None:
				return default

			#以降の処理が進まないので必須ならNone
			return None

		return node.asEnum(enumCls)


	#intとして取得
	def asInt(self) -> int:
		#スカラか
		if not self.chkScalar():
			return 0

		try:
			val = int(self.yaml)
			if val < 0:
				#err:負のint
				AppError(f'{self.FullName}:({val}) must be >= 0')
			return val
		except:
			#@err:intではない
			AppError(f'{self.FullName}:({self.yaml}) must be int type')
			return 0


	#32bit値として取得
	def as32bit(self) -> int:
		#intで取得 空ならエラー
		val = self.asInt()

		if val >= (1 << 32):
			#err:アドレス・サイズが32bit範囲外
			AppError(f'{self.FullName}:(0x{val:>08X}) must be <= 0xFFFF_FFFF')

		return val


	#strとして取得
	def asStr(self) -> str:
		#スカラか
		if not self.chkScalar():
			return None

		try:
			return str(self.yaml)
		except:
			#@err:strではない (多分発生しない)
			AppError(f'{self.FullName}:({self.yaml}) must be string type')
			return None


	#識別子として取得
	def asIdentifier(self) -> str:
		#文字列で取得 空ならエラー
		val = self.asStr()
		if not val:
			return None

		#識別子チェック
		if not chkIdentifier(val):
			#err:無効な識別子
			AppError(f'{self.FullName}:({val}) is invalid as name')

		return val


	#boolとして取得
	def asBool(self) -> bool:
		#スカラか
		if not self.chkScalar():
			return False

		try:
			typ = type(self.yaml)
			#intも許容する
			if not typ is bool and not typ is int:
				raise Exception
			return bool(self.yaml)
		except:
			#err:ノード値がboolではない
			AppError(f'{self.FullName}:({self.yaml}) must be bool type (true | false)')
			return False


	#enumとして取得
	def asEnum(self, enumCls: Type[Enum]):
		#文字列で取得 空ならエラー
		s = self.asStr()
		if not s:
			return None

		try:
			#enumに変換 無効なら例外
			return EnumVal(enumCls, s)
		except EnumValException as e:
			#err:無効なenum値
			AppError(f'{self.FullName}:({s}) is invalid, select {e.names}')
			return None

