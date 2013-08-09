// pearson16.h

#ifndef PEARSON16_H
#define PEARSON16_H

//char *pearson16_byte_perm;
mword *new_hash(void);
//void pearson16_init(void);
extern const char pearson16_byte_perm[4096];

bvm_cache *hash8(bvm_cache *this_bvm);
mword *_hash8(mword *key);

//void phash8(void);
//void phash(void);
//void hash8(void);
//void hash(void);
////mword *_pearson16(mword* sinit, mword *skey);
mword *_pearson16(mword* sinit, mword *skey, mword strlen);

//FIXME: 32-bit specific:
//#define HASH_SIZE 4

#endif //PEARSON16_H

//Clayton Bauman 2011

