// alloc.h
// 

#ifndef ALLOC_H
#define ALLOC_H

//void free_op(void);
//void _free_op(mword *mem);
//void del(void);
//void _del(mword *tree);

#define balloc(x) malloc(x)
#define bfree(x)  free((mword*)(x)-1)

mword *alloc_stack_page(bvm_cache *this_bvm);
void init_stack_page(mword *stack_page);

#endif //ALLOC_H

// Clayton Bauman 2012

