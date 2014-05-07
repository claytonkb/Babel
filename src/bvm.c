// bvm.c
//

#include "babel.h"
#include "bvm.h"
#include "load.h"
#include "stack.h"
#include "arith.h"
#include "io.h"
#include "array.h"
#include "eval.h"
#include "list.h"
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
#include "mem.h"
#include "cache.h"

// Intended for use with babels operator
//
bvm_cache *bvm_new_sparse_bvm(bvm_cache *this_bvm){ // *bvm_new_sparse_bvm#

    this_bvm->flags->MC_GC_BLOCKING = FLAG_SET;

    mword *self = this_bvm->self;

    // sym_table
    this_bvm->sym_table = bvm_sym_table(self);

    // jump_table
    // flags
    // argv
    // envp
    // stdin

    mword *temp_ptr;
    mword  temp_val;

    temp_ptr = (mword*)bvm_code_ptr(self);
    bvm_write_cache(this_bvm, &this_bvm->code_ptr, &temp_ptr);
    set_sym(this_bvm, "code_ptr", temp_ptr );

    temp_ptr = (mword*)bvm_rstack_ptr(self);
    bvm_write_cache(this_bvm, &this_bvm->rstack_ptr, &temp_ptr);
    set_sym(this_bvm, "rstack_ptr", temp_ptr );

    temp_ptr = (mword*)bvm_dstack_ptr(self);
    bvm_write_cache(this_bvm, &this_bvm->dstack_ptr, &temp_ptr);
    set_sym(this_bvm, "dstack_ptr", temp_ptr );

    temp_ptr = (mword*)bvm_ustack_ptr(self);
    bvm_write_cache(this_bvm, &this_bvm->ustack_ptr, &temp_ptr);
    set_sym(this_bvm, "ustack_ptr", temp_ptr );

    temp_ptr = new_hash_table(this_bvm);
    bvm_write_cache(this_bvm, &this_bvm->soft_root, &temp_ptr);
    set_sym(this_bvm, "soft_root", temp_ptr );

    mword *key = C2B("argv");
    mword *hash = _hash8(this_bvm, key);

    _insha( this_bvm, 
            temp_ptr, 
            hash, 
            nil, 
            new_hash_table_entry(
                this_bvm, 
                hash, 
                key, 
                this_bvm->interp->interp_argv));

    temp_val = 0; // FIXME naked const
    bvm_write_cache(this_bvm, &this_bvm->thread_id, &temp_val);
    set_sym(this_bvm, "thread_id", _newva( this_bvm, this_bvm->thread_id ) );

    if(!_exha(
            this_bvm, 
            get_tptr(this_bvm->sym_table), 
            _hash8(this_bvm, C2B("steps")))){

        temp_val = -1; // FIXME naked const
        bvm_write_cache(this_bvm, &this_bvm->steps, &temp_val);
        set_sym(this_bvm, "steps", _newva( this_bvm, this_bvm->steps ) );

    }
    else{

        temp_val = icar(get_sym(this_bvm, "steps"));
        bvm_write_cache(this_bvm, &this_bvm->steps, &temp_val);

    }

    temp_val = BVM_ADVANCE;
    bvm_write_cache(this_bvm, &this_bvm->advance_type, &temp_val);
    set_sym(this_bvm, "advance_type", _newva( this_bvm, this_bvm->advance_type ) );

    tptr_update_hash(this_bvm, this_bvm->self, (mword*)BABEL_TAG_READY_BVM);

    this_bvm->flags->MC_GC_BLOCKING     = FLAG_CLR;

    //Force cache flush:
    this_bvm->flags->BVM_CACHE_INVALID  = FLAG_CLR;
    this_bvm->flags->BVM_CACHE_DIRTY    = FLAG_CLR;

    return this_bvm;

}


