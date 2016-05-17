// init.c
//

#include "babel.h"
#include "init.h"
#include "bstruct.h"
#include "mem.h"
#include "opcode.h"
#include "construct.sp.h"
#include "null.sp.h"
#include "introspect.h"
#include "pearson16.h"
#include "string.h"
#include "tptr.h"
#include "bvm.h"
#include "cache.h"
#include "hash.h"
#include "mt19937ar.h"
#include "io.h"
#include "list.h"
#include "trie.h"
#include "bpdl.h"
#include "bpdl2.h"
#include "xoroshiro128.h"


// Initializes the root Babel Virtual Machine (BVM) and interpreter-only state...
//
bvm_cache *init_interp(bvm_cache *this_bvm, int argc, char **argv, char **envp, jmp_buf *cat_ex){ // init_interp#

#ifdef BABEL_RESET_TRACE
_trace;
#endif

// XXX //SECURITY// **DO NOT REMOVE** FOLLOWING LINES //SECURITY// XXX //
#ifdef MEM_DEBUG
_msg("MEM_DEBUG defined");
#endif

#ifdef DEV_MODE
_msg("DEV_MODE defined");
#endif

#ifdef PROF_MODE
_msg("PROF_MODE defined");
#endif

#ifdef MEM_DEBUG
_d(MEM_DEFAULT_MIN_ALLOC);
#endif
// XXX //SECURITY// **DO NOT REMOVE** ABOVE LINES     //SECURITY// XXX //

    this_bvm->interp     = mem_sys_alloc(sizeof(interp_state));      // XXX WAIVER(mem_sys_alloc) XXX //

    this_bvm->interp->cat_ex     = cat_ex;
    this_bvm->interp->op_restart = NULL;
    this_bvm->interp->argc       = argc;
    this_bvm->interp->argv       = argv;
    this_bvm->interp->envp       = envp;
    this_bvm->interp->limits     = mem_sys_alloc(sizeof(interp_limits));     // XXX WAIVER(mem_sys_alloc) XXX //
    this_bvm->interp->privileges = mem_sys_alloc(sizeof(interp_privileges)); // XXX WAIVER(mem_sys_alloc) XXX //

#ifdef PROF_MODE
    this_bvm->interp->profile    = mem_sys_alloc(sizeof(bvm_profile));       // XXX WAIVER(mem_sys_alloc) XXX //
#endif

    mem_new(this_bvm);

    init_interp_flags(this_bvm);
    init_interp_limits(this_bvm);
    init_interp_privileges(this_bvm);
    init_global_consts(this_bvm);
    init_nil(this_bvm);
    init_interp_consts(this_bvm);
    init_jump_table(this_bvm);
    init_new_epoch(this_bvm);
    init_new_srand(this_bvm);
//    interp_new_null_hash(this_bvm);
//    interp_new_stdin_capture(this_bvm);

//    bpdl_new(this_bvm);
    bpdl_init_opcode_table(this_bvm); // XXX MOVE to bpdl_new()
    bpdl_init_macro_table(this_bvm); // XXX MOVE to bpdl_new()

    this_bvm->interp->thread_counter=0;
    this_bvm->interp->global_tick_count=0;

    init_load_root_bvm(this_bvm);

    init_capture_argv(this_bvm);

#ifdef PROF_MODE
    init_bvm_profile(this_bvm);
#endif

    this_bvm->flags->INTERP_BOOT_IN_PROGRESS = FLAG_CLR;

#ifdef BABEL_RESET_TRACE
_msg("RESET TRACE COMPLETE");
#endif

    return this_bvm;

}


