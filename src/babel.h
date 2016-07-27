// babel.h
//
// Every .c file in the Babel project should include this file

#ifndef BABEL_H
#define BABEL_H

#define MEM_DEBUG
#define DEV_MODE
//#define PROF_MODE
//#define CHK_MODE

//#define LINUX_TARGET
#define WINDOWS_TARGET

/*****************************************************************************
 *                                                                           *
 *                              INCLUDES                                     *
 *                                                                           *
 ****************************************************************************/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stddef.h>
#include <assert.h>
#include <setjmp.h>
#include <stdarg.h>
#include <time.h>

#ifdef WINDOWS_TARGET
#define WINVER 0x0500
#include <windows.h>
#endif

/*****************************************************************************
 *                                                                           *
 *                             CONSTANTS                                     *
 *                                                                           *
 ****************************************************************************/

#define BITS_PER_BYTE 8 // BITS_PER_BYTE#

#define MWORD_SIZE sizeof(mword)                        // MWORD_SIZE#
#define MWORD_BIT_SIZE (MWORD_SIZE * BITS_PER_BYTE)     // MWORD_BIT_SIZE#
#define BYTE_SIZE(x) ((x)*MWORD_SIZE)                   // BYTE_SIZE#
#define MWORDS(x) ((x)/MWORD_SIZE)                      // MWORDS#

#define FLOAT_SIZE       (sizeof(float)       / MWORD_SIZE)
#define LONG_SIZE        (sizeof(long)        / MWORD_SIZE)
#define DOUBLE_SIZE      (sizeof(double)      / MWORD_SIZE)
#define LONG_DOUBLE_SIZE (sizeof(long double) / MWORD_SIZE)
#define LONG_LONG_SIZE   (sizeof(long long)   / MWORD_SIZE)

#define FP_ARITH_SIZE    DOUBLE_SIZE

#define APPROX_DIGITS_PER_BYTE 3

#define EXTENDED_ASCII_ALPHA_SIZE (1<<8)

#define MWORD_MAX_VAL ((mword)-1)

// XXX This should be set to 3 for 64-bit compilation...
// I can't find a simple/direct way to connect this to MWORD_SIZE
// so it has to be defined independently
#define LOG_MWORD_SIZE 2 // LOG_MWORD_SIZE#

// The control field consists of the LSBs of the s-field
// The LSB of the s-field is currently defined for use in 
// bstruct-traversal

#define CTL_MASK (MWORD_SIZE-1) // CTL_MASK#
#define STRLEN(s) (sizeof(s)-1) // STRLEN#
#define C2B(str) (string_c2b(this_bvm, str, STRLEN(str))) // C2B#
#define HASH8(bvm,str) (_hash8(bvm,string_c2b(this_bvm, str, STRLEN(str)))) // HASH8#
#define OVERRUN_LIMIT (1<<16) // OVERRUN_LIMIT#

#define HASH_BIT_SIZE 128 // HASH_BIT_SIZE#
#define HASH_BYTE_SIZE (HASH_BIT_SIZE/BITS_PER_BYTE) // HASH_BYTE_SIZE#
#define HASH_SIZE (HASH_BIT_SIZE/MWORD_BIT_SIZE) // HASH_SIZE#

#define TAG_SIZE (HASH_SIZE*MWORD_SIZE) // TAG_SIZE#
#define MIN_TAG_SIZE TAG_SIZE // MIN_TAG_SIZE#

#define TPTR_SIZE HASH_SIZE+3 // TPTR_SIZE#
                         // 3 = s-field + s-field + car
#define TPTR_PTR_OFFSET  (HASH_SIZE+1) // TPTR_PTR_OFFSET#

#define TPTR_SFIELD 0 // TPTR_SFIELD#
#define LEAF_SFIELD 1 // LEAF_SFIELD#
#define INTE_SFIELD (-1) // INTE_SFIELD#

