// hash.h
// 

#ifndef HASH_H
#define HASH_H

#define BITS_PER_HASH_LEVEL (4)
#define HASH_LEVEL_SIZE     (1<<BITS_PER_HASH_LEVEL)

mword *new_hash_entry(mword *val, mword *hash);
mword *new_hash_key_entry(mword *val, mword *hash, mword *key);
void _insha(mword *hash_table, mword *val);
void _inskha(mword *hash_table, mword *val, mword *key);

#endif //HASH_H

// Clayton Bauman 2011

