// rstack.h
// 

#ifndef RSTACK_H
#define RSTACK_H

#define rstack_ith(bvm,i) _ith(bvm,rci(bvm->rstack_ptr,0),i)         // rstack_ith#

//These define's are the same for all rstack_entry's
#define get_rstack_entry_tag(bvm,entry)           (_ith(bvm,entry,0))  // get_rstack_entry_tag#
#define get_rstack_entry_eval_return(bvm,entry)   (_ith(bvm,entry,1))  // get_rstack_entry_eval_return#
#define get_rstack_entry_eval_body(bvm,entry)     (_ith(bvm,entry,2))  // get_rstack_entry_eval_body#
#define get_rstack_entry_nest_dstack(bvm,entry)   (_ith(bvm,entry,2))  // get_rstack_entry_nest_dstack#
#define get_rstack_entry_times_counter(bvm,entry) (_ith(bvm,entry,3))  // get_rstack_entry_times_counter#

#define get_rstack_entry_each_list(bvm,entry)           (_ith(bvm,entry,4))  // get_rstack_entry_each_list#
#define set_rstack_entry_each_list(bvm,entry,list) (lci(_cdri(bvm,entry,4),0)=list)  // set_rstack_entry_each_list#

#define set_rstack_entry_times_counter(bvm,entry,cnt) (lcl( _ith(bvm,entry,3),0)=cnt)   // set_rstack_entry_times_counter#

#define rstack_empty(bvm) is_nil(rci(bvm->rstack_ptr,0))

bvm_cache *rstack_push(bvm_cache *this_bvm, mword *rstack_entry);
bvm_cache *rstack_pop(bvm_cache *this_bvm);

mword *rstack_first_looplike_entry(bvm_cache *this_bvm, mword *rstack_entry);
void rstack_pop_til_looplike_entry(bvm_cache *this_bvm);
mword rstack_is_entry_looplike(mword *entry_tag);

#endif //RSTACK_H

// Clayton Bauman 2014