#define FLAG_IGN    ((mword)-1) // FLAG_IGN#
#define FLAG_SET    1 // FLAG_SET#
#define FLAG_CLR    0 // FLAG_CLR#
#define TOGGLE_FLAG(x) (((x) == FLAG_SET) ? (x = FLAG_CLR) : (x = FLAG_SET)) // XXX: DO NOT USE ON FLAG_IGN

// This no longer controls memory-size ... use 
// MEM_DEFAULT_MIN_ALLOC instead
#define MEM_SIZE (1<<25)          // MEM_SIZE#

#define ACCESS_SIZE_MWORD 0 // ACCESS_SIZE_MWORD#
#define ACCESS_SIZE_BYTE  1 // ACCESS_SIZE_BYTE#

#define ARRAY_CMP_LEX     0 // ARRAY_CMP_LEX#
#define ARRAY_CMP_ALPHA   1 // ARRAY_CMP_ALPHA#

// BVM
#define BVM_ADVANCE  0 // BVM_ADVANCE#
#define BVM_CONTINUE 1 // BVM_CONTINUE#
#define BVM_RETURN   2 // BVM_RETURN#
#define BVM_NEXT     3 // BVM_NEXT#

#define CAT_EXCEPT          1 // CAT_EXCEPT#
#define INTERP_RESET        2 // INTERP_RESET#
#define OP_RESTART          4 // OP_RESTART#
#define DIE_CODE            1 // DIE_CODE#

/*****************************************************************************
 *                                                                           *
 *                             TYPEDEFS                                      *
 *                                                                           *
 ****************************************************************************/

typedef void(*std_fn_ptr)(void); // std_fn_ptr#

typedef unsigned mword;  // mword#
typedef signed   smword; // smword#


typedef struct { // alloc_bank#

    mword *base_ptr;
    mword *alloc_ptr;
    mword size;

} alloc_bank; 


typedef struct { // mem_context#

    alloc_bank *primary;
    alloc_bank *secondary;

    mword  mem_sys_alloc_count;
    mword  mem_sys_alloc_total;

    mword  GC_count;
    mword  op_restart_alloc_size;
    mword *op_restart_alloc_ptr;
    mword  last_GC_tick_count;

} mem_context;


typedef struct { // interp_limits#

    mword max_num_sys_threads;      // Checked when forking
    mword max_num_steps;            // Checked against global_tick_count on operator boundary
    mword max_bytes_mem_alloc;      // Checked during increment_alloc()
    mword max_num_file_descriptors; // Checked whenever opening I/O
    mword max_num_file_access;      // Checked whenever calling I/O function
    mword max_run_time_ms;          // Checked against epoch on each operator boundary

} interp_limits;


typedef struct { // interp_privileges#

    mword FILE_READ_ALLOWED;  // FLAG_CLR disables file descriptor read
    mword FILE_WRITE_ALLOWED; // FLAG_CLR disables file descriptor write
    mword SYS_EXEC_ALLOWED;   // FLAG_CLR disables system() calls
    mword BVM_EXEC_ALLOWED;   // FLAG_CLR disables babel operator
    mword BVM_FORK_ALLOWED;   // FLAG_CLR disables fork operator

} interp_privileges;


typedef struct { // interp_flags#

    mword MC_ALLOC_BLOCKING;
    mword MC_GC_HIGHWATER;
    mword MC_GC_LOWWATER;
    mword MC_GC_BLOCKING;
    mword MC_GC_PENDING;
    mword MC_GC_CONVERT_PENDING_TO_OP_RESTART;
    mword MC_GC_PNR;
    mword MC_GC_OP_RESTART;
    mword MC_GC_INTERP_BLOCKING;
    mword MC_GC_SECONDARY_BANK_ALLOC;
    mword MC_GC_ON_EVERY_OP;

    mword INTERP_BOOT_IN_PROGRESS;

    mword BVM_INSTR_IN_PROGRESS;
    mword BVM_INCOHERENT;
    mword BVM_SYMBOLS_DEFINED;
    mword BVM_CLEAN;
    mword BVM_INTERP_OP_TRACE;
    mword BVM_INSTRUMENT_TRIGGER;
    mword BVM_CODE_LIST_EMPTY;
    mword BVM_RETURN_TOS_ON_EXIT;

    mword BVM_CACHE_INVALID;
    mword BVM_CACHE_DIRTY;
    mword BVM_CACHE_ONLY;
    mword BVM_CACHE_BLOCKING;

    mword NO_ASYNC;
    mword NO_EXCEPT;

#ifdef MEM_DEBUG
    mword MC_USE_MALLOC;
#endif

} interp_flags;


