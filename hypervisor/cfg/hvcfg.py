##メイン処理モジュール

import os

from params import *
from parse import *
from cfg import *

import error

## @brief	main
## @param
## @return
def main():
	#コマンドライン処理
	param = Params()	#エラーなら終了する

	#エラーで終了設定
	error.setStopOnError(param.isErrStop)

	#CFGファイルが無ければここで終了
	if not os.path.exists(param.Cfg):
		#err:YAMLファイルが無い
		AppError(f'cfg-file not found: {param.Cfg}', True)

	#ターゲット設定ファイルが無ければここで終了
	if not os.path.exists(param.TargetCfg):
		#err:YAMLファイルが無い
		AppError(f'target cfg-file not found: {param.TargetCfg}', True)

	#コンフィグデータ生成
	cfg = Cfg_t(param)

	#パーサー生成
	parser = CfgParser_t(cfg, param.Cfg, param.TargetCfg)

	#ターゲット設定パース
	parser.ParseTarget()	
	#エラーがあればここで終了
	error.ExitIfError()

	#ターゲット設定読込後
	cfg.PostTargetLoad()

	#cfgパース
	parser.ParseCfg()

	#読込後処理
	cfg.PostLoad()

	#エラーがあればここで終了
	error.ExitIfError()

	#ファイル生成
	cfg.Generate()

