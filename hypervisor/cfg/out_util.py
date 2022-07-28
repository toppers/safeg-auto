#コード出力ユーティリティ

from typing import *


#define生成
#	name		マクロ名
#	val			値
#	ccrh_asm	CCRHのasm出力の時True
def outDefine(name: str, val: Union[str, int] = None, ccrh_asm: bool = False) -> str:
	if ccrh_asm:
		return f'{name} .set {1 if val is None else val}'
	else:
		return f"#define {name}{'' if val is None else f' {val}'}"


#オブジェクトのdefXXX関数でdefine生成
#	f			関数 func(val: bool) val:Falseで定義名 Trueなら値
#	ccrh_asm	CCRHのasm出力の時True
def outDefineFunc(f: Callable[[bool], Union[str, int]], ccrh_asm: bool = False) -> str:
	return outDefine(str(f(False)), f(True), ccrh_asm)

