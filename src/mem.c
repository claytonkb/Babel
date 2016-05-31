// mem.c
//

#include "babel.h"
#include "mem.h"
#include "bstruct.h"
#include "cache.h"
#include "introspect.h"
#include "pearson16.h"
#include "array.h"

//#define GC_TRACE

// ideally, this is the ONLY function that calls malloc()
//
void *mem_sys_alloc(int size){ // mem_sys_alloc#

#ifdef MEM_DEBUG
    mem_sys_alloc_count++;
    mem_sys_alloc_total += size;
#endif

    void *alloc_attempt = malloc(size);

    if(alloc_attempt == NULL){ // malloc failed
        _fatal("malloc failed");
    }

    return alloc_attempt;

}


// mem_new
//
void mem_new(bvm_cache *this_bvm, mword init_mem_size){ // mem_new#

#ifdef BABEL_RESET_TRACE
_trace;
#endif

#ifdef MEM_DEBUG
_d(init_mem_size);
#endif

    mem_context *m = mem_sys_alloc(sizeof(mem_context)); // XXX WAIVER(mem_sys_alloc) XXX //

//    mword init_mem_size = MEM_DEFAULT_MIN_ALLOC;

    m->primary   = mem_sys_alloc(sizeof(alloc_bank)); // XXX WAIVER(mem_sys_alloc) XXX //
    m->secondary = mem_sys_alloc(sizeof(alloc_bank)); // XXX WAIVER(mem_sys_alloc) XXX //

    m->GC_count  = 0;
    m->op_restart_alloc_size = 0;
    m->last_GC_tick_count = 0;

    m->primary->base_ptr   = mem_sys_alloc(init_mem_size); // XXX WAIVER(mem_sys_alloc) XXX //
    m->secondary->base_ptr = mem_sys_alloc(init_mem_size); // XXX WAIVER(mem_sys_alloc) XXX //

    m->primary->size   = init_mem_size;
    m->secondary->size = init_mem_size;

    mem_reset_bank(this_bvm, m->primary);
    mem_reset_bank(this_bvm, m->secondary);

    this_bvm->interp->mem = m; // Sets the global memory context

}


