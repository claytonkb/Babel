// bvm.h
// 

#ifndef BVM_H
#define BVM_H

#include "babel.h"

#define bvm_sym_OLD(x,y)                                    \
    _ith(x,                                             \
            _luha(x,                                    \
                tptr_detag(x, tptr_detag(x, x->self)),  \
                y),                                     \
        2)


//mword *trie_lookup_hash(bvm_cache *this_bvm, mword *trie, mword *hash_key, mword *string_key){ // trie_lookup_hash#
#define bvm_sym(x,y)    _ith( x, trie_lookup_hash(x, tptr_detag(x, x->self),         y, nil), 2)
#define   lusym(x,y)    _ith( x, trie_lookup_hash(x, bvm_sym(x,BABEL_SYM_SOFT_ROOT), y, nil), 2)
#define   lusym2(x,y)   trie_lookup_hash(x, bvm_sym(x,BABEL_SYM_SOFT_ROOT), y, nil)
#define   exsym(x,y)    trie_exists( x, bvm_sym(x, BABEL_SYM_SOFT_ROOT), y, nil )
#define   insym(x,y,z)  trie_insert( x, bvm_sym(x, BABEL_SYM_SOFT_ROOT), y, nil, z )
#define   insym2(x,y,z) trie_insert( x, bvm_sym(x, BABEL_SYM_SOFT_ROOT), y, nil, z )

//#define trie_exists(bvm, trie, key, secondary_key) (!is_nil(trie_lookup_hash(bvm, trie, key, secondary_key)))


// see CACHED_FIELDS in babel.h
#define bvm_dstack_ptr(x)      bvm_sym(x, BABEL_SYM_DSTACK_PTR)
#define bvm_ustack_ptr(x)      bvm_sym(x, BABEL_SYM_USTACK_PTR)
#define bvm_rstack_ptr(x)      bvm_sym(x, BABEL_SYM_RSTACK_PTR)
#define bvm_code_ptr(x)        bvm_sym(x, BABEL_SYM_CODE_PTR)
#define bvm_soft_root(x)       bvm_sym(x, BABEL_SYM_SOFT_ROOT)
#define bvm_parent_bvm(x)      bvm_sym(x, BABEL_SYM_PARENT_BVM)
#define bvm_local_root(x)      bvm_sym(x, BABEL_SYM_LOCAL_ROOT)
#define bvm_advance_type(x)    bvm_sym(x, BABEL_SYM_ADVANCE_TYPE)
#define bvm_steps(x)           bvm_sym(x, BABEL_SYM_STEPS)
#define bvm_thread_id(x)       bvm_sym(x, BABEL_SYM_THREAD_ID)
#define bvm_mask_table(x)      bvm_sym(x, BABEL_SYM_MASK_TABLE)


//mword *_babel(bvm_cache *this_bvm, mword *loaded_bvm);
//mword *_babel(bvm_cache *this_bvm, mword *loaded_bvm, mword *arg_stack);
mword *_babel(bvm_cache *this_bvm, mword *loaded_bvm, mword *arg_stack, mword *sym_table);
mword *_babel_root(bvm_cache *this_bvm, mword *loaded_bvm);
bvm_cache *babel_root_code_injection_point(bvm_cache *this_bvm);

bvm_cache *bvm_new(bvm_cache *this_bvm);
bvm_cache *babel(bvm_cache *this_bvm);
bvm_cache *babels(bvm_cache *this_bvm);
bvm_cache *root(bvm_cache *this_bvm);
bvm_cache *argvop(bvm_cache *this_bvm);
bvm_cache *dstack_pop(bvm_cache *this_bvm);
bvm_cache *lusym_d(bvm_cache *this_bvm);
bvm_cache *insym_d(bvm_cache *this_bvm);

#endif //BVM_H

// Clayton Bauman 2014

