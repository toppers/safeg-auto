#hvc_cfg.h 生成

from typing import *
from typing import TextIO

import io

from error import *
from def_global import *

from textwriter import *
from out_util import *

from cfg import *

#ファイル生成
def Generate(file: TextIO, cfg: Cfg_t):
	#ライタ
	defs = TextWriter(io.StringIO())

	#TFNの定義
	for hvc in cfg.HV.HVCs:
		defs.PutSNL(outDefineFunc(hvc.defTFN))
	defs.PutNL()

	#VCOMの機能コード
	if cfg.VCOM.hasVCOM():
		#VCOMのサポート
		defs.PutSNL(outDefineFunc(cfg.VCOM.defSupport))
		defs.PutNL()

		#通信オブジェクト毎のサポート有り
		for comType in cfg.VCOM.comTypes:
			if comType.NumInsts():
				defs.PutSNL(outDefineFunc(comType.defSupport))
		defs.PutNL()

		#オブジェクト型毎に出力
		for comType in cfg.VCOM.comTypes:
			if comType.NumInsts():
				#HVCコード
				for hvc in comType.HVCs:
					defs.PutSNL(outDefineFunc(hvc.defTFN))
				defs.PutNL()

				#ID
				for svar in comType.Insts.values():
					defs.PutSNL(outDefineFunc(svar.defIDName))
				defs.PutNL()

	#HVCテスト関数の実装
	for hvc in cfg.HV.HVCs:
		defs.PutSNL(f'LOCAL_INLINE int')
		defs.PutS(f'{hvc.Name}(')
		if hvc.Args == 0:
			defs.PutS('void')
		else:
			for arg in range(hvc.Args):
				defs.PutS(f'{", " if arg else ""}int arg{arg}')
		defs.PutSNL(')')

		defs.PutSNL( '{')
		defs.PutTabS(f'return CAL_HVC_{hvc.Args}(int, {hvc.defTFN()}', 1)
		for arg in range(hvc.Args):
			defs.PutS(f', int, arg{arg}')
		defs.PutSNL(');')
		defs.PutSNL( '}')


	#テンプレートファイル読込
	tmpf = open(os.path.join(cfg.Param.DataDir, FILE_HVC_H), "r", encoding="UTF8")
	tmpl = tmpf.read()
	tmpf.close()

	tmpl = tmpl.replace('#DEF', defs.f.getvalue())

	file.write(tmpl)
	return