//
//
mword *mem_alloc(bvm_cache *this_bvm, mword sfield){ // *mem_alloc#

#ifdef PROF_MODE
    if(this_bvm->interp->profile->BVM_PROFILE_ENABLE)
        this_bvm->interp->profile->mem_alloc_count++;
#endif

    // mem_alloc is non-reentrant... this is enforced with the MC_ALLOC_BLOCKING flag
    if(this_bvm->flags->MC_ALLOC_BLOCKING == FLAG_SET){
        _fatal("this_bvm->flags->MC_ALLOC_BLOCKING == FLAG_SET");
    }

    mword alloc_request_size = mem_alloc_size(sfield)+1; // +1 is for s-field

    if(alloc_request_size >= MAX_ARRAY_SIZE){
        _fatal("alloc failed: alloc_request_size >= MAX_ARRAY_SIZE");
    }

    mword *return_ptr;

#ifdef MEM_DEBUG
    if(this_bvm->flags->MC_USE_MALLOC == FLAG_SET){
        return_ptr = (void*)mem_sys_alloc(BYTE_SIZE(alloc_request_size));  // XXX WAIVER(mem_sys_alloc) XXX //
        s(return_ptr) = sfield;
        return return_ptr;
    }
#endif

    this_bvm->flags->MC_ALLOC_BLOCKING = FLAG_SET;

    alloc_bank *b = this_bvm->interp->mem->primary;

    if((b->alloc_ptr - alloc_request_size) <= b->base_ptr){ // swap banks and set MC_GC_PENDING

        if(this_bvm->flags->MC_GC_PENDING == FLAG_CLR){

#ifdef GC_TRACE
_msg("MC_GC_PENDING");
#endif

            mem_swap_banks(this_bvm);
            b = this_bvm->interp->mem->primary; // update local copy
            this_bvm->flags->MC_GC_PENDING = FLAG_SET;

        }
        else{

#ifdef GC_TRACE
_msg("MC_GC_OP_RESTART");
#endif

            if(this_bvm->flags->MC_GC_INTERP_BLOCKING == FLAG_SET){
                _fatal("this_bvm->flags->MC_GC_INTERP_BLOCKING == FLAG_SET");
            }

            if(this_bvm->flags->INTERP_BOOT_IN_PROGRESS == FLAG_SET){
                _fatal("MC_GC_OP_RESTART while INTERP_BOOT_IN_PROGRESS");
            }

            this_bvm->flags->MC_GC_BLOCKING = FLAG_CLR;
            this_bvm->flags->MC_ALLOC_BLOCKING = FLAG_CLR;
            this_bvm->interp->mem->op_restart_alloc_size = alloc_request_size;
            _op_restart(this_bvm);

        }

    }

#if 0
    mword mbu = mem_bank_in_use(b);

    if( (mbu+alloc_request_size) >= b->size ){

        if(this_bvm->flags->MC_GC_PENDING == FLAG_CLR){

#ifdef GC_TRACE
_msg("MC_GC_PENDING");
#endif
            mem_swap_banks(this_bvm);
            b = this_bvm->interp->mem->primary; // update local copy
            this_bvm->flags->MC_GC_PENDING = FLAG_SET;

        }
        else{

#ifdef GC_TRACE
_msg("MC_GC_OP_RESTART");
#endif

            if(this_bvm->flags->MC_GC_INTERP_BLOCKING == FLAG_SET){
                _fatal("this_bvm->flags->MC_GC_INTERP_BLOCKING == FLAG_SET");
            }

            if(this_bvm->flags->INTERP_BOOT_IN_PROGRESS == FLAG_SET){
                _msg("MC_GC_OP_RESTART while INTERP_BOOT_IN_PROGRESS");
                _cat_except(this_bvm);
            }

            if(this_bvm->flags->MC_GC_BLOCKING == FLAG_SET){
                this_bvm->flags->MC_GC_BLOCKING = FLAG_CLR;
            }

            this_bvm->flags->MC_ALLOC_BLOCKING = FLAG_CLR;
            this_bvm->interp->mem->op_restart_alloc_size = alloc_request_size;
            _op_restart(this_bvm);

        }

    }
#endif

    b->alloc_ptr -= alloc_request_size;

#if 0
if(b->alloc_ptr <= b->base_ptr){
    _d((mword)b->alloc_ptr);
    _d((mword)b->base_ptr);
    _die;
}
#endif

    return_ptr = b->alloc_ptr+1;

    s(return_ptr) = sfield;

    this_bvm->flags->MC_ALLOC_BLOCKING = FLAG_CLR;
    this_bvm->flags->MC_GC_OP_RESTART  = FLAG_CLR;

//mword bounds_check = mem_bounds_check(this_bvm, return_ptr);
//_d(bounds_check);

    return return_ptr;

}


//
//
void mem_preemptive_op_restart(bvm_cache *this_bvm){ // mem_preemptive_op_restart#

// Refactor-in-progress; see arch_notes.txt

//    alloc_bank *b = this_bvm->interp->mem->primary;
//    mword mbu = mem_bank_in_use(b);
//
//    if(mbu > (0.9*b->size)){ // What if we use more than the 10% before GC runs?
//
//#ifdef GC_TRACE
//_trace;
//#endif
//
//        mem_swap_banks(this_bvm);
//        b = this_bvm->interp->mem->primary;
//        this_bvm->flags->MC_ALLOC_BLOCKING = FLAG_CLR;
//        this_bvm->interp->mem->op_restart_alloc_size = 0;
//        _op_restart(this_bvm);
//
//    }

}


