#hv_cfg_asm 生成

from typing import *
from typing import TextIO

import io

from error import *
from def_global import *

from textwriter import *
from out_util import *

from cfg import *

from cfg_vcom_svar import *
from cfg_vcom_msgq import *
from cfg_vcom_sbuf import *

#ファイル生成
def Generate(dir: str, cfg: Cfg_t):
	defs = TextWriter(io.StringIO())
	fname: str	= None

	ccrh = cfg.Param.Comp == Comp_t.ccrh	#CCRHか

	#サポートコアのビットパターン
	defs.PutSNL(outDefineFunc(cfg.HV.defNumSupportCoreBit, ccrh))

	#リーダコアID
	defs.PutSNL(outDefineFunc(cfg.HV.defLeaderCoreID, ccrh))

	#HVC数
	defs.PutSNL(outDefineFunc(cfg.HV.defNUM_HVC, ccrh))


	#VMCOMがあるか
	if cfg.VCOM.hasVCOM():
		defs.PutNL()
		#VCOMのサポート
		defs.PutSNL(outDefineFunc(cfg.VCOM.defSupport, ccrh))
		defs.PutNL()

		#通信オブジェクト毎のサポート有り
		for comType in cfg.VCOM.comTypes:
			if comType.NumInsts():
				defs.PutSNL(outDefineFunc(comType.defSupport, ccrh))

		#SBUF固有出力
		if cfg.VCOM.SBufs.NumInsts():
			defs.PutNL()
			if ccrh:
				defs.PutSNL(f'stm_mp_sbuf .macro pbase\n\tstm.mp\te4-e{4+cfg.VCOM.SBufs.NumInsts()-1}, [pbase]\n.endm')
			else:
				defs.PutSNL(f'.macro stm_mp_sbuf pbase\n\tstm.mp\te4-e{4+cfg.VCOM.SBufs.NumInsts()-1}, [pbase]\n.endm')
			defs.PutNL()

	if ccrh:
		fname	= FILE_HV_ASM_CCRH
	else:
		fname	= FILE_HV_ASM_GHS

	#テンプレート開く
	tmpf = open(os.path.join(cfg.Param.DataDir, fname), "r", encoding="UTF8")
	tmpl = tmpf.read()
	tmpf.close()

	tmpl = tmpl.replace('#DEF', defs.f.getvalue())

	#出力ファイル
	file = open(os.path.join(dir, fname), "w", encoding="UTF8", newline='\r\n')
	file.write(tmpl)
	return
