/* kernel_cfg.c */
#include "kernel/kernel_int.h"
#include "kernel_cfg.h"

#if TKERNEL_PRID != 0x07u
#error The kernel does not match this configuration file.
#endif

/*
 *  Include Directives (#include)
 */

#include "rca_lib.h"
#include "target_timer.h"
#include "target_syssvc.h"
#include "target_serial.h"
#include "syssvc/serial.h"
#include "syssvc/logtask.h"
#include "syssvc/syslog.h"
#include "syssvc/banner.h"

/*
 *  Default Definitions of Trace Log Macros
 */

#ifndef LOG_ISR_ENTER
#define LOG_ISR_ENTER(intno)
#endif /* LOG_ISR_ENTER */

#ifndef LOG_ISR_LEAVE
#define LOG_ISR_LEAVE(intno)
#endif /* LOG_ISR_LEAVE */

/*
 *  Task Management Functions
 */

const ID _kernel_tmax_tskid = (TMIN_TSKID + TNUM_TSKID - 1);

static STK_T _kernel_stack_RCA_MAINTASK[COUNT_STK_T(RCA_MAINTASK_STACK_SIZE)];
static STK_T _kernel_stack_RCA_IDLE_TASK[COUNT_STK_T(512)];

const TINIB _kernel_tinib_table[TNUM_TSKID] = {
	{ (TA_ACT), (intptr_t)(0), ((TASK)(rca_maintask)), INT_PRIORITY(RCA_MAINTASK_SETUP_PRI), ROUND_STK_T(RCA_MAINTASK_STACK_SIZE), _kernel_stack_RCA_MAINTASK, (TA_NULL), (NULL) },
	{ (TA_ACT), (intptr_t)(0), ((TASK)(rca_idle_task)), INT_PRIORITY(15), ROUND_STK_T(512), _kernel_stack_RCA_IDLE_TASK, (TA_NULL), (NULL) }
};

TCB _kernel_tcb_table[TNUM_TSKID];

const ID _kernel_torder_table[TNUM_TSKID] = {
	RCA_MAINTASK, RCA_IDLE_TASK
};

/*
 *  Semaphore Functions
 */

const ID _kernel_tmax_semid = (TMIN_SEMID + TNUM_SEMID - 1);

const SEMINIB _kernel_seminib_table[TNUM_SEMID] = {
	{ (TA_TPRI), (0), (1) },
	{ (TA_TPRI), (1), (1) },
	{ (TA_TPRI), (1), (1) },
	{ (TA_TPRI), (1), (1) },
	{ (TA_TPRI), (1), (1) }
};

SEMCB _kernel_semcb_table[TNUM_SEMID];

/*
 *  Eventflag Functions
 */

const ID _kernel_tmax_flgid = (TMIN_FLGID + TNUM_FLGID - 1);

TOPPERS_EMPTY_LABEL(const FLGINIB, _kernel_flginib_table);
TOPPERS_EMPTY_LABEL(FLGCB, _kernel_flgcb_table);

/*
 *  Dataqueue Functions
 */

const ID _kernel_tmax_dtqid = (TMIN_DTQID + TNUM_DTQID - 1);

TOPPERS_EMPTY_LABEL(const DTQINIB, _kernel_dtqinib_table);
TOPPERS_EMPTY_LABEL(DTQCB, _kernel_dtqcb_table);

/*
 *  Priority Dataqueue Functions
 */

const ID _kernel_tmax_pdqid = (TMIN_PDQID + TNUM_PDQID - 1);

TOPPERS_EMPTY_LABEL(const PDQINIB, _kernel_pdqinib_table);
TOPPERS_EMPTY_LABEL(PDQCB, _kernel_pdqcb_table);

/*
 *  Mailbox Functions
 */

const ID _kernel_tmax_mbxid = (TMIN_MBXID + TNUM_MBXID - 1);

TOPPERS_EMPTY_LABEL(const MBXINIB, _kernel_mbxinib_table);
TOPPERS_EMPTY_LABEL(MBXCB, _kernel_mbxcb_table);

