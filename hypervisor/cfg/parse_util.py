#パーサーのサブ処理

from typing import *

import re


#有効な識別子かチェック
def chkIdentifier(s: str) -> bool:
	#先頭は_か英字
	ptn = r'[_a-zA-Z]\w*'
	return bool(re.fullmatch(ptn, s, re.ASCII))

