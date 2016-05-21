// bstruct.h
// 

#ifndef BSTRUCT_H
#define BSTRUCT_H

typedef struct { // trav_return#

    mword *unsafe_ptr;
    mword *bs;
    mword index;

} trav_return; 

typedef mword (*bstruct_op_fn_ptr)(bvm_cache *this_bvm, mword *bs, void *v);

void  *_sys_cp(bvm_cache *this_bvm, mword *bs);
void  *_newlf(bvm_cache *this_bvm, mword size);
inline void *_newlfi(bvm_cache *this_bvm, mword size, mword init);
inline mword *_newlfcp(bvm_cache *this_bvm, const mword *array, mword size);
void  *_mklf(bvm_cache *this_bvm, mword array_size, ...);
inline void  *_val(bvm_cache *this_bvm, mword value);
inline void  *_newin(bvm_cache *this_bvm, mword size);
void  *_mkin(bvm_cache *this_bvm, mword array_size, ...);
void   _rclean(bvm_cache *this_bvm, mword *bs);
void   _recurse(bvm_cache *this_bvm, mword *bs, bstruct_op_fn_ptr bfn, void *v);
mword  _fn_recurse(bvm_cache *this_bvm, mword *bs, bstruct_op_fn_ptr bfn, void *v);
mword  _mu(bvm_cache *this_bvm, mword *bs);
mword  _rmu(bvm_cache *this_bvm, mword *bs, void *v);
mword  _nlf(bvm_cache *this_bvm, mword *bs);
mword  _rnlf(bvm_cache *this_bvm, mword *bs, void *v);
mword  _nin(bvm_cache *this_bvm, mword *bs);
mword  _rnin(bvm_cache *this_bvm, mword *bs, void *v);
mword  _ntag(bvm_cache *this_bvm, mword *bs);
mword  _rntag(bvm_cache *this_bvm, mword *bs, void *v);
mword  _nva(bvm_cache *this_bvm, mword *bs);
mword  _rnva(bvm_cache *this_bvm, mword *bs, void *v);
mword  _nptr(bvm_cache *this_bvm, mword *bs);
mword  _rnptr(bvm_cache *this_bvm, mword *bs, void *v);
mword *_load(bvm_cache *this_bvm, mword *bs, mword size);
void   _rload(bvm_cache *this_bvm, mword *tree, mword offset);
mword *_cp(bvm_cache *this_bvm, mword *bs);
mword  _get_rel_offset(bvm_cache *this_bvm, mword *LUT_abs, mword *LUT_rel, mword *entry);
void   _trunc(bvm_cache *this_bvm, mword *operand, mword new_size);
mword *_unload_fast(bvm_cache *this_bvm, mword *bs);
mword *_cp_fast(bvm_cache *this_bvm, mword *bs);
mword *_bs2ar(bvm_cache *this_bvm, mword *bs);
void   _rbs2ar(bvm_cache *this_bvm, mword *bs, mword *arr_list, mword *offset);
mword  _runload_fast(bvm_cache *this_bvm, mword *bs, mword *dest, mword *dest_offset, mword *span_array, mword *offset_array);
mword   get_offset_from_ptr(bvm_cache *this_bvm, mword *span_array, mword *ptr);
void    set_offset_for_ptr(bvm_cache *this_bvm, mword *span_array, mword *ptr, mword *offset_array, mword this_offset);
inline void  *_ptr(bvm_cache *this_bvm, mword *unsafe_ptr);
void    bstruct_substitute(bvm_cache *this_bvm, mword *target_bs, mword *find_bs, mword *sub_bs);
//void   bstruct_substitute_tag(bvm_cache *this_bvm, mword *bs, mword *tag, mword *sub_bs);
mword *bstruct_find(bvm_cache *this_bvm, mword *target_bs, mword *find_bs);
mword *bstruct_load_fast(bvm_cache *this_bvm, mword *bs, mword size);
mword *bstruct_load_template(bvm_cache *this_bvm, mword *bs, mword *rel_offsets, mword *abs_offsets);

trav_return _trav(bvm_cache *this_bvm, mword *bs, mword *pbp);
mword _bscmp(bvm_cache *this_bvm, mword *left, mword *right);

bvm_cache *load_d(bvm_cache *this_bvm);
bvm_cache *newval_d(bvm_cache *this_bvm);
bvm_cache *cp_d(bvm_cache *this_bvm);
bvm_cache *unload_d(bvm_cache *this_bvm);
bvm_cache *msize_d(bvm_cache *this_bvm);
bvm_cache *sfield_d(bvm_cache *this_bvm);
bvm_cache *nva_d(bvm_cache *this_bvm);
bvm_cache *npt_d(bvm_cache *this_bvm);
bvm_cache *nlf_d(bvm_cache *this_bvm);
bvm_cache *nin_d(bvm_cache *this_bvm);
bvm_cache *ntag_d(bvm_cache *this_bvm);
bvm_cache *mu_d(bvm_cache *this_bvm);
bvm_cache *isval_d(bvm_cache *this_bvm);
bvm_cache *isptr_d(bvm_cache *this_bvm);
bvm_cache *istag_d(bvm_cache *this_bvm);
bvm_cache *newptr_d(bvm_cache *this_bvm);
bvm_cache *newtag_d(bvm_cache *this_bvm);
bvm_cache *get_tag_d(bvm_cache *this_bvm);
bvm_cache *tageq_d(bvm_cache *this_bvm);
bvm_cache *is_nilop(bvm_cache *this_bvm);
bvm_cache *is_falseop(bvm_cache *this_bvm);
bvm_cache *is_trueop(bvm_cache *this_bvm);
bvm_cache *is_rejectop(bvm_cache *this_bvm);
bvm_cache *is_acceptop(bvm_cache *this_bvm);
bvm_cache *span_d(bvm_cache *this_bvm);
bvm_cache *bscmp_d(bvm_cache *this_bvm);
bvm_cache *replace_d(bvm_cache *this_bvm);


#endif //BSTRUCT_H

// Clayton Bauman 2014

