// hash.h
// 

#ifndef HASH_H
#define HASH_H

#include "babel.h"
//#include "list.h"

#define HASH_LEVEL_SIZE     (1<<BITS_PER_BYTE)

mword *new_hash_entry(mword *hash, mword *key, mword *val, mword ref_count, mword bounding);

bvm_cache *inskha(bvm_cache *this_bvm);
mword *_inskha(mword *hash_table, mword *key, mword *val);
void _rinskha(mword *hash_table, mword *hash, mword *key, mword *val, mword level);

//mword *new_hash_entry(mword *hash, mword *val, mword *key, mword ref_count);
//
//void exha(void);
//mword *_exha(mword *hash_table, mword *hash);
//mword _exha_tree(mword *hash_table, mword *hash, mword level);
//
//void luha(void);
//mword *_luha(mword *hash_table, mword *hash);
//mword *_luha_tree(mword *hash_table, mword *hash, mword level);
//
//void rmha(void);
//void _rmha(mword *hash_table,mword *hash);
//mword *_rmha_tree(mword *hash_table, mword *hash, mword level);
//
//void insha(void);
//mword *_insha(mword *hash_table, mword *hash, mword *val);
//void _insha_tree(mword *hash_table, mword *hash, mword *val, mword level);
//
//void inskha(void);
//mword *_inskha(mword *hash_table, mword *hash, mword *val);
//void _inskha_tree(mword *hash_table, mword *hash, mword *key, mword *val, mword level);
//
//void keysha(void);
//void _keysha(mword *hash_table, mword **list);
//
//void select_bit_test(void);
//mword select_bit(mword *val, mword bit);
//
#endif //HASH_H

// Clayton Bauman 2011

