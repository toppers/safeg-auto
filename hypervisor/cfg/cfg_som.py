#SOMクラス

from typing import *

from cfg_core import *
from cfg_schedule import *

class Som_t:
	def __init__(self, name:str):
		self.Name:		str			= name
		self.Schedules:	Dict[int, Schedule_t]	= OrderedDict()
		self.ID:	int	= 0

	#SOMID
	def defSOMID(self, val: bool = False) -> Union[str, int]:
		if val:
			return self.ID
		else:
			return f'SOMID_{self.Name.upper()}'