//
//
bvm_cache *init_load_root_bvm(bvm_cache *this_bvm){ // init_load_root_bvm#

#ifdef BABEL_RESET_TRACE
_trace;
#endif

    mword *load_bbl = (void*)mem_alloc(this_bvm, BYTE_SIZE(BBL_SIZE)); // BBL_SIZE is sfield/4
    memcpy(load_bbl, bbl, (size_t)BYTE_SIZE(BBL_SIZE)); // XXX WAIVER(memcpy) XXX //
    this_bvm->self = _load(this_bvm, (mword*)load_bbl, BBL_SIZE);

this_bvm->flags->MC_USE_MALLOC = FLAG_SET; // XXX WAIVER(MC_USE_MALLOC); temporary until large_alloc_list XXX //

    mword *load_null_bvm = (void*)mem_alloc(this_bvm, BYTE_SIZE(NULL_BVM_SIZE)); // NULL_BVM_SIZE is sfield/4
    memcpy(load_null_bvm, null_bvm, (size_t)BYTE_SIZE(NULL_BVM_SIZE)); // XXX WAIVER(memcpy) XXX //
    this_bvm->interp->cold_null_BVM = _load(this_bvm, (mword*)load_null_bvm, NULL_BVM_SIZE);

this_bvm->flags->MC_USE_MALLOC = FLAG_CLR; // XXX WAIVER(MC_USE_MALLOC); temporary until large_alloc_list XXX //

    return this_bvm;

}


//
//
bvm_cache *init_interp_flags(bvm_cache *this_bvm){ // init_interp_flags#

#ifdef BABEL_RESET_TRACE
_trace;
#endif

    interp_flags *f = mem_sys_alloc(sizeof(interp_flags)); // XXX WAIVER(mem_sys_alloc) XXX //

    f->MC_ALLOC_BLOCKING     = FLAG_CLR;
    f->MC_GC_HIGHWATER       = FLAG_CLR;
    f->MC_GC_LOWWATER        = FLAG_CLR;
    f->MC_GC_BLOCKING        = FLAG_CLR;
    f->MC_GC_PENDING         = FLAG_CLR;
    f->MC_GC_CONVERT_PENDING_TO_OP_RESTART = FLAG_CLR;
    f->MC_GC_PNR             = FLAG_CLR;
    f->MC_GC_OP_RESTART      = FLAG_CLR;

    f->INTERP_BOOT_IN_PROGRESS = FLAG_SET;

    f->BVM_INSTR_IN_PROGRESS = FLAG_CLR;
    f->BVM_INCOHERENT        = FLAG_IGN;
    f->BVM_SYMBOLS_DEFINED   = FLAG_IGN;
    f->BVM_CLEAN             = FLAG_IGN;
    f->BVM_INTERP_OP_TRACE   = FLAG_CLR;

//    f->BVM_INSTRUMENT_TRIGGER = FLAG_CLR;
    f->BVM_CODE_LIST_EMPTY   = FLAG_CLR;
    f->BVM_RETURN_TOS_ON_EXIT = FLAG_CLR;

    f->BVM_CACHE_INVALID     = FLAG_CLR;
    f->BVM_CACHE_DIRTY       = FLAG_CLR;
    f->BVM_CACHE_ONLY        = FLAG_IGN;
    f->BVM_CACHE_BLOCKING    = FLAG_CLR;

    f->NO_ASYNC              = FLAG_IGN;
    f->NO_EXCEPT             = FLAG_IGN;

#ifdef MEM_DEBUG
    f->MC_USE_MALLOC         = FLAG_CLR;
#endif

    this_bvm->flags = f;

    GLOBAL_BVM_INSTRUMENT_TRIGGER = FLAG_CLR; // For use with instrument.pl

    return this_bvm;

}


//
//
bvm_cache *init_interp_limits(bvm_cache *this_bvm){ // init_interp_limits#

#ifdef BABEL_RESET_TRACE
_trace;
#endif

    interp_limits *l = mem_sys_alloc(sizeof(interp_limits)); // XXX WAIVER(mem_sys_alloc) XXX //

    l->max_num_sys_threads      = 1;        // XXX unimplemented
    l->max_num_steps            = -1;       // XXX: Implement command-line init of this value
    l->max_bytes_mem_alloc      = (1<<30);  // Arbitrary 1GB limit
    l->max_num_file_descriptors = 0;        // XXX unimplemented
    l->max_num_file_access      = 0;        // XXX unimplemented
    l->max_run_time_ms          = 0;        // XXX unimplemented

    this_bvm->interp->limits = l;

    return this_bvm;

}


