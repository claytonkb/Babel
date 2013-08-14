// interp.c
//

#include "babel.h"
#include "interp.h"
#include "io.h"
#include "bvm.h"
#include "pearson16.h"
#include "debug.h"
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

//
//
void init_nil(void){ // init_nil#

    nil = new_tptr(BABEL_TAG_INTERP_NIL, NULL);
    set_tptr(nil,nil);

}

//
//
void capture_env(bvm_cache *this_bvm, char **envp){ // capture_env#

    char** env;
    mword *env_hash = new_hash_table();

    char *split_point;

    mword *env_var_name;
    mword *env_var_val;

    for(env = envp; *env != NULL; env++){

        //printf("%s\n", *env);
        split_point = strchr(*env,'=');
        env_var_val = _c2b(split_point+1,OVERRUN_LIMIT);
        *split_point = 0;
        env_var_name = _c2b(*env,OVERRUN_LIMIT);

       // hash_insert(env_hash, env_var_name, env_var_val);
        _insha( env_hash,                         
                nil,                                
                env_var_name,                           
                new_hash_table_entry(  nil,         
                                        env_var_name,   
                                        env_var_val ) );

    }

//    mword *path = _luha( get_tptr(env_hash), _hash8(C2B("PATH")) );

//mword *_luha(mword *hash_table, mword *hash){ // _luha#
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
    //return _c2b(time_string, MAX_TIME_STRING_LENGTH);

    set_sym( this_bvm, "epoch", _c2b(time_string, MAX_TIME_STRING_LENGTH) );

}


//
//
void init_srand(bvm_cache *this_bvm){

    // FIXME: strlen... get rid
    // This needs to be enhanced to look for existing srand...
    //mword *time_hash;// = new_hash();
    mword *hash_init = new_hash();
    mword *epoch = get_sym( this_bvm, "epoch" );
    mword *time_hash = _pearson16( hash_init, epoch, _arlen8(epoch) );
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

        temp_argv = _newin(argc-NUM_BABEL_INTERP_ARGS);

        int i;
        for( i = NUM_BABEL_INTERP_ARGS; i < argc; i++ ){
            (mword*)c((mword*)temp_argv, i-NUM_BABEL_INTERP_ARGS)
                = _c2b(argv[i], 100);
        }

    }

//    this_bvm->argv = nil;
    set_sym( this_bvm, "argv", temp_argv );

}


//  Initializes the root Babel Virtual Machine (BVM)
//
bvm_cache *interp_init(bvm_cache *this_bvm, int argc, char **argv, char **envp){ // interp_init#

    #include "construct.sp.c"

    init_nil();    //initialize nil (global constant)

    //load the root bvm
    this_bvm->self = _load((mword*)bbl,BBL_SIZE);
    this_bvm->sym_table = (mword*)bvm_sym_table(this_bvm->self);

    capture_env(this_bvm, envp);

    gen_time_string(this_bvm);

    empty_string = _newlfi(1,0);

    init_srand(this_bvm);

    init_argv(this_bvm, argc, argv);

    mword *jump_table    = init_interp_jump_table();

    set_sym(this_bvm, "steps",          _newva((mword)-1) );
    set_sym(this_bvm, "thread_id",      _newva(0) );
    set_sym(this_bvm, "advance_type",   _newva((mword)BVM_ADVANCE) );
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
mword *init_interp_jump_table(void){

    #include "fixed_opcodes.h"

//    printf("%d\n",sizeof(interp_fixed_opcodes)/sizeof(babel_op));
//    die

    #define num_opcodes (sizeof(interp_fixed_opcodes)/sizeof(babel_op))

    mword *temp = _newlf(num_opcodes);

    int i;
    for(i=0;i<num_opcodes;i++){
        (mword*)temp[i] = (mword*)interp_fixed_opcodes[i];
    }

    return temp;

//    this_bvm->jump_table = _newlf(num_opcodes);
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