/*
 *  Fixed-sized Memorypool Functions
 */

const ID _kernel_tmax_mpfid = (TMIN_MPFID + TNUM_MPFID - 1);

TOPPERS_EMPTY_LABEL(const MPFINIB, _kernel_mpfinib_table);
TOPPERS_EMPTY_LABEL(MPFCB, _kernel_mpfcb_table);

/*
 *  Cyclic Handler Functions
 */

const ID _kernel_tmax_cycid = (TMIN_CYCID + TNUM_CYCID - 1);

const CYCINIB _kernel_cycinib_table[TNUM_CYCID] = {
	{ (TA_STA), (intptr_t)(0), (rca_CycHandler), (1), (1) }
};

CYCCB _kernel_cyccb_table[TNUM_CYCID];

/*
 *  Alarm Handler Functions
 */

const ID _kernel_tmax_almid = (TMIN_ALMID + TNUM_ALMID - 1);

TOPPERS_EMPTY_LABEL(const ALMINIB, _kernel_alminib_table);
TOPPERS_EMPTY_LABEL(ALMCB, _kernel_almcb_table);

/*
 *  Interrupt Management Functions
 */


#define TNUM_INHNO	10
const uint_t _kernel_tnum_inhno = TNUM_INHNO;

INTHDR_ENTRY((16+7), 23, rca_USB_Handler)
INTHDR_ENTRY((16+14), 30, rca_SERCOM5_Handler)
INTHDR_ENTRY((16+9), 25, rca_SERCOM0_Handler)
INTHDR_ENTRY((16+11), 27, rca_SERCOM2_Handler)
INTHDR_ENTRY((16+12), 28, rca_SERCOM3_Handler)
INTHDR_ENTRY((16+13), 29, rca_SERCOM4_Handler)
INTHDR_ENTRY((16+4), 20, rca_EIC_Handler)
INTHDR_ENTRY((16+20), 36, rca_TC5_Handler)
INTHDR_ENTRY((16+3), 19, rca_RTC_Handler)
INTHDR_ENTRY(INHNO_TIMER, 15, target_timer_handler)

const INHINIB _kernel_inhinib_table[TNUM_INHNO] = {
	{ ((16+7)), (TA_NULL), (FP)(INT_ENTRY((16+7), rca_USB_Handler)) },
	{ ((16+14)), (TA_NULL), (FP)(INT_ENTRY((16+14), rca_SERCOM5_Handler)) },
	{ ((16+9)), (TA_NULL), (FP)(INT_ENTRY((16+9), rca_SERCOM0_Handler)) },
	{ ((16+11)), (TA_NULL), (FP)(INT_ENTRY((16+11), rca_SERCOM2_Handler)) },
	{ ((16+12)), (TA_NULL), (FP)(INT_ENTRY((16+12), rca_SERCOM3_Handler)) },
	{ ((16+13)), (TA_NULL), (FP)(INT_ENTRY((16+13), rca_SERCOM4_Handler)) },
	{ ((16+4)), (TA_NULL), (FP)(INT_ENTRY((16+4), rca_EIC_Handler)) },
	{ ((16+20)), (TA_NULL), (FP)(INT_ENTRY((16+20), rca_TC5_Handler)) },
	{ ((16+3)), (TA_NULL), (FP)(INT_ENTRY((16+3), rca_RTC_Handler)) },
	{ (INHNO_TIMER), (TA_NULL), (FP)(INT_ENTRY(INHNO_TIMER, target_timer_handler)) }
};

#define TNUM_INTNO	10
const uint_t _kernel_tnum_intno = TNUM_INTNO;

const INTINIB _kernel_intinib_table[TNUM_INTNO] = {
	{ ((16+7)), (TA_NULL), (-1) },
	{ ((16+14)), (TA_NULL), (-1) },
	{ ((16+9)), (TA_NULL), (-1) },
	{ ((16+11)), (TA_NULL), (-1) },
	{ ((16+12)), (TA_NULL), (-1) },
	{ ((16+13)), (TA_NULL), (-1) },
	{ ((16+4)), (TA_NULL), (-1) },
	{ ((16+20)), (TA_NULL), (-1) },
	{ ((16+3)), (TA_NULL), (-1) },
	{ (INTNO_TIMER), (TA_ENAINT|INTATR_TIMER), (INTPRI_TIMER) }
};

