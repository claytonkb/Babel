// hash.h
// 

#ifndef HASH_H
#define HASH_H

#include "babel.h"

//#include "list.h"

//#define HASH_LEVEL_SIZE     (1<<BITS_PER_BYTE)
//
//#define opposite_side(x) ((x==0)?1:0)
//
//#define HASH_ENTRY_SIZE 5
//
//#define HASH_ENTRY_REF(x) ((mword*)c(x,0))
//#define HASH_ENTRY_KEY(x) ((mword*)c(x,2))
//#define HASH_ENTRY_CNT(x) ((mword*)c(x,3))
//#define HASH_ENTRY_BND(x) ((mword*)c(x,4))

//mword *new_hash_entry(mword *hash, mword *key, mword *val, mword ref_count, mword bounding);

#define HASH_ENTRY_HKEY(x,y) ( _ith(x, detag(x, y), 0) )
#define HASH_ENTRY_KEY(x,y)  ( _ith(x, detag(x, y), 1) )
#define HASH_ENTRY_PAY(x,y)  ( _ith(x, detag(x, y), 2) )
//#define HASH_ENTRY_PAY(x,y)  ( is_nil(y) ? nil : _ith(x, detag(x, y), 2) )

// hash_insert#
#define hash_insert(hash_table, key, payload)       \
    _insha(this_bvm,      hash_table,                         \
                nil,                                \
                C2B(key),                           \
                new_hash_table_entry(this_bvm,   nil,         \
                                        C2B(key),   \
                                        payload ) );

void sym_update(bvm_cache *this_bvm, mword *hash, mword *new_payload);
void sym_update_val(bvm_cache *this_bvm, mword *hash, mword new_payload);

mword *new_hash_table(bvm_cache *this_bvm);
void _insha(bvm_cache *this_bvm, mword *hash_table, mword *hash, mword *key, mword *entry);

mword *new_hash_table_entry(bvm_cache *this_bvm, mword *hash, mword *key, mword *payload);

//static void rinsha(mword *hash_table, mword *hash, mword *key, mword *entry, mword level);
static void rinsha(bvm_cache *this_bvm, mword *hash_table, mword *hash, mword *key, mword *entry, mword level);

//mword _exha(mword *hash_table, mword *hash);
mword _exha(bvm_cache *this_bvm, mword *hash_table, mword *hash);

//static mword rexha(mword *hash_table, mword *hash, mword level);
static mword rexha(bvm_cache *this_bvm, mword *hash_table, mword *hash, mword level);

mword *_luha(bvm_cache *this_bvm, mword *hash_table, mword *hash);

//static mword *rluha(mword *hash_table, mword *hash, mword level);
static mword *rluha(bvm_cache *this_bvm, mword *hash_table, mword *hash, mword level);

mword _rmha(bvm_cache *this_bvm, mword *hash_table, mword *hash);

//static mword rrmha(mword *hash_table, mword *hash, mword level);
static mword rrmha(bvm_cache *this_bvm, mword *hash_table, mword *hash, mword level);

bvm_cache *newha(bvm_cache *this_bvm);
bvm_cache *insha(bvm_cache *this_bvm);
bvm_cache *luha(bvm_cache *this_bvm);
bvm_cache *exha(bvm_cache *this_bvm);
bvm_cache *rmha(bvm_cache *this_bvm);

bvm_cache *inskha(bvm_cache *this_bvm);

//static mword rkeysha(mword *hash_table, mword *hash, mword level);
//mword _keysha(mword *hash_table, mword *hash);

mword *_entha(bvm_cache *this_bvm, mword *hash_table);
mword *rentha(bvm_cache *this_bvm, mword *hash_table, mword level);

bvm_cache *keysha(bvm_cache *this_bvm);

//bvm_cache *inskha(bvm_cache *this_bvm);
//mword *_inskha(mword *hash_table, mword *key, mword *val);
//void _rinskha(mword *hash_table, mword *hash, mword *key, mword *val, mword level);
//
//bvm_cache *exha(bvm_cache *this_bvm);
//mword _exha(mword *hash_table, mword *hash);
//mword _rexha(mword *hash_table, mword *hash, mword level);
//
//bvm_cache *luha(bvm_cache *this_bvm);
//mword *_luha(this_bvm, mword *hash_table, mword *hash);
//mword *_rluha(mword *hash_table, mword *hash, mword level);


//mword *new_hash_entry(mword *hash, mword *val, mword *key, mword ref_count);
//
//void exha(void);
//mword *_exha(mword *hash_table, mword *hash);
//mword _exha_tree(mword *hash_table, mword *hash, mword level);
//
//void luha(void);
//mword *_luha(this_bvm, mword *hash_table, mword *hash);
//mword *_luha_tree(mword *hash_table, mword *hash, mword level);
//
//void rmha(void);
//void _rmha(this_bvm, mword *hash_table,mword *hash);
//mword *_rmha_tree(mword *hash_table, mword *hash, mword level);
//
//void insha(void);
//mword *_insha(this_bvm, mword *hash_table, mword *hash, mword *val);
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

