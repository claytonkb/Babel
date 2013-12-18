// mem.h
// 

#ifndef MEM_H
#define MEM_H

#include "babel.h"

//mem_context *mc_init(void);
//void mc_destroy(mem_context *m);
//mword *mc_alloc(mem_context *m, mword sfield);
//void mc_free(alloc_bank *b, mword *p);
//void mc_reclamate(alloc_bank *b);
//void mc_copy_collect(bvm_cache *this_bvm);

void mc_init(void);
void mc_destroy(void);
mword *mc_alloc(mword sfield);
void mc_free(mword *p);
void mc_reclamate(void);
void mc_reset_bank(alloc_bank *b);
mword mc_bank_usage(alloc_bank *b);
mword mc_copy_collect(bvm_cache *this_bvm);

#endif //MEM_H

// Clayton Bauman 2013

