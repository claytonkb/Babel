// pearson16.h

#ifndef PEARSON16_H
#define PEARSON16_H

#define PERM_TABLE_WIDTH 256

extern const char pearson16_byte_perm[4096];
mword *_hash8(bvm_cache *this_bvm, mword *key);
mword *_hash(bvm_cache *this_bvm, mword *key);
mword *_pearson16(bvm_cache *this_bvm, mword* sinit, mword *skey, mword strlen);
mword *_pearson_marsaglia(bvm_cache *this_bvm, mword* sinit, mword *skey, mword strlen);
mword  _marsaglia_fuzz(mword reset);

#endif //PEARSON16_H

//Clayton Bauman 2014

