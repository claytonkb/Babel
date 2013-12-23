// mem.h
// 

#ifndef MEM_H
#define MEM_H

#include "babel.h"

mword *_new2lf(bvm_cache *this_bvm, mword size);
mword *_new2lfi(bvm_cache *this_bvm, mword size, mword init);
mword *_new2va(bvm_cache *this_bvm, mword value);
//#define _new2va(x,y) _new2lfi(x,1,y)
mword *_new2in(bvm_cache *this_bvm, mword size);
mword *_new2tptr(bvm_cache *this_bvm);
mword *consa2(bvm_cache *this_bvm, mword *car_field, mword *cdr_field);

void mc_init(void);
void mc_destroy(void);
mword *mc_alloc(mword sfield);
mword *mc_2alloc(bvm_cache *this_bvm, mword sfield);
void mc_free(mword *p);
void mc_reclamate(void);
void mc_reset_bank(alloc_bank *b);
mword mc_bank_usage(alloc_bank *b);
mword mc_copy_collect(bvm_cache *this_bvm);

#endif //MEM_H

// Clayton Bauman 2013