#ifdef PROF_MODE
typedef struct {

    mword BVM_PROFILE_ENABLE;   // enable/disable profiling
    mword operator_ms;          // milliseconds spent executing operators

    mword GC_ms;                // milliseconds spent in GC
    mword GC_overshoot;         // amount of memory allocated after signalling MC_GC_PENDING

    mword cache_flush_count;    // number of cache flushes
    mword cache_update_count;   // number of cache updates

    mword mem_alloc_count;      // number of mem_alloc calls
    mword mem_alloc_total;
    mword mem_alloc_largest;

    mword stack_pops;           // number of dstack pops
    mword stack_pushes;         // number of dstack pushes

    mword interp_boot_time;     // time from interp_init to user-code (use dev-code to signal finish)

} bvm_profile;
#endif


typedef struct { // interp_state#

    jmp_buf         *cat_ex;
    jmp_buf         *op_restart;
    int              argc;
    char           **argv;
    char           **envp;
    mword           *interp_argv;
    mem_context     *mem;

    interp_limits       *limits;
    interp_privileges   *privileges;

    mword           *jump_table; // full jump_table

    mword           *nil;
    mword           *empty_string;

    // XXX create a tags-struct
    mword           *alt_tag;
    mword           *seq_tag;
    mword           *fail_tag;
    mword           *pass_tag;
    mword           *accept_tag;
    mword           *reject_tag;
    mword           *true_tag;
    mword           *false_tag;
    mword           *exist_tag;
    mword           *unexist_tag;

    mword           *cold_null_BVM;
    mword           *warm_null_BVM;

    struct tm       *utc_epoch;
    mword            epoch_ms;
    mword           *srand;

    mword            null_hash[HASH_SIZE];
    mword            thread_counter;
    mword            global_tick_count;

#ifdef PROF_MODE
    bvm_profile     *profile;
#endif

} interp_state;


typedef struct { // bvm_cache#

    mword *self;

    mword *code_ptr;
    mword *rstack_ptr;

    mword *dstack_ptr;
    mword *ustack_ptr;

    mword *sym_table;
    mword *soft_root;

    mword *local_root;
    mword *local_path;
    mword *local_pwd;

    mword *thread_id;
    mword *steps;    
    mword *advance_type;
    mword *bvm_initd;

    mword dstack_depth;
    mword dstack_diameter;

    mword *mask_table; // de-privileged jump_table

    interp_flags *flags;
    
    interp_state *interp;

} bvm_cache;


typedef bvm_cache *(*babel_op)(bvm_cache *); // babel_op#


/*****************************************************************************
 *                                                                           *
 *                             GLOBALS                                       *
 *                                                                           *
 ****************************************************************************/

// XXX WAIVER(globals) XXX //
mword *nil;                 // nil#

mword mem_sys_alloc_count; // mem_sys_alloc_count#
mword mem_sys_alloc_total; // mem_sys_alloc_total#
mword *global_dev_overrides; // global_dev_overrides#

mword GLOBAL_BVM_INSTRUMENT_TRIGGER; // For use with instrument.pl

