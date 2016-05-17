// list.h
// 

#ifndef LIST_H
#define LIST_H

#define _new_cons(x)  (_newin(x, 2))             // _new_cons#
#define _new_dcons(x) (_newin(x, 3))             // _new_dcons#

#define CDR_DIRECTION 0
#define CPR_DIRECTION 1

mword *_insls(bvm_cache *this_bvm, mword *src_list, mword *dest_list);
mword *_dlist_end(bvm_cache *this_bvm, mword *list);
mword *_insdls_prev(bvm_cache *this_bvm, mword *src_list, mword *dest_list);
mword *_insdls(bvm_cache *this_bvm, mword *src_list, mword *dest_list);
mword _len_dlist(bvm_cache *this_bvm, mword *list);
//mword *_reverse_dlist(bvm_cache *this_bvm, mword *list, mword *head);
mword *_reverse_dlist(bvm_cache *this_bvm, mword *list, mword *head, mword direction);

mword *_dlist_cut(bvm_cache *this_bvm, mword *list, mword index, mword direction);
mword *_append_direct_dlist(bvm_cache *this_bvm, mword *head_list, mword *tail_list);

mword *_dcons(bvm_cache *this_bvm, mword *car, mword *cdr, mword *cpr);
mword *_mkdls(bvm_cache *this_bvm, mword list_size, ...);

mword *_mkls(bvm_cache *this_bvm, mword list_size, ...);
mword *_cons(bvm_cache *this_bvm, mword *car, mword *cdr);
void   _push(bvm_cache *this_bvm, mword *list, mword *bs);
mword *_pop(bvm_cache *this_bvm, mword *list);
mword *_shift(bvm_cache *this_bvm, mword *list);
mword  _len(bvm_cache *this_bvm, mword *list);
mword *_bons(bvm_cache *this_bvm, mword *list);
mword *_ls2lf(bvm_cache *this_bvm, mword *list);
mword *_lscat8(bvm_cache *this_bvm, mword *list);
mword *_cdri(bvm_cache *this_bvm, mword *list, mword i);
mword *_ith(bvm_cache *this_bvm, mword *list, mword i);
mword *_reverse(bvm_cache *this_bvm, mword *list, mword *new_cdr);
mword *_split(bvm_cache *this_bvm, mword *list, mword *indices);
mword *_rsplit(bvm_cache *this_bvm, mword *list, mword *indices, mword count);
mword *_list_end(bvm_cache *this_bvm, mword *list);
mword *_list_next_to_end(bvm_cache *this_bvm, mword *list);
mword *_list_cut(bvm_cache *this_bvm, mword *list, mword index);
mword *_append(bvm_cache *this_bvm, mword *lists);
mword *_append_direct(bvm_cache *this_bvm, mword *head_list, mword *tail_list);
mword *_ar2ls(bvm_cache *this_bvm, mword *arr);
mword *_unshift(bvm_cache *this_bvm, mword *list, mword *bs);
mword *_unshift_op(bvm_cache *this_bvm, mword *list, mword *bs);

bvm_cache *ith_rd(bvm_cache *this_bvm);
bvm_cache *len_d(bvm_cache *this_bvm);
bvm_cache *bons_d(bvm_cache *this_bvm);
bvm_cache *car_rd(bvm_cache *this_bvm);
bvm_cache *cdr_rd(bvm_cache *this_bvm);
bvm_cache *cons_d(bvm_cache *this_bvm);
bvm_cache *uncons_d(bvm_cache *this_bvm);
bvm_cache *pop_d(bvm_cache *this_bvm);
bvm_cache *push_d(bvm_cache *this_bvm);
bvm_cache *shift_d(bvm_cache *this_bvm);
bvm_cache *unshift_d(bvm_cache *this_bvm);
bvm_cache *ls2lf_d(bvm_cache *this_bvm);
bvm_cache *split_d(bvm_cache *this_bvm);
bvm_cache *append_d(bvm_cache *this_bvm);
bvm_cache *ins_d(bvm_cache *this_bvm);
bvm_cache *rev_d(bvm_cache *this_bvm);
bvm_cache *ar2ls_d(bvm_cache *this_bvm);

//    X(rsvd,        "ar2ls_d",        OP_AR2LS_D,     0x192)   

#endif //LIST_H

// Clayton Bauman 2013

