/* cfg1_out.c */
#define TOPPERS_CFG1_OUT  1
#include "kernel/kernel_int.h"
#include "rca_lib.h"
#include "target_timer.h"
#include "target_syssvc.h"
#include "target_serial.h"
#include "syssvc/serial.h"
#include "syssvc/logtask.h"
#include "syssvc/syslog.h"
#include "syssvc/banner.h"


#ifdef INT64_MAX
  typedef int64_t signed_t;
  typedef uint64_t unsigned_t;
#else
  typedef int32_t signed_t;
  typedef uint32_t unsigned_t;
#endif

#include "target_cfg1_out.h"

const uint32_t TOPPERS_cfg_magic_number = 0x12345678;
const uint32_t TOPPERS_cfg_sizeof_signed_t = sizeof(signed_t);
const uint32_t TOPPERS_cfg_sizeof_pointer = sizeof(const volatile void*);
const unsigned_t TOPPERS_cfg_CHAR_BIT = ((unsigned char)~0u == 0xff ? 8 : 16);
const unsigned_t TOPPERS_cfg_CHAR_MAX = ((char)-1 < 0 ? (char)((unsigned char)~0u >> 1) : (unsigned char)~0u);
const unsigned_t TOPPERS_cfg_CHAR_MIN = (unsigned_t)((char)-1 < 0 ? -((unsigned char)~0u >> 1) - 1 : 0);
const unsigned_t TOPPERS_cfg_SCHAR_MAX = (signed char)((unsigned char)~0u >> 1);
const unsigned_t TOPPERS_cfg_SHRT_MAX = (short)((unsigned short)~0u >> 1);
const unsigned_t TOPPERS_cfg_INT_MAX = (int)(~0u >> 1);
const unsigned_t TOPPERS_cfg_LONG_MAX = (long)(~0ul >> 1);

