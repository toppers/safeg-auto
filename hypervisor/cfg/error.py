##エラー処理モジュール
#	エラー検出時に AppError() を呼ぶ
#	チェック処理が完了後、本処理前に ExitIfError() を呼ぶとそれまでにエラーがあれば終了する
from __future__ import annotations
from typing import *

if TYPE_CHECKING:
	from yaml_node import Node_t

#エラーで終了するか
__StopOnError: bool = False

#エラー数
__cntError: int = 0


#エラーで終了する設定
def setStopOnError(val: bool):
	global __StopOnError
	__StopOnError = val


#アプリエラー
#	mes		メッセージ
#	exit	trueなら即時終了する
def AppError(mes: str, exit: bool = False):
	global __StopOnError
	global __cntError

	__cntError += 1	#エラー数進める
	print(mes)		#メッセージ

	#exitかエラー終了ならここで終了
	if exit or __StopOnError:
		ExitIfError()


#アプリエラーYAMLノード付き
def AppErrorNode(node: Node_t, mes: str, exit: bool = False):
	m: str = f'{node.FullName}:({node.yaml}) {mes}'
	AppError(m, exit)

#アプリエラーYAML最後のノード付き
def AppErrorLastNode(node: Node_t, val, mes: str, exit: bool = False):
	m: str = f'{node.LastName}:({val}) {mes}'
	AppError(m, exit)


#エラーなら終了
def ExitIfError():
	#エラーがあれば終了
	if __cntError:
		#エラー数を表示する
		print(f'Exit due to {__cntError} errors')
		exit(__cntError)	#終了コードはエラー数

