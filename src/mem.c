// mem.c
//

// XXX Might have to perform a special memory check before doing a babelop
// due to potential cache-conflicts with mc_alloc if anything goes wrong...
// basically, anything that uses flush_bvm_cache or update_bvm_cache has
// to be careful that a memory error cannot occur.

/* memory guts

This is the interpreter's memory management subsystem and should not be 
confused with alloc.c which implements dynamic memory allocation operators
for use by the Babel programmer.

The allocator begins by allocating a bank of size MEM\_SIZE/2. MEM\_SIZE
indicates the largest amount of memory that the interpreter is allowed to
use. The alloc_ptr is set to the end of the allocated bank. Allocations 
occur by subtracting from alloc_ptr because, in certain cases, we can free
memory just by adding to alloc_ptr the size of the current elemented pointed
to by alloc_ptr.

Once the bank has been filled, the allocator allocates another bank of 
size MEM\_SIZE/2 and performs an "unload-load" on the entire BVM to move it
from the present bank to the new bank, where all unused memory will have
been released and the entire BVM will be defragmented, all in one step. The
old bank is freed back to the system. This is the only form of garbage-
collection.

This allocator/GC is *not* the final draft allocator because it's an old-
fashioned "stop-and-copy" GC. It will cause intermittent, awkward pauses 
during program execution (which can lead to bugs when dealing with I/O 
systems). It is being used only because it is very easy to ensure the 
implementation is bug-free. Gradually, the GC will be made more 
"incremental".

*/

#include "mem.h"
#include "load.h"
#include "tptr.h"
#include "string.h"


// mc_new
//
void mc_new(bvm_cache *this_bvm){ // mc_new#

    mem_context *m = sys_alloc(sizeof(mem_context)); // XXX WAIVER XXX

    m->primary   = sys_alloc(sizeof(alloc_bank)); // XXX WAIVER XXX
    m->secondary = sys_alloc(sizeof(alloc_bank)); // XXX WAIVER XXX

    m->primary->base_ptr   = sys_alloc(MWORDS(MEM_SIZE>>1)); // XXX WAIVER XXX
    m->secondary->base_ptr = sys_alloc(MWORDS(MEM_SIZE>>1)); // XXX WAIVER XXX

    m->primary->alloc_ptr   = TOP_OF_ALLOC_BANK(m->primary  );
    m->secondary->alloc_ptr = TOP_OF_ALLOC_BANK(m->secondary);

    this_bvm->interp->mem = m; // Sets the global memory context

}


//
//
void *sys_alloc(int size){ // *sys_alloc#

    void *attempt = malloc(size);
    if(attempt == NULL){ // malloc failed
        fatal("malloc failed");
    }

    return attempt;

}


//
//
mword *mc_alloc(bvm_cache *this_bvm, mword sfield){ // *mc_alloc#

    // mc_alloc is non-reentrant... this is enforced with 
    // the MC_ALLOC_BLOCKING flag
    if(this_bvm->flags->MC_ALLOC_BLOCKING == FLAG_SET){
        cat_except(this_bvm);
    }
    this_bvm->flags->MC_ALLOC_BLOCKING = FLAG_SET;

    alloc_bank *b = this_bvm->interp->mem->primary;


    mword mc_size = mc_alloc_size(sfield)+1;

//d(mc_size);

    if(b->alloc_ptr-mc_size < (b->base_ptr)){

        if(this_bvm->flags->MC_GC_PENDING == FLAG_SET){
            cat_except(this_bvm);
        }
        mc_swap_banks(this_bvm);
        b = this_bvm->interp->mem->primary;

//        if(this_bvm->flags->BVM_INSTR_IN_PROGRESS == FLAG_SET){
        this_bvm->flags->MC_GC_PENDING = FLAG_SET;

//        }
//        else{
//            mc_copy_collect(this_bvm);
//        }
    }


    b->alloc_ptr -= mc_size;


    mword *return_ptr = b->alloc_ptr+1;


    s(return_ptr) = sfield;


    this_bvm->flags->MC_ALLOC_BLOCKING = FLAG_CLR;

//mword usage = mc_bank_usage(this_bvm, this_bvm->interp->mem->primary);
//d(usage);

    return return_ptr;

}