// XXX WAIVER(globals) XXX //
mword *SEXPR_IGNORE_SYMBOL; // SEXPR_IGNORE_SYMBOL#
mword *SEXPR_LIST_SYMBOL;   // SEXPR_LIST_SYMBOL#
mword *SEXPR_CODE_SYMBOL;   // SEXPR_CODE_SYMBOL#
mword *SEXPR_HASH_SYMBOL;   // SEXPR_HASH_SYMBOL#
mword *SEXPR_OPER_SYMBOL;   // SEXPR_OPER_SYMBOL#
mword *SEXPR_VAL_SYMBOL;    // SEXPR_VAL_SYMBOL#
mword *SEXPR_PTR_SYMBOL;    // SEXPR_PTR_SYMBOL#
mword *SEXPR_TAG_SYMBOL;    // SEXPR_TAG_SYMBOL#
mword *SEXPR_TPTR_SYMBOL;   // SEXPR_TPTR_SYMBOL#
mword *SEXPR_BYTES_SYMBOL;  // SEXPR_BYTES_SYMBOL#
mword *SEXPR_REF_SYMBOL;    // SEXPR_REF_SYMBOL#
mword *SEXPR_NIL_SYMBOL;    // SEXPR_NIL_SYMBOL#
mword *SEXPR_SYM_SYMBOL;    // SEXPR_SYM_SYMBOL#
mword *SEXPR_SQUOTE_SYMBOL; // SEXPR_SQUOTE_SYMBOL#
mword *SEXPR_DQUOTE_SYMBOL; // SEXPR_DQUOTE_SYMBOL#
mword *SEXPR_QUOTE_SYMBOL;  // SEXPR_QUOTE_SYMBOL#
mword *SEXPR_SEXPR_SYMBOL;  // SEXPR_SEXPR_SYMBOL#
mword *SEXPR_BS_SYMBOL;     // SEXPR_BS_SYMBOL#
mword *SEXPR_QW_SYMBOL;     // SEXPR_QW_SYMBOL#

// XXX WAIVER(globals) XXX //
mword *bpdl_opcode_table;   // bpdl_opcode_table#
mword *bpdl_macro_table;    // bpdl_macro_table#
mword *bpdl_return_opcode;  // bpdl_return_opcode#

//mword PROFILE_ALLOC_SIZE_8;
//mword PROFILE_ALLOC_SIZE_2;
//mword PROFILE_ALLOC_SIZE_3;
//mword PROFILE_ALLOC_SIZE_4;
//mword PROFILE_ALLOC_SIZE_5;
//mword PROFILE_ALLOC_SIZE_6;
//mword PROFILE_ALLOC_SIZE_7;

/*****************************************************************************
 *                                                                           *
 *                            GLOBAL TAGS                                    *
 *                                                                           *
 ****************************************************************************/

