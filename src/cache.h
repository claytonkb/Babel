// cache.h
// 

#ifndef CACHE_H
#define CACHE_H

#define set_advance_type(bvm,val)   (lcl(rci(bvm->advance_type,0),0) = val)
#define get_advance_type(bvm)       (rcl(rci(bvm->advance_type,0),0))

#define set_code_ptr(bvm,val)       (lci(bvm->code_ptr,0) = val)

#define set_rstack_ptr(bvm,val)     (lci(bvm->rstack_ptr,0) = val)

bvm_cache *cache_new(bvm_cache *this_bvm, bvm_cache *new_bvm, mword *loaded_bvm);
bvm_cache *cache_update(bvm_cache *this_bvm);
bvm_cache *cache_flush(bvm_cache *this_bvm);
bvm_cache *cache_cp(bvm_cache *src_bvm, bvm_cache *dest_bvm);
mword     *cache_read_from_bvm(bvm_cache *this_bvm, mword *hash);
bvm_cache *cache_write_to_bvm(bvm_cache *this_bvm, mword *hash, mword *value);
bvm_cache *cache_dump(bvm_cache *this_bvm);
mword *cache_stats_array(bvm_cache *this_bvm);

#endif //CACHE_H

// Clayton Bauman 2014