const unsigned_t TOPPERS_cfg_SIL_ENDIAN_BIG = 
#if defined(SIL_ENDIAN_BIG)
(1);
#else
(0);
#endif
const unsigned_t TOPPERS_cfg_SIL_ENDIAN_LITTLE = 
#if defined(SIL_ENDIAN_LITTLE)
(1);
#else
(0);
#endif
const unsigned_t TOPPERS_cfg_TA_NULL = ( unsigned_t )TA_NULL;
const unsigned_t TOPPERS_cfg_TA_ACT = ( unsigned_t )TA_ACT;
const unsigned_t TOPPERS_cfg_TA_TPRI = ( unsigned_t )TA_TPRI;
const unsigned_t TOPPERS_cfg_TA_MPRI = ( unsigned_t )TA_MPRI;
const unsigned_t TOPPERS_cfg_TA_WMUL = ( unsigned_t )TA_WMUL;
const unsigned_t TOPPERS_cfg_TA_CLR = ( unsigned_t )TA_CLR;
const unsigned_t TOPPERS_cfg_TA_STA = ( unsigned_t )TA_STA;
const unsigned_t TOPPERS_cfg_TA_NONKERNEL = ( unsigned_t )TA_NONKERNEL;
const unsigned_t TOPPERS_cfg_TA_ENAINT = ( unsigned_t )TA_ENAINT;
const unsigned_t TOPPERS_cfg_TA_EDGE = ( unsigned_t )TA_EDGE;
const signed_t TOPPERS_cfg_TMIN_TPRI = ( signed_t )TMIN_TPRI;
const signed_t TOPPERS_cfg_TMAX_TPRI = ( signed_t )TMAX_TPRI;
const signed_t TOPPERS_cfg_TMIN_DPRI = ( signed_t )TMIN_DPRI;
const signed_t TOPPERS_cfg_TMAX_DPRI = ( signed_t )TMAX_DPRI;
const signed_t TOPPERS_cfg_TMIN_MPRI = ( signed_t )TMIN_MPRI;
const signed_t TOPPERS_cfg_TMAX_MPRI = ( signed_t )TMAX_MPRI;
const signed_t TOPPERS_cfg_TMIN_ISRPRI = ( signed_t )TMIN_ISRPRI;
const signed_t TOPPERS_cfg_TMAX_ISRPRI = ( signed_t )TMAX_ISRPRI;
const unsigned_t TOPPERS_cfg_TBIT_TEXPTN = ( unsigned_t )TBIT_TEXPTN;
const unsigned_t TOPPERS_cfg_TBIT_FLGPTN = ( unsigned_t )TBIT_FLGPTN;
const unsigned_t TOPPERS_cfg_TMAX_MAXSEM = ( unsigned_t )TMAX_MAXSEM;
const unsigned_t TOPPERS_cfg_TMAX_RELTIM = ( unsigned_t )TMAX_RELTIM;
const signed_t TOPPERS_cfg_TMIN_INTPRI = ( signed_t )TMIN_INTPRI;
const unsigned_t TOPPERS_cfg_OMIT_INITIALIZE_INTERRUPT = 
#if defined(OMIT_INITIALIZE_INTERRUPT)
(1);
#else
(0);
#endif
const unsigned_t TOPPERS_cfg_OMIT_INITIALIZE_EXCEPTION = 
#if defined(OMIT_INITIALIZE_EXCEPTION)
(1);
#else
(0);
#endif
const unsigned_t TOPPERS_cfg_USE_TSKINICTXB = 
#if defined(USE_TSKINICTXB)
(1);
#else
(0);
#endif
const unsigned_t TOPPERS_cfg_TARGET_TSKATR = 
#if defined(TARGET_TSKATR)
(TARGET_TSKATR);
#else
(0);
#endif
const unsigned_t TOPPERS_cfg_TARGET_INTATR = 
#if defined(TARGET_INTATR)
(TARGET_INTATR);
#else
(0);
#endif
const unsigned_t TOPPERS_cfg_TARGET_INHATR = 
#if defined(TARGET_INHATR)
(TARGET_INHATR);
#else
(0);
#endif
const unsigned_t TOPPERS_cfg_TARGET_ISRATR = 
#if defined(TARGET_ISRATR)
(TARGET_ISRATR);
#else
(0);
#endif
const unsigned_t TOPPERS_cfg_TARGET_EXCATR = 
#if defined(TARGET_EXCATR)
(TARGET_EXCATR);
#else
(0);
#endif
const unsigned_t TOPPERS_cfg_TARGET_MIN_STKSZ = 
#if defined(TARGET_MIN_STKSZ)
(TARGET_MIN_STKSZ);
#else
(0);
#endif
const unsigned_t TOPPERS_cfg_TARGET_MIN_ISTKSZ = 
#if defined(TARGET_MIN_ISTKSZ)
(TARGET_MIN_ISTKSZ);
#else
(0);
#endif
const unsigned_t TOPPERS_cfg_CHECK_STKSZ_ALIGN = 
#if defined(CHECK_STKSZ_ALIGN)
(CHECK_STKSZ_ALIGN);
#else
(0);
#endif
const unsigned_t TOPPERS_cfg_CHECK_FUNC_ALIGN = 
#if defined(CHECK_FUNC_ALIGN)
(CHECK_FUNC_ALIGN);
#else
(0);
#endif
const unsigned_t TOPPERS_cfg_CHECK_FUNC_NONNULL = 
#if defined(CHECK_FUNC_NONNULL)
(1);
#else
(0);
#endif
const unsigned_t TOPPERS_cfg_CHECK_STACK_ALIGN = 
#if defined(CHECK_STACK_ALIGN)
(CHECK_STACK_ALIGN);
#else
(0);
#endif
const unsigned_t TOPPERS_cfg_CHECK_STACK_NONNULL = 
#if defined(CHECK_STACK_NONNULL)
(1);
#else
(0);
#endif
const unsigned_t TOPPERS_cfg_CHECK_MPF_ALIGN = 
#if defined(CHECK_MPF_ALIGN)
(CHECK_MPF_ALIGN);
#else
(0);
#endif
const unsigned_t TOPPERS_cfg_CHECK_MPF_NONNULL = 
#if defined(CHECK_MPF_NONNULL)
(1);
#else
(0);
#endif
const unsigned_t TOPPERS_cfg_sizeof_ID = ( unsigned_t )sizeof(ID);
const unsigned_t TOPPERS_cfg_sizeof_uint_t = ( unsigned_t )sizeof(uint_t);
const unsigned_t TOPPERS_cfg_sizeof_SIZE = ( unsigned_t )sizeof(SIZE);
const unsigned_t TOPPERS_cfg_sizeof_ATR = ( unsigned_t )sizeof(ATR);
const unsigned_t TOPPERS_cfg_sizeof_PRI = ( unsigned_t )sizeof(PRI);
const unsigned_t TOPPERS_cfg_sizeof_void_ptr = ( unsigned_t )sizeof(void*);
const unsigned_t TOPPERS_cfg_sizeof_VP = ( unsigned_t )sizeof(void*);
const unsigned_t TOPPERS_cfg_sizeof_intptr_t = ( unsigned_t )sizeof(intptr_t);
const unsigned_t TOPPERS_cfg_sizeof_FP = ( unsigned_t )sizeof(FP);
const unsigned_t TOPPERS_cfg_sizeof_INHNO = ( unsigned_t )sizeof(INHNO);
const unsigned_t TOPPERS_cfg_sizeof_INTNO = ( unsigned_t )sizeof(INTNO);
const unsigned_t TOPPERS_cfg_sizeof_EXCNO = ( unsigned_t )sizeof(EXCNO);
const unsigned_t TOPPERS_cfg_sizeof_TINIB = ( unsigned_t )sizeof(TINIB);
const unsigned_t TOPPERS_cfg_offsetof_TINIB_tskatr = ( unsigned_t )offsetof(TINIB,tskatr);
const unsigned_t TOPPERS_cfg_offsetof_TINIB_exinf = ( unsigned_t )offsetof(TINIB,exinf);
const unsigned_t TOPPERS_cfg_offsetof_TINIB_task = ( unsigned_t )offsetof(TINIB,task);
const unsigned_t TOPPERS_cfg_offsetof_TINIB_ipriority = ( unsigned_t )offsetof(TINIB,ipriority);
const unsigned_t TOPPERS_cfg_offsetof_TINIB_stksz = 
#if !defined(USE_TSKINICTXB)
(offsetof(TINIB,stksz));
#else
(0);
#endif
const unsigned_t TOPPERS_cfg_offsetof_TINIB_stk = 
#if !defined(USE_TSKINICTXB)
(offsetof(TINIB,stk));
#else
(0);
#endif
const unsigned_t TOPPERS_cfg_offsetof_TINIB_texatr = ( unsigned_t )offsetof(TINIB,texatr);
const unsigned_t TOPPERS_cfg_offsetof_TINIB_texrtn = ( unsigned_t )offsetof(TINIB,texrtn);
const unsigned_t TOPPERS_cfg_sizeof_SEMINIB = ( unsigned_t )sizeof(SEMINIB);
const unsigned_t TOPPERS_cfg_offsetof_SEMINIB_sematr = ( unsigned_t )offsetof(SEMINIB,sematr);
const unsigned_t TOPPERS_cfg_offsetof_SEMINIB_isemcnt = ( unsigned_t )offsetof(SEMINIB,isemcnt);
const unsigned_t TOPPERS_cfg_offsetof_SEMINIB_maxsem = ( unsigned_t )offsetof(SEMINIB,maxsem);
const unsigned_t TOPPERS_cfg_sizeof_FLGPTN = ( unsigned_t )sizeof(FLGPTN);
const unsigned_t TOPPERS_cfg_sizeof_FLGINIB = ( unsigned_t )sizeof(FLGINIB);
const unsigned_t TOPPERS_cfg_offsetof_FLGINIB_flgatr = ( unsigned_t )offsetof(FLGINIB,flgatr);
const unsigned_t TOPPERS_cfg_offsetof_FLGINIB_iflgptn = ( unsigned_t )offsetof(FLGINIB,iflgptn);
const unsigned_t TOPPERS_cfg_sizeof_DTQINIB = ( unsigned_t )sizeof(DTQINIB);
const unsigned_t TOPPERS_cfg_offsetof_DTQINIB_dtqatr = ( unsigned_t )offsetof(DTQINIB,dtqatr);
const unsigned_t TOPPERS_cfg_offsetof_DTQINIB_dtqcnt = ( unsigned_t )offsetof(DTQINIB,dtqcnt);
const unsigned_t TOPPERS_cfg_offsetof_DTQINIB_p_dtqmb = ( unsigned_t )offsetof(DTQINIB,p_dtqmb);
const unsigned_t TOPPERS_cfg_sizeof_PDQINIB = ( unsigned_t )sizeof(PDQINIB);
const unsigned_t TOPPERS_cfg_offsetof_PDQINIB_pdqatr = ( unsigned_t )offsetof(PDQINIB,pdqatr);
const unsigned_t TOPPERS_cfg_offsetof_PDQINIB_pdqcnt = ( unsigned_t )offsetof(PDQINIB,pdqcnt);
const unsigned_t TOPPERS_cfg_offsetof_PDQINIB_maxdpri = ( unsigned_t )offsetof(PDQINIB,maxdpri);
const unsigned_t TOPPERS_cfg_offsetof_PDQINIB_p_pdqmb = ( unsigned_t )offsetof(PDQINIB,p_pdqmb);
const unsigned_t TOPPERS_cfg_sizeof_MBXINIB = ( unsigned_t )sizeof(MBXINIB);
const unsigned_t TOPPERS_cfg_offsetof_MBXINIB_mbxatr = ( unsigned_t )offsetof(MBXINIB,mbxatr);
const unsigned_t TOPPERS_cfg_offsetof_MBXINIB_maxmpri = ( unsigned_t )offsetof(MBXINIB,maxmpri);
const unsigned_t TOPPERS_cfg_sizeof_MPFINIB = ( unsigned_t )sizeof(MPFINIB);
const unsigned_t TOPPERS_cfg_offsetof_MPFINIB_mpfatr = ( unsigned_t )offsetof(MPFINIB,mpfatr);
const unsigned_t TOPPERS_cfg_offsetof_MPFINIB_blkcnt = ( unsigned_t )offsetof(MPFINIB,blkcnt);
const unsigned_t TOPPERS_cfg_offsetof_MPFINIB_blksz = ( unsigned_t )offsetof(MPFINIB,blksz);
const unsigned_t TOPPERS_cfg_offsetof_MPFINIB_mpf = ( unsigned_t )offsetof(MPFINIB,mpf);
const unsigned_t TOPPERS_cfg_offsetof_MPFINIB_p_mpfmb = ( unsigned_t )offsetof(MPFINIB,p_mpfmb);
const unsigned_t TOPPERS_cfg_sizeof_CYCINIB = ( unsigned_t )sizeof(CYCINIB);
const unsigned_t TOPPERS_cfg_offsetof_CYCINIB_cycatr = ( unsigned_t )offsetof(CYCINIB,cycatr);
const unsigned_t TOPPERS_cfg_offsetof_CYCINIB_exinf = ( unsigned_t )offsetof(CYCINIB,exinf);
const unsigned_t TOPPERS_cfg_offsetof_CYCINIB_cychdr = ( unsigned_t )offsetof(CYCINIB,cychdr);
const unsigned_t TOPPERS_cfg_offsetof_CYCINIB_cyctim = ( unsigned_t )offsetof(CYCINIB,cyctim);
const unsigned_t TOPPERS_cfg_offsetof_CYCINIB_cycphs = ( unsigned_t )offsetof(CYCINIB,cycphs);
const unsigned_t TOPPERS_cfg_sizeof_ALMINIB = ( unsigned_t )sizeof(ALMINIB);
const unsigned_t TOPPERS_cfg_offsetof_ALMINIB_almatr = ( unsigned_t )offsetof(ALMINIB,almatr);
const unsigned_t TOPPERS_cfg_offsetof_ALMINIB_exinf = ( unsigned_t )offsetof(ALMINIB,exinf);
const unsigned_t TOPPERS_cfg_offsetof_ALMINIB_almhdr = ( unsigned_t )offsetof(ALMINIB,almhdr);
const unsigned_t TOPPERS_cfg_sizeof_INHINIB = 
#if !defined(OMIT_INITIALIZE_INTERRUPT)
(sizeof(INHINIB));
#else
(0);
#endif
const unsigned_t TOPPERS_cfg_offset_INHINIB_inhno = 
#if !defined(OMIT_INITIALIZE_INTERRUPT)
(offsetof(INHINIB,inhno));
#else
(0);
#endif
const unsigned_t TOPPERS_cfg_offset_INHINIB_inhatr = 
#if !defined(OMIT_INITIALIZE_INTERRUPT)
(offsetof(INHINIB,inhatr));
#else
(0);
#endif
const unsigned_t TOPPERS_cfg_offset_INHINIB_int_entry = 
#if !defined(OMIT_INITIALIZE_INTERRUPT)
(offsetof(INHINIB,int_entry));
#else
(0);
#endif
const unsigned_t TOPPERS_cfg_sizeof_INTINIB = 
#if !defined(OMIT_INITIALIZE_INTERRUPT)
(sizeof(INTINIB));
#else
(0);
#endif
const unsigned_t TOPPERS_cfg_offset_INTINIB_intno = 
#if !defined(OMIT_INITIALIZE_INTERRUPT)
(offsetof(INTINIB,intno));
#else
(0);
#endif
const unsigned_t TOPPERS_cfg_offset_INTINIB_intatr = 
#if !defined(OMIT_INITIALIZE_INTERRUPT)
(offsetof(INTINIB,intatr));
#else
(0);
#endif
const unsigned_t TOPPERS_cfg_offset_INTINIB_intpri = 
#if !defined(OMIT_INITIALIZE_INTERRUPT)
(offsetof(INTINIB,intpri));
#else
(0);
#endif
const unsigned_t TOPPERS_cfg_sizeof_EXCINIB = 
#if !defined(OMIT_INITIALIZE_EXCEPTION)
(sizeof(EXCINIB));
#else
(0);
#endif
const unsigned_t TOPPERS_cfg_offset_EXCINIB_excno = 
#if !defined(OMIT_INITIALIZE_EXCEPTION)
(offsetof(EXCINIB,excno));
#else
(0);
#endif
const unsigned_t TOPPERS_cfg_offset_EXCINIB_excatr = 
#if !defined(OMIT_INITIALIZE_EXCEPTION)
(offsetof(EXCINIB,excatr));
#else
(0);
#endif
const unsigned_t TOPPERS_cfg_offset_EXCINIB_exc_entry = 
#if !defined(OMIT_INITIALIZE_EXCEPTION)
(offsetof(EXCINIB,exc_entry));
#else
(0);
#endif
const unsigned_t TOPPERS_cfg_TMAX_INTNO = ( unsigned_t )TMAX_INTNO;
const unsigned_t TOPPERS_cfg_TBITW_IPRI = ( unsigned_t )TBITW_IPRI;
const unsigned_t TOPPERS_cfg___TARGET_ARCH_THUMB = ( unsigned_t )__TARGET_ARCH_THUMB;
const unsigned_t TOPPERS_cfg_sizeof_TCB = ( unsigned_t )sizeof(TCB);
const unsigned_t TOPPERS_cfg_offsetof_TCB_p_tinib = ( unsigned_t )offsetof(TCB,p_tinib);
const unsigned_t TOPPERS_cfg_offsetof_TCB_texptn = ( unsigned_t )offsetof(TCB,texptn);
const unsigned_t TOPPERS_cfg_offsetof_TCB_sp = ( unsigned_t )offsetof(TCB,tskctxb.sp);
const unsigned_t TOPPERS_cfg_offsetof_TCB_pc = ( unsigned_t )offsetof(TCB,tskctxb.pc);


