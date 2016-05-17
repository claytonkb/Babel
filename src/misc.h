// misc.h
// 

#ifndef MISC_H
#define MISC_H

bvm_cache *randlf_d(bvm_cache *this_bvm);
bvm_cache *dev(bvm_cache *this_bvm);
bvm_cache *deva(bvm_cache *this_bvm);
bvm_cache *timems(bvm_cache *this_bvm);
bvm_cache *fnord(bvm_cache *this_bvm);
bvm_cache *semicolon(bvm_cache *this_bvm);

//mword *_deva(bvm_cache *this_bvm, mword *op0, mword *op1, mword *op2);
//mword *_deva(bvm_cache *this_bvm, mword *op0, mword *op1);
mword *_deva(bvm_cache *this_bvm, mword *op0);
mword *_randlf(bvm_cache *this_bvm, mword num_words);
void _semicolon(bvm_cache *this_bvm, mword *bs);
mword time_ms(void);

#endif //MISC_H

// Clayton Bauman 2014

