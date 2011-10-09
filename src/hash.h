// hash.h
// 

#ifndef HASH_H
#define HASH_H

#include "babel.h"

#define HASH_LEVEL_SIZE     (1<<BITS_PER_BYTE)

mword *new_hash_entry(mword *val, mword *hash);
mword *new_hash_key_entry(mword *val, mword *hash, mword *key);
void   _insha(mword *hash_table, char *hash, mword *val, mword level);
void   _inskha(mword *hash_table, mword *val, mword *key);
mword _exha(mword *hash_table, char *hash, mword *val, mword level);
mword *_luha(mword *hash_table, char *hash, mword *val, mword level);
void insha(void);

#endif //HASH_H

// Clayton Bauman 2011

