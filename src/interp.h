// interp.h
//

//#include "hash.h"

#ifndef INTERP_H
#define INTERP_H

#define code_empty(x) is_nil(rci(x->code_ptr,0)) // code_empty#

int interp_babel(bvm_cache *this_bvm, int argc, char **argv, char **envp);
bvm_cache *interp_core(bvm_cache *this_bvm);
bvm_cache *interp_advance(bvm_cache *this_bvm);
mword *interp_get_next_code_entry(bvm_cache *this_bvm);
bvm_cache *interp_op_exec(bvm_cache *this_bvm, mword opcode);
mword interp_update_steps(bvm_cache *this_bvm);
bvm_cache *rsvd(bvm_cache *this_bvm);
bvm_cache *interp_push_operand(bvm_cache *this_bvm, mword *operand);
bvm_cache *_pre_interp(bvm_cache *this_bvm);
bvm_cache *_post_interp(bvm_cache *this_bvm);
void _interp_exit(bvm_cache *this_bvm);
bvm_cache *interp_exit(bvm_cache *this_bvm);

#endif //INTERP_H

// Clayton Bauman 2013


