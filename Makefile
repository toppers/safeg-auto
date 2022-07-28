MODE = normal
#MODE = perf

HV_OBJ = obj_1pe
#HV_OBJ = obj_2pe
#HV_OBJ = obj_2pe_mvm
#HV_OBJ = obj_2pe_svm
#HV_OBJ = obj_1pe_com
#HV_OBJ = obj_1pe_atk2
#HV_OBJ = obj_2pe_atk2

all: hv vm

hv:
	make all MODE=$(MODE) -C ./hypervisor/obj/$(HV_OBJ); 

ifeq ($(HV_OBJ),obj_1pe)
vm:
	make all MODE=$(MODE) -C ./vm_prog/vm0_0_prog
	make all MODE=$(MODE) -C ./vm_prog/vm0_1_prog
endif

ifeq ($(HV_OBJ),obj_1pe_perf)
vm:
	make all MODE=$(MODE) -C ./vm_prog/vm0_0_perf_prog
	make all MODE=$(MODE) -C ./vm_prog/vm0_1_perf_prog
endif

ifeq ($(HV_OBJ),obj_2pe)
vm:
	make all MODE=$(MODE) -C ./vm_prog/vm0_0_prog
	make all MODE=$(MODE) -C ./vm_prog/vm0_1_prog
	make all MODE=$(MODE) -C ./vm_prog/vm1_0_prog
	make all MODE=$(MODE) -C ./vm_prog/vm1_1_prog
endif

ifeq ($(HV_OBJ),obj_2pe_mvm)
vm:
	make all MODE=$(MODE) -C ./vm_prog/mvm0_prog
	make all MODE=$(MODE) -C ./vm_prog/mvm1_prog
endif

ifeq ($(HV_OBJ),obj_2pe_svm)
vm:
	make all MODE=$(MODE) -C ./vm_prog/vm0_0_prog
	make all MODE=$(MODE) -C ./vm_prog/vm0_1_prog
	make all MODE=$(MODE) -C ./vm_prog/vm1_0_prog
endif

ifeq ($(HV_OBJ),obj_1pe_com)
vm:
	make all MODE=$(MODE) -C ./vm_prog/vm0_0_com_prog
	make all MODE=$(MODE) -C ./vm_prog/vm0_1_com_prog
endif

ifeq ($(HV_OBJ),obj_1pe_ivc)
vm:
	make all MODE=$(MODE) -C ./vm_prog/vm0_0_ivc_prog
	make all MODE=$(MODE) -C ./vm_prog/vm0_1_ivc_prog
endif

ifeq ($(HV_OBJ),obj_1pe_atk2)
vm:
	make -C ./vm_atk2/vm1_atk2
	make -C ./vm_atk2/vm2_atk2

depend:
	make depend -C ./vm_atk2/vm1_atk2
	make depend -C ./vm_atk2/vm2_atk2
endif

ifeq ($(HV_OBJ),obj_2pe_atk2)
vm:
	make -C ./vm_atk2/vm1_atk2
	make -C ./vm_atk2/vm2_atk2
	make -C ./vm_atk2/vm3_atk2
	make -C ./vm_atk2/vm4_atk2

depend:
	make depend -C ./vm_atk2/vm1_atk2
	make depend -C ./vm_atk2/vm2_atk2
	make depend -C ./vm_atk2/vm3_atk2
	make depend -C ./vm_atk2/vm4_atk2
endif

clean: vmclean atk2clean
	make clean -C hypervisor/obj/$(HV_OBJ); 

VMPROGLIST := $(wildcard vm_prog/*_prog)

vmclean: $(VMPROGLIST)

$(VMPROGLIST):
	make clean -C $@

ATK2PROGLIST := $(wildcard vm_atk2/*_atk2)

atk2clean: $(ATK2PROGLIST)

$(ATK2PROGLIST):
	make clean -C $@

.PHONY: $(VMPROGLIST) $(ATK2PROGLIST)
