#HVCクラス

class HVC_t:

	def __init__(self, name: str, args: int = 0, code: str = None) -> None:
		if code is None:
			code = f'TFN_{name.upper()}'

		self.Name:	str	= name
		self.Args:	int = args
		self.Code:	str = code	#TFN_名

		self.ID:	int	= 0		#HVC通し番号

		
	#出力の機能コード
	def defTFN(self, val: bool = False) -> str:
		if val:
			return f'({self.ID}U)'
		else:
			return self.Code
