// bstruct.h
// 

#ifndef BSTRUCT_H
#define BSTRUCT_H

#define TRAVERSED(x) (s(x) & CTL_MASK)
#define MARK_TRAVERSED(x) (s(x) |= 0x1)

#define is_leaf_masked(x)    ((~CTL_MASK & (int)s((mword*)x)) >  0)
#define is_inte_masked(x)    ((~CTL_MASK & (int)s((mword*)x)) <  0)
#define is_tptr_masked(x)   ((~CTL_MASK & (int)s((mword*)x)) == 0)
#define size_masked(x)       (abs(~CTL_MASK & s(x))/MWORD_SIZE)

typedef mword (*bstruct_op_fn_ptr)(mword *bs, void *v);

void rclean(mword *tree);

void _recurse(mword *bs, bstruct_op_fn_ptr bfn, void *v);
mword _fn_recurse(mword *bs, bstruct_op_fn_ptr bfn, void *v);
//mword mem_usage(mword *bs);
//mword _mem_usage(mword *bs, void *v);
bvm_cache *move(bvm_cache *this_bvm);
bvm_cache *move8(bvm_cache *this_bvm);

mword _mu(mword *bs);
mword _rmu(mword *bs, void *v);

mword _nlf(mword *bs);
mword _rnlf(mword *bs, void *v);

mword _nin(mword *bs);
mword _rnin(mword *bs, void *v);

mword _ntag(mword *bs);
mword _rntag(mword *bs, void *v);

mword _nva(mword *bs);
mword _rnva(mword *bs, void *v);

mword _nptr(mword *bs);
mword _rnptr(mword *bs, void *v);

mword _lst(mword *bs);
mword _rlst(mword *bs, void *v);

mword *_cp(mword *bs);


bvm_cache *bbl2str(bvm_cache *this_bvm);
mword rbbl2str(mword *bs, char *buffer);

bvm_cache *mu(bvm_cache *this_bvm);
//mword _mu(mword *bs);
//mword _rmu(mword *bs);

bvm_cache *nin(bvm_cache *this_bvm);
//mword _nin(mword *bs);
//mword _rnin(mword *bs);
//
bvm_cache *nlf(bvm_cache *this_bvm);
//mword _nlf(mword *bs);
//mword _rnlf(mword *bs);

//bvm_cache *nhref(bvm_cache *this_bvm);
//mword _nhref(mword *bs);
//mword _rnhref(mword *bs);

bvm_cache *bs2gv(bvm_cache *this_bvm);
mword *_bs2gv(mword *bs);
mword rbs2gv(mword *bs, char *buffer);

bvm_cache *nva(bvm_cache *this_bvm);
//mword _nva(mword *bs);
//mword _rnva(mword *bs);
bvm_cache *npt(bvm_cache *this_bvm);
//mword _npt(mword *bs);
//mword _rnpt(mword *bs);

bvm_cache *trav(bvm_cache *this_bvm);
mword *_trav(mword *bs, mword *list);

bvm_cache *paste(bvm_cache *this_bvm);
bvm_cache *set(bvm_cache *this_bvm);

bvm_cache *paste8(bvm_cache *this_bvm);

void _wrcxr (mword *dest, mword *src, mword offset);
void _wrcxr8(mword *dest, mword *src, mword offset);

bvm_cache *cp(bvm_cache *this_bvm);
bvm_cache *ducp(bvm_cache *this_bvm);

//void cp(void);
//void span(void);

mword *_bs2ar(mword *bs);
void rbs2ar(mword *bs, mword *arr_list, mword *offset);

bvm_cache *span(bvm_cache *this_bvm);

bvm_cache *nhword(bvm_cache *this_bvm);
mword _nhword(mword *bs);
mword _rnhword(mword *bs);

#endif //BSTRUCT_H

// Clayton Bauman 2012

