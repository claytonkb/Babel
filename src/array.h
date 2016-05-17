// array.h
// 

#ifndef ARRAY_H
#define ARRAY_H

#define ARRAY_SORT_TYPE_LEAF     0
#define ARRAY_SORT_TYPE_NON_LEAF 1

mword  _alignment_word8(bvm_cache *this_bvm, mword size8);
mword  _arlen8(bvm_cache *this_bvm, mword *string);
mword  _dec_alignment_word8(bvm_cache *this_bvm, mword alignment_word);
//void   _trunc(bvm_cache *this_bvm, mword *operand, mword new_size);
mword *_slice(bvm_cache *this_bvm, mword *array, mword start, mword end);
mword  _cxr1(bvm_cache *this_bvm, mword *val, mword bit);
mword *_th(bvm_cache *this_bvm, mword *bs, mword entry);
mword *_arcat(bvm_cache *this_bvm, mword *left, mword *right);
mword  _array8_size(bvm_cache *this_bvm, mword size8);
mword *_arcat8(bvm_cache *this_bvm, mword *left, mword *right);
mword *_lf2by(bvm_cache *this_bvm, mword *leaf_arr);
mword *_by2lf(bvm_cache *this_bvm, mword *array8);
mword *_lf2bi(bvm_cache *this_bvm, mword *array);
mword *_bi2lf(bvm_cache *this_bvm, mword *array8);
mword  _arlen1(bvm_cache *this_bvm, mword *string);
mword  _dec_alignment_word1(bvm_cache *this_bvm, mword alignment_word);
mword  _alignment_word1(bvm_cache *this_bvm, mword size1);
void   _wrcxr1(bvm_cache *this_bvm, mword *arr, mword off, mword val);
mword  _array1_size(bvm_cache *this_bvm, mword size1);
void   _msort(bvm_cache *this_bvm, mword *array);
void   _rmsort(bvm_cache *this_bvm, mword left, mword right, mword *array);
void   _merge(bvm_cache *this_bvm, mword *array, mword left_start, mword left_end, mword right_start, mword right_end);
//void    move(mword *dest, mword dest_index, mword *src, mword src_index, mword size);
void    move(bvm_cache *this_bvm, mword *dest, mword dest_index, mword *src, mword src_index, mword size, mword access_size);
int    _arcmp(mword *left, mword *right);
int _arcmp8(bvm_cache *this_bvm, mword *left, mword *right);
mword *_ar2bytes(bvm_cache *this_bvm, mword *array);
mword *_th8(bvm_cache *this_bvm, mword *bs, mword entry);
mword *array_find_val(bvm_cache *this_bvm, mword *bs, mword *val);
mword *array_find_ptr(bvm_cache *this_bvm, mword *bs, mword *ptr);
mword *_slice8(bvm_cache *this_bvm, mword *array, mword start, mword end);

void array_ptr_sort(bvm_cache *this_bvm, mword *array, mword *comparator);
void array_rptr_sort(bvm_cache *this_bvm, mword left, mword right, mword *array, mword *comparator, mword sort_type);
void array_ptr_sort_merge(bvm_cache *this_bvm, mword *array, mword left_start, mword left_end, mword right_start, mword right_end, mword *comparator, mword sort_type);

int array_cmp_alpha(bvm_cache *this_bvm, mword *left, mword *right, mword lex_alpha_select, mword access_size);

bvm_cache *th_r8d(bvm_cache *this_bvm);
bvm_cache *th_rmd(bvm_cache *this_bvm);
bvm_cache *th_wmp(bvm_cache *this_bvm);
bvm_cache *move_md(bvm_cache *this_bvm);
bvm_cache *move_8d(bvm_cache *this_bvm);
bvm_cache *arlen_md(bvm_cache *this_bvm);
bvm_cache *cat_md(bvm_cache *this_bvm);
bvm_cache *slice_md(bvm_cache *this_bvm);
bvm_cache *slice_8d(bvm_cache *this_bvm);
bvm_cache *arcmp_md(bvm_cache *this_bvm);
bvm_cache *arcmp_8d(bvm_cache *this_bvm);

bvm_cache *sortval_d(bvm_cache *this_bvm);
bvm_cache *merge_sort_d(bvm_cache *this_bvm);

bvm_cache *cat_8d(bvm_cache *this_bvm);
bvm_cache *arlen_8d(bvm_cache *this_bvm);

bvm_cache *strcmp_8d(bvm_cache *this_bvm);
bvm_cache *strcmp_md(bvm_cache *this_bvm);

#endif //ARRAY_H

// Clayton Bauman 2014

