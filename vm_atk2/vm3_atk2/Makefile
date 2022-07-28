#
#  ターゲットの定義
#
include ../../Makefile.config

#
#  アプリケーション名
#
APPLNAME = sample1_vm3

#
#  ターゲットのコンフィギュレーション
#
LDSCRIPT = ./u2a_pe1_vm3
CDEFS := $(CDEFS) -DPE2_ONLY
CDEFS := $(CDEFS) -DHWC_USE_UNIT0
CDEFS := $(CDEFS) -DRLIN3_PORT_NO=$(RLIN3_PORT2)
CDEFS := $(CDEFS) -DOMIT_CLOCK_INIT
CDEFS := $(CDEFS) -DOMIT_PORT_INIT
ifeq ($(TARGET),renesas_rcar_s4)
  CDEFS := $(CDEFS) -DTARGET_RCARS4
endif

OBJNAME = atk2-sc1_vm3

ifeq ($(COMPILER),GHS)
include ../Makefile.ghs
endif

ifeq ($(COMPILER),CCRH)
include ../Makefile.ccrh
endif
