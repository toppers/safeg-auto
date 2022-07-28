#enumユーティリティ
#	enum名の文字列からenumに変換する

from typing import *
from enum import *

from error import *

#文字列からenum値に変換
#	cls	Enumの型
#	s	変換したい文字列
def EnumVal(cls: Type[Enum], s: str):
	if not issubclass(cls, Enum):
		#Enumではない
		AppError(f'{cls} is not Enum', True)

	if not s in cls.__members__.keys():
		#例外で一覧を返す
		raise EnumValException(cls)

	return cls[s]


#enumの名前リスト
def EnumNames(cls: Type[Enum]) -> List[str]:
	names:	List[str]	= list()

	if issubclass(cls, Enum):
		for e in cls:
			names.append(e.name)

	return names


#文字列が範囲外の例外
class EnumValException(Exception):
	def __init__(self, cls: Type[Enum]) -> None:
		self.names:	List[str]	= list()

		if not issubclass(cls, Enum):
			return

		self.names = EnumNames(cls)


########################################
#テスト
#	このモジュールが起動されたらテスト
if __name__ == "__main__":
	class test_t(Enum):
		a = auto()
		b = auto()
		c = auto()

	#正しく変換
	print(EnumVal(test_t, 'c'))

	#範囲外の例外
	try:
		a = EnumVal(test_t, 'd')
	except EnumValException as e:
		print(e.names)