/*
 *  CPU Exception Management Functions
 */

#define TNUM_EXCNO	0
const uint_t _kernel_tnum_excno = TNUM_EXCNO;


TOPPERS_EMPTY_LABEL(const EXCINIB, _kernel_excinib_table);

/*
 *  Stack Area for Non-task Context
 */

#ifdef DEFAULT_ISTK

#define TOPPERS_ISTKSZ		DEFAULT_ISTKSZ
#define TOPPERS_ISTK		DEFAULT_ISTK

#else /* DEAULT_ISTK */

static STK_T _kernel_istack[COUNT_STK_T(DEFAULT_ISTKSZ)];

#define TOPPERS_ISTKSZ		ROUND_STK_T(DEFAULT_ISTKSZ)
#define TOPPERS_ISTK		_kernel_istack

#endif /* DEAULT_ISTK */

const SIZE		_kernel_istksz = TOPPERS_ISTKSZ;
STK_T *const	_kernel_istk = TOPPERS_ISTK;

#ifdef TOPPERS_ISTKPT
STK_T *const	_kernel_istkpt = TOPPERS_ISTKPT(TOPPERS_ISTK, TOPPERS_ISTKSZ);
#endif /* TOPPERS_ISTKPT */

/*
 *  Time Event Management
 */

TMEVTN   _kernel_tmevt_heap[TNUM_TSKID + TNUM_CYCID + TNUM_ALMID];

/*
 *  Module Initialization Function
 */

void
_kernel_initialize_object(void)
{
	_kernel_initialize_task();
	_kernel_initialize_semaphore();
	_kernel_initialize_cyclic();
	_kernel_initialize_interrupt();
	_kernel_initialize_exception();
}

/*
 *  Initialization Routine
 */

void
_kernel_call_inirtn(void)
{
	((INIRTN)(rca_init))((intptr_t)(0));
	((INIRTN)(target_timer_initialize))((intptr_t)(0));
	((INIRTN)(syslog_initialize))((intptr_t)(0));
	((INIRTN)(print_banner))((intptr_t)(0));
}

/*
 *  Termination Routine
 */

void
_kernel_call_terrtn(void)
{
	((TERRTN)(target_timer_terminate))((intptr_t)(0));
}

/*
 *  Target-dependent Definitions (ARM-M)
 */