/* #include "rca_lib.h" */

#line 3 "../../../lib/rca_lib.cfg"
const unsigned_t TOPPERS_cfg_static_api_0 = 0;
const unsigned_t TOPPERS_cfg_valueof_iniatr_0 = ( unsigned_t )( TA_NULL ); 
#line 4 "../../../lib/rca_lib.cfg"
const unsigned_t TOPPERS_cfg_static_api_1 = 1;
#define RCA_MAINTASK	(<>)

#line 4 "../../../lib/rca_lib.cfg"
const unsigned_t TOPPERS_cfg_valueof_tskatr_1 = ( unsigned_t )( TA_ACT ); const signed_t TOPPERS_cfg_valueof_itskpri_1 = ( signed_t )( RCA_MAINTASK_SETUP_PRI ); const unsigned_t TOPPERS_cfg_valueof_stksz_1 = ( unsigned_t )( RCA_MAINTASK_STACK_SIZE ); 
#line 7 "../../../lib/rca_lib.cfg"
const unsigned_t TOPPERS_cfg_static_api_2 = 2;
#define SETUP_SEM	(<>)

#line 7 "../../../lib/rca_lib.cfg"
const unsigned_t TOPPERS_cfg_valueof_sematr_2 = ( unsigned_t )( TA_TPRI ); const unsigned_t TOPPERS_cfg_valueof_isemcnt_2 = ( unsigned_t )( 0 ); const unsigned_t TOPPERS_cfg_valueof_maxsem_2 = ( unsigned_t )( 1 ); 
#if RCA_NUM_TASK > 0