// mem_copy_collect
//
bvm_cache *mem_copy_collect(bvm_cache *this_bvm){ // mem_copy_collect#

#ifdef GC_TRACE
_trace;
_d(this_bvm->flags->MC_GC_PENDING);
_d(this_bvm->flags->MC_GC_BLOCKING);
_d(this_bvm->flags->MC_GC_OP_RESTART);
_d(this_bvm->flags->MC_GC_PNR);
_d(this_bvm->flags->MC_GC_ON_EVERY_OP);
#endif

if(this_bvm->flags->BVM_INTERP_OP_TRACE == FLAG_SET){
    _say(" --> mem_copy_collect()");
}

    // mem_copy_collect is non-reentrant... this is enforced with the MC_GC_BLOCKING flag
    if(this_bvm->flags->MC_GC_BLOCKING == FLAG_SET){
        _fatal("garbage collection failed: MC_GC_BLOCKING");
    }

    // PNR flag catches allocations during operator execution after the point-of-no-return
    if(this_bvm->flags->MC_GC_PNR == FLAG_SET){ 
        _fatal("garbage collection failed: MC_GC_PNR");
    }

    this_bvm->flags->MC_GC_BLOCKING = FLAG_SET;

    mem_context *mem = this_bvm->interp->mem;

    mem->GC_count++;

    cache_flush(this_bvm);

    mword bs_byte_size = _mu(this_bvm, this_bvm->self);
    mword memory_demand_load = bs_byte_size;

#ifdef GC_TRACE
    _d(bs_byte_size);
#endif

#if 0
if(bs_byte_size > mem->primary->size){
    _d(bs_byte_size);
    _fatal("bs_byte_size > mem->primary->size");
}
#endif

    if(this_bvm->flags->MC_GC_OP_RESTART == FLAG_SET){

        memory_demand_load += mem->op_restart_alloc_size;

#ifdef GC_TRACE
    _d(memory_demand_load);
#endif

        mem_increment_alloc(this_bvm, mem, memory_demand_load);

    }
    else{

//        if(mem->last_GC_tick_count < this_bvm->interp->global_tick_count){

            if(memory_demand_load > MEM_HIGHWATER(mem->primary)){
                mem_increment_alloc(this_bvm, mem, memory_demand_load);
            }
            else if(memory_demand_load < MEM_LOWWATER(mem->primary)){
//                mem_decrement_alloc(this_bvm, mem, memory_demand_load);
                mem_isometric_alloc(this_bvm, mem);
            }
            else{
                mem_isometric_alloc(this_bvm, mem);
            }

//        }
//        else{
//            mem_increment_alloc(this_bvm, mem, memory_demand_load);
//        }

    }

    cache_update(this_bvm);

    if(mem->last_GC_tick_count == this_bvm->interp->global_tick_count){
        _msg("no progress");
    }

    mem->last_GC_tick_count = this_bvm->interp->global_tick_count;

    this_bvm->flags->MC_GC_SECONDARY_BANK_ALLOC = FLAG_CLR;
    this_bvm->flags->MC_GC_BLOCKING = FLAG_CLR;
    this_bvm->flags->MC_GC_PENDING  = FLAG_CLR;

#ifdef GC_TRACE
_msg("this_bvm->flags->MC_GC_PENDING = FLAG_CLR");
#endif

    return this_bvm;

}


// Allocate non-Babel memory, in bytes; XXX not initialized XXX
//
void *mem_alloc_non_babel(bvm_cache *this_bvm, mword size8){ // mem_alloc_non_babel#

    mword arlength = _array8_size(this_bvm, size8);

    mword *result = _newlf(this_bvm, arlength); // XXX WAIVER(_newlf) XXX //

    lcl(result, arlength-1) = 0; // alignment_word added for string safety

    this_bvm->flags->MC_GC_CONVERT_PENDING_TO_OP_RESTART = FLAG_SET;

    return result;
    

}


// FIXME SWAP BUG FIXME
//
// Start with 1<<16 mem...
//babel> (1 2 3) {dup cp 2 take} 20 times
//MC_GC_PENDING in mem_alloc(), src/mem.c line 115
//mem_copy_collect() in src/mem.c line 176
//mem_isometric_alloc() in src/mem.c line 302
//MC_GC_PENDING in mem_alloc(), src/mem.c line 115
//mem_copy_collect() in src/mem.c line 176
//mem_isometric_alloc() in src/mem.c line 302
//MC_GC_PENDING in mem_alloc(), src/mem.c line 115
//mem_copy_collect() in src/mem.c line 176
//mem_isometric_alloc() in src/mem.c line 302
//MC_GC_PENDING in mem_alloc(), src/mem.c line 115
//FATAL: garbage collection failed: attempted swap while MC_GC_PENDING in mem_swap_banks() at src/mem.c line 263
//Died at src/mem.c line 263
//clean_babel>

