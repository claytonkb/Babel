// bstruct.h
// 

#ifndef BSTRUCT_H
#define BSTRUCT_H

void   mu           (void);
mword _mu           (mword *tree);

void   nlf          (void);
mword _nlf          (mword *tree);

void   nhref          (void);
mword _nhref          (mword *tree);

void   nin          (void);
mword _nin          (mword *tree);

void   nva          (void);
mword _nva          (mword *tree);

void   npt          (void);
mword _npt          (mword *tree);

void cp(void);
void span(void);
mword *_bs2ar(mword *tree);
void bs2ar_tree(mword *tree, mword *arr_list, mword *offset);

#endif //BSTRUCT_H

// Clayton Bauman 2012