// BABEL_TAGS#
#define BABEL_TAGS                                                           \
    X(BABEL_TAG_NIL              , "/babel/tag/nil")       \
    X(BABEL_TAG_INTERP_NIL       , "nil")       \
    X(BABEL_TAG_REF              , "/babel/tag/ref")       \
    X(BABEL_TAG_PURE_REF         , "/babel/tag/pure_ref")       \
    X(BABEL_TAG_HASH_TABLE       , "/babel/tag/hash_table")       \
    X(BABEL_TAG_HASH_TABLE_ENTRY , "/babel/tag/hash_table_entry")       \
    X(BABEL_TAG_DIRECTORY        , "/babel/tag/directory")       \
    X(BABEL_TAG_BABEL_STRING     , "/babel/tag/babel_string")       \
    X(BABEL_TAG_CSTRING          , "/babel/tag/cstring")       \
    X(BABEL_TAG_STRING_ARRAY     , "/babel/tag/string_array")       \
    X(BABEL_TAG_STRING_LIST      , "/babel/tag/string_list")       \
    X(BABEL_TAG_NUMERIC          , "/babel/tag/numeric")       \
    X(BABEL_TAG_ARRAY8           , "/babel/tag/array8")       \
    X(BABEL_TAG_ARRAY1           , "/babel/tag/array1")       \
    X(BABEL_TAG_BVM              , "/babel/tag/bvm")       \
    X(BABEL_TAG_SYM_BVM          , "/babel/tag/sym_bvm")       \
    X(BABEL_TAG_HIBER_BVM        , "/babel/tag/hiber_bvm")       \
    X(BABEL_TAG_SPARSE_BVM       , "/babel/tag/sparse_bvm")       \
    X(BABEL_TAG_READY_BVM        , "/babel/tag/ready_bvm")       \
    X(BABEL_TAG_BVM_CODE         , "/babel/tag/bvm_code")       \
    X(BABEL_TAG_BVM_STACK        , "/babel/tag/bvm_stack")       \
    X(BABEL_TAG_BVM_RSTACK       , "/babel/tag/bvm_rstack")       \
    X(BABEL_TAG_BVM_JUMP_TABLE   , "/babel/tag/bvm_jump_table")       \
    X(BABEL_TAG_BVM_SYM_TABLE    , "/babel/tag/bvm_sym_table")       \
    X(BABEL_TAG_BVM_STEPS        , "/babel/tag/bvm_steps")       \
    X(BABEL_TAG_EVAL             , "/babel/tag/eval")       \
    X(BABEL_TAG_LOOP             , "/babel/tag/loop")       \
    X(BABEL_TAG_TIMES            , "/babel/tag/times")       \
    X(BABEL_TAG_IFTE             , "/babel/tag/ifte")       \
    X(BABEL_TAG_EACH             , "/babel/tag/each")       \
    X(BABEL_TAG_EACHAR           , "/babel/tag/eachar")       \
    X(BABEL_TAG_WHILE            , "/babel/tag/while")       \
    X(BABEL_TAG_NEST             , "/babel/tag/nest")       \
    X(BABEL_TAG_ALT              , "/babel/tag/alt")       \
    X(BABEL_TAG_SEQ              , "/babel/tag/seq")       \
    X(BABEL_TAG_PASS             , "/babel/tag/pass")       \
    X(BABEL_TAG_FAIL             , "/babel/tag/fail")       \
    X(BABEL_TAG_ACCEPT           , "/babel/tag/accept")       \
    X(BABEL_TAG_REJECT           , "/babel/tag/reject")       \
    X(BABEL_TAG_TRUE             , "/babel/tag/true")       \
    X(BABEL_TAG_FALSE            , "/babel/tag/false")       \
    X(BABEL_TAG_DEF              , "/babel/tag/def")       \
    X(BABEL_TAG_UNDEF            , "/babel/tag/undef")       \
    X(BABEL_TAG_EXIST            , "/babel/tag/exist")       \
    X(BABEL_TAG_UNEXIST          , "/babel/tag/unexist")       \
    X(BABEL_TAG_NAN              , "/babel/tag/nan")       \
    X(BABEL_TAG_LET              , "/babel/tag/let")       \
    X(BABEL_TAG_REF_HASH         , "/babel/tag/ref_hash")       \
    X(BABEL_TAG_REF_STRING       , "/babel/tag/ref_string")       \
    X(BABEL_TAG_REF_ARRAY        , "/babel/tag/ref_array")       \
    X(BABEL_TAG_REF_SYM_LOCAL    , "/babel/tag/ref_sym_local")       \
    X(BABEL_TAG_REF_SYM_GLOBAL   , "/babel/tag/ref_sym_global")       \
    X(BABEL_TAG_COND             , "/babel/tag/cond")       \
    X(BABEL_TAG_TRIE             , "/babel/tag/trie")       \
    X(BABEL_TAG_TRIE_ENTRY       , "/babel/tag/trie_entry")       \
    X(BABEL_TAG_SENTINEL         , "/babel/tag/sentinel")       \
    X(BABEL_TAG_MONKEY           , "/babel/tag/monkey")       \
    X(BABEL_TAG_ROBOT            , "/babel/tag/robot")       \
    X(BABEL_TAG_UNTYPED          , "/babel/tag/untyped")       \
    X(BABEL_TAG_BVM_INITD        , "/babel/tag/bvm_initd")       \
    X(BABEL_SYM_ARGV             , "/babel/sym/argv")       \
    X(BABEL_SYM_PARENT_BVM       , "/babel/sym/parent_bvm")       \
    X(BABEL_SYM_CODE_RESTART_POINT , "/babel/sym/code_restart_point")       

#define X(a,b) mword *a;
BABEL_TAGS
#undef X


/*****************************************************************************
 *                                                                           *
 *                               CACHE                                       *
 *                                                                           *
 ****************************************************************************/

