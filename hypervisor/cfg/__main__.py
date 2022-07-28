
import sys
import os

import hvcfg

#デバッガからmain.pyで起動された場合はモジュール名に変える
if os.path.isfile(sys.argv[0]):
	sys.argv[0] = os.path.dirname(sys.argv[0])

if __name__ == "__main__":
	hvcfg.main()
