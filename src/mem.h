// mem.h
// 

#ifndef MEM_H
#define MEM_H

#define MASKIFY(x) (~(x-1))   // x must be power-of-2
#define ARCH_PAGE_SIZE 4096 // ARCH_PAGE_SIZE#
#define ARCH_PAGE_MASK MASKIFY(ARCH_PAGE_SIZE) // ARCH_PAGE_MASK#

// MAX is BIT_SIZE-1 because the MSB of the s-field defines array-type
#define MAX_ARRAY_SIZE (1<<(MWORD_BIT_SIZE-1)) // MAX_ARRAY_SIZE#

#define MEM_REDLINE     ( 0.9 *(MEM_SIZE>>1)) // MEM_REDLINE#

#define MEM_HIGHWATER(bank)   (bank->size * 0.50) // MEM_HIGHWATER#
#define MEM_LOWWATER(bank)    (bank->size * 0.08) // MEM_LOWWATER#

// MEM_DEFAULT_MIN_ALLOC is the main memory allocation control for now...
#define MEM_DEFAULT_MIN_ALLOC (1<<24)           // MEM_DEFAULT_MIN_ALLOC#
#define MEM_LOW (1<<16)                         // MEM_LOW#  
#define MEM_DEFAULT_MAX (1<<27)                 // MEM_DEFAULT_MAX#

#define MEM_DEFAULT_MAX_ALLOC (1<<27)            // MEM_DEFAULT_MAX_ALLOC#

#define MEM_LIVELOCK_LIMIT 4 // This is a scaling factor... may require re-thinking

#define MEM_ALLOC_REDLINE (0.01 * MEM_DEFAULT_MAX) // MEM_ALLOC_REDLINE#

#define TOP_OF_ALLOC_BANK(x) (x->base_ptr+(x->size-1)) // TOP_OF_ALLOC_BANK#
#define mem_bank_in_use(x) ((mword)TOP_OF_ALLOC_BANK(x) - (mword)x->alloc_ptr) // mem_bank_in_use#

#define MEM_ALLOC_AVAIL(x) ( x->interp->mem->primary->size - mem_bank_in_use(x->interp->mem->primary) )

mword *mem_bulk_alloc(bvm_cache *this_bvm, mword sfield, mword count);

void *mem_sys_alloc(int size);
void mem_new(bvm_cache *this_bvm);
void mem_destroy(bvm_cache *this_bvm);

mword *mem_alloc(bvm_cache *this_bvm, mword sfield);
void *mem_alloc_non_babel(bvm_cache *this_bvm, mword size8);

bvm_cache *mem_copy_collect(bvm_cache *this_bvm);

void mem_swap_banks(bvm_cache *this_bvm);
void mem_reset_bank(bvm_cache *this_bvm, alloc_bank *b);

mword mem_bank_usage(bvm_cache *this_bvm, alloc_bank *b);

void mem_dump_core(bvm_cache *this_bvm);

void mem_isometric_alloc(bvm_cache *this_bvm, mem_context *mem);
void mem_decrement_alloc(bvm_cache *this_bvm, mem_context *mem, mword memory_demand_load);
void mem_increment_alloc(bvm_cache *this_bvm, mem_context *mem, mword memory_demand_load);

mword mem_primary_bounds_check(bvm_cache *this_bvm, mword *ptr);
mword mem_secondary_bounds_check(bvm_cache *this_bvm, mword *ptr);

void mem_preemptive_op_restart(bvm_cache *this_bvm);

mword time_ms(void);

bvm_cache *mem_dump_stats(bvm_cache *this_bvm);
mword *mem_stats_array(bvm_cache *this_bvm);
mword mem_sys_available(bvm_cache *this_bvm);


//                           +--------------+ +--------------+
//                           |     bankA    | |     bankB    |
//                           |______________| |______________|
//                            ^    ^           ^    ^
//                            |    |           |    |
//                            |    |           |    |
// +-------------+            |    |           |    |
// |   primary   |-----------(X)---|----------(X)   |
// +-------------+                 |                |
// |  secondary  |----------------(X)--------------(X)
// +-------------+
//
//
//


#endif //MEM_H

// Clayton Bauman 2013

