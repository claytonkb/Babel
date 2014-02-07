// mem.h
// 

#ifndef MEM_H
#define MEM_H

#include "babel.h"

//mword *_newlf(bvm_cache *this_bvm, mword size);
//mword *_newlfi(bvm_cache *this_bvm, mword size, mword init);
//mword *_newva(bvm_cache *this_bvm, mword value);
////#define _newva(x,y) _newlfi(x,1,y)
//mword *_newin(bvm_cache *this_bvm, mword size);
//mword *_newtptr(bvm_cache *this_bvm);
//mword *consa(bvm_cache *this_bvm, mword *car_field, mword *cdr_field);

void mc_init(bvm_cache *this_bvm);
void mc_destroy(bvm_cache *this_bvm);
mword *mc_alloc(bvm_cache *this_bvm, mword sfield);
//mword *mc_2alloc(bvm_cache *this_bvm, mword sfield);
void mc_free(bvm_cache *this_bvm, mword *p);
void mc_reclamate(bvm_cache *this_bvm);
void mc_reset_bank(bvm_cache *this_bvm, alloc_bank *b);
mword mc_bank_usage(bvm_cache *this_bvm, alloc_bank *b);
mword mc_copy_collect(bvm_cache *this_bvm);
void mc_swap_banks(bvm_cache *this_bvm);
mc_dump_core(bvm_cache *this_bvm);
mc_log(bvm_cache *this_bvm);

#endif //MEM_H

// Clayton Bauman 2013

