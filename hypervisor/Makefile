update_asm:
	cd ./src; ../../utils/s2asm.py hv_support.S
	cd ./src; ../../utils/s2asm.py rvector_pe01.S
	cd ./src; ../../utils/s2asm.py rvector_pe23.S
	cd ./src; ../../utils/s2asm.py vector.S
	cd ./src; ../../utils/s2asm.py start.S
	cd ./src; ../../utils/s2asm.py start_u2a16.S
	cd ./src; ../../utils/s2asm.py start_u2a8.S
	cd ./src; ../../utils/s2asm.py hv_asm.h
	cd ./libguest; ../../utils/s2asm.py hvc_asm.S
	cd ./target/renesas_t1v1; ../../../utils/s2asm.py optionbyte.S
	cd ./target/sunny_gpxu2a; ../../../utils/s2asm.py optionbyte.S
