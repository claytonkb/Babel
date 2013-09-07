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
//#define HASH_ENTRY_VAL(x) ((mword*)c(x,1))
//#define HASH_ENTRY_KEY(x) ((mword*)c(x,2))
//#define HASH_ENTRY_CNT(x) ((mword*)c(x,3))
//#define HASH_ENTRY_BND(x) ((mword*)c(x,4))

//mword *new_hash_entry(mword *hash, mword *key, mword *val, mword ref_count, mword bounding);

// hash_insert#
#define hash_insert(hash_table, key, payload)       \
    _insha(     hash_table,                         \
                nil,                                \
                C2B(key),                           \
                new_hash_table_entry(  nil,         \
                                        C2B(key),   \
                                        payload ) );


mword *new_hash_table(void);
void _insha(mword *hash_table, mword *hash, mword *key, mword *entry);

mword *new_hash_table_entry(mword *hash, mword *key, mword *payload);
static void rinsha(mword *hash_table, mword *hash, mword *key, mword *entry, mword level);

mword _exha(mword *hash_table, mword *hash);
static mword rexha(mword *hash_table, mword *hash, mword level);

mword *_luha(mword *hash_table, mword *hash);
static mword *rluha(mword *hash_table, mword *hash, mword level);

mword _rmha(mword *hash_table, mword *hash);
static mword rrmha(mword *hash_table, mword *hash, mword level);

bvm_cache *newha(bvm_cache *this_bvm);
bvm_cache *insha(bvm_cache *this_bvm);
bvm_cache *luha(bvm_cache *this_bvm);
bvm_cache *exha(bvm_cache *this_bvm);
bvm_cache *rmha(bvm_cache *this_bvm);

static mword rkeysha(mword *hash_table, mword *hash, mword level);
mword _keysha(mword *hash_table, mword *hash);

//bvm_cache *inskha(bvm_cache *this_bvm);
//mword *_inskha(mword *hash_table, mword *key, mword *val);
//void _rinskha(mword *hash_table, mword *hash, mword *key, mword *val, mword level);
//
//bvm_cache *exha(bvm_cache *this_bvm);
//mword _exha(mword *hash_table, mword *hash);
//mword _rexha(mword *hash_table, mword *hash, mword level);
//
//bvm_cache *luha(bvm_cache *this_bvm);
//mword *_luha(mword *hash_table, mword *hash);
//mword *_rluha(mword *hash_table, mword *hash, mword level);


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

