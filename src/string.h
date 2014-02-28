// string.h
// 

#ifndef STRING_H
#define STRING_H

//void b2c(void);
mword *_b2c(bvm_cache *this_bvm, mword *string);
mword *_c2b(bvm_cache *this_bvm, char *string, mword max_safe_length);

bvm_cache *ar2str(bvm_cache *this_bvm);
bvm_cache *str2ar(bvm_cache *this_bvm);
bvm_cache *catoi(bvm_cache *this_bvm);
bvm_cache *dec2ci(bvm_cache *this_bvm);
bvm_cache *cu2dec(bvm_cache *this_bvm);
bvm_cache *ci2dec(bvm_cache *this_bvm);
bvm_cache *hex2cu(bvm_cache *this_bvm);
bvm_cache *cu2hex(bvm_cache *this_bvm);
bvm_cache *ordop(bvm_cache *this_bvm);
bvm_cache *cr(bvm_cache *this_bvm);
bvm_cache *charop(bvm_cache *this_bvm);

#endif //STRING_H

// Clayton Bauman 2011

