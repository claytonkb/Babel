// bvm.c
//

#include "babel.h"
#include "bvm.h"
#include "hash.h"
#include "introspect.h"
#include "tptr.h"
#include "cache.h"
#include "list.h"
#include "interp.h"
#include "stack.h"
#include "operator.h"
#include "bstruct.h"
#include "trie.h"
#include "string.h"
#include "pearson16.h"
#include "io.h"


//
//
bvm_cache *bvm_new(bvm_cache *this_bvm){ // bvm_new#

#ifdef BABEL_RESET_TRACE
_trace;
#endif

    mword *self = tptr_detag(this_bvm, tptr_detag(this_bvm, this_bvm->self));

    lci(bvm_dstack_ptr(this_bvm),0) = stack_new(this_bvm);

#define Y(a,b,c)                            \
    if(!_exha(this_bvm, self, c)){          \
        _insha( this_bvm,                   \
            self,                           \
            c,                              \
            nil,                            \
            hash_new_entry(                 \
                this_bvm,                   \
                c,                          \
                nil,                        \
                nil));                      \
    }        

#define X(a,b,c)                                    \
    if(!trie_exists(this_bvm, self, c, nil)){       \
        trie_insert(this_bvm, self, c, nil, nil);   \
    }

CACHED_FIELDS    

#undef X

    mword *local_root = _ith( this_bvm, trie_lookup_hash(this_bvm, self, BABEL_SYM_LOCAL_ROOT,  nil), 2 );

    if(is_nil(local_root)){
        trie_insert(this_bvm, self, BABEL_SYM_LOCAL_ROOT, nil, 
                _ith( this_bvm, trie_lookup_hash(this_bvm, self, BABEL_SYM_SOFT_ROOT, nil), 2)); 
    }

    cache_update(this_bvm);

    this_bvm->flags->BVM_CACHE_DIRTY   = FLAG_CLR;
    this_bvm->flags->BVM_CACHE_INVALID = FLAG_CLR;

    return this_bvm;

}


//
//
mword *_babel(bvm_cache *this_bvm, mword *loaded_bvm, mword *arg_stack, mword *sym_table){ // _babel#

    bvm_cache new_bvm;
    bvm_cache *new_bvm_ptr = &new_bvm;
    mword *result = nil;

    cache_new(this_bvm, new_bvm_ptr, loaded_bvm);

    mword *self = tptr_detag(new_bvm_ptr, tptr_detag(new_bvm_ptr, new_bvm_ptr->self)); // Could blow up due to mem_alloc()

    if( !trie_exists(new_bvm_ptr, self, BABEL_SYM_BVM_INITD, nil) ){
        trie_insert( new_bvm_ptr, self, BABEL_SYM_BVM_INITD, nil, _val(new_bvm_ptr,1) );
//        trie_insert( new_bvm_ptr, self, BABEL_SYM_BVM_INITD, nil, _val(new_bvm_ptr,0) );
    }

    mword *bvm_initd = rci(cache_read_from_bvm(new_bvm_ptr, BABEL_SYM_BVM_INITD),0);

    if(!rcl(bvm_initd,0)){
        bvm_new(new_bvm_ptr);
        lcl(bvm_initd,0) = 1;
    }
    else{
        cache_update(new_bvm_ptr);
    }

    if( !trie_exists(new_bvm_ptr, self, BABEL_SYM_CODE_RESTART_POINT, nil) ){
        trie_insert( new_bvm_ptr, self, BABEL_SYM_CODE_RESTART_POINT, nil, rci(new_bvm_ptr->code_ptr,0));
    }

    new_bvm_ptr->flags->BVM_CACHE_DIRTY   = FLAG_CLR;
    new_bvm_ptr->flags->BVM_CACHE_INVALID = FLAG_CLR;

    cache_flush(this_bvm);

    if(!is_nil(sym_table)){
        trie_insert(new_bvm_ptr, tptr_detag(new_bvm_ptr, new_bvm_ptr->self), BABEL_SYM_SOFT_ROOT, nil, sym_table);
    }

    trie_insert(new_bvm_ptr, tptr_detag(new_bvm_ptr, new_bvm_ptr->self), BABEL_SYM_PARENT_BVM, nil, this_bvm->self);

    new_bvm_ptr->flags->BVM_CODE_LIST_EMPTY = FLAG_CLR;

    while(!is_nil(arg_stack)){ // give the arg-list onto the BVM's dstack
        interp_push_operand(new_bvm_ptr, rci(arg_stack, 0));
        arg_stack = rci(arg_stack,1);
    }

    interp_core(new_bvm_ptr);

    cache_cp(new_bvm_ptr, this_bvm); //update flags and interp

    this_bvm->self = _ith(  this_bvm, 
                            trie_lookup_hash(
                                new_bvm_ptr, 
                                tptr_detag(new_bvm_ptr, new_bvm_ptr->self), 
                                BABEL_SYM_PARENT_BVM, 
                                nil),
                            2 );

    cache_update(this_bvm);

    this_bvm->flags->BVM_CACHE_DIRTY   = FLAG_CLR;
    this_bvm->flags->BVM_CACHE_INVALID = FLAG_CLR;

    //copy TOS from new_bvm to this_bvm
    oinfo oi;
    oi.default_data = nil;
    oi.required_tag = nil;
    oi.mask = OI_MASK_ANY;
    oi.min_size = 0;
    oi.max_size = 1;

    if( new_bvm_ptr->flags->BVM_RETURN_TOS_ON_EXIT == FLAG_SET 
            &&
        (new_bvm_ptr->flags->BVM_CODE_LIST_EMPTY == FLAG_SET 
            ||
        (get_advance_type(new_bvm_ptr) == BVM_RETURN))){

        get_operands(new_bvm_ptr,1,&oi);
        result = oi.data;

        stack_pop(new_bvm_ptr,rci(new_bvm_ptr->dstack_ptr,0));

//        stack_push(this_bvm,
//                rci(this_bvm->dstack_ptr,0),
//                stack_new_entry(
//                    this_bvm,
//                    oi.data,
//                    nil));

    }

    // Reset all flags in case of re-entry
    new_bvm_ptr->flags->BVM_RETURN_TOS_ON_EXIT = FLAG_CLR; // FIXME: This restore to previous value, not force-clear
    new_bvm_ptr->flags->BVM_CODE_LIST_EMPTY = FLAG_CLR;

    if(get_advance_type(new_bvm_ptr) == BVM_RETURN){
        set_advance_type(new_bvm_ptr, BVM_ADVANCE);
    }

    this_bvm->flags->BVM_RETURN_TOS_ON_EXIT = FLAG_CLR;
    this_bvm->flags->BVM_CODE_LIST_EMPTY    = FLAG_CLR;

    return result;

}


