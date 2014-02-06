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

// mc_init
//
void mc_init(bvm_cache *this_bvm){

    mem_context *m = malloc(sizeof(mem_context));

    m->primary   = malloc(sizeof(alloc_bank));
    m->secondary = malloc(sizeof(alloc_bank));

    m->primary->base_ptr   = malloc(MWORDS(MEM_SIZE>>1));
    m->secondary->base_ptr = malloc(MWORDS(MEM_SIZE>>1));

    m->primary->alloc_ptr   = TOP_OF_ALLOC_BANK(m->primary  );
    m->secondary->alloc_ptr = TOP_OF_ALLOC_BANK(m->secondary);

    this_bvm->mem = m; // Sets the global memory context

}


//
//
mword *mc_alloc(bvm_cache *this_bvm, mword sfield){

    // mc_alloc is non-reentrant... this is enforced with 
    // the MC_ALLOC_BLOCKING flag
    if(this_bvm->flags->MC_ALLOC_BLOCKING == FLAG_SET){
        fatal("allocation failed");
    }

    this_bvm->flags->MC_ALLOC_BLOCKING = FLAG_SET;

    alloc_bank *b = this_bvm->mem->primary;

    mword mc_size = mc_alloc_size(sfield)+1;

    if(b->alloc_ptr-mc_size < (b->base_ptr+1000)){
        mc_swap_banks(this_bvm);
        if(this_bvm->flags->BVM_INSTR_IN_PROGRESS == FLAG_SET){
            this_bvm->flags->MC_GC_PENDING = FLAG_SET;
        }
        else{
            mc_copy_collect(this_bvm);
        }
    }

    b->alloc_ptr -= mc_size;

    mword *return_ptr = b->alloc_ptr+1;

    s(return_ptr) = sfield;

    this_bvm->flags->MC_ALLOC_BLOCKING = FLAG_CLR;

    return return_ptr;

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

// mc_copy_collect
//
mword mc_copy_collect(bvm_cache *this_bvm){

    // mc_copy_collect is non-reentrant... this is enforced with 
    // the MC_GC_BLOCKING flag
    if(this_bvm->flags->MC_GC_BLOCKING == FLAG_SET){
        fatal("garbage collection failed");
    }

    this_bvm->flags->MC_GC_BLOCKING = FLAG_SET;

    //mc_swap_banks(this_bvm);

    flush_bvm_cache2(this_bvm);

    this_bvm->flags->MC_ALLOC_BLOCKING = FLAG_CLR;

    this_bvm->self = _cp(this_bvm, this_bvm->self);

trace;
mword usage = mc_bank_usage(this_bvm, this_bvm->mem->primary);
d(usage);

    update_bvm_cache(this_bvm);

    this_bvm->flags->MC_GC_BLOCKING = FLAG_CLR;
    this_bvm->flags->MC_GC_PENDING  = FLAG_CLR;

}


//
//
void mc_swap_banks(bvm_cache *this_bvm){

    mc_reset_bank(this_bvm, this_bvm->mem->secondary);

    alloc_bank *temp            = this_bvm->mem->primary;
    this_bvm->mem->primary      = this_bvm->mem->secondary;
    this_bvm->mem->secondary    = temp;

}


// mc_reset_bank
//
void mc_reset_bank(bvm_cache *this_bvm, alloc_bank *b){

    b->alloc_ptr = TOP_OF_ALLOC_BANK(b);

}


// mc_bank_usage
//
mword mc_bank_usage(bvm_cache *this_bvm, alloc_bank *b){

    return (TOP_OF_ALLOC_BANK(b) - b->alloc_ptr);

}


//
//
bvm_cache *flush_bvm_cache2(bvm_cache *this_bvm){ // flush_bvm_cache2#

    mword *self = this_bvm->self;
    (mword*)bvm_code_ptr(self)      = this_bvm->code_ptr;
    (mword*)bvm_rstack_ptr(self)    = this_bvm->rstack_ptr;
    (mword*)bvm_dstack_ptr(self)    = this_bvm->dstack_ptr;
    (mword*)bvm_ustack_ptr(self)    = this_bvm->ustack_ptr;
    (mword*)bvm_sym_table(self)     = this_bvm->sym_table;

//sym_update(this_bvm, BABEL_SYM_JUMP_TABLE  , this_bvm->jump_table);
////sym_update(this_bvm, BABEL_SYM_STEPS       , this_bvm->steps);
////sym_update(this_bvm, BABEL_SYM_ADVANCE_TYPE, this_bvm->advance_type);
////sym_update(this_bvm, BABEL_SYM_THREAD_ID   , this_bvm->thread_id);
//
    set_sym(this_bvm, "jump_table",   this_bvm->jump_table);
    set_sym(this_bvm, "steps",        this_bvm->steps);
    set_sym(this_bvm, "advance_type", this_bvm->advance_type);
    set_sym(this_bvm, "thread_id",    this_bvm->thread_id);

    return this_bvm;

}


//
//
mc_dump_core(bvm_cache *this_bvm){

    mword *i; 

    fprintf(stderr,"PRIMARY\n");

    for(
        i=this_bvm->mem->primary->alloc_ptr;
        i<this_bvm->mem->primary->base_ptr;
        i++
    ){ 

        fprintf(stderr,"%08x %08x\n", (mword)i, *i);

    }

    fprintf(stderr,"SECONDARY\n");

    for(
        i=this_bvm->mem->secondary->alloc_ptr;
        i<this_bvm->mem->secondary->base_ptr;
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
void mc_destroy(bvm_cache *this_bvm){

}


// mc_free
//
void mc_free(bvm_cache *this_bvm, mword *p){

}


// mc_reclamate
//
void mc_reclamate(bvm_cache *this_bvm){

}


// Clayton Bauman 2013