// CACHED_FIELDS#
#define CACHED_FIELDS                                             \
    X(ustack_ptr,   "ustack_ptr",    BABEL_SYM_USTACK_PTR)        \
    X(soft_root,    "soft_root",     BABEL_SYM_SOFT_ROOT)         \
    X(rstack_ptr,   "rstack_ptr",    BABEL_SYM_RSTACK_PTR)        \
    X(thread_id,    "thread_id",     BABEL_SYM_THREAD_ID)         \
    X(advance_type, "advance_type",  BABEL_SYM_ADVANCE_TYPE)      \
    X(bvm_initd,    "bvm_initd",     BABEL_SYM_BVM_INITD)         \
    X(dstack_ptr,   "dstack_ptr",    BABEL_SYM_DSTACK_PTR)        \
    X(steps,        "steps",         BABEL_SYM_STEPS)             \
    X(code_ptr,     "code_ptr",      BABEL_SYM_CODE_PTR)          \
    X(local_root,   "local_root",    BABEL_SYM_LOCAL_ROOT)        \
    X(local_path,   "local_path",    BABEL_SYM_LOCAL_PATH)        \
    X(local_pwd,    "local_pwd",     BABEL_SYM_LOCAL_PWD)          

//X(mask_table,   "mask_table",    BABEL_SYM_MASK_TABLE)        

#define X(a,b,c) mword *c;
CACHED_FIELDS
#undef X

mword *BABEL_SYM_SELF; // BABEL_SYM_SELF#
mword *BABEL_TAG_ZERO_HASH; // BABEL_TAG_ZERO_HASH#

/*****************************************************************************
 *                                                                           *
 *                             UTILITIES                                     *
 *                                                                           *
 ****************************************************************************/

#define s(x)          (*((mword*)x-1)) // s#

#define is_leaf(x)    ((int)s((mword*)x) >  0) // is_leaf#
#define is_inte(x)    ((int)s((mword*)x) <  0) // is_inte#
#define is_tptr(x)    ((int)s((mword*)x) == 0) // is_tptr#

#define is_conslike(x) (is_inte(x) && size(x) == 2) // is_conslike#

#define size(x)      (abs(s(x))/MWORD_SIZE) // size#
#define alloc_size(x) (s(x) == 0 ? TPTR_SIZE : size(x)+1) // alloc_size#
#define mem_alloc_size(x) (x == 0 ? TPTR_SIZE : (abs(x)/MWORD_SIZE)) // mem_alloc_size#

//RHS C-dereference for interior array (rci):
//#define rci(x,y) (*((void**)x+y))  // rci#
#define rci(x,y) (*((mword**)x+y))  // rci#

//RHS C-dereference for leaf array (rcl):
#define rcl(x,y) (*((mword*)x+y))  // rcl#

//LHS C-dereference for interior array (lci):
#define lci(x,y) (*((mword**)x+y)) // lci#

//LHS C-dereference for leaf array (lcl):
#define lcl(x,y) (*((mword*)x+y))  // lcl#

#define tagcmp(x,y,z) ( (is_tptr(x) || (size(x) >= HASH_SIZE)) ? (memcmp((mword*)x, y, z)) : -1 ) // tagcmp#
#define tageq(x,y,z)  ( tagcmp(x,y,z) == 0 ) // tageq#

#define is_nil(x)       ( tageq(x,nil,TAG_SIZE) ) // is_nil#
#define is_reject(x)    ( tageq((x),BABEL_TAG_REJECT,MIN_TAG_SIZE) ) // is_reject#
#define is_unexist(x,y) ( tageq(y, x->interp->unexist_tag, TAG_SIZE) ) // is_unexist#

// XXX UNSAFE; use this only where we know for sure we have a tptr...
#define is_nil_fast(x) ( (memcmp(x, nil, HASH_SIZE*MWORD_SIZE)) == 0 ) // is_nil_fast#

// XXX UNSAFE; use this only where we know for sure we are 
// working with interprer-allocated mem (fastest is_nil)
#define is_nil_interp(x) ( (mword*)x == nil ) // is_nil_interp#

