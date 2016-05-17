// tptr.h
// 

#ifndef TPTR_H
#define TPTR_H

#define tptr_simple_detag(x) rci(x,HASH_SIZE+1)

mword *tptr_new(bvm_cache *this_bvm, const mword *hash, mword *bs);
mword *tptr_extract_hash(bvm_cache *this_bvm, mword *tptr);
void   tptr_update_hash(bvm_cache *this_bvm, mword *tptr, mword *hash);
mword *tptr_extract_ptr(bvm_cache *this_bvm, mword *tptr);
mword *tptr_detag(bvm_cache *this_bvm, mword *tptr);
mword *tptr_hard_detag(bvm_cache *this_bvm, mword *tptr);
mword *tptr_find_tag_unsafe(bvm_cache *this_bvm, mword *bs, mword *tag);
mword *tptr_find_tag(bvm_cache *this_bvm, mword *search_bs, mword *tag);

bvm_cache *detag_d(bvm_cache *this_bvm);
bvm_cache *find_tag_d(bvm_cache *this_bvm);

#endif //TPTR_H

// Clayton Bauman 2013

