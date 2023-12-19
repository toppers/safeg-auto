#TDMAクラス

from typing import *

from collections import OrderedDict

from cfg_som import *

class TDMA_t:
	def __init__(self):
		self.Interval:	int						= 0
		self.SystemOperationModes:	Dict[str, Som_t]	= OrderedDict()
		self.NumTW: Dict[int, int] = OrderedDict()
