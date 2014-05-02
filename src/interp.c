// interp.c
//

#include "babel.h"
#include "interp.h"
#include "io.h"
#include "bvm.h"
#include "pearson16.h"
#include <time.h>
#include "mt19937ar.h"
#include "array.h"
#include "string.h"
#include "util.h"
#include "load.h"
#include "bstruct.h"
#include "stack.h"
#include "tags.h"
#include "hash.h"
#include "tptr.h"
#include "list.h"
#include <stdarg.h>
#include "ref.h"
#include "mem.h"
#include <windows.h>
#include "cache.h"

//
//
int babel_interp(bvm_cache *this_bvm, int argc, char **argv, char **envp){ // babel_interp#


    clock_t wall_clock_time;
    wall_clock_time = clock();

    jmp_buf cat_ex;

    int val;
    val = setjmp(cat_ex);

    if(val==CAT_EXCEPT){
        fprintf(stderr,"CATASTROPHIC EXCEPTION: babel\n");
        die;
    }
    else if(val==INTERP_RESET){
        fprintf(stderr,"INTERP_RESET: babel\n");
    }

    interp_init(this_bvm, argc, argv, envp, &cat_ex);

    bvm_interp(this_bvm);

    wall_clock_time = (clock() - wall_clock_time) / CLOCKS_PER_SEC;

    return wall_clock_time;

}


//  Initializes the root Babel Virtual Machine (BVM)
//  and interpreter-only state...
bvm_cache *interp_init(bvm_cache *this_bvm, int argc, char **argv, char **envp, jmp_buf *cat_ex){ // interp_init#

    // Initialize this_bvm->interp ...
    interp_new(this_bvm, argc, argv, envp, cat_ex);

    // Load the Construct ...
    mword *load_bbl = mc_alloc(this_bvm, MWORDS(BBL_SIZE));
    memcpy(load_bbl, bbl, MWORDS(BBL_SIZE));
    this_bvm->self = _load(this_bvm, (mword*)load_bbl,BBL_SIZE);

    // Initialize symbolic constants
    // FIXME: Move this down into interp_new()
    interp_init_symbolic_constants(this_bvm);

// BVM_SYM:
//    this_bvm->flags->BVM_CACHE_DIRTY  = FLAG_CLR;
//    this_bvm->flags->BVM_CACHE_INVALID = FLAG_CLR;
//
//    bvm_sym_update_cache(this_bvm);
//    bvm_interp(this_bvm);
//    die;

    // Initialize the BVM
    bvm_new_sparse_bvm(this_bvm);

// TODO:
// bvm_sym_update_cache(this_bvm);
// initialize parent_bvm?

    //TODO: Configure root BVM resource limits

    return this_bvm;

}


//
//
bvm_cache *interp_new(bvm_cache *this_bvm, int argc, char **argv, char **envp, jmp_buf *cat_ex){ // interp_new#

    this_bvm->interp = sys_alloc(sizeof(interp_state)); // XXX WAIVER XXX

    this_bvm->interp->cat_ex    = cat_ex;
    this_bvm->interp->argc      = argc;
    this_bvm->interp->argv      = argv;
    this_bvm->interp->envp      = envp;

    mc_new(this_bvm);

    interp_new_flags(this_bvm);
    interp_new_nil(this_bvm);
    interp_new_jump_table(this_bvm);
    interp_new_empty_string(this_bvm);
    interp_new_epoch(this_bvm);
    interp_new_srand(this_bvm);
    interp_new_null_hash(this_bvm);
    interp_new_stdin_capture(this_bvm);
    interp_capture_argv(this_bvm);

    this_bvm->interp->thread_counter=0;
    this_bvm->interp->global_tick_count=0;

    return this_bvm;

}


//
//
bvm_cache *interp_new_jump_table(bvm_cache *this_bvm){ // interp_new_jump_table#


    #include "fixed_opcodes.h"

    #define num_opcodes (sizeof(interp_fixed_opcodes)/sizeof(babel_op))

    mword *temp = sys_alloc(MWORDS(num_opcodes+1)); //XXX WAIVER XXX

    temp[0] = num_opcodes * MWORD_SIZE;
    temp++;

    int i;
    for(i=0;i<num_opcodes;i++){
        (mword*)temp[i] = (mword*)interp_fixed_opcodes[i];
    }

    this_bvm->interp->jump_table = temp;

    return this_bvm;

}