#line 10 "../../../lib/rca_lib.cfg"
const unsigned_t TOPPERS_cfg_static_api_3 = 3;
#define RCA_TASK1	(<>)

#line 10 "../../../lib/rca_lib.cfg"
const unsigned_t TOPPERS_cfg_valueof_tskatr_3 = ( unsigned_t )( TA_ACT ); const signed_t TOPPERS_cfg_valueof_itskpri_3 = ( signed_t )( RCA_TASK1_SETUP_PRI ); const unsigned_t TOPPERS_cfg_valueof_stksz_3 = ( unsigned_t )( RCA_TASK1_STACK_SIZE ); 
#endif 

#if RCA_NUM_TASK > 1

#line 14 "../../../lib/rca_lib.cfg"
const unsigned_t TOPPERS_cfg_static_api_4 = 4;
#define RCA_TASK2	(<>)

#line 14 "../../../lib/rca_lib.cfg"
const unsigned_t TOPPERS_cfg_valueof_tskatr_4 = ( unsigned_t )( TA_ACT ); const signed_t TOPPERS_cfg_valueof_itskpri_4 = ( signed_t )( RCA_TASK2_SETUP_PRI ); const unsigned_t TOPPERS_cfg_valueof_stksz_4 = ( unsigned_t )( RCA_TASK2_STACK_SIZE ); 
#endif 

#if RCA_NUM_TASK > 2

#line 18 "../../../lib/rca_lib.cfg"
const unsigned_t TOPPERS_cfg_static_api_5 = 5;
#define RCA_TASK3	(<>)

