// hash.h
// 

#ifndef HASH_H
#define HASH_H

#define HASH_ENTRY_HKEY(x,y) ( _ith(x, detag(x, y), 0) )
#define HASH_ENTRY_KEY(x,y)  ( _ith(x, detag(x, y), 1) )
#define HASH_ENTRY_PAY(x,y)  ( _ith(x, detag(x, y), 2) )

#define hash_entry_get_hkey(bvm,tag) ( _ith(bvm, tptr_detag(bvm, tag), 0) )
#define hash_entry_get_key(bvm,tag)  ( _ith(bvm, tptr_detag(bvm, tag), 1) )
#define hash_entry_get_pay(bvm,tag)  ( _ith(bvm, tptr_detag(bvm, tag), 2) )

// hash_insert#
#define hash_insert(hash_table, key, payload)       \
    _insha(this_bvm,      hash_table,                         \
                nil,                                \
                C2B(key),                           \
                new_hash_table_entry(this_bvm,   nil,         \
                                        C2B(key),   \
                                        payload ) );

mword *_entha(bvm_cache *this_bvm, mword *hash_table);
mword *_rentha(bvm_cache *this_bvm, mword *hash_table, mword level);
mword *hash_new_table(bvm_cache *this_bvm);
mword *hash_new_entry(bvm_cache *this_bvm, mword *hash, mword *key, mword *payload);
void _insha(bvm_cache *this_bvm, mword *hash_table, mword *hash, mword *key, mword *entry);
void _rinsha(bvm_cache *this_bvm, mword *hash_table, mword *hash, mword *key, mword *entry, mword level);
mword _exha(bvm_cache *this_bvm, mword *hash_table, mword *hash);
mword _rexha(bvm_cache *this_bvm, mword *hash_table, mword *hash, mword level);
mword *_luha(bvm_cache *this_bvm, mword *hash_table, mword *hash);
mword *_rluha(bvm_cache *this_bvm, mword *hash_table, mword *hash, mword level);
mword *_rluha(bvm_cache *this_bvm, mword *hash_table, mword *hash, mword level);
mword _rmha(bvm_cache *this_bvm, mword *hash_table, mword *hash);
mword _rrmha(bvm_cache *this_bvm, mword *hash_table, mword *hash, mword level);
mword _rrmha(bvm_cache *this_bvm, mword *hash_table, mword *hash, mword level);
void _insha_trie(bvm_cache *this_bvm, mword *hash_table, mword *hash, mword *key, mword *entry);
void _rinsha_trie(bvm_cache *this_bvm, mword *hash_table, mword *hash, mword *key, mword *entry, mword level);

bvm_cache *hash_8d(bvm_cache *this_bvm);
bvm_cache *hash_md(bvm_cache *this_bvm);
bvm_cache *newha(bvm_cache *this_bvm);
bvm_cache *luha_d(bvm_cache *this_bvm);
bvm_cache *insha_d(bvm_cache *this_bvm);
bvm_cache *inskha_d(bvm_cache *this_bvm);
bvm_cache *exha_d(bvm_cache *this_bvm);
bvm_cache *entsha_d(bvm_cache *this_bvm);
bvm_cache *rmha_d(bvm_cache *this_bvm);


#endif //HASH_H

// Clayton Bauman 2014

