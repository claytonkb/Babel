// array.h
// 

#ifndef ARRAY_H
#define ARRAY_H

//bvm_cache *cxr(bvm_cache *this_bvm);
bvm_cache *sfield(bvm_cache *this_bvm);
bvm_cache *arlen(bvm_cache *this_bvm);
bvm_cache *islf(bvm_cache *this_bvm);
bvm_cache *isinte(bvm_cache *this_bvm);
bvm_cache *istptrop(bvm_cache *this_bvm);
bvm_cache *slice(bvm_cache *this_bvm);
bvm_cache *arlen8(bvm_cache *this_bvm);
bvm_cache *cut(bvm_cache *this_bvm);
bvm_cache *newin(bvm_cache *this_bvm);
bvm_cache *newlf(bvm_cache *this_bvm);
bvm_cache *trunc(bvm_cache *this_bvm);
//bvm_cache *cxr(bvm_cache *this_bvm);
bvm_cache *arcat(bvm_cache *this_bvm);
bvm_cache *arcmp(bvm_cache *this_bvm);
bvm_cache *ar2ls(bvm_cache *this_bvm);
bvm_cache *arcat8(bvm_cache *this_bvm);
bvm_cache *slice8(bvm_cache *this_bvm);
mword array8_size(bvm_cache *this_bvm, mword size8);
bvm_cache *th(bvm_cache *this_bvm);
bvm_cache *msortlf(bvm_cache *this_bvm);
void _msortlf(bvm_cache *this_bvm, mword *array);
void _rmsortlf(bvm_cache *this_bvm, mword left, mword right, mword *array);
void _mergelf(bvm_cache *this_bvm, mword *array, mword left_start, mword left_end, mword right_start, mword right_end);
bvm_cache *msortar(bvm_cache *this_bvm);
void _msortar(bvm_cache *this_bvm, mword *array);
void _rmsortar(bvm_cache *this_bvm, mword left, mword right, mword *array);
void _mergear(bvm_cache *this_bvm, mword *array, mword left_start, mword left_end, mword right_start, mword right_end);
bvm_cache *shuf(bvm_cache *this_bvm);
void _shuf(bvm_cache *this_bvm, mword *src);
mword rand_range(mword min, mword max);
mword *_newlf8(bvm_cache *this_bvm, mword size8);
mword *_slice(bvm_cache *this_bvm, mword *array, mword start, mword end);
mword *mklf(bvm_cache *this_bvm, mword array_size, ...);
mword *mkin(bvm_cache *this_bvm, mword array_size, ...);

//mword *_th(mword *bs, mword entry);
mword *_th(bvm_cache *this_bvm, mword *bs, mword entry);

//XXX DEPRECATED
//bvm_cache *w(bvm_cache *this_bvm);
//bvm_cache *paste(bvm_cache *this_bvm);
//bvm_cache *save(bvm_cache *this_bvm);

mword *_newlf(bvm_cache *this_bvm, mword size);
mword *_newin(bvm_cache *this_bvm, mword size);
//mword *_newlfz(mword size);
mword *_newlfi(bvm_cache *this_bvm, mword size, mword init);
//mword *_newtptr(bvm_cache *this_bvm);

inline mword* val(bvm_cache *this_bvm, mword *leaf, mword index);
mword *_newva(bvm_cache *this_bvm, mword value);

mword _arlen8(bvm_cache *this_bvm, mword *string);
mword  alignment_word8(bvm_cache *this_bvm, mword size8);
mword  dec_alignment_word8(bvm_cache *this_bvm, mword alignment_word);
void   _trunc(bvm_cache *this_bvm, mword *operand, mword new_size);
int    _arcmp(bvm_cache *this_bvm, mword *left, mword *right);
mword  _cxr1(bvm_cache *this_bvm, mword *val, mword bit);
mword _cxr8(mword *val, mword bit);
//mword *_ar2ls(mword *arr);
mword *_ar2ls(bvm_cache *this_bvm, mword *arr);
//mword *_cxr(mword *array, mword offset);

bvm_cache *perm(bvm_cache *this_bvm);
void _perm(bvm_cache *this_bvm, mword *src, mword *dest, mword *perm_matrix);



#endif //ARRAY_H

// Clayton Bauman 2011

