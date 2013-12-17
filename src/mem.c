// mem.c
//

/* memory guts

This is the interpreter's memory management subsystem and should not be 
confused with alloc.c which implements dynamic memory allocation operators
for use by the Babel programmer.

The allocator begins by allocating a block of size MEM\_SIZE/2. MEM\_SIZE
indicates the largest amount of memory that the interpreter is allowed to
use. The alloc_ptr is set to the end of the allocated block. Allocations 
occur by subtracting from alloc_ptr because, in certain cases, we can free
memory just by adding to alloc_ptr the size of the current elemented pointed
to by alloc_ptr.

Once the block has been filled, the allocator allocates another block of 
size MEM\_SIZE/2 and performs an "unload-load" on the entire BVM to move it
from the present block to the new block, where all unused memory will have
been released and the entire BVM will be defragmented, all in one step. The
old block is freed back to the system. This is the only form of garbage-
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

// mc_init
//
mem_context *mc_init(void){

    mem_context *m = malloc(sizeof(mem_context));

    m->primary   = malloc(sizeof(alloc_block));
    m->secondary = malloc(sizeof(alloc_block));

    m->primary->base_ptr   = malloc(MWORDS(MEM_SIZE>>1));
    m->secondary->base_ptr = malloc(MWORDS(MEM_SIZE>>1));

//    m->primary->alloc_ptr   = m->primary->base_ptr   + (MEM_SIZE>>1)-1;
//    m->secondary->alloc_ptr = m->secondary->base_ptr + (MEM_SIZE>>1)-1;

    m->primary->alloc_ptr   = TOP_OF_ALLOC_BLOCK(m->primary  );
    m->secondary->alloc_ptr = TOP_OF_ALLOC_BLOCK(m->secondary);

    return m;

}

// mc_destroy
//
// Undoes mc_init
//
void mc_destroy(mem_context *m){

    free(m->primary->base_ptr  );
    free(m->secondary->base_ptr);

    free(m->primary);
    free(m->secondary);

    free(m);

}

// mc_alloc
//
mword *mc_alloc(mem_context *m, mword sfield){

    alloc_block *b = m->primary;
    mword mc_size = mc_alloc_size(sfield);

    if(b->alloc_ptr-mc_size-1 < b->base_ptr){

        // copy-collect

    }

    b->alloc_ptr -= (mc_size+1);

    mword *return_ptr = b->alloc_ptr+2;

    r(return_ptr) = ALLOC_ENTRY_IN_USE;
    s(return_ptr) = sfield;

    return return_ptr;

}

// mc_free
//
void mc_free(alloc_block *b, mword *p){

    r(p) = ALLOC_ENTRY_FREE;

    mc_reclamate(b);

}

// mc_reclamate
//
void mc_reclamate(alloc_block *b){

    while( b->alloc_ptr < TOP_OF_ALLOC_BLOCK(b) 
            && *(b->alloc_ptr+1) == ALLOC_ENTRY_FREE ){
        b->alloc_ptr += mc_alloc_size( *(b->alloc_ptr+2) );
    }

}

// mc_copy_collect
//
void mc_copy_collect(bvm_cache *this_bvm){

    flush_bvm_cache(this_bvm);

    mword *temp_bvm = _unload(this_bvm->self);

    
    // swap primary<->secondary alloc blocks
    // _unload this_bvm

}


// Clayton Bauman 2013

