#スケジュールクラス

from typing import *

from cfg_core import *
from cfg_timewin import *

class Schedule_t:
	def __init__(self):
		self.Core:		Core_t				= None
		self.TimeWins:	List[TimeWindow_t]	= list()
