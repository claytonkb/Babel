// cache.c
//

#include "babel.h"
#include "cache.h"
#include "hash.h"
#include "tptr.h"
#include "introspect.h"
#include "list.h"
#include "mem.h"
#include "bvm.h"
#include "trie.h"
#include "stack.h"
#include "bstruct.h"


//
//
bvm_cache *cache_new(bvm_cache *this_bvm, bvm_cache *new_bvm, mword *loaded_bvm){ // cache_new#

    cache_cp(this_bvm, new_bvm);

    new_bvm->self = loaded_bvm;

//    bvm_new(new_bvm);

    return new_bvm;

}


//
//
bvm_cache *cache_update(bvm_cache *this_bvm){ // cache_update#

    if(    this_bvm->flags->BVM_CACHE_DIRTY    == FLAG_SET
        || this_bvm->flags->BVM_CACHE_BLOCKING == FLAG_SET ){
        _cat_except(this_bvm);
    }

    mword save_ALLOC_BLOCKING = this_bvm->flags->MC_ALLOC_BLOCKING;
    mword save_GC_BLOCKING    = this_bvm->flags->MC_GC_BLOCKING;

    this_bvm->flags->BVM_CACHE_BLOCKING = FLAG_SET;
    this_bvm->flags->MC_ALLOC_BLOCKING  = FLAG_SET;
    this_bvm->flags->MC_GC_BLOCKING     = FLAG_SET;

#define X(a,b,c) this_bvm->a = cache_read_from_bvm(this_bvm, c);
CACHED_FIELDS
#undef X    

    dstack_depth_update(this_bvm);

    this_bvm->flags->MC_GC_BLOCKING     = save_GC_BLOCKING;
    this_bvm->flags->MC_ALLOC_BLOCKING  = save_ALLOC_BLOCKING;
    this_bvm->flags->BVM_CACHE_BLOCKING = FLAG_CLR;

    this_bvm->flags->BVM_CACHE_INVALID  = FLAG_CLR;

    return this_bvm;

}


//
//
bvm_cache *cache_flush(bvm_cache *this_bvm){ // cache_flush#

    if(    this_bvm->flags->BVM_CACHE_INVALID  == FLAG_SET
        || this_bvm->flags->BVM_CACHE_BLOCKING == FLAG_SET ){
        _cat_except(this_bvm);
    }

    mword save_ALLOC_BLOCKING = this_bvm->flags->MC_ALLOC_BLOCKING;
    mword save_GC_BLOCKING    = this_bvm->flags->MC_GC_BLOCKING;

    this_bvm->flags->BVM_CACHE_BLOCKING = FLAG_SET;
    this_bvm->flags->MC_ALLOC_BLOCKING  = FLAG_SET;
    this_bvm->flags->MC_GC_BLOCKING     = FLAG_SET;

#define X(a,b,c) cache_write_to_bvm(this_bvm, c, this_bvm->a);
CACHED_FIELDS
#undef X

//_trace;
//cache_write_to_bvm(this_bvm, BABEL_SYM_CODE_PTR,            this_bvm->code_ptr);
//_trace;
//cache_write_to_bvm(this_bvm, BABEL_SYM_RSTACK_PTR,          this_bvm->rstack_ptr);
//_trace;
//cache_write_to_bvm(this_bvm, BABEL_SYM_DSTACK_PTR,          this_bvm->dstack_ptr);
//_trace;
//cache_write_to_bvm(this_bvm, BABEL_SYM_USTACK_PTR,          this_bvm->ustack_ptr);
//_trace;
//cache_write_to_bvm(this_bvm, BABEL_SYM_SOFT_ROOT,           this_bvm->soft_root);
//_trace;
//cache_write_to_bvm(this_bvm, BABEL_SYM_LOCAL_ROOT,          this_bvm->local_root);
//_trace;
//cache_write_to_bvm(this_bvm, BABEL_SYM_ADVANCE_TYPE,        this_bvm->advance_type);
//_trace;
//cache_write_to_bvm(this_bvm, BABEL_SYM_STEPS,               this_bvm->steps);
//_trace;
//cache_write_to_bvm(this_bvm, BABEL_SYM_THREAD_ID,           this_bvm->thread_id);
//_trace;
//cache_write_to_bvm(this_bvm, BABEL_SYM_MASK_TABLE,          this_bvm->mask_table);
//_trace;
//
    this_bvm->flags->MC_GC_BLOCKING     = save_GC_BLOCKING;
    this_bvm->flags->MC_ALLOC_BLOCKING  = save_ALLOC_BLOCKING;
    this_bvm->flags->BVM_CACHE_BLOCKING = FLAG_CLR;

    this_bvm->flags->BVM_CACHE_DIRTY    = FLAG_CLR;

    return this_bvm;

}


