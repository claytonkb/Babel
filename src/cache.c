// cache.c
//

#include "babel.h"
#include "cache.h"
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


//
//
bvm_cache *set_sym_ptr(bvm_cache *this_bvm, mword *hash, mword *payload){ // set_sym_ptr#

    _insha( this_bvm, 
            get_tptr(this_bvm->self), 
            hash, 
            nil, 
            new_hash_table_entry(
                this_bvm, 
                hash, 
                nil, 
                payload));

    return this_bvm;

}


//
//
mword *get_sym_ptr(bvm_cache *this_bvm, mword *hash){ // get_sym_ptr#

    mword *payload = 
        HASH_ENTRY_PAY( 
            this_bvm, 
                _luha(this_bvm,  
                    get_tptr(get_tptr(this_bvm->self)), 
                        hash) );

    return payload;

}

//
//
bvm_cache *set_sym_val(bvm_cache *this_bvm, mword *hash, mword payload){ // set_sym_ptr#

//Look up symbol in root symbol-table
//Get the 2 ith entry
//Set the car of it to payload

//    _insha( this_bvm, 
//            get_tptr(this_bvm->self), 
//            hash, 
//            nil, 
//            new_hash_table_entry(
//                this_bvm, 
//                hash, 
//                nil, 
//                payload));

    return this_bvm;

}


//
//
bvm_cache *coherent_write_cache(bvm_cache *this_bvm, mword *cache_field, 
                                mword *symbol, mword *payload){ // coherent_write_cache#

    *cache_field = *payload;
    set_sym_ptr(this_bvm, symbol, payload);

}


//
//
bvm_cache *incoherent_write_cache(bvm_cache *this_bvm, void *field, void *payload){ // incoherent_write_cache#

    // cache should have been updated before calling incoherent_write_cache()
    if(this_bvm->flags->BVM_CACHE_INVALID == FLAG_SET){
        cat_except(this_bvm);
    }

    *field = *payload;
    this_bvm->flags->BVM_CACHE_DIRTY = FLAG_SET;

    return this_bvm;

}


//
//
bvm_cache *incoherent_write_symbol(bvm_cache *this_bvm, mword *symbol, void *payload){ // incoherent_write_symbol#

    // cache should have been flushed before calling incoherent_write_symbol()
    if(this_bvm->flags->BVM_CACHE_DIRTY == FLAG_SET){ 
        cat_except(this_bvm);
    }

    set_sym_ptr(this_bvm, symbol, payload);
    this_bvm->flags->BVM_CACHE_INVALID = FLAG_SET;

    return this_bvm;

}


//
//
bvm_cache *bvm_sym_update_cache(bvm_cache *this_bvm){ // bm_sym_update_cache#

    if(    this_bvm->flags->BVM_CACHE_DIRTY    == FLAG_SET
        || this_bvm->flags->BVM_CACHE_BLOCKING == FLAG_SET ){
        cat_except(this_bvm);
    }

    if(this_bvm->flags->BVM_CACHE_ONLY == FLAG_SET){
        return;
    }

    this_bvm->flags->BVM_CACHE_BLOCKING = FLAG_SET;
    this_bvm->flags->MC_GC_BLOCKING     = FLAG_SET;

    mword *self = this_bvm->self;

    mword *temp;

    // code_ptr
    temp = (mword*)
                HASH_ENTRY_PAY( 
                    this_bvm, 
                        _luha( this_bvm, 
                            get_tptr(get_tptr(self)), 
                            BABEL_SYM_CODE_PTR ));

    incoherent_write_cache(this_bvm, &this_bvm->code_ptr, &temp);


    // rstack_ptr
    temp = (mword*)
                HASH_ENTRY_PAY( 
                    this_bvm, 
                        _luha( this_bvm, 
                            get_tptr(get_tptr(self)), 
                            BABEL_SYM_RSTACK_PTR ));

    incoherent_write_cache(this_bvm, &this_bvm->rstack_ptr, &temp);


    // dstack_ptr
    temp = (mword*)
                HASH_ENTRY_PAY( 
                    this_bvm, 
                        _luha( this_bvm, 
                            get_tptr(get_tptr(self)), 
                            BABEL_SYM_DSTACK_PTR ));

    incoherent_write_cache(this_bvm, &this_bvm->dstack_ptr, &temp);


    // ustack_ptr
    temp = (mword*)
                HASH_ENTRY_PAY( 
                    this_bvm, 
                        _luha( this_bvm, 
                            get_tptr(get_tptr(self)), 
                            BABEL_SYM_USTACK_PTR ));

    incoherent_write_cache(this_bvm, &this_bvm->ustack_ptr, &temp);


    // soft_root
    temp = (mword*)
                HASH_ENTRY_PAY( 
                    this_bvm, 
                        _luha( this_bvm, 
                            get_tptr(get_tptr(self)), 
                            BABEL_SYM_SOFT_ROOT ));

    incoherent_write_cache(this_bvm, &this_bvm->soft_root, &temp);


    // local_root
    temp = (mword*)
                HASH_ENTRY_PAY( 
                    this_bvm, 
                        _luha( this_bvm, 
                            get_tptr(get_tptr(self)), 
                            BABEL_SYM_LOCAL_ROOT ));

    incoherent_write_cache(this_bvm, &this_bvm->local_root, &temp);


    mword temp_val;

    // advance_type
    temp_val = icar(
                    HASH_ENTRY_PAY( 
                        this_bvm, 
                            _luha( this_bvm, 
                                get_tptr(get_tptr(self)), 
                                BABEL_SYM_ADVANCE_TYPE )));

    incoherent_write_cache(this_bvm, &this_bvm->advance_type, &temp_val);


    // steps
    temp_val = icar(
                    HASH_ENTRY_PAY( 
                        this_bvm, 
                            _luha( this_bvm, 
                                get_tptr(get_tptr(self)), 
                                BABEL_SYM_STEPS )));

    incoherent_write_cache(this_bvm, &this_bvm->steps, &temp_val);


    // thread_id
    temp_val = icar(
                    HASH_ENTRY_PAY( 
                        this_bvm, 
                            _luha( this_bvm, 
                                get_tptr(get_tptr(self)), 
                                BABEL_SYM_THREAD_ID )));

    incoherent_write_cache(this_bvm, &this_bvm->thread_id, &temp_val);


    this_bvm->flags->BVM_CACHE_BLOCKING = FLAG_CLR;
    this_bvm->flags->MC_GC_BLOCKING     = FLAG_CLR;

    this_bvm->flags->BVM_CACHE_DIRTY    = FLAG_CLR;
    this_bvm->flags->BVM_CACHE_INVALID  = FLAG_CLR;

    return this_bvm;

}