#line 18 "../../../lib/rca_lib.cfg"
const unsigned_t TOPPERS_cfg_valueof_tskatr_5 = ( unsigned_t )( TA_ACT ); const signed_t TOPPERS_cfg_valueof_itskpri_5 = ( signed_t )( RCA_TASK3_SETUP_PRI ); const unsigned_t TOPPERS_cfg_valueof_stksz_5 = ( unsigned_t )( RCA_TASK3_STACK_SIZE ); 
#endif 

#if RCA_NUM_TASK > 3

#line 22 "../../../lib/rca_lib.cfg"
const unsigned_t TOPPERS_cfg_static_api_6 = 6;
#define RCA_TASK4	(<>)

#line 22 "../../../lib/rca_lib.cfg"
const unsigned_t TOPPERS_cfg_valueof_tskatr_6 = ( unsigned_t )( TA_ACT ); const signed_t TOPPERS_cfg_valueof_itskpri_6 = ( signed_t )( RCA_TASK4_SETUP_PRI ); const unsigned_t TOPPERS_cfg_valueof_stksz_6 = ( unsigned_t )( RCA_TASK4_STACK_SIZE ); 
#endif 

#if RCA_NUM_TASK > 4

#line 26 "../../../lib/rca_lib.cfg"
const unsigned_t TOPPERS_cfg_static_api_7 = 7;
#define RCA_TASK5	(<>)

#line 26 "../../../lib/rca_lib.cfg"
const unsigned_t TOPPERS_cfg_valueof_tskatr_7 = ( unsigned_t )( TA_ACT ); const signed_t TOPPERS_cfg_valueof_itskpri_7 = ( signed_t )( RCA_TASK5_SETUP_PRI ); const unsigned_t TOPPERS_cfg_valueof_stksz_7 = ( unsigned_t )( RCA_TASK5_STACK_SIZE ); 
#endif 

#ifdef RCA_ENABLE_PROFILING

#line 30 "../../../lib/rca_lib.cfg"
const unsigned_t TOPPERS_cfg_static_api_8 = 8;
#define RCA_IDLE_TASK	(<>)

#line 30 "../../../lib/rca_lib.cfg"
const unsigned_t TOPPERS_cfg_valueof_tskatr_8 = ( unsigned_t )( TA_ACT ); const signed_t TOPPERS_cfg_valueof_itskpri_8 = ( signed_t )( 15 ); const unsigned_t TOPPERS_cfg_valueof_stksz_8 = ( unsigned_t )( 512 ); 
#endif 

#line 34 "../../../lib/rca_lib.cfg"
const unsigned_t TOPPERS_cfg_static_api_9 = 9;
#define RCA_CYCHDR	(<>)

#line 34 "../../../lib/rca_lib.cfg"
const unsigned_t TOPPERS_cfg_valueof_cycatr_9 = ( unsigned_t )( TA_STA ); const unsigned_t TOPPERS_cfg_valueof_cyctim_9 = ( unsigned_t )( 1 ); const unsigned_t TOPPERS_cfg_valueof_cycphs_9 = ( unsigned_t )( 1 ); 
#line 37 "../../../lib/rca_lib.cfg"
const unsigned_t TOPPERS_cfg_static_api_10 = 10;
const unsigned_t TOPPERS_cfg_valueof_inhno_10 = ( unsigned_t )( (16+7) ); const unsigned_t TOPPERS_cfg_valueof_inhatr_10 = ( unsigned_t )( TA_NULL ); 
#line 38 "../../../lib/rca_lib.cfg"
const unsigned_t TOPPERS_cfg_static_api_11 = 11;
const unsigned_t TOPPERS_cfg_valueof_intno_11 = ( unsigned_t )( (16+7) ); const unsigned_t TOPPERS_cfg_valueof_intatr_11 = ( unsigned_t )( TA_NULL ); const signed_t TOPPERS_cfg_valueof_intpri_11 = ( signed_t )( -1 ); 
#ifdef TOPPERS_USE_ARDUINO_SERIAL

#line 42 "../../../lib/rca_lib.cfg"
const unsigned_t TOPPERS_cfg_static_api_12 = 12;
const unsigned_t TOPPERS_cfg_valueof_inhno_12 = ( unsigned_t )( (16+14) ); const unsigned_t TOPPERS_cfg_valueof_inhatr_12 = ( unsigned_t )( TA_NULL ); 
#line 43 "../../../lib/rca_lib.cfg"
const unsigned_t TOPPERS_cfg_static_api_13 = 13;
const unsigned_t TOPPERS_cfg_valueof_intno_13 = ( unsigned_t )( (16+14) ); const unsigned_t TOPPERS_cfg_valueof_intatr_13 = ( unsigned_t )( TA_NULL ); const signed_t TOPPERS_cfg_valueof_intpri_13 = ( signed_t )( -1 ); 
#endif 

#line 47 "../../../lib/rca_lib.cfg"
const unsigned_t TOPPERS_cfg_static_api_14 = 14;
const unsigned_t TOPPERS_cfg_valueof_inhno_14 = ( unsigned_t )( (16+9) ); const unsigned_t TOPPERS_cfg_valueof_inhatr_14 = ( unsigned_t )( TA_NULL ); 
#line 48 "../../../lib/rca_lib.cfg"
const unsigned_t TOPPERS_cfg_static_api_15 = 15;
const unsigned_t TOPPERS_cfg_valueof_intno_15 = ( unsigned_t )( (16+9) ); const unsigned_t TOPPERS_cfg_valueof_intatr_15 = ( unsigned_t )( TA_NULL ); const signed_t TOPPERS_cfg_valueof_intpri_15 = ( signed_t )( -1 ); 
#ifdef RCA_USE_SERIAL3