__attribute__ ((section(".isr_vector"))) 
const FP _kernel_vector_table[] =      
{                                    
	(FP)(TOPPERS_ISTKPT(TOPPERS_ISTK, TOPPERS_ISTKSZ)), /* 0 The initial stack pointer */
	(FP)_kernel__start,                    /* 1 The reset handler */
 	(FP)(_kernel_core_exc_entry), /* 2 */
 	(FP)(_kernel_core_exc_entry), /* 3 */
 	(FP)(_kernel_core_exc_entry), /* 4 */
 	(FP)(_kernel_core_exc_entry), /* 5 */
 	(FP)(_kernel_core_exc_entry), /* 6 */
 	(FP)(_kernel_core_exc_entry), /* 7 */
 	(FP)(_kernel_core_exc_entry), /* 8 */
 	(FP)(_kernel_core_exc_entry), /* 9 */
 	(FP)(_kernel_core_exc_entry), /* 10 */
 	(FP)(_kernel_core_exc_entry), /* 11 */
 	(FP)(_kernel_core_exc_entry), /* 12 */
 	(FP)(_kernel_core_exc_entry), /* 13 */
 	(FP)(pendsvc_handler),        /* 14 PandSVCall handler */
 	(FP)(_kernel_core_int_entry), /* 15 */
 	(FP)(_kernel_core_int_entry), /* 16 */
 	(FP)(_kernel_core_int_entry), /* 17 */
 	(FP)(_kernel_core_int_entry), /* 18 */
 	(FP)(_kernel_core_int_entry), /* 19 */
 	(FP)(_kernel_core_int_entry), /* 20 */
 	(FP)(_kernel_core_int_entry), /* 21 */
 	(FP)(_kernel_core_int_entry), /* 22 */
 	(FP)(_kernel_core_int_entry), /* 23 */
 	(FP)(_kernel_core_int_entry), /* 24 */
 	(FP)(_kernel_core_int_entry), /* 25 */
 	(FP)(_kernel_core_int_entry), /* 26 */
 	(FP)(_kernel_core_int_entry), /* 27 */
 	(FP)(_kernel_core_int_entry), /* 28 */
 	(FP)(_kernel_core_int_entry), /* 29 */
 	(FP)(_kernel_core_int_entry), /* 30 */
 	(FP)(_kernel_core_int_entry), /* 31 */
 	(FP)(_kernel_core_int_entry), /* 32 */
 	(FP)(_kernel_core_int_entry), /* 33 */
 	(FP)(_kernel_core_int_entry), /* 34 */
 	(FP)(_kernel_core_int_entry), /* 35 */
 	(FP)(_kernel_core_int_entry), /* 36 */
 	(FP)(_kernel_core_int_entry), /* 37 */
 	(FP)(_kernel_core_int_entry), /* 38 */
 	(FP)(_kernel_core_int_entry), /* 39 */
 	(FP)(_kernel_core_int_entry), /* 40 */
 	(FP)(_kernel_core_int_entry), /* 41 */
 	(FP)(_kernel_core_int_entry), /* 42 */
 	(FP)(_kernel_core_int_entry), /* 43 */
 	(FP)(_kernel_core_int_entry), /* 44 */

};


const FP _kernel_exc_tbl[] = 
{
	(FP)(_kernel_default_exc_handler), /* 0 */
	(FP)(_kernel_default_exc_handler), /* 1 */
	(FP)(_kernel_default_exc_handler), /* 2 */
	(FP)(_kernel_default_exc_handler), /* 3 */
	(FP)(_kernel_default_exc_handler), /* 4 */
	(FP)(_kernel_default_exc_handler), /* 5 */
	(FP)(_kernel_default_exc_handler), /* 6 */
	(FP)(_kernel_default_exc_handler), /* 7 */
	(FP)(_kernel_default_exc_handler), /* 8 */
	(FP)(_kernel_default_exc_handler), /* 9 */
	(FP)(_kernel_default_exc_handler), /* 10 */
	(FP)(_kernel_default_exc_handler), /* 11 */
	(FP)(_kernel_default_exc_handler), /* 12 */
	(FP)(_kernel_default_exc_handler), /* 13 */
	(FP)(_kernel_default_exc_handler), /* 14 */
	(FP)(target_timer_handler), /* 15 */
	(FP)(_kernel_default_int_handler), /* 16 */
	(FP)(_kernel_default_int_handler), /* 17 */
	(FP)(_kernel_default_int_handler), /* 18 */
	(FP)(rca_RTC_Handler), /* 19 */
	(FP)(rca_EIC_Handler), /* 20 */
	(FP)(_kernel_default_int_handler), /* 21 */
	(FP)(_kernel_default_int_handler), /* 22 */
	(FP)(rca_USB_Handler), /* 23 */
	(FP)(_kernel_default_int_handler), /* 24 */
	(FP)(rca_SERCOM0_Handler), /* 25 */
	(FP)(_kernel_default_int_handler), /* 26 */
	(FP)(rca_SERCOM2_Handler), /* 27 */
	(FP)(rca_SERCOM3_Handler), /* 28 */
	(FP)(rca_SERCOM4_Handler), /* 29 */
	(FP)(rca_SERCOM5_Handler), /* 30 */
	(FP)(_kernel_default_int_handler), /* 31 */
	(FP)(_kernel_default_int_handler), /* 32 */
	(FP)(_kernel_default_int_handler), /* 33 */
	(FP)(_kernel_default_int_handler), /* 34 */
	(FP)(_kernel_default_int_handler), /* 35 */
	(FP)(rca_TC5_Handler), /* 36 */
	(FP)(_kernel_default_int_handler), /* 37 */
	(FP)(_kernel_default_int_handler), /* 38 */
	(FP)(_kernel_default_int_handler), /* 39 */
	(FP)(_kernel_default_int_handler), /* 40 */
	(FP)(_kernel_default_int_handler), /* 41 */
	(FP)(_kernel_default_int_handler), /* 42 */
	(FP)(_kernel_default_int_handler), /* 43 */
	(FP)(_kernel_default_int_handler), /* 44 */

};

