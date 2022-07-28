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
TARGET_NAME = vm1_1_prog

#
#  ユーザーオブジェクト  
#
COBJS = sample.o

#
#  ソースディレクトリ
#
SRC_DIR = ./src_vm1_1

#
#  追加のインクルードディレクトリ
#
INCLUDES := -I$(TOP_DIR)/hypervisor/obj/obj_2pe

#
#  ターゲット毎の定義の使用
#
CFLAGS := $(CFLAGS) -DRLIN3_PORT_NO=$(RLIN3_PORT1) -DOSTM_CLK_MHZ=$(OSTM_CLK_MHZ)

#
#  リンカスクリプト
#
LDSCRIPT = rh850v2_ghs

#
#  共通定義のインクルード
#
include ../common/Makefile.common
