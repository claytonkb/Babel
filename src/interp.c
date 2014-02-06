// interp.c
//

#include "babel.h"
#include "interp.h"
#include "io.h"
#include "bvm.h"
#include "pearson16.h"
//#include "debug.h"
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
//#include "construct.sp.h"

//
//
void init_nil(bvm_cache *this_bvm){ // init_nil#

//    nil = new_tptr(this_bvm, BABEL_TAG_INTERP_NIL, NULL);
//    set_tptr(nil,nil);

    mword *temp = malloc( MWORDS(TPTR_SIZE) ); // XXX WAIVER XXX
    temp[0]=0;
    nil=temp+1;

    int i;
    for(i=0; i<HASH_SIZE; i++){
        nil[i] = BABEL_TAG_INTERP_NIL[i];
    }

    nil[HASH_SIZE  ] = -1*MWORD_SIZE;
    set_tptr(nil,nil);

}

//
//
void flag_init(bvm_cache *this_bvm){ // flag_init#

    bvm_flags *f = malloc(sizeof(bvm_flags)); // XXX WAIVER XXX

    f->MC_ALLOC_BLOCKING     = FLAG_CLR;
    f->MC_GC_BLOCKING        = FLAG_CLR;
    f->MC_GC_PENDING         = FLAG_CLR;
    f->BVM_INSTR_IN_PROGRESS = FLAG_CLR;
    f->BVM_INCOHERENT        = FLAG_IGN;
    f->BVM_CACHE_VALID       = FLAG_IGN;
    f->BVM_CACHE_DIRTY       = FLAG_IGN;
    f->BVM_CLEAN             = FLAG_IGN;
    f->NO_ASYNC              = FLAG_IGN;
    f->NO_EXCEPT             = FLAG_IGN;

    this_bvm->flags = f;

}

//
//
void capture_env(bvm_cache *this_bvm, char **envp){ // capture_env#

    char** env;
    mword *env_hash = new_hash_table(this_bvm);

    char *split_point;

    mword *env_var_name;
    mword *env_var_val;

    for(env = envp; *env != NULL; env++){

        split_point = strchr(*env,'=');

        env_var_val = _c2b(this_bvm, split_point+1,OVERRUN_LIMIT);
        *split_point = 0;
        env_var_name = _c2b(this_bvm, *env,OVERRUN_LIMIT);

       // hash_insert(env_hash, env_var_name, env_var_val);
        _insha(this_bvm,  env_hash,                         
                nil,                                
                env_var_name,                           
                new_hash_table_entry(this_bvm,   nil,         
                                        env_var_name,   
                                        env_var_val ) );

    }

//    mword *path = _luha(this_bvm,  get_tptr(env_hash), _hash8(this_bvm, C2B("PATH")) );

//mword *_luha(this_bvm, mword *hash_table, mword *hash){ // _luha#
//    _dump(path);

//    printf("%s\n", (char*)path);
//    die;

//    return env_hash;
    set_sym( this_bvm, "env", env_hash );

//    _dump(env_hash);
//    die;

}


//
//
void gen_time_string(bvm_cache *this_bvm){

    #define MAX_TIME_STRING_LENGTH 30

    time_t rawtime;
    char time_string[MAX_TIME_STRING_LENGTH];
    time( &rawtime );    
    strcpy( time_string, ctime(&rawtime) );
    //return _c2b(this_bvm, time_string, MAX_TIME_STRING_LENGTH);

    set_sym( this_bvm, "epoch", _c2b(this_bvm, time_string, MAX_TIME_STRING_LENGTH) );

}


//
//
void init_srand(bvm_cache *this_bvm){

    // FIXME: strlen... get rid
    // This needs to be enhanced to look for existing srand...
    //mword *time_hash;// = new_hash(this_bvm);
    mword *hash_init = _newlfi(this_bvm, HASH_SIZE, 0); //new_hash(this_bvm);
    mword *epoch = get_sym( this_bvm, "epoch" );
    mword *time_hash = _pearson16(this_bvm,  hash_init, epoch, _arlen8(this_bvm, epoch) );
    init_by_array( time_hash, HASH_SIZE*(sizeof(mword)/sizeof(unsigned long)));

    set_sym( this_bvm, "srand", time_hash );

}


//
//
void init_argv(bvm_cache *this_bvm, int argc, char **argv){

    mword *temp_argv;

    //initialize argv
    //XXX This will change when we add CLI processing:
    #define NUM_BABEL_INTERP_ARGS 1 
    if(argc > NUM_BABEL_INTERP_ARGS){

        temp_argv = _newin(this_bvm, argc-NUM_BABEL_INTERP_ARGS);

        int i;
        for( i = NUM_BABEL_INTERP_ARGS; i < argc; i++ ){
            (mword*)c((mword*)temp_argv, i-NUM_BABEL_INTERP_ARGS)
                = _c2b(this_bvm, argv[i], 100);
        }

    }

//    this_bvm->argv = nil;
    set_sym( this_bvm, "argv", temp_argv );

}