const uint32_t _kernel_bitpat_cfgint[3] = {
	UINT32_C(0x7a988000), 
	UINT32_C(0x00000010), 
	UINT32_C(0x00000000), 

};

const uint8_t _kernel_int_iipm_tbl[] = {
	UINT8_C(0x00), /* 0x000 */
	UINT8_C(0x00), /* 0x001 */
	UINT8_C(0x00), /* 0x002 */
	UINT8_C(0x00), /* 0x003 */
	UINT8_C(0x00), /* 0x004 */
	UINT8_C(0x00), /* 0x005 */
	UINT8_C(0x00), /* 0x006 */
	UINT8_C(0x00), /* 0x007 */
	UINT8_C(0x00), /* 0x008 */
	UINT8_C(0x00), /* 0x009 */
	UINT8_C(0x00), /* 0x00a */
	UINT8_C(0x00), /* 0x00b */
	UINT8_C(0x00), /* 0x00c */
	UINT8_C(0x00), /* 0x00d */
	UINT8_C(0x00), /* 0x00e */
	UINT8_C(0x03), /* 0x00f */
	UINT8_C(0xff), /* 0x010 */
	UINT8_C(0xff), /* 0x011 */
	UINT8_C(0xff), /* 0x012 */
	UINT8_C(0x03), /* 0x013 */
	UINT8_C(0x03), /* 0x014 */
	UINT8_C(0xff), /* 0x015 */
	UINT8_C(0xff), /* 0x016 */
	UINT8_C(0x03), /* 0x017 */
	UINT8_C(0xff), /* 0x018 */
	UINT8_C(0x03), /* 0x019 */
	UINT8_C(0xff), /* 0x01a */
	UINT8_C(0x03), /* 0x01b */
	UINT8_C(0x03), /* 0x01c */
	UINT8_C(0x03), /* 0x01d */
	UINT8_C(0x03), /* 0x01e */
	UINT8_C(0xff), /* 0x01f */
	UINT8_C(0xff), /* 0x020 */
	UINT8_C(0xff), /* 0x021 */
	UINT8_C(0xff), /* 0x022 */
	UINT8_C(0xff), /* 0x023 */
	UINT8_C(0x03), /* 0x024 */
	UINT8_C(0xff), /* 0x025 */
	UINT8_C(0xff), /* 0x026 */
	UINT8_C(0xff), /* 0x027 */
	UINT8_C(0xff), /* 0x028 */
	UINT8_C(0xff), /* 0x029 */
	UINT8_C(0xff), /* 0x02a */
	UINT8_C(0xff), /* 0x02b */
	UINT8_C(0xff), /* 0x02c */
};

const uint32_t _kernel_iipm_enable_irq_tbl[]={
	UINT32_C(0x00000000), /* 0(-4) */
	UINT32_C(0x00000000), /* 1(-3) */
	UINT32_C(0x00000000), /* 2(-2) */
	UINT32_C(0x00000000), /* 3(-1) */
	UINT32_C(0x00107a98), /* 4(0) */
};

const uint8_t _kernel_iipm_enable_systic_tbl[]={
	UINT8_C(0x00), /* 0(-4) */
	UINT8_C(0x00), /* 1(-3) */
	UINT8_C(0x00), /* 2(-2) */
	UINT8_C(0x00), /* 3(-1) */
	UINT8_C(0x01), /* 4(0) */
};