//
//
bvm_cache *init_interp_privileges(bvm_cache *this_bvm){ // init_interp_privileges#

#ifdef BABEL_RESET_TRACE
_trace;
#endif

    interp_privileges *p = mem_sys_alloc(sizeof(interp_privileges)); // XXX WAIVER(mem_sys_alloc) XXX //

    p->FILE_READ_ALLOWED  = FLAG_SET;
    p->FILE_WRITE_ALLOWED = FLAG_SET;
    p->SYS_EXEC_ALLOWED   = FLAG_SET;
    p->BVM_EXEC_ALLOWED   = FLAG_SET;
    p->BVM_FORK_ALLOWED   = FLAG_SET;

    this_bvm->interp->privileges = p;

    return this_bvm;

}


//
//
bvm_cache *init_jump_table(bvm_cache *this_bvm){ // init_jump_table#

#ifdef BABEL_RESET_TRACE
_trace;
#endif

//    this_bvm->interp->jump_table = 
//        _newlfcp(this_bvm, (mword*)opcode_array, NUM_INTERP_OPCODES);

    this_bvm->interp->jump_table = mem_sys_alloc(BYTE_SIZE(NUM_INTERP_OPCODES));

    int i;
    for(i=0;i<NUM_INTERP_OPCODES;i++){
        this_bvm->interp->jump_table[i] = (mword)opcode_array[i];
    }

    return this_bvm;

}


//
//
bvm_cache *init_new_epoch(bvm_cache *this_bvm){ // init_new_epoch#

#ifdef BABEL_RESET_TRACE
_trace;
#endif

    time_t rawtime;
    struct tm *utc_epoch;

    time( &rawtime );
    utc_epoch = gmtime( &rawtime );

    //    tm_sec     int    seconds after the minute    0-61*
    //    tm_min     int    minutes after the hour      0-59
    //    tm_hour    int    hours since midnight        0-23
    //    tm_mday    int    day of the month            1-31
    //    tm_mon     int    months since January        0-11
    //    tm_year    int    years since 1900
    //    tm_wday    int    days since Sunday           0-6
    //    tm_yday    int    days since January 1        0-365

    struct tm *bvm_utc_epoch = mem_sys_alloc(sizeof(struct tm)); //&this_bvm->interp->utc_epoch;

    bvm_utc_epoch->tm_sec  = utc_epoch->tm_sec;
    bvm_utc_epoch->tm_min  = utc_epoch->tm_min;
    bvm_utc_epoch->tm_hour = utc_epoch->tm_hour;
    bvm_utc_epoch->tm_mday = utc_epoch->tm_mday;
    bvm_utc_epoch->tm_mon  = utc_epoch->tm_mon;
    bvm_utc_epoch->tm_year = utc_epoch->tm_year;
    bvm_utc_epoch->tm_wday = utc_epoch->tm_wday;
    bvm_utc_epoch->tm_yday = utc_epoch->tm_yday;

    this_bvm->interp->utc_epoch = bvm_utc_epoch;

    return this_bvm;

}


// must be called AFTER init_new_epoch
//
void init_new_srand(bvm_cache *this_bvm){ // init_new_srand#

#ifdef BABEL_RESET_TRACE
_trace;
#endif

    #define SRAND_STRING_SIZE 32

    char srand_string[SRAND_STRING_SIZE];

    sprintf(srand_string, 
            "%d%d%d%d%d%d", 
            this_bvm->interp->utc_epoch->tm_sec,
            this_bvm->interp->utc_epoch->tm_min,
            this_bvm->interp->utc_epoch->tm_hour,
            this_bvm->interp->utc_epoch->tm_mday,
            this_bvm->interp->utc_epoch->tm_mon,
            this_bvm->interp->utc_epoch->tm_year);

    mword *babel_srand_string = string_c2b(this_bvm, srand_string, SRAND_STRING_SIZE);

    mword *time_hash = _hash8(this_bvm, babel_srand_string);

//    mword *time_hash = _pearson16(
//                            this_bvm,  
//                            hash_init, 
//                            babel_srand_string,
//                            _arlen8(this_bvm, babel_srand_string) );

    init_by_array( (unsigned long *)time_hash, HASH_SIZE*(sizeof(mword)/sizeof(unsigned long)));

    this_bvm->interp->srand = time_hash;

    time_hash = _hash(this_bvm, time_hash);

    xoroshiro128_s[0] = time_hash[0];
    xoroshiro128_s[1] = time_hash[1];

//    return time_hash;

}