// Intended for use with babels operator
//
bvm_cache *bvm_new_sparse_bvm2(bvm_cache *this_bvm){ // *bvm_new_sparse_bvm2#

    this_bvm->flags->MC_GC_BLOCKING = FLAG_SET;

    mword *self = this_bvm->self;

//////////////

    this_bvm->flags->MC_GC_BLOCKING     = FLAG_CLR;

    //Force cache flush:
    this_bvm->flags->BVM_CACHE_INVALID  = FLAG_CLR;
    this_bvm->flags->BVM_CACHE_DIRTY    = FLAG_CLR;

    return this_bvm;
}

//
//
void bvm_new_argv(bvm_cache *this_bvm, int argc, char **argv){ // bvm_new_argv#



}


#ifndef BVM_TRACE
//
//
bvm_cache *bvm_interp(bvm_cache *this_bvm){ // bvm_interp#

    bvm_cache *discard;
    babel_op op_ptr;

//printf("%s\n", (char*)_bbl2str(this_bvm, this_bvm->self));
//_dump(this_bvm->self);
//die;

#ifdef BVM_OP_TRACE
printf("babel begin\n");
#endif

    while( this_bvm->steps ){

#ifdef BVM_OP_TRACE_STEP
int c = fgetc(stdin);
#endif

        this_bvm->flags->BVM_INSTR_IN_PROGRESS = FLAG_SET;

        if(code_empty(this_bvm)){
            if(!rstack_empty(this_bvm)){
                _next(this_bvm);
                continue;
            }
            break;
        }

        mword *next_entry = (mword*)icar(icar(this_bvm->code_ptr));

        if( is_inte(next_entry) ){

#ifdef BVM_OP_TRACE
#ifdef BVM_OP_TRACE_BBL2STR
printf("push: %s\n", (char*)_bbl2str(this_bvm, (mword*)icar(next_entry)));
#else
printf("push:\n");
_mema((mword*)icar(next_entry));    
#endif
#endif

            pushd( this_bvm, (mword*)icar(next_entry), IMMORTAL );

        }
        else if( is_leaf(next_entry) ){
            mword opcode = c(next_entry,0);

#ifdef BVM_OP_TRACE
d(opcode);
#endif

            op_ptr = (babel_op)this_bvm->interp->jump_table[ opcode % NUM_INTERP_OPCODES ];
            discard = op_ptr(this_bvm);
        }
        else{ // is_tptr(next_entry)
            if(is_nil(next_entry)){
                //die;
                pushd( this_bvm, nil, IMMORTAL );
            }
            else{
                fatal("tptr detected in code_list");
            }
        }
        this_bvm->flags->BVM_INSTR_IN_PROGRESS = FLAG_CLR;
        if(this_bvm->flags->MC_GC_PENDING == FLAG_SET){
            //die;
            mc_copy_collect(this_bvm);
        }

        if(this_bvm->advance_type == BVM_ADVANCE){
            icar(this_bvm->code_ptr) = cdr(car(this_bvm->code_ptr));
        }
        else if(this_bvm->advance_type == BVM_RETURN){
            //die;
            break;
        }
        else{
            //icar(this_bvm->advance_type) = BVM_ADVANCE;
#ifdef BVM_OP_TRACE
printf("BVM_CONTINUE\n");
#endif
            set_bvm_advance_type(this_bvm, BVM_ADVANCE);
        }

        dec_bvm_steps(this_bvm);
        this_bvm->interp->global_tick_count++;

    }

    bvm_flush_cache(this_bvm);
//    bvm_sym_flush_cache(this_bvm);

#ifdef BVM_OP_TRACE
printf("babel end\n");
#endif

    return this_bvm;

}


#else

