#定数定義


#入力ファイル名
FILE_CFG_DEFAULT	= "hv_cfg.yaml"

#生成ファイル名
FILE_HVC_H			= "hvc_cfg.h"
FILE_HV_H			= "hv_cfg.h"
FILE_HV_C			= "hv_cfg.c"
FILE_HV_ASM_GHS		= "hv_cfg_asm.h"
FILE_HV_ASM_CCRH	= "hv_cfg_asm.inc"

#ディレクトリ名
DIR_DATA			= "data"

#cfg定数
MAX_HOST_ENTRY_MPU	= 32
MAX_HVC_ARGS		= 3
MAX_SPID			= 31
NUM_INTC1_INT		= 32
MAX_INT_PRIORITY	= 63
MAX_VM				= 32
MAX_VM_CORE			= 8

#生成
TNUM_PHYS_CORE		= 4
TNUM_PHYS_CORE_HALF	= 2

#VCOMのHVC
HVC_VCOM_STATE_VAR	= [	("ReadStateVariable",		"TFN_HVC_RSVAR"),
						("WriteStateVariable",		"TFN_HVC_WSVAR"),
						("DeactivateStateVariable",	"TFN_HVC_DSVAR")]

HVC_VCOM_MSG_QUEUE	= [ ("ReadMessageQueue",		"TFN_HVC_RMSGQ"),
						("WriteMessageQueue",		"TFN_HVC_WMSGQ"),
						("DeactivateMessageQueue",	"TFN_HVC_DMSGQ")]

HVC_VCOM_SHARED_BUF	= [ ("AcquireSharedBuffer",		"TFN_HVC_AQSB"),
						("ReleaseSharedBuffer",		"TFN_HVC_RSSB")]

#出力
STR_TAB	= "    "	#タブ出力文字列
