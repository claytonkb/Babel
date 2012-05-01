// list.h
// 

#ifndef LIST_H
#define LIST_H

bvm_cache *carindex(bvm_cache *this_bvm);
bvm_cache *cdrindex(bvm_cache *this_bvm);
bvm_cache *isnil(bvm_cache *this_bvm);
bvm_cache *consls(bvm_cache *this_bvm);
bvm_cache *uncons(bvm_cache *this_bvm);
bvm_cache *len(bvm_cache *this_bvm);
bvm_cache *bons(bvm_cache *this_bvm);
bvm_cache *ls2lf(bvm_cache *this_bvm);
bvm_cache *ith(bvm_cache *this_bvm);
bvm_cache *list_end(bvm_cache *this_bvm);
bvm_cache *push(bvm_cache *this_bvm);
bvm_cache *pop(bvm_cache *this_bvm);
bvm_cache *unshift(bvm_cache *this_bvm);
bvm_cache *shift(bvm_cache *this_bvm);

mword *_bons(mword *list);
mword *_ls2lf(mword *list);
mword *_ith(mword *list, mword i);
mword *_list_end(mword *list);
mword *_list_next_to_end(mword *list);
mword *_consls(mword *car_field, mword *cdr_field);
mword _len(mword *list);

#endif //LIST_H

// Clayton Bauman 2011

