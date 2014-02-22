// list.h
// 

#ifndef LIST_H
#define LIST_H

bvm_cache *split(bvm_cache *this_bvm);
mword *_split(bvm_cache *this_bvm, mword *list, mword *indices);
mword *_rsplit(bvm_cache *this_bvm, mword *list, mword *indices, mword curr_index);
mword *_list_cut(bvm_cache *this_bvm, mword *list, mword index);
mword *_append(bvm_cache *this_bvm, mword *lists);
mword *_append_direct(bvm_cache *this_bvm, mword *head_list, mword *tail_list);
bvm_cache *append(bvm_cache *this_bvm);
bvm_cache *set_ith(bvm_cache *this_bvm);
bvm_cache *set_car(bvm_cache *this_bvm);
bvm_cache *set_cdr(bvm_cache *this_bvm);
mword *_cdri(bvm_cache *this_bvm, mword *list, mword i);

bvm_cache *insls(bvm_cache *this_bvm);

inline mword *_push(bvm_cache *this_bvm, mword *list, mword *bs);
inline mword *_pop(bvm_cache *this_bvm, mword *list);
inline mword *_unshift(bvm_cache *this_bvm, mword *list, mword *bs);
inline mword *_shift(bvm_cache *this_bvm, mword *list);
//mword *_reverse(this_bvm, bvm_cache *this_bvm, mword *list, mword *new_cdr);
mword *_reverse(bvm_cache *this_bvm, mword *list, mword *new_cdr);
//mword *_bons(mword *list);
mword *_bons(bvm_cache *this_bvm, mword *list);
mword *_ls2lf(bvm_cache *this_bvm, mword *list);
mword *_ith(bvm_cache *this_bvm, mword *list, mword i);
mword *_list_end(bvm_cache *this_bvm, mword *list);
mword *_list_next_to_end(bvm_cache *this_bvm, mword *list);
mword *_consls(bvm_cache *this_bvm, mword *car_field, mword *cdr_field);
mword *consa(bvm_cache *this_bvm, mword *car_field, mword *cdr_field);
mword _len(bvm_cache *this_bvm, mword *list);
mword *_walk(bvm_cache *this_bvm, mword *bs, mword *walk_list);

bvm_cache *walk(bvm_cache *this_bvm);

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
bvm_cache *reverse(bvm_cache *this_bvm);

#endif //LIST_H

// Clayton Bauman 2011