//
//
void init_global_consts(bvm_cache *this_bvm){ // init_global_consts#

#ifdef BABEL_RESET_TRACE
_trace;
#endif

    mword *temp = _newlfi(this_bvm, HASH_SIZE,0);
//    BABEL_TAG_ZERO_HASH = _sys_cp( this_bvm, _newlfi(this_bvm, HASH_SIZE,0) );
    BABEL_TAG_ZERO_HASH = _sys_cp( this_bvm, temp );
    BABEL_SYM_SELF      = _sys_cp( this_bvm, HASH8(this_bvm, "self") );

#define X(a, b) a = _sys_cp( this_bvm, HASH8(this_bvm, b) );
BABEL_TAGS    
#undef X

#define X(a, b, c) c = _sys_cp( this_bvm, HASH8(this_bvm, b) );
CACHED_FIELDS
#undef X

    // sexpr constants
    SEXPR_IGNORE_SYMBOL   = _sys_cp( this_bvm, _mklf( this_bvm, 6, 0x69, 0x67, 0x6e, 0x6f, 0x72, 0x65 ));
    SEXPR_QUOTE_SYMBOL    = _sys_cp( this_bvm, _mklf( this_bvm, 5, 0x71, 0x75, 0x6f, 0x74, 0x65 ));
    SEXPR_SEXPR_SYMBOL    = _sys_cp( this_bvm, _mklf( this_bvm, 5, 0x73, 0x65, 0x78, 0x70, 0x72 ));
    SEXPR_BYTES_SYMBOL    = _sys_cp( this_bvm, _mklf( this_bvm, 5, 0x62, 0x79, 0x74, 0x65, 0x73 ));
    SEXPR_LIST_SYMBOL     = _sys_cp( this_bvm, _mklf( this_bvm, 4, 0x6c, 0x69, 0x73, 0x74 ));
    SEXPR_CODE_SYMBOL     = _sys_cp( this_bvm, _mklf( this_bvm, 4, 0x63, 0x6f, 0x64, 0x65 ));
    SEXPR_HASH_SYMBOL     = _sys_cp( this_bvm, _mklf( this_bvm, 4, 0x68, 0x61, 0x73, 0x68 ));
    SEXPR_OPER_SYMBOL     = _sys_cp( this_bvm, _mklf( this_bvm, 4, 0x6f, 0x70, 0x65, 0x72 ));
    SEXPR_TPTR_SYMBOL     = _sys_cp( this_bvm, _mklf( this_bvm, 4, 0x74, 0x70, 0x74, 0x72 ));
    SEXPR_VAL_SYMBOL      = _sys_cp( this_bvm, _mklf( this_bvm, 3, 0x76, 0x61, 0x6c ));
    SEXPR_PTR_SYMBOL      = _sys_cp( this_bvm, _mklf( this_bvm, 3, 0x70, 0x74, 0x72 ));
    SEXPR_TAG_SYMBOL      = _sys_cp( this_bvm, _mklf( this_bvm, 3, 0x74, 0x61, 0x67 ));
    SEXPR_REF_SYMBOL      = _sys_cp( this_bvm, _mklf( this_bvm, 3, 0x72, 0x65, 0x66 ));
    SEXPR_NIL_SYMBOL      = _sys_cp( this_bvm, _mklf( this_bvm, 3, 0x6e, 0x69, 0x6c ));
    SEXPR_SYM_SYMBOL      = _sys_cp( this_bvm, _mklf( this_bvm, 3, 0x73, 0x79, 0x6d ));
    SEXPR_SQUOTE_SYMBOL   = _sys_cp( this_bvm, _mklf( this_bvm, 1, 0x27 ));
    SEXPR_DQUOTE_SYMBOL   = _sys_cp( this_bvm, _mklf( this_bvm, 1, 0x22 ));
    SEXPR_BS_SYMBOL       = _sys_cp( this_bvm, _mklf( this_bvm, 2, 0x62, 0x73 ));
    SEXPR_QW_SYMBOL       = _sys_cp( this_bvm, _mklf( this_bvm, 2, 0x71, 0x77 ));

}


