// array.h
// 

#ifndef ARRAY_H
#define ARRAY_H

void sfield(void);
void arlen(void);
void islf(void);
void isinte(void);
inline mword *_newlf(mword size);
inline mword *_newin(mword size);
void cindex(void);
//void pindex(void);
//void vindex(void);
void arlen8(void);
inline mword _arlen8(mword *string);
void w(void);
void paste(void);
void cut(void);
void newin(void);
void newlf(void);
//void del(void);
//void cp(void);
//void _cp(mword offset, mword *dest, mword *src);
//void _del(mword *mem);
void trunc(void);
mword alignment_word8(mword size8);
mword dec_alignment_word8(mword alignment_word);
void _trunc(mword *operand, mword new_size);
void cxr(void);
void save(void);
void arcat(void);
void arcmp(void);
int _arcmp(mword *left, mword *right);
inline mword *_newin_blank(mword size);
void ar2ls(void);
mword *_ar2ls(mword *arr);
void cxr8(void);
mword _cxr8(mword *val, mword bit);
void cxr1(void);
mword _cxr1(mword *val, mword bit);

#endif //ARRAY_H

// Clayton Bauman 2011