// mc_copy_collect
//
bvm_cache *mc_copy_collect(bvm_cache *this_bvm){ // mc_copy_collect#

    // mc_copy_collect is non-reentrant... this is enforced with 
    // the MC_GC_BLOCKING flag
    if(this_bvm->flags->MC_GC_BLOCKING == FLAG_SET){
        fatal("garbage collection failed");
    }

    this_bvm->flags->MC_GC_BLOCKING = FLAG_SET;

    bvm_flush_cache(this_bvm);

    this_bvm->flags->MC_ALLOC_BLOCKING = FLAG_CLR;//trace;

    this_bvm->self = _cp(this_bvm, this_bvm->self);

//trace;
    mword usage = mc_bank_usage(this_bvm, this_bvm->interp->mem->primary);
//d(usage);

    if(usage > (0.9 * (MEM_SIZE>>1))){
        fatal("memory usage above redline");
    }

    //init_bvm_cache(this_bvm);

    bvm_update_cache(this_bvm);

    this_bvm->flags->MC_GC_BLOCKING = FLAG_CLR;
    this_bvm->flags->MC_GC_PENDING  = FLAG_CLR;

    return this_bvm;
    
}


//
//
void mc_swap_banks(bvm_cache *this_bvm){ // mc_swap_banks#

    mc_reset_bank(this_bvm, this_bvm->interp->mem->secondary);

    alloc_bank *temp                    = this_bvm->interp->mem->primary;
    this_bvm->interp->mem->primary      = this_bvm->interp->mem->secondary;
    this_bvm->interp->mem->secondary    = temp;

}


// mc_reset_bank
//
void mc_reset_bank(bvm_cache *this_bvm, alloc_bank *b){ // mc_reset_bank#

    b->alloc_ptr = TOP_OF_ALLOC_BANK(b);

}


// mc_bank_usage
//
mword mc_bank_usage(bvm_cache *this_bvm, alloc_bank *b){ // mc_bank_usage#

    return (TOP_OF_ALLOC_BANK(b) - b->alloc_ptr);

}


//
//
mc_dump_core(bvm_cache *this_bvm){

    mword *i; 

    fprintf(stderr,"PRIMARY\n");

    for(
        i=this_bvm->interp->mem->primary->alloc_ptr;
        i<this_bvm->interp->mem->primary->base_ptr;
        i++
    ){ 

        fprintf(stderr,"%08x %08x\n", (mword)i, *i);

    }

    fprintf(stderr,"SECONDARY\n");

    for(
        i=this_bvm->interp->mem->secondary->alloc_ptr;
        i<this_bvm->interp->mem->secondary->base_ptr;
        i++
    ){ 

        fprintf(stderr,"%08x %08x\n", (mword)i, *i);

    }

}


//
//
mc_log(bvm_cache *this_bvm){

    // add log file to bvm_cache...

}


// mc_destroy
//
void mc_destroy(bvm_cache *this_bvm){ // mc_destroy#

}


// mc_free
//
void mc_free(bvm_cache *this_bvm, mword *p){ // mc_free#

}


// mc_reclamate
//
void mc_reclamate(bvm_cache *this_bvm){ // mc_reclamate#

}


//// mc_alloc
////
//mword *mc_alloc(bvm_cache *this_bvm, mword sfield){
//
//trace;
//    // mc_alloc is non-reentrant... this is enforced with 
//    // the MC_ALLOC_BLOCKING flag
//    if(this_bvm->flags->MC_ALLOC_BLOCKING == FLAG_SET){
//        fatal("allocation failed");
//    }
//
//    this_bvm->flags->MC_ALLOC_BLOCKING = FLAG_SET;
//
//    alloc_bank *b = this_bvm->mem->primary;
//
//    mword mc_size = mc_alloc_size(sfield)+1;
//
//    if(b->alloc_ptr-mc_size < b->base_ptr){
//        mc_swap_banks(this_bvm);
//        if(this_bvm->flags->BVM_INSTR_IN_PROGRESS == FLAG_SET){
//trace;
//            this_bvm->flags->MC_GC_PENDING = FLAG_SET;
//        }
//        else{
//trace;
//            mc_copy_collect(this_bvm);
//        }
//    }
//trace;
//    b->alloc_ptr -= mc_size;
//
//    mword *return_ptr = b->alloc_ptr+1;
//
//    s(return_ptr) = sfield;
//
//    this_bvm->flags->MC_ALLOC_BLOCKING = FLAG_CLR;
//
//    return return_ptr;
//
//}
//

// Clayton Bauman 2013