//
//
bvm_cache *bvm_interp(bvm_cache *this_bvm){ // bvm_interp#

    bvm_cache *discard;
    babel_op op_ptr;

printf("opcode begin\n");

    while( this_bvm->steps ){

trace;
        this_bvm->flags->BVM_INSTR_IN_PROGRESS = FLAG_SET;

        if(code_empty(this_bvm)){
trace;
            if(!rstack_empty(this_bvm)){
trace;
                _next(this_bvm);
                continue;
            }
            break;
        }

trace;

        mword *next_entry = (mword*)icar(icar(this_bvm->code_ptr));

        if( is_inte(next_entry) ){

printf("opcode push\n");
_mem((mword*)icar(next_entry));    

            pushd( this_bvm, (mword*)icar(next_entry), IMMORTAL );

        }
        else if( is_leaf(next_entry) ){
            mword opcode = c(next_entry,0);

d(opcode);

            op_ptr = (babel_op)this_bvm->interp->jump_table[ opcode % NUM_INTERP_OPCODES ];
            discard = op_ptr(this_bvm);
        }
        else{ // is_tptr(next_entry)            
            fatal("tptr detected in code_list");
        }

        this_bvm->flags->BVM_INSTR_IN_PROGRESS = FLAG_CLR;
trace;

        if(this_bvm->flags->MC_GC_PENDING == FLAG_SET){
trace;
            mc_copy_collect(this_bvm);
        }

        if(this_bvm->advance_type == BVM_ADVANCE){
trace;
            icar(this_bvm->code_ptr) = cdr(car(this_bvm->code_ptr));
        }
        else if(this_bvm->advance_type == BVM_RETURN){
trace;
            //die;
            break;
        }
        else{
trace;
            //icar(this_bvm->advance_type) = BVM_ADVANCE;
            set_bvm_advance_type(this_bvm, BVM_ADVANCE);
        }

trace;
        dec_bvm_steps(this_bvm);
        this_bvm->interp->global_tick_count++;

    }

    bvm_flush_cache(this_bvm);

printf("opcode end\n");

    return this_bvm;

}

#endif



//
//
void dec_bvm_steps(bvm_cache *this_bvm){ // dec_bvm_steps#

    if(this_bvm->steps != -1){
        this_bvm->steps--;
        this_bvm->flags->BVM_CACHE_DIRTY = FLAG_SET;
    }

}


//
//
void set_bvm_advance_type(bvm_cache *this_bvm, mword advance_type){ // set_bvm_advance_type#

    this_bvm->advance_type = advance_type;
    this_bvm->flags->BVM_CACHE_DIRTY = FLAG_SET;

}


//
//
bvm_cache *bvm_update_cache(bvm_cache *this_bvm){ // bvm_update_cache#

//trace;
//_mem(this_bvm->self);

    if(    this_bvm->flags->BVM_CACHE_DIRTY == FLAG_SET
        || this_bvm->flags->BVM_CACHE_BLOCKING == FLAG_SET ){
        cat_except(this_bvm);
    }
    if(this_bvm->flags->BVM_CACHE_ONLY == FLAG_SET){
        return;
    }

    this_bvm->flags->BVM_CACHE_BLOCKING = FLAG_SET;
    this_bvm->flags->MC_ALLOC_BLOCKING = FLAG_SET;

    mword *self = this_bvm->self;

    mword *temp;

    temp = (mword*)bvm_code_ptr(self);
    bvm_write_cache(this_bvm, &this_bvm->code_ptr, &temp);

    temp = (mword*)bvm_rstack_ptr(self);
    bvm_write_cache(this_bvm, &this_bvm->rstack_ptr, &temp);

    temp = (mword*)bvm_dstack_ptr(self);
    bvm_write_cache(this_bvm, &this_bvm->dstack_ptr, &temp);

    temp = (mword*)bvm_ustack_ptr(self);
    bvm_write_cache(this_bvm, &this_bvm->ustack_ptr, &temp);

    temp = (mword*)bvm_sym_table(self);
    bvm_write_cache(this_bvm, &this_bvm->sym_table, &temp);

    if(!is_nil(this_bvm->sym_table)){

        mword temp_val;

        temp_val = icar(
                        HASH_ENTRY_PAY( 
                            this_bvm, 
                                _luha( this_bvm, 
                                    get_tptr(this_bvm->sym_table), 
                                    BABEL_SYM_ADVANCE_TYPE )));
        bvm_write_cache(this_bvm, &this_bvm->advance_type, &temp_val);

    //        temp_val = icar(
    //                        HASH_ENTRY_PAY( 
    //                            this_bvm, 
    //                                _luha( this_bvm, 
    //                                    get_tptr(this_bvm->sym_table), 
    //                                    BABEL_SYM_SOFT_ROOT )));
    //        bvm_write_cache(this_bvm, &this_bvm->soft_root, &temp_val);

        temp_val = icar(
                        HASH_ENTRY_PAY( 
                            this_bvm, 
                                _luha( this_bvm, 
                                    get_tptr(this_bvm->sym_table), 
                                    BABEL_SYM_STEPS )));
        bvm_write_cache(this_bvm, &this_bvm->steps, &temp_val);

        temp_val = icar(
                        HASH_ENTRY_PAY( 
                            this_bvm, 
                                _luha( this_bvm, 
                                    get_tptr(this_bvm->sym_table), 
                                    BABEL_SYM_THREAD_ID )));
        bvm_write_cache(this_bvm, &this_bvm->thread_id, &temp_val);

    }

    this_bvm->flags->BVM_CACHE_BLOCKING = FLAG_CLR;
    this_bvm->flags->MC_ALLOC_BLOCKING  = FLAG_CLR;
    this_bvm->flags->BVM_CACHE_INVALID  = FLAG_CLR;

    return this_bvm;

}