//
//
void mem_swap_banks(bvm_cache *this_bvm){ // mem_swap_banks#

#ifdef GC_TRACE
_trace;
#endif

//    if(this_bvm->flags->MC_GC_PENDING == FLAG_SET){
//        _fatal("mem_swap_banks while MC_GC_PENDING");
//    }

    if(this_bvm->flags->MC_GC_SECONDARY_BANK_ALLOC == FLAG_SET){
        _fatal("mem_swap_banks while MC_GC_SECONDARY_BANK_ALLOC");
    }

    this_bvm->flags->MC_GC_SECONDARY_BANK_ALLOC = FLAG_SET;

    mem_reset_bank(this_bvm, this_bvm->interp->mem->secondary);

//_d((mword)this_bvm->interp->mem->primary->base_ptr);
//_d((mword)this_bvm->interp->mem->secondary->base_ptr);

    alloc_bank *temp                    = this_bvm->interp->mem->primary;
    this_bvm->interp->mem->primary      = this_bvm->interp->mem->secondary;
    this_bvm->interp->mem->secondary    = temp;

//    this_bvm->flags->MC_GC_SECONDARY_BANK_ALLOC = FLAG_SET;


//_d((mword)this_bvm->interp->mem->primary->base_ptr);
//_d((mword)this_bvm->interp->mem->secondary->base_ptr);

}


// mem_reset_bank
//
void mem_reset_bank(bvm_cache *this_bvm, alloc_bank *b){ // mem_reset_bank#

#ifdef GC_TRACE
_trace;
#endif

    b->alloc_ptr  = (mword*)TOP_OF_ALLOC_BANK2(b);

}


// mem_bank_usage
//
mword mem_bank_usage(bvm_cache *this_bvm, alloc_bank *b){ // mem_bank_usage#

    return (TOP_OF_ALLOC_BANK(b) - b->alloc_ptr);

}


//
//
void mem_isometric_alloc(bvm_cache *this_bvm, mem_context *mem){ // mem_isometric_alloc#

#ifdef GC_TRACE
_trace;
#endif

    this_bvm->self = _cp_fast(this_bvm, this_bvm->self);

}


//
//
void mem_increment_alloc(bvm_cache *this_bvm, mem_context *mem, mword memory_demand_load){ // mem_increment_alloc#

#ifdef GC_TRACE
_trace;
#endif

    mword *old_primary   = mem->primary->base_ptr;
    mword *old_secondary = mem->secondary->base_ptr;

    if(mem->primary->size >= MEM_DEFAULT_MAX_ALLOC){
        _fatal("mem->primary->size >= MEM_DEFAULT_MAX_ALLOC");
    }

    do{
//        mem->primary->size   = mem->primary->size   * 4;
//        mem->secondary->size = mem->secondary->size * 4;
        mem->primary->size   = mem->primary->size   * 2;
        mem->secondary->size = mem->secondary->size * 2;
    }while(memory_demand_load > MEM_HIGHWATER(mem->primary));

    if((mem->primary->size+mem->secondary->size) > (mem_sys_available(this_bvm)/2)){ // Divide-by-2 required to catch out-of-memory on Windows
        mem_dump_stats(this_bvm);
        _fatal("ran out of memory"); //FIXME: hibernate and notify the user gracefully
    }

    mem->primary->base_ptr   = mem_sys_alloc(mem->primary->size);
    mem->secondary->base_ptr = mem_sys_alloc(mem->secondary->size);

    mem_reset_bank(this_bvm, mem->primary);
    mem_reset_bank(this_bvm, mem->secondary);

    this_bvm->self = _cp_fast(this_bvm, this_bvm->self);

    free(old_primary);
    free(old_secondary);

}


//
//
void mem_decrement_alloc(bvm_cache *this_bvm, mem_context *mem, mword memory_demand_load){ // mem_decrement_alloc#

#ifdef GC_TRACE
_trace;
#endif

    mword *old_primary   = mem->primary->base_ptr;
    mword *old_secondary = mem->secondary->base_ptr;

    mem->primary->size   = mem->primary->size   / 2;
    mem->secondary->size = mem->secondary->size / 2;

    mem->primary->base_ptr   = mem_sys_alloc(mem->primary->size);
    mem->secondary->base_ptr = mem_sys_alloc(mem->secondary->size);

    mem_reset_bank(this_bvm, mem->primary);
    mem_reset_bank(this_bvm, mem->secondary);

    this_bvm->self = _cp_fast(this_bvm, this_bvm->self);

    free(old_primary);
    free(old_secondary);

}


