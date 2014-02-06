// interp.h
//

#include "hash.h"

#ifndef INTERP_H
#define INTERP_H

mword *BABEL_SYM_STEPS;
mword *BABEL_SYM_THREAD_ID;
mword *BABEL_SYM_ADVANCE_TYPE;
mword *BABEL_SYM_SOFT_ROOT;
mword *BABEL_SYM_JUMP_TABLE;

bvm_cache *interp_init(bvm_cache *this_bvm, int argc, char **argv, char **envp);
mword *init_interp_jump_table(bvm_cache *this_bvm);
bvm_cache *endian(bvm_cache *this_bvm);
void capture_env(bvm_cache *this_bvm, char **envp);
void init_tags(void);
void gen_time_string(bvm_cache *this_bvm);
void init_srand(bvm_cache *this_bvm);
void init_argv(bvm_cache *this_bvm, int argc, char **argv);
void init_nil(bvm_cache *this_bvm);
void flag_init(bvm_cache *this_bvm);
void init_symbolic_constants(bvm_cache *this_bvm);

#endif //INTERP_H

// Clayton Bauman 2013


