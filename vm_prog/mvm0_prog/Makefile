#
#  トップディレクトリ
#
TOP_DIR  = ../..

#
#  ターゲットの指定
#
include $(TOP_DIR)/Makefile.config

#
#  ターゲット名
#
TARGET_NAME = mvm0_prog

#
#  ユーザーオブジェクト  
#
COBJS = sample.o

#
#  ソースディレクトリ
#
SRC_DIR = ./src_mvm0

#
#  追加のインクルードディレクトリ
#
INCLUDES := -I$(TOP_DIR)/hypervisor/obj/obj_2pe_mvm

#
#  リンカスクリプト
#
LDSCRIPT = rh850v2_ghs

#
#  ターゲット毎の定義の使用
#
CFLAGS := $(CFLAGS) -DRLIN3_PORT_NO_CORE0=$(RLIN3_PORT0) -DRLIN3_PORT_NO_CORE1=$(RLIN3_PORT1) \
          -DOSTM_CLK_MHZ=$(OSTM_CLK_MHZ)

#
#  共通定義のインクルード
#
include ../common/Makefile.common
