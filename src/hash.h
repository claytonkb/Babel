// hash.h
// 

#ifndef HASH_H
#define HASH_H

mword *mk_hash_entry(mword *val);
mword *mk_hash_key_entry(mword *val, mword *key);
void _insha(mword *hash_table, mword *val);
void _inskha(mword *hash_table, mword *val, mword *key);

#endif //HASH_H

// Clayton Bauman 2011

