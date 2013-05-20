// tptr.h
// 

#ifndef TPTR_H
#define TPTR_H

#include "babel.h"

#define get_tptr(x) (mword*)c((mword*)x,HASH_SIZE+1)

mword *new_tptr(mword *hash, mword *list);
mword *extract_hash(mword *tptr);
mword *detag(mword *tptr);
mword *hard_detag(mword *tptr);

#endif //TPTR_H

// Clayton Bauman 2013

