#
#  HVソーストップ
#
TOP_DIR = ../..

#
#  ターゲットの指定
#
include $(TOP_DIR)/../Makefile.config

#
#  ターゲット依存部のディレクトリの定義
#
TARGETDIR = $(TOP_DIR)/target/$(TARGET)

#
#  ターゲット依存の定義のインクルード
#
include $(TARGETDIR)/Makefile.target

#
#  ユーザープログラム
#
USER_COBJS = sample1.o

#
#  IVCを使用するか
#
USE_IVCCOM = false

#
#  共通定義のインクルード
#
include $(TOP_DIR)/src/Makefile.hv

#
#  コンフィギュレータ入力ファイル
#
CFG_INPUT = sample1.yaml