#line 52 "../../../lib/rca_lib.cfg"
const unsigned_t TOPPERS_cfg_static_api_16 = 16;
const unsigned_t TOPPERS_cfg_valueof_inhno_16 = ( unsigned_t )( (16+11) ); const unsigned_t TOPPERS_cfg_valueof_inhatr_16 = ( unsigned_t )( TA_NULL ); 
#line 53 "../../../lib/rca_lib.cfg"
const unsigned_t TOPPERS_cfg_static_api_17 = 17;
const unsigned_t TOPPERS_cfg_valueof_intno_17 = ( unsigned_t )( (16+11) ); const unsigned_t TOPPERS_cfg_valueof_intatr_17 = ( unsigned_t )( TA_NULL ); const signed_t TOPPERS_cfg_valueof_intpri_17 = ( signed_t )( -1 ); 
#endif 

#line 57 "../../../lib/rca_lib.cfg"
const unsigned_t TOPPERS_cfg_static_api_18 = 18;
const unsigned_t TOPPERS_cfg_valueof_inhno_18 = ( unsigned_t )( (16+12) ); const unsigned_t TOPPERS_cfg_valueof_inhatr_18 = ( unsigned_t )( TA_NULL ); 
#line 58 "../../../lib/rca_lib.cfg"
const unsigned_t TOPPERS_cfg_static_api_19 = 19;
const unsigned_t TOPPERS_cfg_valueof_intno_19 = ( unsigned_t )( (16+12) ); const unsigned_t TOPPERS_cfg_valueof_intatr_19 = ( unsigned_t )( TA_NULL ); const signed_t TOPPERS_cfg_valueof_intpri_19 = ( signed_t )( -1 ); 
#line 61 "../../../lib/rca_lib.cfg"
const unsigned_t TOPPERS_cfg_static_api_20 = 20;
const unsigned_t TOPPERS_cfg_valueof_inhno_20 = ( unsigned_t )( (16+13) ); const unsigned_t TOPPERS_cfg_valueof_inhatr_20 = ( unsigned_t )( TA_NULL ); 
#line 62 "../../../lib/rca_lib.cfg"
const unsigned_t TOPPERS_cfg_static_api_21 = 21;
const unsigned_t TOPPERS_cfg_valueof_intno_21 = ( unsigned_t )( (16+13) ); const unsigned_t TOPPERS_cfg_valueof_intatr_21 = ( unsigned_t )( TA_NULL ); const signed_t TOPPERS_cfg_valueof_intpri_21 = ( signed_t )( -1 ); 
#line 65 "../../../lib/rca_lib.cfg"
const unsigned_t TOPPERS_cfg_static_api_22 = 22;
const unsigned_t TOPPERS_cfg_valueof_inhno_22 = ( unsigned_t )( (16+4) ); const unsigned_t TOPPERS_cfg_valueof_inhatr_22 = ( unsigned_t )( TA_NULL ); 
#line 66 "../../../lib/rca_lib.cfg"
const unsigned_t TOPPERS_cfg_static_api_23 = 23;
const unsigned_t TOPPERS_cfg_valueof_intno_23 = ( unsigned_t )( (16+4) ); const unsigned_t TOPPERS_cfg_valueof_intatr_23 = ( unsigned_t )( TA_NULL ); const signed_t TOPPERS_cfg_valueof_intpri_23 = ( signed_t )( -1 ); 
#line 69 "../../../lib/rca_lib.cfg"
const unsigned_t TOPPERS_cfg_static_api_24 = 24;
const unsigned_t TOPPERS_cfg_valueof_inhno_24 = ( unsigned_t )( (16+20) ); const unsigned_t TOPPERS_cfg_valueof_inhatr_24 = ( unsigned_t )( TA_NULL ); 
#line 70 "../../../lib/rca_lib.cfg"
const unsigned_t TOPPERS_cfg_static_api_25 = 25;
const unsigned_t TOPPERS_cfg_valueof_intno_25 = ( unsigned_t )( (16+20) ); const unsigned_t TOPPERS_cfg_valueof_intatr_25 = ( unsigned_t )( TA_NULL ); const signed_t TOPPERS_cfg_valueof_intpri_25 = ( signed_t )( -1 ); 
#line 73 "../../../lib/rca_lib.cfg"
const unsigned_t TOPPERS_cfg_static_api_26 = 26;
const unsigned_t TOPPERS_cfg_valueof_inhno_26 = ( unsigned_t )( (16+3) ); const unsigned_t TOPPERS_cfg_valueof_inhatr_26 = ( unsigned_t )( TA_NULL ); 
#line 74 "../../../lib/rca_lib.cfg"
const unsigned_t TOPPERS_cfg_static_api_27 = 27;
const unsigned_t TOPPERS_cfg_valueof_intno_27 = ( unsigned_t )( (16+3) ); const unsigned_t TOPPERS_cfg_valueof_intatr_27 = ( unsigned_t )( TA_NULL ); const signed_t TOPPERS_cfg_valueof_intpri_27 = ( signed_t )( -1 ); 
#line 77 "../../../lib/rca_lib.cfg"
const unsigned_t TOPPERS_cfg_static_api_28 = 28;
#define SPI_SEM	(<>)

#line 77 "../../../lib/rca_lib.cfg"
const unsigned_t TOPPERS_cfg_valueof_sematr_28 = ( unsigned_t )( TA_TPRI ); const unsigned_t TOPPERS_cfg_valueof_isemcnt_28 = ( unsigned_t )( 1 ); const unsigned_t TOPPERS_cfg_valueof_maxsem_28 = ( unsigned_t )( 1 ); 
#line 80 "../../../lib/rca_lib.cfg"
const unsigned_t TOPPERS_cfg_static_api_29 = 29;
#define ESP8266_SEM	(<>)