//
//
bvm_cache *interp_new_flags(bvm_cache *this_bvm){ // interp_new_flags#

    interp_flags *f = sys_alloc(sizeof(interp_flags)); // XXX WAIVER XXX

    f->MC_ALLOC_BLOCKING     = FLAG_CLR;
    f->MC_GC_BLOCKING        = FLAG_CLR;
    f->MC_GC_PENDING         = FLAG_CLR;
    f->BVM_INSTR_IN_PROGRESS = FLAG_CLR;
    f->BVM_CACHE_DIRTY       = FLAG_CLR;
    f->BVM_CACHE_INVALID     = FLAG_SET;
    f->BVM_INCOHERENT        = FLAG_IGN;
    f->BVM_CLEAN             = FLAG_IGN;
    f->NO_ASYNC              = FLAG_IGN;
    f->NO_EXCEPT             = FLAG_IGN;
    f->BVM_CACHE_ONLY        = FLAG_IGN;
    f->BVM_SYMBOLS_DEFINED   = FLAG_IGN;

    this_bvm->flags = f;

    return this_bvm;

}


//
//
bvm_cache *interp_new_nil(bvm_cache *this_bvm){ // interp_new_nil#


    mword *temp = sys_alloc( MWORDS(TPTR_SIZE) ); // XXX WAIVER XXX
    temp[0]=0;
    this_bvm->interp->nil=temp+1;

    int i;
    for(i=0; i<HASH_SIZE; i++){
        this_bvm->interp->nil[i] = BABEL_TAG_INTERP_NIL[i];
    }

    this_bvm->interp->nil[HASH_SIZE  ] = -1*MWORD_SIZE;
    set_tptr(this_bvm->interp->nil,this_bvm->interp->nil);

    nil = this_bvm->interp->nil;

    return this_bvm;

}


//
//
bvm_cache *interp_new_empty_string(bvm_cache *this_bvm){ // interp_new_empty_string#

    this_bvm->interp->empty_string = _newlfi(this_bvm, 1, 0);

    return this_bvm;

}


//
//
bvm_cache *interp_new_null_hash(bvm_cache *this_bvm){ // interp_new_null_hash#


    // XXX 32-bit NON-PORTABLE
    this_bvm->interp->null_hash[0] = 0x01ab0dc1;
    this_bvm->interp->null_hash[1] = 0x831422c3;
    this_bvm->interp->null_hash[2] = 0x0b7c30af;
    this_bvm->interp->null_hash[3] = 0x88e9045b;

    return this_bvm;

}


