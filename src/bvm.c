// bvm.c
//

#include "babel.h"
#include "bvm.h"
#include "load.h"
#include "stack.h"
//#include "bvm_opcodes.h"
//#include "bvm_interp.h"
#include "arith.h"
#include "io.h"
#include "array.h"
#include "eval.h"
#include "list.h"
//#include "debug.h"
#include "pearson16.h"
#include "except.h"
#include "cmp.h"
#include "string.h"
#include "shift.h"
#include "logic.h"
#include "util.h"
#include <time.h>
#include "mt19937ar.h"
#include "hash.h"
#include "bstruct.h"
#include "alloc.h"
#include "tptr.h"
#include "ref.h"
#include <windows.h>

/* bvm operator
**code**  
**stack**  
**rstack**  
**bvmstep**  
**mkbvm**  
**bvmkill**  
**halt**  
**fork**  
*/


/*
**shell**  
> equivalent to Perl's back-tick quotes  
*/


/* bvm operator
 **exec**  
> does a fork/exec in Linux; CreateProcess in Windows
> equivalent to Perl's system()
*/
bvm_cache *exec(bvm_cache *this_bvm){ // exec#

#ifdef WINDOWS

    char *process_string = (char*)_b2c(dstack_get(this_bvm,0));
    popd(this_bvm);

    mword *result;

    STARTUPINFO si;
    PROCESS_INFORMATION pi;

    ZeroMemory( &si, sizeof(si) );
    si.cb = sizeof(si);
    ZeroMemory( &pi, sizeof(pi) );

    if( !CreateProcess(NULL,//process_string,    // No module name (use command line)
        process_string, //argv[1],        // Command line
        NULL,           // Process handle not inheritable
        NULL,           // Thread handle not inheritable
        FALSE,          // Set handle inheritance to FALSE
        0,              // No creation flags
        NULL,           // Use parent's environment block
        NULL,           // Use parent's starting directory 
        &si,            // Pointer to STARTUPINFO structure
        &pi )           // Pointer to PROCESS_INFORMATION structure
        )
    {
        result = _newva(0);
    }

    result = _newva(1);
   
    pushd(this_bvm, result, IMMORTAL); 

#endif

    return this_bvm;

}


//
//
bvm_cache *bvm_interp(bvm_cache *this_bvm){ // bvm_interp#

    bvm_cache *discard;
    babel_op op_ptr;

#ifdef BVM_TRACE
    trace;
#endif

    while( icar(this_bvm->steps) ){//FIXME: This is not correct long-term   

#ifdef BVM_TRACE
    trace;
#endif

        if(code_empty(this_bvm)){
            if(!rstack_empty(this_bvm)){

#ifdef BVM_TRACE
    trace;
#endif        

                _next(this_bvm);
                continue;
            }

#ifdef BVM_TRACE
    trace;
#endif

            break;
        }

        mword *next_entry = (mword*)icar(icar(this_bvm->code_ptr));

        if( is_inte(next_entry) ){

#ifdef BVM_TRACE
    trace;
    _mem((mword*)icar(next_entry));
#endif

            pushd( this_bvm, (mword*)icar(next_entry), IMMORTAL );
        }
        else if( is_leaf(next_entry) ){

            mword opcode = c(next_entry,0);

#ifdef BVM_TRACE
    trace;
    d(opcode);
#endif

            op_ptr = (babel_op)this_bvm->jump_table[ opcode % NUM_INTERP_OPCODES ];
            discard = op_ptr(this_bvm);
        }
        else{ // is_tptr(next_entry)
            fatal("hash-reference detected in code");
        }

        if(icar(this_bvm->advance_type) == BVM_ADVANCE){

#ifdef BVM_TRACE
    trace;
#endif

            icar(this_bvm->code_ptr) = cdr(car(this_bvm->code_ptr));
        }
        else if(icar(this_bvm->advance_type) == BVM_RETURN){
            //die;
            break;
        }
        else{

#ifdef BVM_TRACE
    trace;
#endif

            icar(this_bvm->advance_type) = BVM_ADVANCE;
        }

        icar(this_bvm->steps)--;
        
    }

#ifdef BVM_TRACE
    trace;
#endif

    flush_bvm_cache(this_bvm);

    return this_bvm;

}


