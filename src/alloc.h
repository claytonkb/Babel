// alloc.h
// 

#ifndef ALLOC_H
#define ALLOC_H

bvm_cache *free_op(bvm_cache *this_bvm);

//#define balloc(x) _balloc(x)
//#define bfree(x)  free((mword*)(x)-1)

//mword *_balloc(mword size);

bvm_cache *del(bvm_cache *this_bvm);
void _del(mword *bs);

#endif //ALLOC_H

// Clayton Bauman 2012

