// list.h
// 

#ifndef LIST_H
#define LIST_H

inline mword *new_cons(void);
inline mword *new_atom(void);
void carindex(void);
void cdrindex(void);
void isnil(void);
void consls(void);
mword *_consls(mword *car_field, mword *cdr_field);
void uncons(void);
void push(void);
void pop(void);
void len(void);
mword _len(mword *list);

#endif //LIST_H

// Clayton Bauman 2011