mword *_bvm_init(mword *bvm_to_load){ // _bvm_init#

//    mword *bvm_to_load = dstack_get(this_bvm,0);   
//    popd(this_bvm);


    mword *list = get_tptr(bvm_to_load);

    mword load_length = _len(list);

    int i;
    mword *loaded_bvm;

    if(tageq(bvm_to_load,BABEL_TAG_SYM_BVM,TAG_SIZE)){


        mword *base_hash_table  = new_hash_table();
        mword *sym_table        = new_hash_table();


        _insha( base_hash_table,      
                _hash8(C2B("/babel/tag/sym_table")),
                nil,                           
                new_hash_table_entry(  _hash8(C2B("/babel/tag/sym_table")),
                                       nil,   
                                       sym_table ) );


        for(i=0; i<load_length; i++){


            mword *payload = (mword*)icar(list);
            mword *hash = _ith(payload,0);
            mword *value = _ith(payload,1);

            if(
                tageq(   hash,  BABEL_TAG_BVM_CODE,         TAG_SIZE)
                || tageq(hash,  BABEL_TAG_BVM_STACK,        TAG_SIZE)
                || tageq(hash,  BABEL_TAG_BVM_RSTACK,       TAG_SIZE)
                || tageq(hash,  BABEL_TAG_BVM_JUMP_TABLE,   TAG_SIZE)){
            //insert into base hash table:

                _insha( base_hash_table,                         
                    hash,                                
                    nil,                           
                    new_hash_table_entry(   hash,         
                                            nil,   
                                            hash ) );
            }
            else{ //insert into sym_table

                _insha( sym_table,
                    hash,
                    nil,
                    new_hash_table_entry(   hash,
                                            nil,
                                            hash ) );
            }


            list = (mword*)cdr(list);

        }


        loaded_bvm = new_tptr(_hash8(C2B("/babel/tag/bvm")), base_hash_table);


        _insha( base_hash_table,
                _hash8(C2B("/babel/tag/self")),
                nil,                           
                new_hash_table_entry(  _hash8(C2B("/babel/tag/self")),
                                       nil,   
                                       loaded_bvm ) );


    }
    else{

        fatal("Unrecognized bvm tag");
    }

//    pushd(this_bvm, loaded_bvm, IMMORTAL);
//
//    return this_bvm;

    return loaded_bvm;

}



// This is a temporary operator to enable the REPL
//
bvm_cache *bvm_step(bvm_cache *this_bvm){ // bvm_step#

    //Check for existence of sym_table
    //if !exists -> initialize BVM
    //else if exists -> don't reinitialize, already done

    bvm_cache new_bvm_cache;
    bvm_cache *new_bvm_ptr = &new_bvm_cache;

    mword *op_bvm = dstack_get(this_bvm,0);
    popd(this_bvm);

    new_bvm_ptr->self      = op_bvm;
    new_bvm_ptr->sym_table = (mword*)bvm_sym_table(new_bvm_cache.self);

    mword *op_bvm_sym_table = (mword*)bvm_sym_table(op_bvm);
           op_bvm_sym_table = hard_detag(op_bvm_sym_table);

    if(is_nil(car(op_bvm_sym_table)) && is_nil(cdr(op_bvm_sym_table))){ // sym_table is uninitialized

        if(tageq(new_bvm_cache.self,BABEL_TAG_SPARSE_BVM,TAG_SIZE)){

            //FIXME: Assumes we want to clone:
            set_sym(new_bvm_ptr, "env",    (mword*)get_sym(this_bvm, "env")   );
            set_sym(new_bvm_ptr, "epoch",  (mword*)get_sym(this_bvm, "epoch") );
            set_sym(new_bvm_ptr, "argv",   (mword*)get_sym(this_bvm, "argv")  );
            set_sym(new_bvm_ptr, "env",    (mword*)get_sym(this_bvm, "env")   );

            set_sym(new_bvm_ptr, "thread_id",      _newva( icar( get_sym(this_bvm, "thread_id") ) + 1 ) );
            set_sym(new_bvm_ptr, "advance_type",   _newva((mword)BVM_ADVANCE) );
            set_sym(new_bvm_ptr, "soft_root",      nil );
            set_sym(new_bvm_ptr, "jump_table",     get_sym(this_bvm, "jump_table") );

        }

    }

    set_sym( new_bvm_ptr, "steps", _newva((mword)1) );

    flush_bvm_cache(this_bvm);
    update_bvm_cache(new_bvm_ptr);

    bvm_interp(new_bvm_ptr);

    flush_bvm_cache(new_bvm_ptr);
    update_bvm_cache(this_bvm); // Technically, this is not necessary
                                // but it doesn't hurt

    //FIXME - push stack of new_bvm_cache onto this_bvm when bvm_interp returns
    //push_alloc(this_bvm, new_bvm_cache.stack_ptr, BVMEXEC);

    return this_bvm;

}