//FIXME: Dump to file
//
void mem_dump_core(bvm_cache *this_bvm){ // mem_dump_core#

    mword *i; 

    fprintf(stderr,"PRIMARY\n");

//    b->alloc_ptr  = TOP_OF_ALLOC_BANK(b);

    for(
        i=(this_bvm->interp->mem->primary->alloc_ptr)-1;
        i<=TOP_OF_ALLOC_BANK(this_bvm->interp->mem->primary);
        i++
    ){ 

        fprintf(stderr,"%08x %08x\n", (mword)i, *i);

    }

    fprintf(stderr,"SECONDARY\n");

    for(
        i=(this_bvm->interp->mem->secondary->alloc_ptr)-1;
        i<=TOP_OF_ALLOC_BANK(this_bvm->interp->mem->secondary);
        i++
    ){ 

        fprintf(stderr,"%08x %08x\n", (mword)i, *i);

    }

}


//
//
mword mem_primary_bounds_check(bvm_cache *this_bvm, mword *ptr){

    return ((ptr >= this_bvm->interp->mem->primary->base_ptr) 
                && (ptr <= TOP_OF_ALLOC_BANK(this_bvm->interp->mem->primary)));

}


//
//
mword mem_secondary_bounds_check(bvm_cache *this_bvm, mword *ptr){

    return ((ptr >= this_bvm->interp->mem->secondary->base_ptr) 
                && (ptr <= TOP_OF_ALLOC_BANK(this_bvm->interp->mem->secondary)));

}


// DEBUG USE ONLY
//
bvm_cache *mem_dump_stats(bvm_cache *this_bvm){ // mem_dump_stats#

    _d((mword)this_bvm->self);
//    _d(rcl(rci(this_bvm->thread_id,0),0));
    _d((mword)this_bvm->flags->MC_ALLOC_BLOCKING);
    _d((mword)this_bvm->flags->MC_GC_BLOCKING);
    _d((mword)this_bvm->flags->MC_GC_PENDING);
    _d((mword)this_bvm->flags->MC_GC_PNR);
    _d((mword)this_bvm->flags->MC_GC_OP_RESTART);
    _d((mword)this_bvm->flags->BVM_CACHE_INVALID);
    _d((mword)this_bvm->flags->BVM_CACHE_DIRTY);
    _d((mword)this_bvm->flags->BVM_CACHE_BLOCKING);
    _d((mword)this_bvm->interp->nil);
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


mword *mem_stats_array(bvm_cache *this_bvm){

//        lcl(temp,0) = this_bvm->interp->mem->primary->size;
//        lcl(temp,1) = mem_bank_in_use(this_bvm->interp->mem->primary);
//        lcl(temp,2) = (rcl(temp,0) - rcl(temp,1));

    mword *result = _newlfi(this_bvm, 19, 0);

    lcl(result,0) = this_bvm->flags->MC_ALLOC_BLOCKING;
    lcl(result,1) = this_bvm->flags->MC_GC_BLOCKING;
    lcl(result,2) = this_bvm->flags->MC_GC_PENDING;
    lcl(result,3) = this_bvm->flags->MC_GC_PNR;
    lcl(result,4) = this_bvm->flags->MC_GC_OP_RESTART;
    lcl(result,5) = this_bvm->flags->BVM_CACHE_INVALID;
    lcl(result,6) = this_bvm->flags->BVM_CACHE_DIRTY;
    lcl(result,7) = this_bvm->flags->BVM_CACHE_BLOCKING;
    lcl(result,8) = (mword)this_bvm->interp->nil;
    lcl(result,9) = this_bvm->interp->global_tick_count;

    lcl(result,10) = (mword)this_bvm->interp->mem->primary->base_ptr;
    lcl(result,11) = (mword)this_bvm->interp->mem->primary->alloc_ptr;
    lcl(result,12) = this_bvm->interp->mem->primary->size;

    lcl(result,13) = (mword)this_bvm->interp->mem->secondary->base_ptr;
    lcl(result,14) = (mword)this_bvm->interp->mem->secondary->alloc_ptr;
    lcl(result,15) = this_bvm->interp->mem->secondary->size;

    lcl(result,16) = this_bvm->interp->mem->GC_count;
    lcl(result,17) = this_bvm->interp->mem->op_restart_alloc_size;
    lcl(result,18) = this_bvm->interp->mem->last_GC_tick_count;

    return result;

}

//
//
mword mem_sys_available(bvm_cache *this_bvm){ // mem_sys_available#

#ifdef WINDOWS_TARGET

    MEMORYSTATUSEX statex;
    statex.dwLength = sizeof(statex);
    GlobalMemoryStatusEx(&statex);

    return MIN( (mword)statex.ullAvailPhys, this_bvm->interp->limits->max_bytes_mem_alloc );

#endif

}


// Clayton Bauman 2014