//
//
bvm_cache *bvm_flush_cache(bvm_cache *this_bvm){ // bvm_flush_cache#

//trace;
//_mem(this_bvm->self);

    if(    this_bvm->flags->BVM_CACHE_INVALID  == FLAG_SET
        || this_bvm->flags->BVM_CACHE_BLOCKING == FLAG_SET ){
        cat_except(this_bvm);
    }

    if(this_bvm->flags->BVM_CACHE_ONLY == FLAG_SET){
        return;
    }

    this_bvm->flags->MC_ALLOC_BLOCKING  = FLAG_SET;
    this_bvm->flags->BVM_CACHE_BLOCKING = FLAG_SET;

    mword *self = this_bvm->self;

    (mword*)bvm_code_ptr(self)      = this_bvm->code_ptr;
    (mword*)bvm_rstack_ptr(self)    = this_bvm->rstack_ptr;

    (mword*)bvm_dstack_ptr(self)    = this_bvm->dstack_ptr;
    (mword*)bvm_ustack_ptr(self)    = this_bvm->ustack_ptr;

    (mword*)bvm_sym_table(self)     = this_bvm->sym_table;

    //set_sym vs. sym_update
    //if(this_bvm->flags->BVM_SYMBOLS_DEFINED == FLAG_SET){
    if(tageq(this_bvm->self, BABEL_TAG_READY_BVM, TAG_SIZE)){

        if(!_exha(
                this_bvm, 
                get_tptr(this_bvm->sym_table), 
                (mword*)BABEL_TAG_BVM_STEPS)){

            sym_update_val(this_bvm, BABEL_SYM_ADVANCE_TYPE, this_bvm->advance_type);
            sym_update_val(this_bvm, BABEL_SYM_STEPS       , this_bvm->steps);
            sym_update_val(this_bvm, BABEL_SYM_THREAD_ID   , this_bvm->thread_id);
            //sym_update(this_bvm, BABEL_SYM_SOFT_ROOT   , &this_bvm->soft_root);

        }
        else{

            this_bvm->flags->MC_ALLOC_BLOCKING  = FLAG_CLR;

            set_sym(this_bvm, "advance_type", _newva( this_bvm, this_bvm->advance_type));
            set_sym(this_bvm, "soft_root"   , this_bvm->soft_root);
            set_sym(this_bvm, "steps"       , _newva( this_bvm, this_bvm->steps));
            set_sym(this_bvm, "thread_id"   , _newva( this_bvm, this_bvm->thread_id));

        }

    }
    else{

        cat_except(this_bvm);

    }

//    this_bvm->mask_table
//    this_bvm->flags
//    this_bvm->interp

    this_bvm->flags->BVM_CACHE_BLOCKING = FLAG_CLR;
    this_bvm->flags->MC_ALLOC_BLOCKING  = FLAG_CLR;
    this_bvm->flags->BVM_CACHE_DIRTY    = FLAG_CLR;
    
    return this_bvm;

}


