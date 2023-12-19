#hv_cfg.c 生成

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
	defs = TextWriter(io.StringIO())

	#VCOMあるか
	if cfg.VCOM.hasVCOM():
		defs.PutSNL('#include "vmcom.h"')
	defs.PutNL()

	#コアID名
	for core in cfg.HV.Cores.values():
		defs.PutSNL(outDefineFunc(core.defIDName))
	defs.PutNL()

	#SPIDテーブル
	defs.PutSNL('const uint8 hvspid_table[TNUM_PHYS_CORE] ={')
	defs.TabUp()
	cores = [f for f in cfg.HV.Cores.values()]
	for i in range(TNUM_PHYS_CORE):
		if i == TNUM_PHYS_CORE_HALF:
			defs.PutTabSNL('#if TNUM_PHYS_CORE == 4', 0)
		core = cfg.HV.Cores.get(i)
		if core:
			defs.PutTabSNL(f'{core.defSPID()},')
		else:
			defs.PutTabSNL('0,')
	defs.TabDown()
	defs.PutSNL('#endif /* TNUM_PHYS_CORE == 4 */')
	defs.PutSNL('};')
	defs.PutNL()

	#CCB
	for core in cfg.HV.Cores.values():
		OutSectionStart(cfg, defs, Section_t.CCB, core.Sections)
		defs.PutSNL(f'static CCB ccb_core{core.ID};')
		OutSectionEnd(cfg, defs, Section_t.CCB, core.Sections)
		defs.PutNL()

	#テーブル
	defs.PutSNL('CCB *const p_ccb_table[TNUM_PHYS_CORE] = {')
	defs.TabUp()
	cores = [f for f in cfg.HV.Cores.values()]
	for i in range(TNUM_PHYS_CORE):
		if i == TNUM_PHYS_CORE_HALF:
			defs.PutTabSNL('#if TNUM_PHYS_CORE == 4', 0)
		if i < len(cfg.HV.Cores):
			core = cores[i]
			defs.PutTabSNL(f'&ccb_core{core.ID},')
		else:
			defs.PutTabSNL('NULL,')
	defs.TabDown()
	defs.PutSNL('#endif /* TNUM_PHYS_CORE == 4 */')
	defs.PutSNL('};')
	defs.PutNL()

	#HVTWDCB
	for core in cfg.HV.Cores.values():
		OutSectionStart(cfg, defs, Section_t.HVTWDCB, core.Sections)
		defs.PutSNL(f'HVTWDCB hvtwdcb_core{core.ID};')
		OutSectionEnd(cfg, defs, Section_t.HVTWDCB, core.Sections)
		defs.PutNL()

	#テーブル
	defs.PutSNL('HVTWDCB *const p_hvtwdcb_table[TNUM_PHYS_CORE] = {')
	defs.TabUp()
	cores = [f for f in cfg.HV.Cores.values()]
	for i in range(TNUM_PHYS_CORE):
		if i == TNUM_PHYS_CORE_HALF:
			defs.PutTabSNL('#if TNUM_PHYS_CORE == 4', 0)
		if i < len(cfg.HV.Cores):
			core = cores[i]
			defs.PutTabSNL(f'&hvtwdcb_core{core.ID},')
		else:
			defs.PutTabSNL('NULL,')
	defs.TabDown()
	defs.PutSNL('#endif /* TNUM_PHYS_CORE == 4 */')
	defs.PutSNL('};')
	defs.PutNL()

	#VM初期化ブロックテーブル
	defs.PutSNL('const VMINIB vminib_table[TNUM_VM] = {')
	defs.TabUp()
	for vm in cfg.VMs.values():
		defs.PutTabSNL('{')
		defs.TabUp()
		defs.PutTabSNL(f'{vm.Core.defIDName()},')
		defs.PutTabSNL(f'0x{vm.ResetBase:>08X},')
		defs.PutTabSNL(f'{vm.defGPID()},')
		defs.PutTabSNL(f'{vm.Name}_SPIDLIST,')
		defs.PutTabSNL(f'{vm.Name}_SPID')
		defs.TabDown()
		defs.PutTabSNL('},')
	defs.TabDown()
	defs.PutSNL('};')
	defs.PutNL()

	#VMコントロールブロック
	for core in cfg.HV.Cores.values():
		OutSectionStart(cfg, defs, Section_t.VMCB, core.Sections)
		for vm in core.VMs:
			defs.PutSNL(f'static VMCB vmcb_{vm.Name};')
		OutSectionEnd(cfg, defs, Section_t.VMCB, core.Sections)
		defs.PutNL()

	#テーブル
	defs.PutSNL('VMCB *const p_vmcb_table[TNUM_VM] = {')
	for vm in cfg.VMs.values():
		defs.PutTabSNL(f'&vmcb_{vm.Name},', 1)
	defs.PutSNL('};')
	defs.PutNL()

	#VM割込初期化ブロックテーブル
	defs.PutSNL('const VMINTINIB vmintinib_table[TNUM_VMINT] = {')
	for vm in cfg.VMs.values():
		for no in vm.Ints:
			defs.PutTabSNL(f'{{{no:>3}, {vm.defVMID()}, {vm.Core.defIDName()}}},', 1)
	defs.PutSNL('};')
	defs.PutNL()

	#TWTG割込カウンタ
	for core in cfg.HV.Cores.values():
		defs.PutSNL(f'uint32 twtgcnt_core{core.ID}[TNUM_TW_CORE{core.ID}];')
	defs.PutNL()

	twtgcnt = {};
	for core in cfg.HV.Cores.values():
		twtgcnt[core.ID] = 0;
	
	#TW初期化ブロック
	for somID, som in cfg.TDMA.SystemOperationModes.items():
		for coreID, sched in som.Schedules.items():
			defs.PutSNL(f'const TWDINIB twdinib_{somID}_core{coreID}[] = {{')
			defs.TabUp()
			for i, tw in enumerate(sched.TimeWins):
				defs.PutTabSNL('{')
				defs.TabUp()
				if tw.type == PEType_t.VM:
					defs.PutTabSNL(f'{tw.VM.defVMID()},')
				elif tw.type == PEType_t.HV:
					defs.PutTabSNL( 'VMID_HVTWD,')
				defs.PutTabSNL(f'TWDTIMER_TO_CYC({tw.Duration}U),')
				defs.PutTabSNL(f'{tw.TwtgInt if tw.TwtgInt > 0 else "0xFFFFFFFF"},')
				defs.PutTabSNL(f'{tw.TwtgInterval},')
				defs.PutTabSNL(f'&twtgcnt_core{coreID}[{twtgcnt[coreID]}]')
				twtgcnt[coreID] += 1
				defs.TabDown()
				defs.PutTabSNL('},')
			defs.PutTabSNL('{')
			defs.PutTabSNL('    VMID_IDLE,')
			defs.PutTabSNL('    0,')
			defs.PutTabSNL('    0xFFFFFFFF,')
			defs.PutTabSNL('    0,')
			defs.PutTabSNL('    0')
			defs.PutTabSNL('}')
			defs.TabDown()
			defs.PutSNL('};')
			defs.PutNL()

	#SOMINIBテーブル
	defs.PutSNL('const SOMINIB	sominib_table[] = {')
	defs.TabUp()
	for somID, som in cfg.TDMA.SystemOperationModes.items():
		defs.PutTabSNL('{')
		#p_twdinib
		defs.TabUp()
		defs.PutTabSNL('{')
		cores = [f for f in cfg.HV.Cores.values()]
		for i in range(TNUM_PHYS_CORE):
			if i == TNUM_PHYS_CORE_HALF:
				defs.PutTabSNL('#if TNUM_PHYS_CORE == 4', 0)
			if i < len(cfg.HV.Cores):
				core = cores[i]
				if (core.ID in som.Schedules):
					defs.PutTabSNL(f'twdinib_{somID}_core{core.ID},')
				else:
					defs.PutTabSNL('NULL,')
			else:
				defs.PutTabSNL('NULL,')
		defs.PutSNL('#endif /* TNUM_PHYS_CORE == 4 */')		
		defs.PutTabSNL('},')
		defs.TabDown()

		defs.PutTabSNL('},')
	defs.TabDown()
	defs.PutSNL('};')
	defs.PutNL()

	#HEMPU初期化テーブル
	defs.PutSNL('const HEMPUINIB hempuinib_table[][TNUM_HEMPU] = {')
	defs.TabUp()
	for vm in cfg.VMs.values():
		defs.PutTabSNL('{')
		defs.TabUp()
		defs.PutTabSNL(f'/* {vm.Name} */')
		for mem in vm.Mems:
			defs.PutTabSNL('{')
			defs.TabUp()
			defs.PutTabSNL(f'0x{mem.base:>08X},')
			defs.PutTabSNL(f'0x{mem.base:>08X} + 0x{mem.size:>08X} -1,')
			defs.PutTabSNL(f'(MPU_E_MASK|MPU_{MemType_t.str(mem.type)}|MPU_WG_MASK|MPU_RG_MASK)')
			defs.TabDown()
			defs.PutTabSNL('},')
		defs.TabDown()
		defs.PutTabSNL('},')
	defs.TabDown()
	defs.PutSNL('};')
	defs.PutNL()

	#HV処理スタックサイズ
	for core in cfg.HV.Cores.values():
		defs.PutSNL(outDefine(f'HV_STACKSIZE_CORE{core.ID}', f'{core.StackHV}'))
	defs.PutNL()

	#HV処理スタック
	for core in cfg.HV.Cores.values():
		OutSectionStart(cfg, defs, Section_t.StackHV, core.Sections)
		defs.PutSNL(f'uint8 hv_stack_core{core.ID}[HV_STACKSIZE_CORE{core.ID}];')
		OutSectionEnd(cfg, defs, Section_t.StackHV, core.Sections)
		defs.PutNL()

	#テーブル
	defs.PutSNL('uint8 *const hv_stkpt_table[TNUM_PHYS_CORE] = {')
	defs.TabUp()
	cores = [c for c in cfg.HV.Cores.values()]
	for i in range(TNUM_PHYS_CORE):
		if i == TNUM_PHYS_CORE_HALF:
			defs.PutTabSNL('#if TNUM_PHYS_CORE == 4', 0)
		if i < len(cores):
			core = cores[i]
			defs.PutTabSNL(f'(hv_stack_core{core.ID} + HV_STACKSIZE_CORE{core.ID}),')
		else:
			defs.PutTabSNL('NULL,')
	defs.TabDown()
	defs.PutSNL('#endif /* TNUM_PHYS_CORE == 4 */')
	defs.PutSNL('};')
	defs.PutNL()

	#HVTW処理スタックサイズ
	for core in cfg.HV.Cores.values():
		defs.PutSNL(outDefine(f'HVTWD_STACKSIZE_CORE{core.ID}', f'{core.StackHVTW}'))
	defs.PutNL()

	#HVTW処理スタック
	for core in cfg.HV.Cores.values():
		OutSectionStart(cfg, defs, Section_t.StackHVTW, core.Sections)
		defs.PutSNL(f'uint8 hvtwd_stack_core{core.ID}[HVTWD_STACKSIZE_CORE{core.ID}];')
		OutSectionEnd(cfg, defs, Section_t.StackHVTW, core.Sections)
		defs.PutNL()

	#テーブル
	defs.PutSNL('uint8 *const hvtwd_stkpt_table[TNUM_PHYS_CORE] = {')
	defs.TabUp()
	cores = [c for c in cfg.HV.Cores.values()]
	for i in range(TNUM_PHYS_CORE):
		if i == TNUM_PHYS_CORE_HALF:
			defs.PutTabSNL('#if TNUM_PHYS_CORE == 4', 0)
		if i < len(cores):
			core = cores[i]
			defs.PutTabSNL(f'(hvtwd_stack_core{core.ID} + HVTWD_STACKSIZE_CORE{core.ID}),')
		else:
			defs.PutTabSNL('NULL,')
	defs.TabDown()
	defs.PutSNL('#endif /* TNUM_PHYS_CORE == 4 */')
	defs.PutSNL('};')
	defs.PutNL()

	#割込初期化ブロック
	defs.PutSNL('HVINTINIB hvinib_table[TNUM_HVINT] = {')
	defs.TabUp()
	for core in cfg.HV.Cores.values():
		for Int in core.Ints.values():
			defs.PutTabSNL(f'{{{Int.no:>3}, {core.defIDName()}, {Int.pri}, {str(Int.enable).lower()}}},')
	defs.TabDown()
	defs.PutSNL('};')
	defs.PutNL()

	#専用割込ハンドラテーブル
	for core in cfg.HV.Cores.values():
		defs.PutSNL(f'const FP hvint_intc1_core{core.ID}[INTC1_INTNO_MAX+1] = {{')
		defs.TabUp()
		for handler in core.IntTbl.tbl:
			defs.PutTabSNL(f'/* {handler.no:>2} */	{handler.handler if handler.handler else "NULL"},')
		defs.TabDown()
		defs.PutSNL('};')
		defs.PutNL()

	#テーブル
	defs.PutSNL('const FP *const p_hvint_intc1_table[TNUM_PHYS_CORE] = {')
	defs.TabUp()
	cores = [c for c in cfg.HV.Cores.values()]
	for i in range(TNUM_PHYS_CORE):
		if i == TNUM_PHYS_CORE_HALF:
			defs.PutTabSNL('#if TNUM_PHYS_CORE == 4', 0)
		if i < len(cores):
			core = cores[i]
			defs.PutTabSNL(f'hvint_intc1_core{core.ID},')
		else:
			defs.PutTabSNL('NULL,')
	defs.TabDown()
	defs.PutSNL('#endif /* TNUM_PHYS_CORE == 4 */')
	defs.PutSNL('};')
	defs.PutNL()

	#共用割込ハンドラテーブル
	defs.PutSNL('const FP hvint_intc2_table[INTC2_INTNO_MAX-INTC2_INTNO_MIN+1] = {')
	defs.TabUp()
	for handler in cfg.IntTbl.tbl:
		defs.PutTabSNL(f'/* {handler.no:>3} */	{handler.handler if handler.handler else "NULL"},')
	defs.TabDown()
	defs.PutSNL('};')
	defs.PutNL()

	#HVC登録テーブル
	defs.PutSNL('void * const hvc_table[TNUM_HVCID] = {')
	defs.TabUp()
	for hvc in cfg.HV.AllHVCs:
		defs.PutTabSNL(f'(void *) &{hvc.Name},')
	defs.TabDown()
	defs.PutSNL('};')
	defs.PutNL()

	#状態変数
	if cfg.VCOM.SVars.NumInsts():
		defs.PutSNL('const ID tmax_svarid = (TMIN_SVARID + TNUM_SVARID - 1);')
		defs.PutNL()

		for svar in cfg.VCOM.SVars.Insts.values():
			defs.PutSNL(f'static uintptr svarbuf_{svar.Name}[VMCOM_COUNT_SZ({svar.Size:>4}, sizeof(uintptr))];')
		defs.PutNL()

		defs.PutSNL('const SVARINIB svarinib_table[TNUM_SVARID] = {')
		defs.TabUp()
		for svar in cfg.VCOM.SVars.Insts.values():
			defs.PutTabSNL(f'{{{svar.Size:>4}, {svar.wrVM.defVMID():<20}, {str(svar.Init).lower():<5}, svarbuf_{svar.Name}}},')
		defs.TabDown()
		defs.PutSNL('};')
		defs.PutNL()

		for svar in cfg.VCOM.SVars.Insts.values():
			defs.PutSNL(f'static SVARCB svarcb_{svar.Name};')
		defs.PutNL()

		defs.PutSNL('SVARCB* const p_svarcb_table[TNUM_SVARID] = {')
		defs.TabUp()
		for svar in cfg.VCOM.SVars.Insts.values():
			defs.PutTabSNL(f'&svarcb_{svar.Name},')
		defs.TabDown()
		defs.PutSNL('};')
		defs.PutNL()

	#メッセージキュー
	if cfg.VCOM.MsgQs.NumInsts():
		defs.PutSNL('const ID tmax_msgqid = (TMIN_MSGQID + TNUM_MSGQID - 1);')
		defs.PutNL()

		for msgq in cfg.VCOM.MsgQs.Insts.values():
			defs.PutSNL(f'static uintptr msgqbuf_{msgq.Name}[VMCOM_COUNT_SZ({msgq.Size:>4}, sizeof(uintptr))];')
		defs.PutNL()

		defs.PutSNL('const MSGQINIB msgqinib_table[TNUM_MSGQID] = {')
		defs.TabUp()
		for msgq in cfg.VCOM.MsgQs.Insts.values():
			defs.PutTabSNL(f'{{{msgq.MaxSize:>4}, {msgq.Size:>4}, {msgq.wrVM.defVMID():<20}, {msgq.rdVM.defVMID():<20}, {str(msgq.Init).lower():<5}, msgqbuf_{msgq.Name}}},')
		defs.TabDown()
		defs.PutSNL('};')
		defs.PutNL()

		for msgq in cfg.VCOM.MsgQs.Insts.values():
			defs.PutSNL(f'static MSGQCB msgqcb_{msgq.Name};')
		defs.PutNL()

		defs.PutSNL('MSGQCB* const p_msgqcb_table[TNUM_MSGQID] = {')
		defs.TabUp()
		for msgq in cfg.VCOM.MsgQs.Insts.values():
			defs.PutTabSNL(f'&msgqcb_{msgq.Name},')
		defs.TabDown()
		defs.PutSNL('};')
		defs.PutNL()

	#共有バッファ
	if cfg.VCOM.SBufs.NumInsts():
		defs.PutSNL('const ID tmax_sbufid = (TMIN_SBUFID + TNUM_SBUFID - 1);')
		defs.PutNL()

		for sbuf in cfg.VCOM.SBufs.Insts.values():
			defs.PutSNL(f'static uintptr sbuf_{sbuf.Name}[VMCOM_COUNT_SZ({sbuf.Size:>4}, sizeof(uintptr))];')
		defs.PutNL()

		impubit = 0
		for i in range(cfg.HV.HostEntryMPUs, cfg.HV.HostEntryMPUs + cfg.VCOM.SBufs.NumInsts()):
			impubit |= (1 << i)

		defs.PutSNL('const SBUFINIB sbufinib_table[TNUM_SBUFID] = {')
		defs.TabUp()
		for sbuf in cfg.VCOM.SBufs.Insts.values():
			accessvm = 0
			for vm in sbuf.AccesVMs:
				accessvm |= (1 << (vm.ID - 1))

			defs.PutTabSNL(f'{{(uint8*)sbuf_{sbuf.Name}, (uint8*)(&sbuf_{sbuf.Name}[VMCOM_COUNT_SZ({sbuf.Size:>4}, sizeof(uintptr))-1]), 0x{accessvm:>02X}, 0x{impubit:>08X}}},')
		defs.TabDown()
		defs.PutSNL('};')
		defs.PutNL()

		for sbuf in cfg.VCOM.SBufs.Insts.values():
			defs.PutSNL(f'static SBUFCB sbufcb_{sbuf.Name};')
		defs.PutNL()

		defs.PutSNL('SBUFCB* const p_sbufcb_table[TNUM_SBUFID] = {')
		defs.TabUp()
		for sbuf in cfg.VCOM.SBufs.Insts.values():
			defs.PutTabSNL(f'&sbufcb_{sbuf.Name},')
		defs.TabDown()
		defs.PutSNL('};')
		defs.PutNL()


	#テンプレート開く
	tmpf = open(os.path.join(cfg.Param.DataDir, FILE_HV_C), "r", encoding="UTF8")
	tmpl = tmpf.read()
	tmpf.close()

	tmpl = tmpl.replace('#DEF', defs.f.getvalue())

	#出力ファイル
	file.write(tmpl)
	return

#セクション切替開始
def OutSectionStart(cfg: Cfg_t, wr: TextWriter, sec: Section_t, secs: Dict[Section_t, str]):
	if not sec in secs:
		return
	name = secs[sec]

	if cfg.Param.Comp == Comp_t.ccrh:
		wr.PutSNL(f'#pragma section {name}')
	else:
		wr.PutSNL(f'#pragma ghs section bss="{name}"')


#セクション切替終了
def OutSectionEnd(cfg: Cfg_t, wr: TextWriter, sec: Section_t, secs: Dict[Section_t, str]):
	if not sec in secs:
		return
	name = secs[sec]

	if cfg.Param.Comp == Comp_t.ccrh:
		wr.PutSNL(f'#pragma section default')
	else:
		wr.PutSNL(f'#pragma ghs section bss=default')