//
//
mword *cache_read_from_bvm(bvm_cache *this_bvm, mword *hash){ // cache_read_from_bvm#

//    return _ith(this_bvm,
//                _luha(this_bvm, 
//                    tptr_detag(this_bvm, 
//                        tptr_detag(this_bvm, this_bvm->self)), 
//                    hash),
//                2);

    return _ith(this_bvm,
                trie_lookup_hash(this_bvm, 
                    tptr_detag(this_bvm, this_bvm->self), 
                    hash,
                    nil),
                2);

}


//
//
bvm_cache *cache_write_to_bvm(bvm_cache *this_bvm, mword *hash, mword *value){ // cache_write_to_bvm#

//    lci( _cdri(this_bvm,
//                _luha(this_bvm, 
//                    tptr_detag(this_bvm, 
//                        tptr_detag(this_bvm, this_bvm->self)), 
//                    hash),
//                2), 0) = value;
//

    // We use lookup instead of insert to avoid memory allocation issues...
    lci( _cdri(this_bvm,
                trie_lookup_hash(this_bvm, 
                    tptr_detag(this_bvm, this_bvm->self), 
                    hash, 
                    nil),
                2), 
        0 )         
        = value;

    return this_bvm;

}


//
//
bvm_cache *cache_cp(bvm_cache *src_bvm, bvm_cache *dest_bvm){ // cache_cp#

#define X(a,b,c) dest_bvm->a = src_bvm->a;
CACHED_FIELDS
#undef X

    dest_bvm->flags  = src_bvm->flags;
    dest_bvm->interp = src_bvm->interp;

    return dest_bvm;

}


