// tptr.h
// 

#ifndef TPTR_H
#define TPTR_H

#include "babel.h"

#define TPTR_PTR      (HASH_SIZE+1)
#define get_tptr(x)   (mword*)c((mword*)x,TPTR_PTR)
//#define get_tptr2(x)   (mword*)(x+TPTR_PTR)
#define set_tptr(x,y) (mword*)c((mword*)x,TPTR_PTR)=(mword*)y;

bvm_cache *detag_op(bvm_cache *this_bvm);

bvm_cache *get_tag(bvm_cache *this_bvm);

//mword *new_tptr(this_bvm, mword *hash, mword *list);
mword *new_tptr(bvm_cache *this_bvm, const mword *hash, mword *bs);

mword *tptr_extract_hash(bvm_cache *this_bvm, mword *tptr);
void tptr_update_hash(bvm_cache *this_bvm, mword *tptr, mword *hash);

mword *tptr_extract_ptr(bvm_cache *this_bvm, mword *tptr); 
    //Different from get_tptr because get_tptr returns the ptr for 
    //THIS tptr whereas tptr_extract_ptr is recursive
mword *detag(bvm_cache *this_bvm, mword *tptr); 
mword *hard_detag(bvm_cache *this_bvm, mword *tptr);
bvm_cache *newtptrop(bvm_cache *this_bvm);

#endif //TPTR_H

// Clayton Bauman 2013