#line 80 "../../../lib/rca_lib.cfg"
const unsigned_t TOPPERS_cfg_valueof_sematr_29 = ( unsigned_t )( TA_TPRI ); const unsigned_t TOPPERS_cfg_valueof_isemcnt_29 = ( unsigned_t )( 1 ); const unsigned_t TOPPERS_cfg_valueof_maxsem_29 = ( unsigned_t )( 1 ); 
#line 83 "../../../lib/rca_lib.cfg"
const unsigned_t TOPPERS_cfg_static_api_30 = 30;
#define WIRE_SEM	(<>)

#line 83 "../../../lib/rca_lib.cfg"
const unsigned_t TOPPERS_cfg_valueof_sematr_30 = ( unsigned_t )( TA_TPRI ); const unsigned_t TOPPERS_cfg_valueof_isemcnt_30 = ( unsigned_t )( 1 ); const unsigned_t TOPPERS_cfg_valueof_maxsem_30 = ( unsigned_t )( 1 ); 
#line 86 "../../../lib/rca_lib.cfg"
const unsigned_t TOPPERS_cfg_static_api_31 = 31;
#define MILKCOCOA_SEM	(<>)

#line 86 "../../../lib/rca_lib.cfg"
const unsigned_t TOPPERS_cfg_valueof_sematr_31 = ( unsigned_t )( TA_TPRI ); const unsigned_t TOPPERS_cfg_valueof_isemcnt_31 = ( unsigned_t )( 1 ); const unsigned_t TOPPERS_cfg_valueof_maxsem_31 = ( unsigned_t )( 1 ); /* #include "target_timer.h" */

#line 8 "../../../asp_1.9.2/arch/arm_m_gcc/common/core_timer.cfg"
const unsigned_t TOPPERS_cfg_static_api_32 = 32;
const unsigned_t TOPPERS_cfg_valueof_iniatr_32 = ( unsigned_t )( TA_NULL ); 
#line 9 "../../../asp_1.9.2/arch/arm_m_gcc/common/core_timer.cfg"
const unsigned_t TOPPERS_cfg_static_api_33 = 33;
const unsigned_t TOPPERS_cfg_valueof_teratr_33 = ( unsigned_t )( TA_NULL ); 
#line 10 "../../../asp_1.9.2/arch/arm_m_gcc/common/core_timer.cfg"
const unsigned_t TOPPERS_cfg_static_api_34 = 34;
const unsigned_t TOPPERS_cfg_valueof_inhno_34 = ( unsigned_t )( INHNO_TIMER ); const unsigned_t TOPPERS_cfg_valueof_inhatr_34 = ( unsigned_t )( TA_NULL ); 
#line 11 "../../../asp_1.9.2/arch/arm_m_gcc/common/core_timer.cfg"
const unsigned_t TOPPERS_cfg_static_api_35 = 35;
const unsigned_t TOPPERS_cfg_valueof_intno_35 = ( unsigned_t )( INTNO_TIMER ); const unsigned_t TOPPERS_cfg_valueof_intatr_35 = ( unsigned_t )( TA_ENAINT|INTATR_TIMER ); const signed_t TOPPERS_cfg_valueof_intpri_35 = ( signed_t )( INTPRI_TIMER ); 
#ifdef TOPPERS_USE_LOGTASK
/* #include "target_syssvc.h" */
/* #include "target_serial.h" */

#line 10 "../../../asp_1.9.2/target/arduino_m0_gcc/target_serial.cfg"
const unsigned_t TOPPERS_cfg_static_api_36 = 36;
const unsigned_t TOPPERS_cfg_valueof_iniatr_36 = ( unsigned_t )( TA_NULL ); 
#line 11 "../../../asp_1.9.2/target/arduino_m0_gcc/target_serial.cfg"
const unsigned_t TOPPERS_cfg_static_api_37 = 37;
const unsigned_t TOPPERS_cfg_valueof_isratr_37 = ( unsigned_t )( TA_NULL ); const unsigned_t TOPPERS_cfg_valueof_intno_37 = ( unsigned_t )( INTNO_SIO ); const signed_t TOPPERS_cfg_valueof_isrpri_37 = ( signed_t )( 1 ); 
#line 12 "../../../asp_1.9.2/target/arduino_m0_gcc/target_serial.cfg"
const unsigned_t TOPPERS_cfg_static_api_38 = 38;
const unsigned_t TOPPERS_cfg_valueof_intno_38 = ( unsigned_t )( INTNO_SIO ); const unsigned_t TOPPERS_cfg_valueof_intatr_38 = ( unsigned_t )( TA_ENAINT|INTATR_SIO ); const signed_t TOPPERS_cfg_valueof_intpri_38 = ( signed_t )( INTPRI_SIO ); /* #include "syssvc/serial.h" */

#line 13 "../../../asp_1.9.2/syssvc/serial.cfg"
const unsigned_t TOPPERS_cfg_static_api_39 = 39;
const unsigned_t TOPPERS_cfg_valueof_iniatr_39 = ( unsigned_t )( TA_NULL ); 
#line 15 "../../../asp_1.9.2/syssvc/serial.cfg"
const unsigned_t TOPPERS_cfg_static_api_40 = 40;
#define SERIAL_RCV_SEM1	(<>)

#line 15 "../../../asp_1.9.2/syssvc/serial.cfg"
const unsigned_t TOPPERS_cfg_valueof_sematr_40 = ( unsigned_t )( TA_TPRI ); const unsigned_t TOPPERS_cfg_valueof_isemcnt_40 = ( unsigned_t )( 0 ); const unsigned_t TOPPERS_cfg_valueof_maxsem_40 = ( unsigned_t )( 1 ); 
#line 16 "../../../asp_1.9.2/syssvc/serial.cfg"
const unsigned_t TOPPERS_cfg_static_api_41 = 41;
#define SERIAL_SND_SEM1	(<>)

