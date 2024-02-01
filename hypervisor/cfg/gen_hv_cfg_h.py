#hv_cfg.h 生成

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
	defs= TextWriter(io.StringIO())

	#サポートコア数
	defs.PutSNL(outDefineFunc(cfg.HV.defNumSupportCore))
		
	#コアのビットパターン
	defs.PutSNL(outDefineFunc(cfg.HV.defNumSupportCoreBit))

	#リーダコアID
	defs.PutSNL(outDefineFunc(cfg.HV.defLeaderCoreID))

	#シングルVMコアのビットパターン
	defs.PutSNL(outDefineFunc(cfg.HV.defSingleVMCoreBit))

	#各コアのSPID
	defs.PutNL()
	for core in cfg.HV.Cores.values():
		defs.PutSNL(outDefineFunc(core.defSPID))
	defs.PutNL()

	#全HVのSPIDのビットパターン
	defs.PutSNL(outDefineFunc(cfg.HV.defHV_SPID))

	#各コアのVMのSPIDのビットパターン
	defs.PutNL()
	for core in cfg.HV.Cores.values():
		defs.PutSNL(outDefineFunc(core.defVMSPID))
	defs.PutNL()

	#全コアのHVとVMのSPIDのビットパターン
	defs.PutSNL(outDefineFunc(cfg.HV.defAllSPID))
	defs.PutNL()
		
	#CRAM使用サイズ
	for cram in cfg.HV.CRAMs:
		defs.PutSNL(outDefine(f'CRAM_USAGE_SIZE_{cram.id}', f'0x{cram.size:>08X}'))

	#VM数
	defs.PutNL()
	defs.PutSNL(outDefine(f'TNUM_VM ', f'{len(cfg.VMs)}'))
	for core in cfg.HV.Cores.values():
		defs.PutSNL(outDefine(f'TNUM_VM_CORE{core.ID}', f'{len(core.VMs)}'))

	#VMID
	defs.PutNL()
	for vm in cfg.VMs.values():
		defs.PutSNL(outDefine(f'{vm.defVMID()}', f'{vm.defVMID(True)}'))
	defs.PutNL()

	#ホストエントリMPU数
	defs.PutSNL(outDefine(f'TNUM_HEMPU', f'{cfg.HV.HostEntryMPUs}'))
	defs.PutSNL(outDefine(f'TMAX_HEMPU', f'{cfg.HV.HostEntryMPUs-1}'))

	#HVC数
	defs.PutNL()
	defs.PutSNL(outDefineFunc(cfg.HV.defNUM_HVC))

	#VMのGPID コア毎に出力する
	for core in cfg.HV.Cores.values():
		defs.PutNL()
		for vm in core.VMs:
			defs.PutSNL(outDefine(f'{vm.defGPID()}', f'{vm.defGPID(True)}'))

	#VMのSPIDリスト
	defs.PutNL()
	for vm in cfg.VMs.values():
		ptn = 0
		for spid in vm.SPIDList:
			ptn |= (1 << spid)
		defs.PutSNL(outDefine(f'{vm.Name}_SPIDLIST', f'0x{ptn:>08X}'))

	#VMのSPID
	defs.PutNL()
	for vm in cfg.VMs.values():
		defs.PutSNL(outDefine(f'{vm.Name}_SPID', f'{vm.SPID}'))
	defs.PutNL()

	#VM割込数
	vmint = 0
	for vm in cfg.VMs.values():
		vmint += len(vm.Ints)
	defs.PutSNL(outDefine(f'TNUM_VMINT', f'{vmint}'))
	defs.PutNL()
	for vm in cfg.VMs.values():
		defs.PutSNL(outDefine(f'{vm.Name}_TNUM_VMINT', f'{len(vm.Ints)}'))
	defs.PutNL()

	#HV割込数
	hvint = 0
	for core in cfg.HV.Cores.values():
		hvint += len(core.Ints)
	defs.PutSNL(outDefine(f'TNUM_HVINT', f'{hvint}'))

	#タイムウインドウ数
	defs.PutNL()
	for coreID, numtw in cfg.TDMA.NumTW.items():
		defs.PutSNL(outDefine(f'TNUM_TW_CORE{coreID}', f'{numtw}'))
	defs.PutNL()

	#システム周期
	defs.PutSNL(outDefine(f'SYSTEM_INTERVAL_US', f'{cfg.TDMA.Interval}U'))

	#SOM数
	defs.PutNL()
	defs.PutSNL(outDefine(f'TNUM_SOM ', f'{len(cfg.TDMA.SystemOperationModes)}'))
	defs.PutNL()

	#SOMID
	for som in cfg.TDMA.SystemOperationModes.values():
		defs.PutSNL(outDefine(f'{som.defSOMID()}', f'{som.defSOMID(True)}'))
	defs.PutNL()

	
	#########################
	#VCOM

	#VMCOMがあるか
	if cfg.VCOM.hasVCOM():
		#VCOMのサポート
		defs.PutSNL(outDefineFunc(cfg.VCOM.defSupport))
		defs.PutNL()

		#通信オブジェクト毎のサポート有り
		for comType in cfg.VCOM.comTypes:
			if comType.NumInsts():
				defs.PutSNL(outDefineFunc(comType.defSupport))
		defs.PutNL()

		#通信オブジェクト数
		for comType in cfg.VCOM.comTypes:
			if comType.NumInsts():
				defs.PutSNL(outDefineFunc(comType.defTNUM))
		defs.PutNL()

		#SBUF固有出力
		if cfg.VCOM.SBufs.NumInsts():
			defs.PutSNL(outDefine(f'TNUM_HEMPU_SBUF', f'{cfg.VCOM.SBufs.NumInsts()}'))
			defs.PutNL()
			defs.PutSNL(outDefine(f'TMIN_HEMPU_SBUF', f'{cfg.HV.HostEntryMPUs}'))
			defs.PutSNL(outDefine(f'TMAX_HEMPU_SBUF', f'{cfg.HV.HostEntryMPUs + cfg.VCOM.SBufs.NumInsts() - 1}'))
			defs.PutNL()

	#########################
	#テンプレートファイル読込
	tmpf = open(os.path.join(cfg.Param.DataDir, FILE_HV_H), "r", encoding="UTF8")
	tmpl = tmpf.read()
	tmpf.close()

	tmpl = tmpl.replace('#DEF', defs.f.getvalue())

	file.write(tmpl)
	return
