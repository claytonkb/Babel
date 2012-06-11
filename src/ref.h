// ref.h
// 

#ifndef REF_H
#define REF_H

mword *_newref(mword *hash);
bvm_cache *ishref(bvm_cache *this_bvm);
bvm_cache *deref(bvm_cache *this_bvm);

//void newref(void);
//void ishref(void);
//
#endif //REF_H

// Clayton Bauman 2011