//
//
bvm_cache *bvm_sym_flush_cache(bvm_cache *this_bvm){ // bvm_sym_flush_cache#

//trace;
//_mem(this_bvm->self);

    if(    this_bvm->flags->BVM_CACHE_INVALID  == FLAG_SET
        || this_bvm->flags->BVM_CACHE_BLOCKING == FLAG_SET ){
        cat_except(this_bvm);
    }

    if(this_bvm->flags->BVM_CACHE_ONLY == FLAG_SET){
        return;
    }

    this_bvm->flags->MC_GC_BLOCKING     = FLAG_SET;
    this_bvm->flags->BVM_CACHE_BLOCKING = FLAG_SET;

    set_sym_ptr(this_bvm, BABEL_SYM_CODE_PTR  ,     this_bvm->code_ptr);
    set_sym_ptr(this_bvm, BABEL_SYM_RSTACK_PTR,     this_bvm->rstack_ptr);
    set_sym_ptr(this_bvm, BABEL_SYM_DSTACK_PTR,     this_bvm->dstack_ptr);
    set_sym_ptr(this_bvm, BABEL_SYM_USTACK_PTR,     this_bvm->ustack_ptr);
    set_sym_ptr(this_bvm, BABEL_SYM_SOFT_ROOT ,     this_bvm->soft_root);
    set_sym_ptr(this_bvm, BABEL_SYM_LOCAL_ROOT,     this_bvm->code_ptr);
    set_sym_val(this_bvm, BABEL_SYM_ADVANCE_TYPE,   this_bvm->advance_type);
    set_sym_val(this_bvm, BABEL_SYM_STEPS     ,     this_bvm->steps);
    set_sym_val(this_bvm, BABEL_SYM_THREAD_ID ,     this_bvm->thread_id);

    this_bvm->flags->BVM_CACHE_BLOCKING = FLAG_CLR;
    this_bvm->flags->MC_GC_BLOCKING     = FLAG_CLR;

    this_bvm->flags->BVM_CACHE_DIRTY    = FLAG_CLR;
    this_bvm->flags->BVM_CACHE_INVALID  = FLAG_CLR;

    return this_bvm;

}


//
//
bvm_cache *bvm_write_cache(bvm_cache *this_bvm, void *field, void *value){ // bvm_write_cache#

    *field = *value;
    this_bvm->flags->BVM_CACHE_DIRTY = FLAG_SET;

    return this_bvm;

}


//
//
bvm_cache *bvm_cp_cache(bvm_cache *src_bvm, bvm_cache *dest_bvm){ // bvm_cp_cache#

    dest_bvm->self          = src_bvm->self;

    dest_bvm->code_ptr      = src_bvm->code_ptr;
    dest_bvm->rstack_ptr    = src_bvm->rstack_ptr;

    dest_bvm->dstack_ptr    = src_bvm->dstack_ptr;
    dest_bvm->ustack_ptr    = src_bvm->ustack_ptr;

    dest_bvm->sym_table     = src_bvm->sym_table;
    dest_bvm->soft_root     = src_bvm->soft_root;
    dest_bvm->local_root    = src_bvm->local_root;

    dest_bvm->advance_type  = src_bvm->advance_type;
    dest_bvm->steps         = src_bvm->steps;
    dest_bvm->thread_id     = src_bvm->thread_id;

    dest_bvm->mask_table    = src_bvm->mask_table;

    dest_bvm->flags         = src_bvm->flags;

    dest_bvm->interp        = src_bvm->interp;

    return dest_bvm;

}

// Clayton Bauman 2014