#line 16 "../../../asp_1.9.2/syssvc/serial.cfg"
const unsigned_t TOPPERS_cfg_valueof_sematr_41 = ( unsigned_t )( TA_TPRI ); const unsigned_t TOPPERS_cfg_valueof_isemcnt_41 = ( unsigned_t )( 1 ); const unsigned_t TOPPERS_cfg_valueof_maxsem_41 = ( unsigned_t )( 1 ); 
#if TNUM_PORT >= 2

#line 18 "../../../asp_1.9.2/syssvc/serial.cfg"
const unsigned_t TOPPERS_cfg_static_api_42 = 42;
#define SERIAL_RCV_SEM2	(<>)

#line 18 "../../../asp_1.9.2/syssvc/serial.cfg"
const unsigned_t TOPPERS_cfg_valueof_sematr_42 = ( unsigned_t )( TA_TPRI ); const unsigned_t TOPPERS_cfg_valueof_isemcnt_42 = ( unsigned_t )( 0 ); const unsigned_t TOPPERS_cfg_valueof_maxsem_42 = ( unsigned_t )( 1 ); 
#line 19 "../../../asp_1.9.2/syssvc/serial.cfg"
const unsigned_t TOPPERS_cfg_static_api_43 = 43;
#define SERIAL_SND_SEM2	(<>)

#line 19 "../../../asp_1.9.2/syssvc/serial.cfg"
const unsigned_t TOPPERS_cfg_valueof_sematr_43 = ( unsigned_t )( TA_TPRI ); const unsigned_t TOPPERS_cfg_valueof_isemcnt_43 = ( unsigned_t )( 1 ); const unsigned_t TOPPERS_cfg_valueof_maxsem_43 = ( unsigned_t )( 1 ); 
#endif 

#if TNUM_PORT >= 3

#line 22 "../../../asp_1.9.2/syssvc/serial.cfg"
const unsigned_t TOPPERS_cfg_static_api_44 = 44;
#define SERIAL_RCV_SEM3	(<>)

#line 22 "../../../asp_1.9.2/syssvc/serial.cfg"
const unsigned_t TOPPERS_cfg_valueof_sematr_44 = ( unsigned_t )( TA_TPRI ); const unsigned_t TOPPERS_cfg_valueof_isemcnt_44 = ( unsigned_t )( 0 ); const unsigned_t TOPPERS_cfg_valueof_maxsem_44 = ( unsigned_t )( 1 ); 
#line 23 "../../../asp_1.9.2/syssvc/serial.cfg"
const unsigned_t TOPPERS_cfg_static_api_45 = 45;
#define SERIAL_SND_SEM3	(<>)

#line 23 "../../../asp_1.9.2/syssvc/serial.cfg"
const unsigned_t TOPPERS_cfg_valueof_sematr_45 = ( unsigned_t )( TA_TPRI ); const unsigned_t TOPPERS_cfg_valueof_isemcnt_45 = ( unsigned_t )( 1 ); const unsigned_t TOPPERS_cfg_valueof_maxsem_45 = ( unsigned_t )( 1 ); 
#endif 

#if TNUM_PORT >= 4

#line 26 "../../../asp_1.9.2/syssvc/serial.cfg"
const unsigned_t TOPPERS_cfg_static_api_46 = 46;
#define SERIAL_RCV_SEM4	(<>)

#line 26 "../../../asp_1.9.2/syssvc/serial.cfg"
const unsigned_t TOPPERS_cfg_valueof_sematr_46 = ( unsigned_t )( TA_TPRI ); const unsigned_t TOPPERS_cfg_valueof_isemcnt_46 = ( unsigned_t )( 0 ); const unsigned_t TOPPERS_cfg_valueof_maxsem_46 = ( unsigned_t )( 1 ); 
#line 27 "../../../asp_1.9.2/syssvc/serial.cfg"
const unsigned_t TOPPERS_cfg_static_api_47 = 47;
#define SERIAL_SND_SEM4	(<>)

#line 27 "../../../asp_1.9.2/syssvc/serial.cfg"
const unsigned_t TOPPERS_cfg_valueof_sematr_47 = ( unsigned_t )( TA_TPRI ); const unsigned_t TOPPERS_cfg_valueof_isemcnt_47 = ( unsigned_t )( 1 ); const unsigned_t TOPPERS_cfg_valueof_maxsem_47 = ( unsigned_t )( 1 ); 
#endif 
/* #include "syssvc/logtask.h" */

#line 10 "../../../asp_1.9.2/syssvc/logtask.cfg"
const unsigned_t TOPPERS_cfg_static_api_48 = 48;
#define LOGTASK	(<>)

#line 10 "../../../asp_1.9.2/syssvc/logtask.cfg"
const unsigned_t TOPPERS_cfg_valueof_tskatr_48 = ( unsigned_t )( TA_ACT ); const signed_t TOPPERS_cfg_valueof_itskpri_48 = ( signed_t )( LOGTASK_PRIORITY ); const unsigned_t TOPPERS_cfg_valueof_stksz_48 = ( unsigned_t )( LOGTASK_STACK_SIZE ); 
#line 12 "../../../asp_1.9.2/syssvc/logtask.cfg"
const unsigned_t TOPPERS_cfg_static_api_49 = 49;
const unsigned_t TOPPERS_cfg_valueof_teratr_49 = ( unsigned_t )( TA_NULL ); 
#endif 
/* #include "syssvc/syslog.h" */

#line 10 "../../../asp_1.9.2/syssvc/syslog.cfg"
const unsigned_t TOPPERS_cfg_static_api_50 = 50;
const unsigned_t TOPPERS_cfg_valueof_iniatr_50 = ( unsigned_t )( TA_NULL ); /* #include "syssvc/banner.h" */

#line 10 "../../../asp_1.9.2/syssvc/banner.cfg"
const unsigned_t TOPPERS_cfg_static_api_51 = 51;
const unsigned_t TOPPERS_cfg_valueof_iniatr_51 = ( unsigned_t )( TA_NULL ); 