/* bvm operator
**babel**
> This operator "execs" a loaded BVM on TOS
*/
bvm_cache *babelop(bvm_cache *this_bvm){ // babelop# babel#

    bvm_cache new_bvm;
    bvm_cache *new_bvm_ptr = &new_bvm;

    new_bvm.self = dstack_get(this_bvm,0);
    popd(this_bvm);

    new_bvm.sym_table = (mword*)bvm_sym_table(new_bvm.self);

    if(tageq(new_bvm.self,BABEL_TAG_SPARSE_BVM,TAG_SIZE)){
        //FIXME: Assumes we want to clone:
        set_sym(new_bvm_ptr, "env",    (mword*)get_sym(this_bvm, "env")   );
        set_sym(new_bvm_ptr, "epoch",  (mword*)get_sym(this_bvm, "epoch") );
        set_sym(new_bvm_ptr, "argv",   (mword*)get_sym(this_bvm, "argv")  );
        set_sym(new_bvm_ptr, "env",    (mword*)get_sym(this_bvm, "env")   );

        set_sym(new_bvm_ptr, "steps",          _newva((mword)-1) );
        set_sym(new_bvm_ptr, "thread_id",      _newva( icar( get_sym(this_bvm, "thread_id") ) + 1 ) );
        set_sym(new_bvm_ptr, "advance_type",   _newva((mword)BVM_ADVANCE) );
        set_sym(new_bvm_ptr, "soft_root",      nil );
        set_sym(new_bvm_ptr, "jump_table",     get_sym(this_bvm, "jump_table") );
    }

    flush_bvm_cache(this_bvm);
    update_bvm_cache(&new_bvm);

    bvm_interp(&new_bvm);

    flush_bvm_cache(&new_bvm);
    update_bvm_cache(this_bvm); // Technically, this is not necessary
                                // but it doesn't hurt

    //FIXME - push stack of new_bvm onto this_bvm when bvm_interp returns
    //push_alloc(this_bvm, new_bvm.stack_ptr, BVMEXEC);

    return this_bvm;

}


/* bvm operator
**hibernate**
Saves the current BVM off to disk. Takes one argument
to specify the filename to save to.
*/
bvm_cache *hibernate(bvm_cache *this_bvm){ // hibernate#

    mword *filename = dstack_get(this_bvm,0);
    popd(this_bvm);

    icar(this_bvm->code_ptr) = cdr(car(this_bvm->code_ptr));
    icar(this_bvm->advance_type) = BVM_ADVANCE;
    icar(this_bvm->steps)--;
    
    flush_bvm_cache(this_bvm);

    mword *bvm_out = get_tptr(this_bvm->self);

    mword *temp_bvm_out = new_tptr(_hash8(C2B("/babel/tag/hiber_bvm")), bvm_out);

    bvm_out = _unload(temp_bvm_out);

    _spit_mword((char*)_b2c(filename), bvm_out);

    icar(this_bvm->advance_type) = BVM_RETURN;    

    return this_bvm;

}



//
// babel_operator
bvm_cache *bvmcode(bvm_cache *this_bvm){

   pushd(this_bvm, this_bvm->code_ptr, IMMORTAL); 
    // XXX: These operators are inherently dangerous in 
    // combination with the mortal operator

    return this_bvm;

}

//
// babel_operator
bvm_cache *bvmstack(bvm_cache *this_bvm){

    pushd(this_bvm, this_bvm->dstack_ptr, IMMORTAL);

    return this_bvm;

}

//
// babel_operator
bvm_cache *bvmustack(bvm_cache *this_bvm){

    pushd(this_bvm, this_bvm->ustack_ptr, IMMORTAL);

    return this_bvm;

}

//
// babel_operator
bvm_cache *bvmrstack(bvm_cache *this_bvm){

    pushd(this_bvm, this_bvm->rstack_ptr, IMMORTAL);

    return this_bvm;

}

// babel_operator
bvm_cache *bvmsym(bvm_cache *this_bvm){

    pushd(this_bvm, this_bvm->sym_table, IMMORTAL);

    return this_bvm;

}


//
// babel_operator
bvm_cache *self(bvm_cache *this_bvm){

    flush_bvm_cache(this_bvm);

//    mword *result = _bs2gv(this_bvm->self);
//    push_alloc(this_bvm, result, IMMORTAL);

    pushd(this_bvm, this_bvm->self, IMMORTAL);

    return this_bvm;

}

//
// babel_operator
bvm_cache *rsvd(bvm_cache *this_bvm){

    d(car(car(this_bvm->code_ptr)))

    error("An attempt was made to execute a reserved opcode");
    die;

}

//
// babel_operator
bvm_cache *boilerplate(bvm_cache *this_bvm){

//    printf(
//"Babel %s\n\
//    Built %s %s\n\
//    Clayton Bauman (c) 2012, see LICENSE for details\n\n", 
//    BABEL_VERSION, __DATE__, __TIME__);

    fatal("stack fix not done");
    char msg[256];

    sprintf(msg, 
"Babel %s\n\
    Built %s %s\n\
    Clayton Bauman (c) 2012, see LICENSE for details\n\n", 
    BABEL_VERSION, __DATE__, __TIME__);

    push_alloc( this_bvm, C2B(msg), MORTAL );

}


