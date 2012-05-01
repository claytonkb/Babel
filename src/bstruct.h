// bstruct.h
// 

#ifndef BSTRUCT_H
#define BSTRUCT_H

#define TRAVERSED(x) (s(x) & CTL_MASK)
#define MARK_TRAVERSED(x) (s(x) |= 0x1)

void rclean(mword *tree);
bvm_cache *bbl2str(bvm_cache *this_bvm);
mword rbbl2str(mword *bs, char *buffer);

mword _mu(mword *bs);
mword _rmu(mword *bs);

mword _nin(mword *bs);
mword _rnin(mword *bs);

mword _nlf(mword *bs);
mword _rnlf(mword *bs);

mword _nhref(mword *bs);
mword _rnhref(mword *bs);

bvm_cache *bs2gv(bvm_cache *this_bvm);
mword *_bs2gv(mword *bs);
mword rbs2gv(mword *bs, char *buffer);


//#include "list.h"
//
//void wr(void);
//void _wr(mword *tree, mword *list, mword* src);
//
//void trav(void);
//mword *_trav(mword *tree, mword *list);
//
//void   mu           (void);
//mword _mu           (mword *tree);
//
//void   nlf          (void);
//mword _nlf          (mword *tree);
//
//void   nhref          (void);
//mword _nhref          (mword *tree);
//
//void   nin          (void);
//mword _nin          (mword *tree);
//
//void   nva          (void);
//mword _nva          (mword *tree);
//
//void   npt          (void);
//mword _npt          (mword *tree);
//
//void cp(void);
//void span(void);

mword *_bs2ar(mword *bs);
void rbs2ar(mword *bs, mword *arr_list, mword *offset);

bvm_cache *span(bvm_cache *this_bvm);

#endif //BSTRUCT_H

// Clayton Bauman 2012