void init_symbolic_constants(bvm_cache *this_bvm){

    BABEL_SYM_STEPS         = _hash8(this_bvm, C2B("steps"));
    BABEL_SYM_THREAD_ID     = _hash8(this_bvm, C2B("thread_id"));
    BABEL_SYM_ADVANCE_TYPE  = _hash8(this_bvm, C2B("advance_type"));
    BABEL_SYM_SOFT_ROOT     = _hash8(this_bvm, C2B("soft_root"));
    BABEL_SYM_JUMP_TABLE    = _hash8(this_bvm, C2B("jump_table"));

}

//  Initializes the root Babel Virtual Machine (BVM)
//  and interpreter-only state...
bvm_cache *interp_init(bvm_cache *this_bvm, int argc, char **argv, char **envp){ // interp_init#

    mc_init(this_bvm);
    flag_init(this_bvm);
    init_nil(this_bvm);    //initialize nil (global constant)

//    mword *load_bbl = malloc(MWORDS(BBL_SIZE));
//    memcpy(load_bbl, bbl, MWORDS(BBL_SIZE));

    mword *load_bbl = mc_alloc(this_bvm, MWORDS(BBL_SIZE));
    memcpy(load_bbl, bbl, MWORDS(BBL_SIZE));

//    int i;
//    for(i=0;i<BBL_SIZE;i++){
//        printf("%08x\n", c(bbl,i));
//        //printf("%08x\n", c((this_bvm->self-1),i));
//    }
//
//    die;

    this_bvm->self = _load(this_bvm, (mword*)load_bbl,BBL_SIZE);
//    die;
//
//    int i;
//    for(i=0;i<BBL_SIZE;i++){
//        printf("%08x\n", c(bbl,i));
//        //printf("%08x\n", c((this_bvm->self-1),i));
//    }
//
//    die;

    this_bvm->sym_table = (mword*)bvm_sym_table(this_bvm->self);

    capture_env(this_bvm, envp); // FIXME: DIES HERE 1/28/14

    gen_time_string(this_bvm);

    empty_string = _newlfi(this_bvm, 1,0);

    init_srand(this_bvm);

    init_argv(this_bvm, argc, argv);

    init_symbolic_constants(this_bvm);

    mword *jump_table    = init_interp_jump_table(this_bvm);

    //this_bvm->mem = mc_init(this_bvm);

//    mword *z = this_bvm->mem->primary->base_ptr;
//    z[(MEM_SIZE>>1)-1] = 0xbabeface;
//    d(z[(MEM_SIZE>>1)-1]);

//    mword *z = this_bvm->mem->secondary->alloc_ptr;
//    *z = 0xbabeface;
//    d(*z);

    set_sym(this_bvm, "steps",          _newva( this_bvm, (mword)-1) );
    set_sym(this_bvm, "thread_id",      _newva( this_bvm, 0) );
    set_sym(this_bvm, "advance_type",   _newva( this_bvm, (mword)BVM_ADVANCE) );
    set_sym(this_bvm, "soft_root",      nil );
    set_sym(this_bvm, "jump_table",     jump_table );

    update_bvm_cache(this_bvm);

//    this_bvm->code_ptr      = (mword*)bvm_code_ptr(this_bvm->self);
//    this_bvm->rstack_ptr    = (mword*)bvm_rstack_ptr(this_bvm->self);
//    this_bvm->dstack_ptr    = (mword*)bvm_dstack_ptr(this_bvm->self);
//    this_bvm->ustack_ptr    = (mword*)bvm_ustack_ptr(this_bvm->self);
//    this_bvm->sym_table     = (mword*)bvm_sym_table(this_bvm->self);
//    this_bvm->jump_table    = get_sym(this_bvm, "jump_table");
//    this_bvm->steps         = get_sym(this_bvm, "steps");
//    this_bvm->advance_type  = get_sym(this_bvm, "advance_type");
//    this_bvm->thread_id     = get_sym(this_bvm, "thread_id");

    return this_bvm;

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

    //TODO: Configure root BVM resource limits

}


//
mword *init_interp_jump_table(bvm_cache *this_bvm){

    #include "fixed_opcodes.h"

//    printf("%d\n",sizeof(interp_fixed_opcodes)/sizeof(babel_op));
//    die

    #define num_opcodes (sizeof(interp_fixed_opcodes)/sizeof(babel_op))

    mword *temp = malloc(MWORDS(num_opcodes+1)); //XXX WAIVER XXX
    temp[0] = num_opcodes * MWORD_SIZE;
    temp++;

    int i;
    for(i=0;i<num_opcodes;i++){
        (mword*)temp[i] = (mword*)interp_fixed_opcodes[i];
    }

    return temp;

//    this_bvm->jump_table = _newlf(this_bvm, num_opcodes);
//
//    int i;
//    for(i=0;i<num_opcodes;i++){
//        (mword*)this_bvm->jump_table[i] = (mword*)interp_fixed_opcodes[i];
//    }

}


//
// babel_operator
bvm_cache *endian(bvm_cache *this_bvm){

    fatal("stack fix not done");
    mword test = 1;
    char *test_addr = (char *)&test;
    mword *result = new_atom;

    if(*test_addr){
        *result = LITTLE_ENDIAN;
    }
    else{
        *result = BIG_ENDIAN;
    }

    push_alloc(this_bvm, result, IMMORTAL);

    return this_bvm;

}

// Clayton Bauman 2013

