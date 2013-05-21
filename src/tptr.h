// tptr.h
// 

#ifndef TPTR_H
#define TPTR_H

#include "babel.h"

#define get_tptr(x) (mword*)c((mword*)x,HASH_SIZE+1)

mword *new_tptr(mword *hash, mword *list);
mword *tptr_extract_hash(mword *tptr);
mword *tptr_extract_ptr(mword *tptr); 
    //Different from get_tptr because get_tptr returns the ptr for 
    //THIS tptr whereas tptr_extract_ptr is recursive
mword *detag(mword *tptr);
mword *hard_detag(mword *tptr);

#endif //TPTR_H

// Clayton Bauman 2013