//
//
bvm_cache *interp_new_epoch(bvm_cache *this_bvm){ // interp_new_epoch#

    time_t rawtime;
    struct tm *utc_epoch;

    time( &rawtime );
    utc_epoch = gmtime( &rawtime );

    //tm_sec	int	seconds after the minute	0-61*
    //tm_min	int	minutes after the hour	    0-59
    //tm_hour	int	hours since midnight	    0-23
    //tm_mday	int	day of the month	        1-31
    //tm_mon	int	months since January	    0-11
    //tm_year	int	years since 1900	
    //tm_wday	int	days since Sunday	        0-6
    //tm_yday	int	days since January 1	    0-365

    struct tm *bvm_utc_epoch = sys_alloc(sizeof(struct tm)); //&this_bvm->interp->utc_epoch;

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


// must be called AFTER interp_new_epoch
//
void interp_new_srand(bvm_cache *this_bvm){ // interp_new_srand#


    #define SRAND_STRING_SIZE 32

    char srand_string[SRAND_STRING_SIZE];

#ifdef WINDOWS
    // Sleep for 1000ms to ensure we get a unique srand_string...
    Sleep(1000);
//#elsif STAR_NIX ...
#endif

    sprintf(srand_string, 
            "%d%d%d%d%d%d", 
            this_bvm->interp->utc_epoch->tm_sec,
            this_bvm->interp->utc_epoch->tm_min,
            this_bvm->interp->utc_epoch->tm_hour,
            this_bvm->interp->utc_epoch->tm_mday,
            this_bvm->interp->utc_epoch->tm_mon,
            this_bvm->interp->utc_epoch->tm_year);

    mword *babel_srand_string = _c2b(this_bvm, srand_string, SRAND_STRING_SIZE);

    mword *hash_init = _newlfi(this_bvm, HASH_SIZE, 0);

    mword *time_hash = _hash8(this_bvm, babel_srand_string);

//    mword *time_hash = _pearson16(
//                            this_bvm,  
//                            hash_init, 
//                            babel_srand_string,
//                            _arlen8(this_bvm, babel_srand_string) );

    init_by_array( time_hash, HASH_SIZE*(sizeof(mword)/sizeof(unsigned long)));

    this_bvm->interp->srand = time_hash;

    return time_hash;

}


//
//
void interp_init_symbolic_constants(bvm_cache *this_bvm){ //interp_init_symbolic_constants#

    BABEL_SYM_CODE_PTR      = _hash8(this_bvm, C2B("code_ptr"));
    BABEL_SYM_RSTACK_PTR    = _hash8(this_bvm, C2B("rstack_ptr"));
    BABEL_SYM_DSTACK_PTR    = _hash8(this_bvm, C2B("dstack_ptr"));
    BABEL_SYM_USTACK_PTR    = _hash8(this_bvm, C2B("ustack_ptr"));
    BABEL_SYM_SOFT_ROOT     = _hash8(this_bvm, C2B("soft_root"));
    BABEL_SYM_LOCAL_ROOT    = _hash8(this_bvm, C2B("local_root"));
    BABEL_SYM_ADVANCE_TYPE  = _hash8(this_bvm, C2B("advance_type"));
    BABEL_SYM_STEPS         = _hash8(this_bvm, C2B("steps"));
    BABEL_SYM_THREAD_ID     = _hash8(this_bvm, C2B("thread_id"));
    BABEL_SYM_JUMP_TABLE    = _hash8(this_bvm, C2B("jump_table"));
    BABEL_SYM_PARENT_BVM    = _hash8(this_bvm, C2B("parent_bvm"));

}


//
//
bvm_cache *interp_new_stdin_capture(bvm_cache *this_bvm){ // interp_new_stdin_capture#

// Check if user gave filename "-", indicating that input is coming on
// STDIN.

//    int c = fgetc(stdin);
//    printf("%c\n",c);
//    die;

    //TODO
    //- Check stdin
    //    - if non-empty:
    //        read and place lines on stack
    //        unless user uses a switch to say "don't do this"
    //    - If empty, check command-line
    //        perform CLI auto-parse:
    //            TOS contains a list of all default arguments - if no default
    //            arguments, nil
    //            TOS-1 contains a hash with each CLI parameter and the 
    //            argument following it, if any. Can handle --switch,
    //            -switch=1, etc. Can't handle flags but those can be 
    //            decoded by hand.
    //        unless user uses a switch to say "don't do this"

}


//
//
bvm_cache *interp_capture_argv(bvm_cache *this_bvm){ // interp_capture_argv#

    // 1. babel
    // 2. switches
    // 3. - OR .bbl file name

    char **argv = this_bvm->interp->argv;
    int    argc = this_bvm->interp->argc;

    mword *temp_argv;

    //initialize argv
    //XXX This will change when we add CLI processing:
    #define NUM_BABEL_INTERP_ARGS 1 

    if(argc > NUM_BABEL_INTERP_ARGS){

        temp_argv = consa(
                        this_bvm, 
                        _c2b(this_bvm, argv[NUM_BABEL_INTERP_ARGS], 100), nil);

        int i;
        for( i = NUM_BABEL_INTERP_ARGS+1; i < argc; i++ ){

            temp_argv = _unshift(this_bvm, temp_argv, _c2b(this_bvm, argv[i], 100));

        }

    }

    this_bvm->interp->interp_argv = temp_argv;

    return this_bvm;

}


//
//
bvm_cache *interp_reset(bvm_cache *this_bvm){ // interp_reset#

    //Free the memory buffers
    mc_destroy(this_bvm);

    longjmp(*(this_bvm->interp->cat_ex),INTERP_RESET);

}


//
//
void get_operands(bvm_cache *this_bvm, operand_info **oinfo, mword num_operands){

//    d(oinfo[0]->mask);
//    die;

    //FIXME: This is a perf-killer:
    mword stack_depth = _len(this_bvm, (mword*)icar(this_bvm->dstack_ptr));

    int i;

    for(i=0;i<num_operands;i++){

        if(i < stack_depth){

            mword *stack_operand = dstack_get(this_bvm,0);
            popd(this_bvm);

            if(     ((oinfo[i]->mask & OI_MASK_LEAF) && is_leaf(stack_operand))
                 || ((oinfo[i]->mask & OI_MASK_INTE) && is_inte(stack_operand))){

                mword operand_size = size(stack_operand);

                if(oinfo[i]->min_size){
                    if(operand_size < oinfo[i]->min_size){
                        fatal("bvm_fault");
                    }
                }

                if(oinfo[i]->max_size){
                    if(operand_size > oinfo[i]->max_size){
                        fatal("bvm_fault");
                    }
                }

                oinfo[i]->data = stack_operand;

            }
            else if((oinfo[i]->mask & OI_MASK_TPTR) && is_tptr(stack_operand)){
                oinfo[i]->data = stack_operand;
            }
            else{
                fatal("bvm_fault");
            }
    
        }
        else{ // i >= stack_depth
die;
            if(oinfo[i]->mask & OI_MASK_NONE){
                oinfo[i]->data = oinfo[i]->default;                
            }
            else{
                fatal("bvm_fault");
            }

        }

    }

}

// Clayton Bauman 2013

