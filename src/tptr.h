// tptr.h
// 

#ifndef TPTR_H
#define TPTR_H

#include "babel.h"

#define TPTR_PTR      HASH_SIZE+1
#define get_tptr(x)   (mword*)c((mword*)x,TPTR_PTR)
#define set_tptr(x,y) (mword*)c((mword*)x,TPTR_PTR)=(mword*)y;

//mword *new_tptr(mword *hash, mword *list);
mword *new_tptr(const mword *hash, mword *bs);

mword *tptr_extract_hash(mword *tptr);
mword *tptr_extract_ptr(mword *tptr); 
    //Different from get_tptr because get_tptr returns the ptr for 
    //THIS tptr whereas tptr_extract_ptr is recursive
mword *detag(mword *tptr);
mword *hard_detag(mword *tptr);

#endif //TPTR_H

// Clayton Bauman 2013

