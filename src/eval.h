// eval.h
// 

#ifndef EVAL_H
#define EVAL_H

#define mk_rstack_entry_eval(bvm,tag,ret)               _cons(bvm, _mkls(bvm, 2,             ret, tag), nil)   // mk_rstack_entry_eval#
#define mk_rstack_entry_nest(bvm,tag,ret,dstack)        _cons(bvm, _mkls(bvm, 3,     dstack, ret, tag), nil)   // mk_rstack_entry_nest#
#define mk_rstack_entry_loop(bvm,tag,ret,body,cnt)      _cons(bvm, _mkls(bvm, 4,  cnt, body, ret, tag), nil)   // mk_rstack_entry_loop#
#define mk_rstack_entry_times(bvm,tag,ret,body,cnt)     _cons(bvm, _mkls(bvm, 4,  cnt, body, ret, tag), nil)   // mk_rstack_entry_times#
#define mk_rstack_entry_each(bvm,tag,ret,body,cnt,list) _cons(bvm, _mkls(bvm, 5, list, cnt, body, ret, tag), nil)   // mk_rstack_entry_each#

#define get_code_next_entry(x) rci(rci(x->code_ptr,0),1)

bvm_cache *_next(bvm_cache *this_bvm);

mword _last(bvm_cache *this_bvm, mword *rstack_entry);
bvm_cache *last(bvm_cache *this_bvm);

mword *_iter(bvm_cache *this_bvm, mword *rstack_entry);
bvm_cache *iter(bvm_cache *this_bvm);
void update_iter(bvm_cache *this_bvm, mword *rstack_entry, int delta);
//mword update_iter(bvm_cache *this_bvm, int delta);

bvm_cache *sel(bvm_cache *this_bvm);
bvm_cache *selr(bvm_cache *this_bvm);

void _eval( bvm_cache *this_bvm, mword *eval_body, mword *eval_return);
void _nest( bvm_cache *this_bvm, mword *eval_body, mword *eval_return, mword *list);
void _times(bvm_cache *this_bvm, mword *eval_body, mword *eval_return, mword *counter);
void _loop( bvm_cache *this_bvm, mword *eval_body, mword *eval_return);
void _each( bvm_cache *this_bvm, mword *eval_body, mword *eval_return, mword *list);
void _eachar(bvm_cache *this_bvm, mword *eval_body, mword *eval_return, mword *list);

mword is_entry_looplike(mword *entry_tag);
//bvm_cache *dig_rstack_for_looplike_entry(bvm_cache *this_bvm);

bvm_cache *_next_eval(bvm_cache *this_bvm);
bvm_cache *_next_loop(bvm_cache *this_bvm);
bvm_cache *_next_times(bvm_cache *this_bvm);
bvm_cache *_next_each(bvm_cache *this_bvm);
bvm_cache *_next_eachar(bvm_cache *this_bvm);
bvm_cache *_next_while(bvm_cache *this_bvm);
bvm_cache *_next_nest(bvm_cache *this_bvm);
bvm_cache *_next_let(bvm_cache *this_bvm);
bvm_cache *_next_cond(bvm_cache *this_bvm);
bvm_cache *_next_alt(bvm_cache *this_bvm);
bvm_cache *_next_seq(bvm_cache *this_bvm);

bvm_cache *fail_op(bvm_cache *this_bvm);
bvm_cache *pass_op(bvm_cache *this_bvm);
bvm_cache *reject_op(bvm_cache *this_bvm);
bvm_cache *accept_op(bvm_cache *this_bvm);
bvm_cache *return_op(bvm_cache *this_bvm);

bvm_cache *eval(bvm_cache *this_bvm);
bvm_cache *nest(bvm_cache *this_bvm);
bvm_cache *times(bvm_cache *this_bvm);
bvm_cache *each(bvm_cache *this_bvm);
bvm_cache *eachar(bvm_cache *this_bvm);
bvm_cache *loop(bvm_cache *this_bvm);
bvm_cache *die(bvm_cache *this_bvm);

#endif //EVAL_H

// Clayton Bauman 2014