// DEBUG USE ONLY
// FIXME: Add limits & privileges
bvm_cache *cache_dump(bvm_cache *this_bvm){ // cache_dump#

    _d((mword)this_bvm->self);

    _d((mword)this_bvm->code_ptr);

    _d((mword)this_bvm->rstack_ptr);

    _d((mword)this_bvm->dstack_ptr);

    _d((mword)this_bvm->ustack_ptr);

    _d((mword)this_bvm->sym_table);

    _d((mword)this_bvm->soft_root);

    _d((mword)this_bvm->thread_id);
    _d(rcl(rci(this_bvm->thread_id,0),0));

    _d((mword)this_bvm->steps);
    _d(rcl(rci(this_bvm->steps,0),0));

    _d((mword)this_bvm->advance_type);
    _d(rcl(rci(this_bvm->advance_type,0),0));

    _d((mword)this_bvm->mask_table);

    _d((mword)this_bvm->flags);

    _d((mword)this_bvm->flags->MC_ALLOC_BLOCKING);
    _d((mword)this_bvm->flags->MC_GC_BLOCKING);
    _d((mword)this_bvm->flags->MC_GC_PENDING);
    _d((mword)this_bvm->flags->MC_GC_PNR);
    _d((mword)this_bvm->flags->MC_GC_OP_RESTART);

    _d((mword)this_bvm->flags->BVM_INSTR_IN_PROGRESS);
    _d((mword)this_bvm->flags->BVM_INCOHERENT);
    _d((mword)this_bvm->flags->BVM_CACHE_INVALID);
    _d((mword)this_bvm->flags->BVM_CACHE_DIRTY);
    _d((mword)this_bvm->flags->BVM_CACHE_ONLY);
    _d((mword)this_bvm->flags->BVM_CACHE_BLOCKING);
    _d((mword)this_bvm->flags->BVM_SYMBOLS_DEFINED);
    _d((mword)this_bvm->flags->BVM_CLEAN);
    _d((mword)this_bvm->flags->BVM_INTERP_OP_TRACE);

    _d((mword)this_bvm->flags->NO_ASYNC);
    _d((mword)this_bvm->flags->NO_EXCEPT);

    _d((mword)this_bvm->interp);

    _d((mword)this_bvm->interp->cat_ex);
    _d((mword)this_bvm->interp->argc);
    _d((mword)this_bvm->interp->argv);
    _d((mword)this_bvm->interp->envp);
    _d((mword)this_bvm->interp->interp_argv);
    _d((mword)this_bvm->interp->mem);
    _d((mword)this_bvm->interp->nil);
    _d((mword)this_bvm->interp->jump_table);
    _d((mword)this_bvm->interp->empty_string);
    _d((mword)this_bvm->interp->utc_epoch);
    _d((mword)this_bvm->interp->srand);
    _d((mword)this_bvm->interp->null_hash);
    _d((mword)this_bvm->interp->thread_counter);
    _d((mword)this_bvm->interp->global_tick_count);

    _d((mword)this_bvm->interp->mem->primary->base_ptr);
    _d((mword)this_bvm->interp->mem->primary->alloc_ptr);
    _d((mword)this_bvm->interp->mem->primary->size);

    _d((mword)this_bvm->interp->mem->secondary->base_ptr);
    _d((mword)this_bvm->interp->mem->secondary->alloc_ptr);
    _d((mword)this_bvm->interp->mem->secondary->size);

    _d((mword)this_bvm->interp->mem->GC_count);
    _d((mword)this_bvm->interp->mem->op_restart_alloc_size);
    _d((mword)this_bvm->interp->mem->last_GC_tick_count);

   return this_bvm;

}