//#include "tags.h"
//#include "construct.sp.h"
//#include "hello.sp.bbc.h"
//#include "run2.sp.bbc.h"

#define is_bvm(x) ( tagcmp((x),BABEL_TAG_BVM,MIN_TAG_SIZE) ) // is_bvm#
#define is_ref(x) ( tageq((x),BABEL_TAG_REF,MIN_TAG_SIZE) ) // is_ref#

//nil-safe car/cdr:
#define scar(x)     (is_nil(x) ? nil : car(x)) // scar#
#define scdr(x)     (is_nil(x) ? nil : cdr(x)) // scdr#

//car/cdr rework:
//internal car/cdr (not list-safe or tptr-safe):
#define icar(x)     rcl((mword*)(x),0) // icar#
#define icdr(x)     rcl((mword*)(x),1) // icdr#
#define icpr(x)     rcl((mword*)(x),2) // icpr#

//list-safe car/cdr (not tptr safe):
#define lcar(x)     (is_nil(x) ? nil : (mword*)icar(x)) // lcar#
#define lcdr(x)     (is_nil(x) ? nil : (mword*)icdr(x)) // lcdr#

//safest car/cdr
#define car(x)      ( is_nil(x) ? nil : (mword*)icar(tptr_detag(this_bvm, (mword*)x)) ) // car#
#define cdr(x)      ( is_nil(x) ? nil : (mword*)icdr(tptr_detag(this_bvm, (mword*)x)) ) // cdr#
#define cpr(x)      ( is_nil(x) ? nil : (mword*)icpr(tptr_detag(this_bvm, (mword*)x)) ) // cpr#

#define  cxr(bvm,ptr,off)   ( rci((mword*)(tptr_detag(bvm, ptr)),off) ) // cxr#

#define get_tptr(x)   (mword*)rci((mword*)x,TPTR_PTR_OFFSET) // get_tptr#
#define set_tptr(x,y) lci((mword*)x,TPTR_PTR_OFFSET)=(mword*)y; // set_tptr#


#define TRAVERSED(x) (s(x) & CTL_MASK) // TRAVERSED#
#define MARK_TRAVERSED(x) (s(x) |= 0x1) // MARK_TRAVERSED#

#define is_leaf_masked(x)    ((~CTL_MASK & (int)s((mword*)x)) >  0) // is_leaf_masked#
#define is_inte_masked(x)    ((~CTL_MASK & (int)s((mword*)x)) <  0) // is_inte_masked#
#define is_tptr_masked(x)    ((~CTL_MASK & (int)s((mword*)x)) == 0) // is_tptr_masked#
#define size_masked(x)       (abs(~CTL_MASK & s(x))/MWORD_SIZE) // size_masked#

#define MAX(a,b) ((a>=b) ? (a) : (b)) // MAX#
#define MIN(a,b) ((a<=b) ? (a) : (b)) // MIN#

/*****************************************************************************
 *                                                                           *
 *                             UTILITIES                                     *
 *                                                                           *
 ****************************************************************************/

//Uncomment to turn on tracing in bvm_interp():
//#define BABEL_RESET_TRACE
//#define INTERP_CORE_OP_TRACE
//#define INTERP_CORE_TRACE
//#define BPDL_TRACE
//#define BPDL2_TRACE
//#define GC_TRACE

//DEV GLOBALS
FILE *dev_dump_file; // dev_dump_file#

//  vvvvv REPLACE ALL THESE LOCAL _i 's!!!
int dev_i;  // dev_i#

