// ref.h
// 

#ifndef REF_H
#define REF_H

#include "babel.h"

//mword *_deref(mword *bs, mword *ref_list);
mword *_deref(bvm_cache *this_bvm, mword *ref_list);
//static mword *rderef(mword *bs, mword *ref_list);
static mword *rderef(bvm_cache *this_bvm, mword *bs, mword *ref_list);
//mword *_chain_deref(mword *bs, mword *ref_list);
mword *_chain_deref(bvm_cache *this_bvm, mword *ref_list);
bvm_cache *deref(bvm_cache *this_bvm);

#endif //HASH_H

// Clayton Bauman 2013