//
//
bvm_cache *init_nil(bvm_cache *this_bvm){ // init_nil#

#ifdef BABEL_RESET_TRACE
_trace;
#endif

    mword *nil_hash = _sys_cp( this_bvm, HASH8(this_bvm, "nil") );

    nil = mem_sys_alloc(BYTE_SIZE(TPTR_SIZE));
    nil++;

    s(nil) = 0;

    memcpy(nil, nil_hash, BYTE_SIZE(HASH_SIZE)); // XXX WAIVER(memcpy) XXX

    lcl(nil,HASH_SIZE)   = -1 * BYTE_SIZE(1);
    lci(nil,HASH_SIZE+1) = nil;

    this_bvm->interp->nil = nil;

    return this_bvm;

}


//
//
void init_interp_consts(bvm_cache *this_bvm){ // init_interp_consts#

#ifdef BABEL_RESET_TRACE
_trace;
#endif

    this_bvm->interp->empty_string = _newlfi(this_bvm, 1, 0);
    this_bvm->interp->alt_tag      = _sys_cp(this_bvm, BABEL_TAG_ALT);
    this_bvm->interp->seq_tag      = _sys_cp(this_bvm, BABEL_TAG_SEQ);
    this_bvm->interp->fail_tag     = _sys_cp(this_bvm, BABEL_TAG_FAIL);
    this_bvm->interp->pass_tag     = _sys_cp(this_bvm, BABEL_TAG_PASS);
    this_bvm->interp->accept_tag   = _sys_cp(this_bvm, BABEL_TAG_ACCEPT);
    this_bvm->interp->reject_tag   = _sys_cp(this_bvm, BABEL_TAG_REJECT);
    this_bvm->interp->true_tag     = _sys_cp(this_bvm, BABEL_TAG_TRUE);
    this_bvm->interp->false_tag    = _sys_cp(this_bvm, BABEL_TAG_FALSE);
    this_bvm->interp->exist_tag    = _sys_cp(this_bvm, BABEL_TAG_EXIST);
    this_bvm->interp->unexist_tag  = _sys_cp(this_bvm, BABEL_TAG_UNEXIST);

}


//
//
bvm_cache *init_capture_argv(bvm_cache *this_bvm){ // init_capture_argv#

#ifdef BABEL_RESET_TRACE
_trace;
#endif

    // 1. babel
    // 2. switches
    // 3. - OR .bbl file name

    char **argv = this_bvm->interp->argv;
    int    argc = this_bvm->interp->argc;

    mword *temp_argv = nil;

    //initialize argv
    //XXX This will change when we add CLI processing:
    #define NUM_BABEL_INTERP_ARGS 1 

    if(argc > NUM_BABEL_INTERP_ARGS){

        temp_argv = _cons(
                        this_bvm, 
                        string_c2b(this_bvm, argv[NUM_BABEL_INTERP_ARGS], 100), nil);

        int i;
        for( i = NUM_BABEL_INTERP_ARGS+1; i < argc; i++ ){

            temp_argv = _unshift(this_bvm, temp_argv, string_c2b(this_bvm, argv[i], 100));

        }

    }

    this_bvm->interp->interp_argv = temp_argv;

    trie_insert(
            this_bvm, 
            tptr_detag(this_bvm, 
                this_bvm->self), 
            BABEL_SYM_ARGV, 
            nil, 
            temp_argv);

    return this_bvm;

}


//
//
#ifdef PROF_MODE
void init_bvm_profile(bvm_cache *this_bvm){

    bvm_profile *p = this_bvm->interp->profile;

    p->BVM_PROFILE_ENABLE   = FLAG_CLR;
    p->operator_ms          = 0;
    p->GC_ms                = 0;
    p->cache_flush_count    = 0;
    p->cache_update_count   = 0;
    p->mem_alloc_count      = 0;
    p->stack_pops           = 0;
    p->stack_pushes         = 0;
    p->GC_overshoot         = 0;
    p->interp_boot_time     = 0;

}
#endif

// Clayton Bauman 2014

