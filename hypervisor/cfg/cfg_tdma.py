#TDMAクラス

from typing import *

from collections import OrderedDict

from cfg_schedule import *

class TDMA_t:
	def __init__(self):
		self.Interval:	int						= 0
		self.Schedules:	Dict[int,Schedule_t]	= OrderedDict()




