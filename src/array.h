// array.h
// 

#ifndef ARRAY_H
#define ARRAY_H

bvm_cache *cxr(bvm_cache *this_bvm);

//#include "list.h"
//
//void sfield(void);
//void arlen(void);
//void islf(void);
//void isinte(void);
inline mword *_newlf(mword size);
mword *_newin(mword size);
//void slice(void);
////void pindex(void);
////void vindex(void);
//void arlen8(void);
inline mword _arlen8(mword *string);
//void w(void);
//void paste(void);
//void cut(void);
//void newin(void);
//void newlf(void);
//void trunc(void);
mword alignment_word8(mword size8);
mword dec_alignment_word8(mword alignment_word);
void _trunc(mword *operand, mword new_size);
//void cxr(void);
//void save(void);
//void arcat(void);
//void arcmp(void);
int _arcmp(mword *left, mword *right);
//inline mword *_newin_blank(mword size);
//void ar2ls(void);
//mword *_ar2ls(mword *arr);
//mword _cxr8(mword *val, mword bit);
mword _cxr1(mword *val, mword bit);
mword *_newlfz(mword size);
//void arcat8(void);
//void cr(void);
//void slice8(void);
////void ls2ar(void);
////void _ls2ar(mword *list, mword *array, mword offset);
//
#endif //ARRAY_H

// Clayton Bauman 2011

