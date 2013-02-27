// ref.h
// 

#ifndef REF_H
#define REF_H

#include "babel.h"

mword *_deref(mword *bs, mword *ref_list);
static mword *rderef(mword *bs, mword *ref_list);
mword *_chain_deref(mword *bs, mword *ref_list);

#endif //HASH_H

// Clayton Bauman 2013

