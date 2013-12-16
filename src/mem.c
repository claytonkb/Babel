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

mem_context *init_mem_context(void){
}

void destroy_mem_context(mem_context *m){
}

mword *mc_alloc(alloc_block *b, int size){
}

int mc_free(mword *p){ // <-- if matches alloc_ptr, can be freed!
//      returns the amount of memory freed
}

void copy_collect(mem_context *m){
}

// Clayton Bauman 2013

