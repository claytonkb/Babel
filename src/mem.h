// mem.h
// 

#ifndef MEM_H
#define MEM_H

#include "babel.h"

// Memory size is 128MB
#define MEM_SIZE 1<<27

typedef struct {

    mword *base_ptr;
    mword *alloc_ptr;

} alloc_block; // alloc_block#

typedef struct {

    alloc_block *primary;
    alloc_block *secondary;

} mem_context; // mem_context#

mem_context *init_mem_context(void);
void destroy_mem_context(mem_context *m);
mword *mc_alloc(alloc_block *b, int size);
int mc_free(mword *p);
void copy_collect(mem_context *m);

#endif //MEM_H

// Clayton Bauman 2013