//
//
bvm_cache *update_bvm_cache(bvm_cache *this_bvm){ // update_bvm_cache#

    mword *self = this_bvm->self;

    this_bvm->code_ptr      = (mword*)bvm_code_ptr(self);
    this_bvm->rstack_ptr    = (mword*)bvm_rstack_ptr(self);
    this_bvm->dstack_ptr    = (mword*)bvm_dstack_ptr(self);
    this_bvm->ustack_ptr    = (mword*)bvm_ustack_ptr(self);
    this_bvm->sym_table     = (mword*)bvm_sym_table(self);
    this_bvm->jump_table    = get_sym(this_bvm, "jump_table"); //(mword*)bvm_jump_table(self);
    this_bvm->steps         = get_sym(this_bvm, "steps");
    this_bvm->advance_type  = get_sym(this_bvm, "advance_type");
    this_bvm->thread_id     = get_sym(this_bvm, "thread_id");

    return this_bvm;

}


//
//
bvm_cache *flush_bvm_cache(bvm_cache *this_bvm){ // flush_bvm_cache#

    mword *self = this_bvm->self;

    (mword*)bvm_code_ptr(self)      = this_bvm->code_ptr;
    (mword*)bvm_rstack_ptr(self)    = this_bvm->rstack_ptr;
    (mword*)bvm_dstack_ptr(self)    = this_bvm->dstack_ptr;
    (mword*)bvm_ustack_ptr(self)    = this_bvm->ustack_ptr;
    (mword*)bvm_sym_table(self)     = this_bvm->sym_table;
    set_sym(this_bvm, "jump_table",   this_bvm->jump_table);
    set_sym(this_bvm, "steps",        this_bvm->steps);
    set_sym(this_bvm, "advance_type", this_bvm->advance_type);
    set_sym(this_bvm, "thread_id",    this_bvm->thread_id);

    return this_bvm;

}

////#ifdef DEBUG
////
////void internal_bvmroot(void){
////
////    push_alloc(internal_global_VM, INTERNAL_BVMROOT);
////
////}
////
////#endif

////DEPRECATED
//void bbl2gv(mword *tree){
//
//    int i;
//
//    printf("digraph babel {\nnode [shape=record];\n");
//    printf("graph [rankdir = \"LR\"];\n");
//
//    tree_bbl2gv(tree);
//
//    printf("}\n");
//
//    rclean(tree);
//
//}
//
//
////  
////
//mword tree_bbl2gv(mword *tree){
//
//    int i;
////    mword count = 0;
//
//    if( s(tree) & (MWORD_SIZE-1) ){
//        return 0;
//    }
//
//    if( is_href(tree) ){
//        printf("s%08x [style=dashed,shape=record,label=\"", (mword)tree);
//        for(i=0; i<HASH_SIZE; i++){
//            printf("<f%d> %x", i, *(mword *)(tree+i));
//            if(i<(HASH_SIZE-1)){
//                printf("|");
//            }
//        }
//        printf("\"];\n");
//        s(tree) |= 0x1;
//        return 0;
//    }
//
//    int num_elem = size(tree);
//
////    count = num_elem + 1;
//
//    s(tree) |= 0x1;
//
//    if(is_inte(tree)){
//
//        printf("\"s%08x\" [shape=record,label=\"", (mword)tree);
//        for(i=0; i<num_elem; i++){
//            printf("<f%d> %d", i, i);
//            if(i<(num_elem-1)){
//                printf("|");
//            }
//        }
//        printf("\"];\n");
//
//        for(i=0; i<num_elem; i++){
////            if(*(mword *)(tree+i) == nil && tree != (mword*)nada){
//            if(is_nil((mword *)scar(tree+i))){// == nil && tree != (mword*)nada){
//                continue;
//            }
//            printf("\"s%08x\":f%d -> \"s%08x\":f0;\n", (mword)tree, i, *(mword *)(tree+i));
//            tree_bbl2gv((mword *)*(tree+i));
//        }
//
//    }
//    else{ // is_leaf
//        printf("s%08x [style=bold,shape=record,label=\"", (mword)tree);
//        for(i=0; i<num_elem; i++){
//            printf("<f%d> %x", i, *(mword *)(tree+i));
//            if(i<(num_elem-1)){
//                printf("|");
//            }
//        }
//        printf("\"];\n");
//    }
//
////    return count;
//
//}

// Clayton Bauman 2011

