// cache.h
// 

#ifndef CACHE_H
#define CACHE_H

bvm_cache *set_sym_ptr(bvm_cache *this_bvm, mword *hash, mword *payload);
mword *get_sym_ptr(bvm_cache *this_bvm, mword *hash);
bvm_cache *set_sym_val(bvm_cache *this_bvm, mword *hash, mword payload);
bvm_cache *coherent_write_cache(bvm_cache *this_bvm, mword *cache_field, mword *symbol, mword *payload);
bvm_cache *incoherent_write_cache(bvm_cache *this_bvm, void *field, void *payload);
bvm_cache *incoherent_write_symbol(bvm_cache *this_bvm, mword *symbol, void *payload);
bvm_cache *bvm_sym_flush_cache(bvm_cache *this_bvm);
bvm_cache *bvm_sym_update_cache(bvm_cache *this_bvm);
bvm_cache *bvm_write_cache(bvm_cache *this_bvm, void *field, void *value);
bvm_cache *bvm_cp_cache(bvm_cache *src_bvm, bvm_cache *dest_bvm);

#endif //CACHE_H

// Clayton Bauman 2014

