// bstruct.h
// 

#ifndef BSTRUCT_H
#define BSTRUCT_H

//////////////////////////////////////////////////////////////////////////////
// XXX GC-bug workaround: GCWA XXX
//////////////////////////////////////////////////////////////////////////////
mword *_cp_GCWA(bvm_cache *this_bvm, mword *bs);
mword _mu_GCWA(bvm_cache *this_bvm, mword *bs);
mword _rmu_GCWA(bvm_cache *this_bvm, mword *bs, void *v);
//////////////////////////////////////////////////////////////////////////////

#define TRAVERSED(x) (s(x) & CTL_MASK)
#define MARK_TRAVERSED(x) (s(x) |= 0x1)

#define is_leaf_masked(x)    ((~CTL_MASK & (int)s((mword*)x)) >  0)
#define is_inte_masked(x)    ((~CTL_MASK & (int)s((mword*)x)) <  0)
#define is_tptr_masked(x)   ((~CTL_MASK & (int)s((mword*)x)) == 0)
#define size_masked(x)       (abs(~CTL_MASK & s(x))/MWORD_SIZE)

typedef mword (*bstruct_op_fn_ptr)(bvm_cache *this_bvm, mword *bs, void *v);

void rclean(bvm_cache *this_bvm, mword *tree);

void     _recurse(bvm_cache *this_bvm, mword *bs, bstruct_op_fn_ptr bfn, void *v);
mword _fn_recurse(bvm_cache *this_bvm, mword *bs, bstruct_op_fn_ptr bfn, void *v);
//mword mem_usage(mword *bs);
//mword _mem_usage(mword *bs, void *v);
bvm_cache *move(bvm_cache *this_bvm);
bvm_cache *move8(bvm_cache *this_bvm);

mword _mu(bvm_cache *this_bvm, mword *bs);
mword _rmu(bvm_cache *this_bvm, mword *bs, void *v);

mword _nlf(bvm_cache *this_bvm, mword *bs);
mword _rnlf(bvm_cache *this_bvm, mword *bs, void *v);

mword _nin(bvm_cache *this_bvm, mword *bs);
mword _rnin(bvm_cache *this_bvm, mword *bs, void *v);

mword _ntag(bvm_cache *this_bvm, mword *bs);
mword _rntag(bvm_cache *this_bvm, mword *bs, void *v);

mword _nva(bvm_cache *this_bvm, mword *bs);
mword _rnva(bvm_cache *this_bvm, mword *bs, void *v);

mword _nptr(bvm_cache *this_bvm, mword *bs);
mword _rnptr(bvm_cache *this_bvm, mword *bs, void *v);

mword _lst(bvm_cache *this_bvm, mword *bs);
mword _rlst(bvm_cache *this_bvm, mword *bs, void *v);

mword *_cp(bvm_cache *this_bvm, mword *bs);

mword *_bbl2str(bvm_cache *this_bvm, mword *operand);
bvm_cache *bbl2str(bvm_cache *this_bvm);
mword rbbl2str(bvm_cache *this_bvm, mword *bs, char *buffer);

bvm_cache *mu(bvm_cache *this_bvm);
//mword _mu(this_bvm, mword *bs);
//mword _rmu(this_bvm, mword *bs);

bvm_cache *nin(bvm_cache *this_bvm);
//mword _nin(this_bvm, mword *bs);
//mword _rnin(this_bvm, mword *bs);
//
bvm_cache *nlf(bvm_cache *this_bvm);
//mword _nlf(this_bvm, mword *bs);
//mword _rnlf(this_bvm, mword *bs);

//bvm_cache *nhref(bvm_cache *this_bvm);
//mword _nhref(mword *bs);
//mword _rnhref(mword *bs);

bvm_cache *bs2gv(bvm_cache *this_bvm);
mword *_bs2gv(bvm_cache *this_bvm, mword *bs);
mword rbs2gv(bvm_cache *this_bvm, mword *bs, char *buffer);

bvm_cache *nva(bvm_cache *this_bvm);
//mword _nva(this_bvm, mword *bs);
//mword _rnva(this_bvm, mword *bs);
bvm_cache *npt(bvm_cache *this_bvm);
//mword _npt(mword *bs);
//mword _rnpt(mword *bs);

bvm_cache *trav(bvm_cache *this_bvm);
mword *_trav(bvm_cache *this_bvm, mword *bs, mword *list);

bvm_cache *paste(bvm_cache *this_bvm);
bvm_cache *set(bvm_cache *this_bvm);

bvm_cache *paste8(bvm_cache *this_bvm);

void _wrcxr(bvm_cache *this_bvm, mword *dest, mword *src, mword offset);
void _wrcxr8(bvm_cache *this_bvm, mword *dest, mword *src, mword offset);

bvm_cache *cp(bvm_cache *this_bvm);
bvm_cache *ducp(bvm_cache *this_bvm);

//void cp(void);
//void span(void);

//mword *_bs2ar(mword *bs);
mword *_bs2ar(bvm_cache *this_bvm, mword *bs);
void rbs2ar(bvm_cache *this_bvm, mword *bs, mword *arr_list, mword *offset);

bvm_cache *span(bvm_cache *this_bvm);

bvm_cache *nhword(bvm_cache *this_bvm);
mword _nhword(mword *bs);
mword _rnhword(mword *bs);

#endif //BSTRUCT_H

// Clayton Bauman 2012