//
//
mword *_babel_root(bvm_cache *this_bvm, mword *loaded_bvm){ // _babel_root#

#ifdef BABEL_RESET_TRACE
_trace;
#endif

    bvm_cache new_bvm;
    bvm_cache *new_bvm_ptr = &new_bvm;

    cache_new(this_bvm, new_bvm_ptr, loaded_bvm);

    mword *bvm_initd = rci(cache_read_from_bvm(this_bvm, BABEL_SYM_BVM_INITD),0);

    if(!rcl(bvm_initd,0)){
        bvm_new(new_bvm_ptr);
        lcl(bvm_initd,0) = 1;
    }
    else{
        cache_update(new_bvm_ptr);
    }

    new_bvm_ptr->flags->BVM_CACHE_DIRTY   = FLAG_CLR;
    new_bvm_ptr->flags->BVM_CACHE_INVALID = FLAG_CLR;

    babel_root_code_injection_point(new_bvm_ptr);

    new_bvm_ptr->flags->BVM_CODE_LIST_EMPTY = FLAG_CLR;

    interp_core(new_bvm_ptr);

    this_bvm->flags->BVM_CACHE_DIRTY   = FLAG_CLR;
    this_bvm->flags->BVM_CACHE_INVALID = FLAG_CLR;

// XXX Enhancement: handle return-from-root
//    if(new_bvm_ptr->flags->BVM_CODE_LIST_EMPTY == FLAG_SET){
//        _msg("BVM_CODE_LIST_EMPTY");
//    }

    return nil;

}


// FIXME: Remove
//
bvm_cache *babel_root_code_injection_point(bvm_cache *this_bvm){ // babel_root_code_injection_point#

//    interp_push_operand(this_bvm, C2B("Hello, world!\n") );
//    stdout_8d(this_bvm);

    return this_bvm;

}


//
//
bvm_cache *dstack_pop(bvm_cache *this_bvm){ // dstack_pop#

    this_bvm->dstack_depth = 0;

    return this_bvm;

}


/*****************************************************************************
 *                                                                           *
 *                            BVM OPERATORS                                  *
 *                                                                           *
 ****************************************************************************/


//#define BABEL_OPERATIONS
//    result0 = _babel(this_bvm, oi0.data, nil);
//
//OPERATORA_R1_W1_D(babel, BABEL_OPERATIONS, nil, OI_MASK_INTE|OI_MASK_TPTR, 0, 0)


#define BABEL_OPERATIONS \
    result0 = _babel(this_bvm, oi0.data, oi1.data, nil);

OPERATORA_R2_W1_D(babel, BABEL_OPERATIONS, 
    nil, OI_MASK_INTE|OI_MASK_TPTR, 0, 0,
    nil, OI_MASK_INTE|OI_MASK_TPTR, 0, 0)


#define BABELS_OPERATIONS \
    result0 = _babel(this_bvm, oi0.data, oi1.data, oi2.data);

OPERATORA_R3_W1_D(babels, BABELS_OPERATIONS, 
    nil, OI_MASK_INTE|OI_MASK_TPTR, 0, 0,
    nil, OI_MASK_INTE|OI_MASK_TPTR, 0, 0,
    nil, OI_MASK_INTE|OI_MASK_TPTR, 0, 0)

//OPERATORA_R2_W0_D(insym_d,INSYM_D_OPERATIONS,
//        nil,OI_MASK_LEAF,0,1,
//        nil,OI_MASK_ANY,0,0)


//FIXME: This should be a dev() code and this operator should instead put
// soft_root on TOS
#define ROOT_OPERATIONS \
    result0 = this_bvm->soft_root; 

OPERATORA_R0_W1(root, ROOT_OPERATIONS)


#define ARGVOP_OPERATIONS \
    result0 = _ith( this_bvm, \
                    trie_lookup_hash( \
                        this_bvm,  \
                        tptr_detag(this_bvm, this_bvm->self), \
                        BABEL_SYM_PARENT_BVM, \
                        nil), \
                    2 );

OPERATORA_R0_W1(argvop, ARGVOP_OPERATIONS)


// lusym and insym are temporary operators...
#define LUSYM_D_OPERATIONS \
    result0 = lusym(this_bvm, _hash8(this_bvm,oi0.data)); 

OPERATORA_R1_W1_D(lusym_d, LUSYM_D_OPERATIONS, nil, OI_MASK_LEAF, 0, 0)


#define INSYM_D_OPERATIONS \
    insym2(this_bvm, _hash8(this_bvm,oi0.data), oi1.data);

OPERATORA_R2_W0_D(insym_d,INSYM_D_OPERATIONS,
        nil,OI_MASK_LEAF,0,1,
        nil,OI_MASK_ANY,0,0)




// Clayton Bauman 2014