mword *cache_stats_array(bvm_cache *this_bvm){

//        lcl(temp,0) = this_bvm->interp->mem->primary->size;
//        lcl(temp,1) = mem_bank_in_use(this_bvm->interp->mem->primary);
//        lcl(temp,2) = (rcl(temp,0) - rcl(temp,1));

    mword *result = _newlfi(this_bvm, 66, 0);

    lcl(result, 0) = (mword)this_bvm->self;
    lcl(result, 1) = (mword)this_bvm->code_ptr;
    lcl(result, 2) = (mword)this_bvm->rstack_ptr;
    lcl(result, 3) = (mword)this_bvm->dstack_ptr;
    lcl(result, 4) = (mword)this_bvm->ustack_ptr;
    lcl(result, 5) = (mword)this_bvm->sym_table;
    lcl(result, 6) = (mword)this_bvm->soft_root;

//    mword *local_root;
//    mword *local_path;
//    mword *local_pwd;
    lcl(result, 7) = (mword)this_bvm->local_root;
    lcl(result, 8) = (mword)this_bvm->local_path;
    lcl(result, 9) = (mword)this_bvm->local_pwd;

    lcl(result,10) = (mword)this_bvm->thread_id;
    lcl(result,11) = rcl(rci(this_bvm->thread_id,0),0);
    lcl(result,12) = (mword)this_bvm->steps;
    lcl(result,13) = rcl(rci(this_bvm->steps,0),0);
    lcl(result,14) = (mword)this_bvm->advance_type;
    lcl(result,15) = rcl(rci(this_bvm->advance_type,0),0);

//    mword *bvm_initd;
//    mword dstack_depth;
//    mword dstack_diameter;
    lcl(result,16) = (mword)this_bvm->bvm_initd;
    lcl(result,17) = (mword)this_bvm->dstack_depth;
    lcl(result,18) = (mword)this_bvm->dstack_diameter;

    lcl(result,19) = (mword)this_bvm->mask_table;
    lcl(result,20) = (mword)this_bvm->flags;

    lcl(result,21) = this_bvm->flags->MC_ALLOC_BLOCKING;
    lcl(result,22) = this_bvm->flags->MC_GC_BLOCKING;
    lcl(result,23) = this_bvm->flags->MC_GC_PENDING;
    lcl(result,24) = this_bvm->flags->MC_GC_CONVERT_PENDING_TO_OP_RESTART;
    lcl(result,25) = this_bvm->flags->MC_GC_PNR;
    lcl(result,26) = this_bvm->flags->MC_GC_OP_RESTART;

    lcl(result,27) = this_bvm->flags->INTERP_BOOT_IN_PROGRESS;

    lcl(result,28) = this_bvm->flags->BVM_INSTR_IN_PROGRESS;
    lcl(result,29) = this_bvm->flags->BVM_INCOHERENT;
    lcl(result,30) = this_bvm->flags->BVM_SYMBOLS_DEFINED;
    lcl(result,31) = this_bvm->flags->BVM_CLEAN;
    lcl(result,32) = this_bvm->flags->BVM_INTERP_OP_TRACE;

    lcl(result,33) = this_bvm->flags->BVM_CODE_LIST_EMPTY;
    lcl(result,34) = this_bvm->flags->BVM_RETURN_TOS_ON_EXIT;

    lcl(result,35) = this_bvm->flags->BVM_CACHE_INVALID;
    lcl(result,36) = this_bvm->flags->BVM_CACHE_DIRTY;
    lcl(result,37) = this_bvm->flags->BVM_CACHE_ONLY;
    lcl(result,38) = this_bvm->flags->BVM_CACHE_BLOCKING;

    lcl(result,39) = this_bvm->flags->NO_ASYNC;
    lcl(result,40) = this_bvm->flags->NO_EXCEPT;

    lcl(result,41) = this_bvm->flags->MC_USE_MALLOC;

    lcl(result,42) = (mword)this_bvm->interp;

    lcl(result,43) = (mword)this_bvm->interp->cat_ex;
    lcl(result,44) = (mword)this_bvm->interp->argc;
    lcl(result,45) = (mword)this_bvm->interp->argv;
    lcl(result,46) = (mword)this_bvm->interp->envp;
    lcl(result,47) = (mword)this_bvm->interp->interp_argv;
    lcl(result,48) = (mword)this_bvm->interp->mem;
    lcl(result,49) = (mword)this_bvm->interp->nil;
    lcl(result,50) = (mword)this_bvm->interp->jump_table;
    lcl(result,51) = (mword)this_bvm->interp->empty_string;
    lcl(result,52) = (mword)this_bvm->interp->utc_epoch;
    lcl(result,53) = (mword)this_bvm->interp->srand;
    lcl(result,54) = (mword)this_bvm->interp->null_hash;
    lcl(result,55) = (mword)this_bvm->interp->thread_counter;
    lcl(result,56) = (mword)this_bvm->interp->global_tick_count;

    lcl(result,57) = (mword)this_bvm->interp->mem->primary->base_ptr;
    lcl(result,58) = (mword)this_bvm->interp->mem->primary->alloc_ptr;
    lcl(result,59) = (mword)this_bvm->interp->mem->primary->size;

    lcl(result,60) = (mword)this_bvm->interp->mem->secondary->base_ptr;
    lcl(result,61) = (mword)this_bvm->interp->mem->secondary->alloc_ptr;
    lcl(result,62) = (mword)this_bvm->interp->mem->secondary->size;

    lcl(result,63) = (mword)this_bvm->interp->mem->GC_count;
    lcl(result,64) = (mword)this_bvm->interp->mem->op_restart_alloc_size;
    lcl(result,65) = (mword)this_bvm->interp->mem->last_GC_tick_count;

    return result;

}



// Clayton Bauman 2014

