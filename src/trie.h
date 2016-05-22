// trie.h
// 

#ifndef TRIE_H
#define TRIE_H

#define trie_new_entry(bvm,payload,secondary_key,key) _mkls(bvm, 3, payload, secondary_key, key)  // trie_new_entry#

#define trie_entry_get_key(bvm,entry)           _ith(bvm, entry, 0) // trie_entry_get_key#
#define trie_entry_get_secondary_key(bvm,entry) _ith(bvm, entry, 1) // trie_entry_get_secondary_key#
#define trie_entry_get_payload(bvm,entry)       _ith(bvm, entry, 2) // trie_entry_get_payload#
#define trie_entry_get_payload2(bvm,entry)       _cdri(bvm, entry, 2) // trie_entry_get_payload2#

#define trie_entry_set_payload(bvm,entry,payload)     (lci(trie_entry_get_payload(bvm,entry), 0)=(payload)) // trie_entry_set_payload#
#define trie_entry_set_payload2(bvm,entry,payload)    (lci(trie_entry_get_payload2(bvm,entry),0)=(payload)) // trie_entry_set_payload2#

#define trie_exists(bvm, trie, key, secondary_key) (!is_nil(trie_lookup_hash(bvm, trie, key, secondary_key))) // trie_exists#

mword *trie_new(bvm_cache *this_bvm);

mword *trie_new_cell(bvm_cache *this_bvm, mword *key, mword *secondary_key, mword *payload);
mword *trie_new_cell2(bvm_cache *this_bvm, mword *key, mword *secondary_key, mword *payload);

void trie_insert(bvm_cache *this_bvm, mword *trie, mword *key, mword *secondary_key, mword *payload);
void rtrie_insert(bvm_cache *this_bvm, mword *trie, mword *key, mword *entry, mword level);

//void trie_insert2(bvm_cache *this_bvm, mword *trie, mword *key, mword *secondary_key, mword *payload);
void rtrie_insert2(bvm_cache *this_bvm, mword *trie, mword *key, mword *secondary_key, mword *payload, mword level);

mword *trie_lookup_hash(bvm_cache *this_bvm, mword *trie, mword *hash_key, mword *string_key);
mword *trie_lookup_binary(bvm_cache *this_bvm, mword *trie, mword *binary_key);
mword *rtrie_lookup(bvm_cache *this_bvm, mword *trie, mword *key, mword level);

mword trie_remove(bvm_cache *this_bvm, mword *trie, mword *key, mword *secondary_key);
mword rtrie_remove(bvm_cache *this_bvm, mword *trie, mword *key, mword level);

mword *rtrie_entries(bvm_cache *this_bvm, mword *trie, mword level);
mword *trie_entries(bvm_cache *this_bvm, mword *trie);

#endif //TRIE_H

// Clayton Bauman 2014