// [dict    "stack"         (stack)
//          "code"          {code}
//          "sym_list"      (sym_list) ]  babel
//
// (stack) {code} babelf   --> "fast babel" ... no local symbol table
//
// hibernated_bvm resume
//
// sparse_bvm babels

// Intended for use with babel operator
//
bvm_cache *bvm_new(bvm_cache *this_bvm){ // *bvm_new#
}


// Intended for use with babelf operator
//
bvm_cache *bvm_new_fast(bvm_cache *this_bvm){ // *bvm_new_fast#
}


// Intended for use with resume operator
//
bvm_cache *bvm_new_hiber_bvm(bvm_cache *this_bvm){ // *bvm_new_hiber_bvm#
}



//
//
bvm_cache *bvm_new_sym_table(bvm_cache *this_bvm){ // *bvm_new_sym_table#



}


//
//
int bvm_sym_table_exists(bvm_cache *this_bvm){ // bvm_sym_table_exists#

    mword *this_bvm_sym_table = (mword*)bvm_sym_table(this_bvm);
trace;
           this_bvm_sym_table = hard_detag(this_bvm, this_bvm_sym_table);
trace;

    if( is_nil(car(this_bvm_sym_table)) 
        && is_nil(cdr(this_bvm_sym_table))){ // sym_table is uninitialized
trace;
        return 0;
    }
trace;

    return 1; 

}


//
//
bvm_cache *bvm_step(bvm_cache *this_bvm){ // bvm_step#



}


/* bvm operator
**babel**
> This operator "execs" a loaded BVM on TOS
*/
bvm_cache *babelop2(bvm_cache *this_bvm){ // babelop2#

    bvm_cache new_bvm;
    bvm_cache *new_bvm_ptr = &new_bvm;

    mword *new_stack = dstack_get(this_bvm,1);
    mword *self      = dstack_get(this_bvm,0);

    popd(this_bvm);
    popd(this_bvm);

    new_bvm_ptr->self = self;
    bvm_cp_cache(this_bvm, new_bvm_ptr);
    new_bvm_ptr->self = self;

    _babelop2(this_bvm, new_bvm_ptr);

    return this_bvm;

}



bvm_cache *_babelop2(bvm_cache *this_bvm, bvm_cache *new_bvm_ptr){ // _babelop2#

    new_bvm_ptr->flags->BVM_CACHE_DIRTY  = FLAG_CLR;
    new_bvm_ptr->flags->BVM_CACHE_INVALID = FLAG_CLR;

    bvm_sym_update_cache(new_bvm_ptr);
    bvm_flush_cache(this_bvm);

//_dump(this_bvm->self);
//printf("%s\n", (char*)_bbl2str(this_bvm, this_bvm->self));
//die;

trace;
    set_sym_ptr(new_bvm_ptr, BABEL_SYM_SOFT_ROOT, this_bvm->self);
trace;

    bvm_interp(new_bvm_ptr);
die;

    bvm_cp_cache(new_bvm_ptr, this_bvm);
    this_bvm->self = get_sym_ptr(new_bvm_ptr, BABEL_SYM_PARENT_BVM);
    bvm_update_cache(this_bvm);

    bvm_sym_flush_cache(new_bvm_ptr);

    return this_bvm;

}


