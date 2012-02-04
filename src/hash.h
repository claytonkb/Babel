// hash.h
// 

#ifndef HASH_H
#define HASH_H

#include "babel.h"

#define HASH_LEVEL_SIZE     (1<<BITS_PER_BYTE)

mword *new_hash_entry(mword *hash, mword *val, mword *key, mword ref_count);
//mword *new_hash_key_entry(mword *val, mword *hash, mword *key);
//void _insha(mword *hash_table, mword *hash, mword *val, mword level);
void _insha(mword *hash_table, mword *hash, mword *val, mword level);
void _inskha(mword *hash_table, mword *hash, mword *key, mword *val, mword level);
mword _exha(mword *hash_table, mword *hash, mword level);
mword *_luha(mword *hash_table, mword *hash, mword level);
mword *_rmha(mword *hash_table, mword *hash, mword level);
void rmha(void);
void insha(void);
void inskha(void);
void luha(void);
void exha(void);
void select_bit_test(void);
mword select_bit(mword *val, mword bit);

#endif //HASH_H

// Clayton Bauman 2011

