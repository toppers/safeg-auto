#
#  トップディレクトリ
#
TOP_DIR  = ../..

#
#  ターゲットの指定等の読込み
#
include $(TOP_DIR)/Makefile.config

#
#  ターゲット名
#
TARGET_NAME = vm0_0_prog

#
#  ユーザーオブジェクト  
#
COBJS = sample.o

#
#  ソースディレクトリ
#
SRC_DIR = ./src_vm0_0

#
#  追加のインクルードディレクトリ
#
INCLUDES := -I$(TOP_DIR)/hypervisor/obj/obj_1pe

#
#  リンカスクリプト
#
LDSCRIPT = rh850v2_ghs

#
#  ターゲット毎の定義の使用
#
CFLAGS := $(CFLAGS) -DRLIN3_PORT_NO=$(RLIN3_PORT0) -DOSTM_CLK_MHZ=$(OSTM_CLK_MHZ)

#
#  共通定義のインクルード
#
include ../common/Makefile.common