////    _insha( this_bvm, 
////            temp_ptr, 
////            hash, 
////            nil, 
////            new_hash_table_entry(
////                this_bvm, 
////                BABEL_SYM_PARENT_BVM, 
////                "parent_bvm", 
////                this_bvm->interp->interp_argv));
////
//
////    bvm_dump_cache(new_bvm_ptr);
//
////    pushd(this_bvm, _newva(this_bvm, 0xdeadbeef), IMMORTAL);
////    _dump(this_bvm->code_ptr);
////    die;
//
////    pushd(new_bvm_ptr, _newva(this_bvm, 0xdeadbeef), IMMORTAL);
//
//    _dump(new_bvm_ptr->code_ptr);
//    die;
//
//die;    
//    mword *temp_ptr;
//
//    temp_ptr = 
//        HASH_ENTRY_PAY( new_bvm_ptr,
//                _luha( new_bvm_ptr, 
//                    get_tptr(get_tptr(new_bvm_ptr->self)), 
//                        BABEL_SYM_DSTACK_PTR ));
//
//    new_bvm_ptr->dstack_ptr = temp_ptr;
//
//    pushd(new_bvm_ptr, _newva(this_bvm, 0xdeadbeef), IMMORTAL);
//    pushd(new_bvm_ptr, _newva(this_bvm, 0xacedcafe), IMMORTAL);
//    pushd(new_bvm_ptr, _newva(this_bvm, 0xfacefeed), IMMORTAL);
//
//    _dump(new_bvm_ptr->dstack_ptr);
//    die;
//
//    temp_ptr =
//        HASH_ENTRY_PAY( new_bvm_ptr,
//                _luha( new_bvm_ptr, 
//                    get_tptr(get_tptr(new_bvm_ptr->self)), 
//                        BABEL_SYM_CODE_PTR ));
//
//    (mword*)c(temp_ptr,0) = _newva(this_bvm, 0xbabeface);
//
//    mword *temp_ptr =
//        HASH_ENTRY_PAY( new_bvm_ptr,
//                _luha( new_bvm_ptr, 
//                    get_tptr(get_tptr(new_bvm_ptr->self)), 
//                        BABEL_SYM_CODE_PTR ));
//
//    _dump(temp_ptr);
//    die;
//
//    mword *temp_val =
//        HASH_ENTRY_PAY( new_bvm_ptr,
//                _luha( new_bvm_ptr, 
//                    get_tptr(get_tptr(new_bvm_ptr->self)), 
//                        BABEL_SYM_THREAD_ID ));
//
//    c(temp_val,0) = 0xdeadbeef;
//
//    temp_val =
//        HASH_ENTRY_PAY( new_bvm_ptr,
//                _luha( new_bvm_ptr, 
//                    get_tptr(get_tptr(new_bvm_ptr->self)), 
//                        BABEL_SYM_THREAD_ID ));
//
//    _dump(temp_val);
//    die;
//


/* bvm operator
**babel**
> This operator "execs" a loaded BVM on TOS
*/
bvm_cache *babelop(bvm_cache *this_bvm){ // babelop# babel#

    bvm_cache new_bvm;
    bvm_cache *new_bvm_ptr = &new_bvm;

    mword *new_stack = dstack_get(this_bvm,1);
    mword *self      = dstack_get(this_bvm,0);
    new_bvm_ptr->self = self;

    popd(this_bvm);
    popd(this_bvm);

    if(!tageq(new_bvm_ptr->self, BABEL_TAG_READY_BVM, TAG_SIZE)){

        if(tageq(new_bvm_ptr->self, BABEL_TAG_SPARSE_BVM, TAG_SIZE)){

            bvm_cp_cache(this_bvm, new_bvm_ptr);
            new_bvm_ptr->self = self;

            set_sym(this_bvm, "steps", _newva( this_bvm, this_bvm->steps ) );

            bvm_new_sparse_bvm(new_bvm_ptr);

            //Force cache update:
            new_bvm_ptr->flags->BVM_CACHE_DIRTY = FLAG_CLR;
            bvm_update_cache(new_bvm_ptr);

        }

    }
    else{

        bvm_cp_cache(this_bvm, new_bvm_ptr);
        new_bvm_ptr->self = self;

        //Force cache update:
        new_bvm_ptr->flags->BVM_CACHE_DIRTY = FLAG_CLR;
        bvm_update_cache(new_bvm_ptr);

    }

    if(!is_nil(new_stack)){
        rgive(new_bvm_ptr, new_stack);
    }

    bvm_flush_cache(this_bvm);

    set_sym(new_bvm_ptr, "parent_bvm", this_bvm->self);

    bvm_interp(new_bvm_ptr);

    //Force cache flush:
    new_bvm_ptr->flags->BVM_CACHE_INVALID = FLAG_CLR;
    bvm_flush_cache(new_bvm_ptr);

    this_bvm->self = get_sym(new_bvm_ptr, "parent_bvm");

    //Force cache update:
    this_bvm->flags->BVM_CACHE_DIRTY   = FLAG_CLR;
    bvm_update_cache(this_bvm);

    this_bvm->flags->MC_ALLOC_BLOCKING = FLAG_CLR;

    return this_bvm;

}


