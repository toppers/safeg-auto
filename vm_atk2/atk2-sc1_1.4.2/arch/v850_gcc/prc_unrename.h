/* This file is generated from prc_rename.def by genrename. */

/* This file is included only when prc_rename.h has been included. */
#ifdef TOPPERS_PRC_RENAME_H
#undef TOPPERS_PRC_RENAME_H

/*
 *  prc_config.c
 */
#undef except_nest_cnt
#undef v850_cpu_exp_no
#undef v850_cpu_exp_pc
#undef v850_cpu_exp_sp
#undef current_iintpri
#undef pmr_setting_tbl
#undef nested_lock_os_int_cnt
#undef current_intpri
#undef prc_hardware_initialize
#undef prc_initialize
#undef prc_terminate
#undef x_config_int
#undef default_int_handler
#undef target_is_int_controllable

/*
 *  Os_Lcfg.c
 */
#undef pmr_isr2_mask
#undef pmr_isr1_mask
#undef isr2max_iintpri
#undef isr_tbl
#undef isr_p_isrcb_tbl
#undef intbp_tbl

/*
 *  prc_support.S
 */
#undef fe_exception_entry
#undef ei_exception_entry
#undef interrupt
#undef dispatch
#undef start_dispatch
#undef exit_and_dispatch
#undef start_r
#undef stack_change_and_call_func_1
#undef stack_change_and_call_func_2
#undef return_main

#ifdef TOPPERS_LABEL_ASM

/*
 *  prc_config.c
 */
#undef _except_nest_cnt
#undef _v850_cpu_exp_no
#undef _v850_cpu_exp_pc
#undef _v850_cpu_exp_sp
#undef _current_iintpri
#undef _pmr_setting_tbl
#undef _nested_lock_os_int_cnt
#undef _current_intpri
#undef _prc_hardware_initialize
#undef _prc_initialize
#undef _prc_terminate
#undef _x_config_int
#undef _default_int_handler
#undef _target_is_int_controllable

/*
 *  Os_Lcfg.c
 */
#undef _pmr_isr2_mask
#undef _pmr_isr1_mask
#undef _isr2max_iintpri
#undef _isr_tbl
#undef _isr_p_isrcb_tbl
#undef _intbp_tbl

/*
 *  prc_support.S
 */
#undef _fe_exception_entry
#undef _ei_exception_entry
#undef _interrupt
#undef _dispatch
#undef _start_dispatch
#undef _exit_and_dispatch
#undef _start_r
#undef _stack_change_and_call_func_1
#undef _stack_change_and_call_func_2
#undef _return_main

#endif /* TOPPERS_LABEL_ASM */


#endif /* TOPPERS_PRC_RENAME_H */
