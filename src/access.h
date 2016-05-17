// access.h
// 

#ifndef ACCESS_H
#define ACCESS_H

mword *_get(bvm_cache *this_bvm, mword *bs, mword *pbp);
mword _set(bvm_cache *this_bvm, mword *bs, mword *pbp, mword *payload);
bvm_cache *get(bvm_cache *this_bvm);
bvm_cache *set(bvm_cache *this_bvm);
bvm_cache *get2(bvm_cache *this_bvm);
bvm_cache *set2(bvm_cache *this_bvm);

#endif //ACCESS_H

// Clayton Bauman 2014