/* bvm operator
 **exec**  
> does a fork/exec in Linux; CreateProcess in Windows
> equivalent to Perl's system()
*/
bvm_cache *exec(bvm_cache *this_bvm){ // exec#

#ifdef WINDOWS

    char *process_string = (char*)_b2c(this_bvm, dstack_get(this_bvm,0));
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
        result = _newva( this_bvm, 0);
    }

    result = _newva( this_bvm, 1);
   
    pushd(this_bvm, result, IMMORTAL); 

#endif

    return this_bvm;

}



/* bvm operator
**hibernate**
Saves the current BVM off to disk. Takes one argument
to specify the filename to save to.
*/
bvm_cache *hibernate(bvm_cache *this_bvm){ // hibernate#

    fatal("unimplemented");

//    mword *filename = dstack_get(this_bvm,0);
//    popd(this_bvm);
//
//    icar(this_bvm->code_ptr) = cdr(car(this_bvm->code_ptr));
//    icar(this_bvm->advance_type) = BVM_ADVANCE;
//    icar(this_bvm->steps)--;
//    
//    bvm_flush_cache(this_bvm);
//
//    mword *bvm_out = get_tptr(this_bvm->self);
//
//    mword *temp_bvm_out = new_tptr(this_bvm, _hash8(this_bvm, C2B("/babel/tag/hiber_bvm")), bvm_out);
//
//    bvm_out = _unload(this_bvm, temp_bvm_out);
//
//    _spit_mword(this_bvm, (char*)_b2c(this_bvm, filename), bvm_out);
//
//    icar(this_bvm->advance_type) = BVM_RETURN;    
//
//    return this_bvm;

}



//
// babel_operator
bvm_cache *bvmcode(bvm_cache *this_bvm){ // *bvmcode#

   pushd(this_bvm, this_bvm->code_ptr, IMMORTAL); 
    // XXX: These operators are inherently dangerous in 
    // combination with the mortal operator

    return this_bvm;

}

//
// babel_operator
bvm_cache *bvmstack(bvm_cache *this_bvm){ // *bvmstack#

    pushd(this_bvm, this_bvm->dstack_ptr, IMMORTAL);

    return this_bvm;

}

//
// babel_operator
bvm_cache *bvmustack(bvm_cache *this_bvm){ // *bvmustack#

    pushd(this_bvm, this_bvm->ustack_ptr, IMMORTAL);

    return this_bvm;

}

//
// babel_operator
bvm_cache *bvmrstack(bvm_cache *this_bvm){ // *bvmrstack#

    pushd(this_bvm, this_bvm->rstack_ptr, IMMORTAL);

    return this_bvm;

}

// babel_operator
bvm_cache *bvmsym(bvm_cache *this_bvm){ // *bvmsym#

    pushd(this_bvm, this_bvm->sym_table, IMMORTAL);

    return this_bvm;

}


//
// babel_operator
bvm_cache *self(bvm_cache *this_bvm){ // *self#

    pushd(this_bvm, this_bvm->soft_root, IMMORTAL);

    return this_bvm;

}


//
// babel_operator
bvm_cache *bvm_exit(bvm_cache *this_bvm){ // bvm_exit#

    this_bvm->advance_type = BVM_RETURN;

    return this_bvm;

}


//
// babel_operator
bvm_cache *rsvd(bvm_cache *this_bvm){ // *rsvd#

    d(car(car(this_bvm->code_ptr)))

    error("An attempt was made to execute a reserved opcode");
    die;

}

//
// babel_operator
bvm_cache *boilerplate(bvm_cache *this_bvm){ // *boilerplate#

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

// Clayton Bauman 2014

