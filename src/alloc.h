// alloc.h
// 

#ifndef ALLOC_H
#define ALLOC_H

bvm_cache *free_op(bvm_cache *this_bvm);

#define balloc(x) malloc(x)
#define bfree(x)  free((mword*)(x)-1)

mword *alloc_stack_page(bvm_cache *this_bvm);
void init_stack_page(mword *stack_page);

bvm_cache *del(bvm_cache *this_bvm);
void _del(mword *bs);

#endif //ALLOC_H

// Clayton Bauman 2012