#define QUOTEME(x)      #x
#define _d(x)           fprintf(stderr, "%s %08x\n", QUOTEME(x), x);  // d#
#define _dd(x)          fprintf(stderr, "%s %d\n", QUOTEME(x), x);    // dd#
#define _ds(x)          fprintf(stderr, "%s %08x ", QUOTEME(x), x);   // ds#
#define _mem(x)         int _i; printf("%08x\n", s(x)); for(_i=0; _i<alloc_size(x)-1; _i++){ printf("%08x\n", c(x,_i)); } // _mem#
#define _memi(x)        fprintf(stderr, "---- %08x\n", s(x)); for(dev_i=0; dev_i<alloc_size(x)-1; dev_i++){ if(dev_i>=0){fprintf(stderr, "%04x ", (unsigned)dev_i*MWORD_SIZE);} fprintf(stderr, "%08x\n", (unsigned)rci(x,dev_i)); } // _memi#
#define _meml(x)        int _i; fprintf(stderr, "%08x\n", s(x)); for(_i=0; _i<alloc_size(x)-1; _i++){ fprintf(stderr, "%08x\n", (unsigned)rcl(x,_i)); } // _meml#
#define _dump(x)        printf("%s\n", (char*)_bs2gv(this_bvm, x));  // _dump#
#define _dumpd(x)       printf("%s\n", _bs2gv(this_bvm, x->dstack_ptr));  // _dumpd#
#define _die            fprintf(stderr, "Died at %s line %d\n", __FILE__, __LINE__); exit(DIE_CODE);  // die#
#define _warn(x)        fprintf(stderr, "WARNING: %s in %s() at %s line %d\n", x, __func__, __FILE__, __LINE__);  // warn#
#define _error(x)       fprintf(stderr, "ERROR: %s in %s() at %s line %d\n", x, __func__, __FILE__, __LINE__); // error#
#define _fatal(x)       fprintf(stderr, "FATAL: %s in %s() at %s line %d\n", x, __func__, __FILE__, __LINE__); _die;  // _fatal#
#define _cat_except(x)  _trace; longjmp(*(x->interp->cat_ex),CAT_EXCEPT); // _cat_except#
#define _op_restart(x)  longjmp(*(x->interp->op_restart),OP_RESTART); // _op_restart#
#define _trace          fprintf(stderr, "%s() in %s line %d\n", __func__, __FILE__, __LINE__);   // _trace#
#define _msg(x)         fprintf(stderr, "%s in %s(), %s line %d\n", x, __func__, __FILE__, __LINE__);   // _msg#
#define _say(x)         fprintf(stderr, "%s\n", x);   // _say#
#define _prn(x)         fprintf(stderr, "%s", x); // _prn#
#define _enhance(x)     fprintf(stderr, "ENHANCEMENT: %s in %s at %s line %d\n", x, __func__, __FILE__, __LINE__); // enhance#
#define _mema(x)        int _i; printf("%08x %08x\n", (mword)(x-1), s(x)); for(_i=0; _i<alloc_size(x)-1; _i++){ printf("%08x %08x\n", (mword)(x+_i), c(x,_i)); } // _mema#
#define _timestamp(x)   (time_ms(void) - x->interp->epoch_ms)

#define _opcode_trace(x) fprintf(stderr, "%08x %03x %s\n", this_bvm->interp->global_tick_count, x, interp_opcode_names[x]);

// _dumpf#
#define _dumpf(x)       \
    dev_dump_file = fopen("test.dot", "w"); \
    fprintf(dev_dump_file, (char*)_bs2gv(this_bvm, x));  \
    fclose(dev_dump_file);

// _memf#
#define _memf(x, y) \
    dev_dump_file = fopen(x, "w"); \
    fprintf(dev_dump_file, "---- %08x\n", s(y));  \
    for(dev_i=0; dev_i<alloc_size(y)-1; dev_i++){  \
        if(dev_i>=0){ \
            fprintf(dev_dump_file, "%04x ", (unsigned)dev_i*MWORD_SIZE); \
        } \
        fprintf(dev_dump_file, "%08x\n", (unsigned)rci(y,dev_i));  \
    } \
    fclose(dev_dump_file);

// _bounds_check#
#define _bounds_check(bvm, x) \
    if(mem_primary_bounds_check(bvm, x)){ \
        _msg("_bounds_check: primary"); \
    } \
    else if(mem_secondary_bounds_check(bvm, x)){ \
        _msg("_bounds_check: secondary"); \
    } \
    else{ \
        _msg("_bounds_check: neither"); \
    }


#endif //BABEL_H

// Clayton Bauman 2014


