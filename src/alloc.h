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

#endif //ALLOC_H

// Clayton Bauman 2012

